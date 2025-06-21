/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <samt/core.h>      /* core                                                 */
#include <samt/writemem.h>  /* WritePointer                                         */
#include <samt/writeop.h>   /* WriteJump, WriteCall, WriteNoOP                      */
#include <samt/memory.h>    /* MemCopy                                              */
#include <samt/funchook.h>  /* funchook                                             */

/****** System **********************************************************************/
#include <samt/shinobi/sg_pad.h>    /* PDS_PERIPHERAL, PeripheralData               */

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_window.h>      /* WindowUpdate                                         */
#include <ic_socmagic.h>    /* SetSocPeripheral                                     */
#include <ic_sdl2.h>        /* ICSDL_PollEvents                                     */

/****** Config **********************************************************************/
#include <cnf.h>            /* CnfGet##                                             */

/****** Std *************************************************************************/
#include <math.h>           /* ceil, floor                                          */


/****** Self ************************************************************************/
#include <ic_input.h>               /* self                                         */
#include <ic_input/inpt_internal.h> /* internal                                     */

/************************/
/*  Function Types      */
/************************/
/****** User Callback ***************************************************************/
typedef void(__cdecl FN_USER_CALLBACK)(int ixUser, IC_USER* pUser);

/************************/
/*  Structures          */
/************************/
/****** Peripheral Settings *********************************************************/
typedef struct
{
    eIC_GAMEPAD_NUM  gp;    /* gamepad number                                       */
    eIC_KEYBOARD_NUM kb;    /* keyboard layout number                               */
}
USER_PERI;

/************************/
/*  Data                */
/************************/
/****** Input Settings **************************************************************/
static bool UseRawAnalog;   /* use raw analog values                                */

/****** Digital Trigger *************************************************************/
static Sint16 DgtTrigOn[NB_IC_USER];  /* digital trigger on setting                 */
static Sint16 DgtTrigOff[NB_IC_USER]; /* digital trigger off setting                */

/****** User Input ******************************************************************/
static IC_USER Users[NB_IC_USER]; /* user input structure                           */

/****** Pds Peripheral **************************************************************/
static PDS_PERIPHERALINFO PdsInfo[NB_IC_USER]; /* pds peri info                     */
static PDS_PERIPHERAL     PdsData[NB_IC_USER]; /* pds peripheral                    */

/****** Peripheral ******************************************************************/
static USER_PERI UserPeris[NB_IC_USER]; /* user peripheral settings                 */

/****** Callback ********************************************************************/
static FN_USER_CALLBACK* UserCallback; /* user input callback function for API      */

/************************/
/*  Game Data           */
/************************/
/****** Input Way *******************************************************************/
#define ga_InputWay                 DATA_ARY(s32, 0x0174B5FC, [4])

/************************/
/*  Game Functions      */
/************************/
/****** Input Update ****************************************************************/
#define UpdateRawXInput             FUNC_PTR(int, __cdecl, (void), 0x00425700)
#define UpdateControllers           FUNC_PTR(int, __cdecl, (void), 0x0077E780)

/************************/
/*  Source              */
/************************/
/****** Callback ********************************************************************/
void
UserSetCallback(void(__cdecl* callback)(int ixUser, IC_USER* pUser))
{
    UserCallback = callback;
}

/****** Static **********************************************************************/
static void
UserInputExec(void)
{
    for ( int ix_user = 0; ix_user < ARYLEN(Users); ++ix_user )
    {
        /** Setup **/
        IC_USER*   const p_user = &Users[ix_user];
        USER_PERI* const p_peri = &UserPeris[ix_user];

        INPUT_OUT input_gp = {0};
        INPUT_OUT input_kb = {0};

        /** Get inputs **/
        GamepadSetUserInput(  p_peri->gp, &input_gp );
        KeyboardSetUserInput( p_peri->kb, &input_kb );

        /** Axis Info **/
        p_user->x1 = MAX_ABS(input_gp.x1, input_kb.x1);
        p_user->y1 = MAX_ABS(input_gp.y1, input_kb.y1);
        p_user->x2 = MAX_ABS(input_gp.x2, input_kb.x2);
        p_user->y2 = MAX_ABS(input_gp.y2, input_kb.y2);

        p_user->r = MAX_ABS(input_gp.r, input_kb.r);
        p_user->l = MAX_ABS(input_gp.l, input_kb.l);

        /** Button Info **/
        const u32 btn_old = p_user->down;

        const u32 btn_new = input_gp.down | input_kb.down;

        p_user->down    = btn_new;
        p_user->press   = btn_new & ~btn_old;
        p_user->release = btn_old & ~btn_new;

        if ( UserCallback && (GamepadValid(p_peri->gp) || (p_peri->kb != IC_KEYBOARD_NONE)) )
        {
            UserCallback( ix_user, p_user );
        }
    }
}

static uint32_t
UserToDreamcastButton(uint32_t ubtn)
{
    uint32_t result = 0;

    result |= ( ubtn & USRBTN_DPAD_UP    ? PDD_DGT_KU : 0 );
    result |= ( ubtn & USRBTN_DPAD_DOWN  ? PDD_DGT_KD : 0 );
    result |= ( ubtn & USRBTN_DPAD_LEFT  ? PDD_DGT_KL : 0 );
    result |= ( ubtn & USRBTN_DPAD_RIGHT ? PDD_DGT_KR : 0 );

    result |= ( ubtn & USRBTN_A          ? PDD_DGT_TA : 0 );
    result |= ( ubtn & USRBTN_B          ? PDD_DGT_TB : 0 );
    result |= ( ubtn & USRBTN_X          ? PDD_DGT_TX : 0 );
    result |= ( ubtn & USRBTN_Y          ? PDD_DGT_TY : 0 );

    result |= ( ubtn & USRBTN_START      ? PDD_DGT_ST : 0 );
    result |= ( ubtn & USRBTN_BACK       ? PDD_DGT_TD : 0 );

    result |= ( ubtn & USRBTN_ZL         ? PDD_DGT_TC : 0 );
    result |= ( ubtn & USRBTN_ZR         ? PDD_DGT_TZ : 0 );

    return result;
}

static Sint16
UserToPdsStick(f64 mag)
{
    /** Map raw '-1.0' to '1.0' input value to PDS '-128' to '128' **/

    return (Sint16)(mag * 128.0);
}

static Uint16
UserToPdsTrigger(f64 mag)
{
    /** Map raw '0.0' to '1.0' input value to PDS '0' to '255' **/

    return (Uint16)(mag * 255.0);
}

static void
PushToSa2Peri(int ixPeri)
{
    PDS_PERIPHERAL*     const p_pad     = &PdsData[ixPeri];
    PDS_PERIPHERALINFO* const p_padinfo = &PdsInfo[ixPeri];

    PDS_PERIPHERAL*     const p_sa2pad     = &PeripheralData[ixPeri];
    PDS_PERIPHERALINFO* const p_sa2padinfo = &PeripheralInfo[ixPeri];

    *p_sa2pad     = *p_pad;
    *p_sa2padinfo = *p_padinfo;

    // reset info pointer
    p_sa2pad->info = p_sa2padinfo;
}

static void
PdsPeripheralExec(void)
{
    for (int ix_peri = 0; ix_peri < ARYLEN(PdsData); ++ix_peri)
    {
        PDS_PERIPHERAL*  const p_pad  = &PdsData[ix_peri];
        const IC_USER*   const p_user = &Users[ix_peri];
        const USER_PERI* const p_peri = &UserPeris[ix_peri];

        /** If the emulated Dreamcast controller can't recieve input, then we need
            to emulate the controller being disconnected **/
        if ( !GamepadValid(p_peri->gp) && p_peri->kb == IC_KEYBOARD_NONE )
        {
            *p_pad = (PDS_PERIPHERAL){ .info = &PdsInfo[ix_peri] };

            p_pad->info->type = 0;

            // update game peripherals
            PushToSa2Peri(ix_peri);
            continue;
        }

        p_pad->support =
            PDD_DEV_SUPPORT_TC|PDD_DEV_SUPPORT_TB|PDD_DEV_SUPPORT_TA|
            PDD_DEV_SUPPORT_ST|PDD_DEV_SUPPORT_KU|PDD_DEV_SUPPORT_KD|
            PDD_DEV_SUPPORT_KL|PDD_DEV_SUPPORT_KR|PDD_DEV_SUPPORT_TZ|
            PDD_DEV_SUPPORT_TY|PDD_DEV_SUPPORT_TX|PDD_DEV_SUPPORT_TD|
            PDD_DEV_SUPPORT_AR|PDD_DEV_SUPPORT_AL|
            PDD_DEV_SUPPORT_AX1|PDD_DEV_SUPPORT_AY1|
            PDD_DEV_SUPPORT_AX2|PDD_DEV_SUPPORT_AY2;

        /** Get analog inputs **/
        {
            p_pad->x1 = UserToPdsStick(p_user->x1);
            p_pad->y1 = UserToPdsStick(p_user->y1);

            p_pad->x2 = UserToPdsStick(p_user->x2);
            p_pad->y2 = UserToPdsStick(p_user->y2);

            p_pad->r = UserToPdsTrigger(p_user->r);
            p_pad->l = UserToPdsTrigger(p_user->l);
        }

        /** Get button inputs **/
        {
            const u32 old_on = p_pad->on;

            /** Calculate emulated trigger buttons **/
            const u32 trig_on = ( (old_on & PDD_DGT_TL) ? (p_pad->l > DgtTrigOff[ix_peri] ? PDD_DGT_TL : 0) : (p_pad->l >= DgtTrigOn[ix_peri] ? PDD_DGT_TL : 0) ) |
                                ( (old_on & PDD_DGT_TR) ? (p_pad->r > DgtTrigOff[ix_peri] ? PDD_DGT_TR : 0) : (p_pad->r >= DgtTrigOn[ix_peri] ? PDD_DGT_TR : 0) );

            u32 btn_on = UserToDreamcastButton(p_user->down) | trig_on;

            /** Keep opposite dpad directions exclusive **/
            {
                constexpr u32 dgt_ud = (PDD_DGT_KU|PDD_DGT_KD);
                constexpr u32 dgt_rl = (PDD_DGT_KR|PDD_DGT_KL);

                if ((btn_on & dgt_ud) == dgt_ud) btn_on &= ~dgt_ud;
                if ((btn_on & dgt_rl) == dgt_rl) btn_on &= ~dgt_rl;
            }

            p_pad->on  =  btn_on;
            p_pad->off = ~btn_on;

            p_pad->press   = btn_on & ~old_on;
            p_pad->release = old_on & ~btn_on;
        }

        PDS_PERIPHERALINFO* const p_padinfo = p_pad->info;

        const IC_GAMEPAD* p_gp = GamepadGetGamepad(p_peri->gp);

        p_padinfo->type = (p_gp && p_gp->support & GPDDEV_SUPPORT_RUMBLE) ?
                          (PDD_DEVTYPE_CONTROLLER|PDD_DEVTYPE_VIBRATION) :
                          (PDD_DEVTYPE_CONTROLLER);

        // update game peripherals
        PushToSa2Peri(ix_peri);
    }
}

static int
PollPeripheral(void)
{
    ICSDL_PollEvents();

    WND_Update();

    GamepadInputPoll();
    KeyboardInputPoll();
    MouseInputPoll();

    return 0;
}

#define GetSwitchData_p         FUNC_PTR(void, __cdecl, (void), 0x00441BA0)

static hook_info GetSwitchDataHookInfo[1];
static void
ExecPeripheral(void)
{
    GamepadInputExec();
    KeyboardInputExec();
    MouseInputExec();

    UserInputExec();

    SocPeripheralExec();
    PdsPeripheralExec();

    FuncHookCall( GetSwitchDataHookInfo, GetSwitchData_p() );
}

#define EventWaitVsyncCount     DATA_REF(s32, 0x01DEB514)

static void
EventWaitVsyncFix(void)
{
    if ( EventWaitVsyncCount != 1 )
    {
        ExecPeripheral();
    }
}

/****** Extern **********************************************************************/
const IC_USER*
UserGetInput(const eIC_USER_NUM nbUser)
{
    return &Users[nbUser];
}

eIC_GAMEPAD_NUM
UserGetGamepadNum(const eIC_USER_NUM nbUser)
{
    return UserPeris[nbUser].gp;
}

eIC_KEYBOARD_NUM
UserGetKeyboardNum(const eIC_USER_NUM nbUser)
{
    return UserPeris[nbUser].kb;
}

bool
ICF_UseRawAnalog(void)
{
    return UseRawAnalog;
}

/****** Init ************************************************************************/
void
IC_InputInit(void)
{
    UseRawAnalog = CNF_GetInt(CNF_MAIN_RAWANALOG);

    /** User peripheral indexes **/
    UserPeris[0].gp = CNF_GetInt( CNF_USER1_GAMEPD_NB );
    UserPeris[0].kb = CNF_GetInt( CNF_USER1_KEYBRD_NB );

    UserPeris[1].gp = CNF_GetInt( CNF_USER2_GAMEPD_NB );
    UserPeris[1].kb = CNF_GetInt( CNF_USER2_KEYBRD_NB );

    UserPeris[2].gp = CNF_GetInt( CNF_USER3_GAMEPD_NB );
    UserPeris[2].kb = CNF_GetInt( CNF_USER3_KEYBRD_NB );

    UserPeris[3].gp = CNF_GetInt( CNF_USER4_GAMEPD_NB );
    UserPeris[3].kb = CNF_GetInt( CNF_USER4_KEYBRD_NB );

    /** Digital Trigger **/
    DgtTrigOn[0]  = CNF_GetInt( CNF_USER1_DGTLR_ON  );
    DgtTrigOff[0] = CNF_GetInt( CNF_USER1_DGTLR_OFF );

    DgtTrigOn[1]  = CNF_GetInt( CNF_USER2_DGTLR_ON  );
    DgtTrigOff[1] = CNF_GetInt( CNF_USER2_DGTLR_OFF );

    DgtTrigOn[2]  = CNF_GetInt( CNF_USER3_DGTLR_ON  );
    DgtTrigOff[2] = CNF_GetInt( CNF_USER3_DGTLR_OFF );

    DgtTrigOn[3]  = CNF_GetInt( CNF_USER4_DGTLR_ON  );
    DgtTrigOff[3] = CNF_GetInt( CNF_USER4_DGTLR_OFF );

    /** Sub-module Init **/
    GamepadInit();
    KeyboardInit();
    MouseInit();

    PdsData[0].info = &PdsInfo[0];
    PdsData[1].info = &PdsInfo[1];
    PdsData[2].info = &PdsInfo[2];
    PdsData[3].info = &PdsInfo[3];

    /** The rest of UpdateControllers needs to run, as the mod loader hooks the retn
        op at the end to run OnInput. By NOP'ing the entire function up until the
        return, we maintain compatibility with the mod loader.

        The reason we're leaving 5 bytes at the beginning is because of the Cheat
        Table commonly used for SA2. It's hardcoded to push the registers like the
        original code does, so we have no choice but to leave the beginning 'push's
        and end 'pop's as is to maintain compatibility with a Cheat Engine Table
        made in 2013 **/
    WriteNOP( 0x0077E785, 0x0077E892);      // UpdateControllers (0x0077E780) + 5
    WriteCall(0x0077E785, PollPeripheral);  // ^^

    // Hook GetSwitchData() to execute input data
    FuncHook(GetSwitchDataHookInfo, GetSwitchData_p, ExecPeripheral);

    WriteNOP( 0x005FB493, 0x005FB4C0);        // Event WaitVsync fix
    WriteCall(0x005FB493, EventWaitVsyncFix); // ^^

    /** Fix cart controls being *0.5 **/
    WriteNOP(0x0061F5E2, 0x0061F5E8);
    WriteNOP(0x0061E4FC, 0x0061E502);
    static const f64 mulf = 1.0;
    WritePointer(0x0061E04E, &mulf);

    /** Text Mode **/
    const s32 text_md = CNF_GetInt( CNF_MISC_TEXTMD );

    ga_InputWay[0] = text_md;
    ga_InputWay[1] = text_md;
    ga_InputWay[2] = text_md;
    ga_InputWay[3] = text_md;
}
