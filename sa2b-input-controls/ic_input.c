/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */
#include <sa2b/writemem.h>  /* WritePointer                                         */
#include <sa2b/writeop.h>   /* WriteJump, WriteCall, WriteNoOP                      */
#include <sa2b/memory.h>    /* MemCopy                                              */

/****** System **********************************************************************/
#include <sa2b/shinobi/sg_pad.h>    /* PDS_PERIPHERAL, PeripheralData               */

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_window.h>      /* WindowUpdate                                         */
#include <ic_config.h>      /* CnfGet___                                            */
#include <ic_socmagic.h>    /* SetSocPeripheral                                     */

/****** Self ************************************************************************/
#include <ic_input.h>               /* self                                         */
#include <ic_input/inpt_internal.h> /* internal                                     */

/************************/
/*  Structures          */
/************************/
typedef struct
{
    eGAMEPAD_NUM  nbGp;
    eKEYBOARD_NUM nbKb;
    eINPUT_MODE   mode;
}
USER_INFO;

/************************/
/*  File Variables      */
/************************/
static bool UseRawAnalog;
static bool X2SetsLR;

static Sint16 DgtLROn;
static Sint16 DgtLROff;

static USER_INFO UserInfos[NB_USER];

static USER_INPUT UserInput[NB_USER];

/************************/
/*  Game Functions      */
/************************/
#define UpdateRawXInput             FUNC_PTR(int, __cdecl, (void), 0x00425700)
#define UpdateControllers           FUNC_PTR(int, __cdecl, (void), 0x0077E780)

/************************/
/*  Source              */
/************************/
eINPUT_MODE
UserGetInputMode(const eUSER_NUM nbUser)
{
    return UserInfos[nbUser].mode;
}

eGAMEPAD_NUM
UserGetGamepadNum(const eUSER_NUM nbUser)
{
    return UserInfos[nbUser].nbGp;
}

eKEYBOARD_NUM
UserGetKeyboardNum(const eUSER_NUM nbUser)
{
    return UserInfos[nbUser].nbKb;
}

const USER_INPUT*
UserGetInput(const eUSER_NUM nbUser)
{
    return &UserInput[nbUser];
}

static void
SetUserInput(void)
{
    for (int i = 0; i < ARYLEN(UserInput); ++i)
    {
        /** Setup **/
        USER_INFO*  const p_info  = &UserInfos[i];
        USER_INPUT* const p_input = &UserInput[i];

        INPUT_OUT input_gp = {0};
        INPUT_OUT input_kb = {0};

        /** This looks confusing, I know, but trust me this is the best idea I had
            on how to do this. I tried others, it was messy. This though is fine **/
        switch (p_info->mode) {
        case INPUT_MD_NONE:
            break;

        case INPUT_MD_GAMEPAD:
        {
            GamepadSetUserInput(p_info->nbGp, &input_gp);
            break;
        }
        case INPUT_MD_KEYBOARD: 
        {
            KeyboardSetUserInput(p_info->nbKb, &input_kb);
            break;
        }
        case INPUT_MD_SWITCH:
        {
            if ( !GamepadSetUserInput(p_info->nbGp, &input_gp) )
            {
                KeyboardSetUserInput(p_info->nbKb, &input_kb);
            }
            break;
        }
        case INPUT_MD_BOTH:
        {
            GamepadSetUserInput(p_info->nbGp, &input_gp);
            KeyboardSetUserInput(p_info->nbKb, &input_kb);
            break;
        }
        }

        /** Axis Info **/
        p_input->x1 = MAX_ABS(input_gp.x1, input_kb.x1);
        p_input->y1 = MAX_ABS(input_gp.y1, input_kb.y1);
        p_input->x2 = MAX_ABS(input_gp.x2, input_kb.x2);
        p_input->y2 = MAX_ABS(input_gp.y2, input_kb.y2);

        p_input->r = MAX_ABS(input_gp.r, input_kb.r);
        p_input->l = MAX_ABS(input_gp.l, input_kb.l);

        /** Button Info **/
        const u32 btn_old = p_input->down;

        const u32 btn_new = input_gp.down | input_kb.down;

        p_input->down    = btn_new;
        p_input->press   = btn_new & ~btn_old;
        p_input->release = btn_old & ~btn_new;
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
    return (Sint16)(mag * (f64)PDSLIM_XY_MAX); // -128~128
}

static Uint16
UserToPdsTrigger(f64 mag)
{
    return (Uint16)(mag * (f64)PDSLIM_LR_MAX);
}

static void
SetPdsPeripheral(void)
{
    for (int i = 0; i < ARYLEN(PeripheralData); ++i)
    {
        PDS_PERIPHERAL*   const p_pad   = &PeripheralData[i];
        const USER_INPUT* const p_input = &UserInput[i];

        /** If the emulated Dreamcast controller can't recieve input, then we need
            to emulate the controller being disconnected **/
        if (!GamepadValid(UserInfos[i].nbGp) && UserInfos[i].nbKb == KEYBOARD_NONE)
        {
            *p_pad = (PDS_PERIPHERAL){0};

            if (p_pad->info)
                p_pad->info->type = 0;

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
            p_pad->x1 = UserToPdsStick(p_input->x1);
            p_pad->y1 = UserToPdsStick(p_input->y1);

            p_pad->x2 = UserToPdsStick(p_input->x2);
            p_pad->y2 = UserToPdsStick(p_input->y2);

            p_pad->r = UserToPdsTrigger(p_input->r);
            p_pad->l = UserToPdsTrigger(p_input->l);

            if (X2SetsLR && p_input->x2)
            {
                int x2_lr = (int)(p_input->x2 * (f32)PDSLIM_LR_MAX);

                if (p_pad->l < -x2_lr) p_pad->l = -x2_lr;
                if (p_pad->r <  x2_lr) p_pad->r =  x2_lr;
            }
        }

        /** Get button inputs **/
        {
            const u32 old_on = p_pad->on;

            /** Calculate emulated trigger buttons **/
            const u32 trig_on = ( (old_on & PDD_DGT_TL) ? (p_pad->l > DgtLROff ? PDD_DGT_TL : 0) : (p_pad->l >= DgtLROn ? PDD_DGT_TL : 0) ) |
                                ( (old_on & PDD_DGT_TR) ? (p_pad->r > DgtLROff ? PDD_DGT_TR : 0) : (p_pad->r >= DgtLROn ? PDD_DGT_TR : 0) );

            u32 btn_on = UserToDreamcastButton(p_input->down) | trig_on;

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

        const GAMEPAD* p_gp = GamepadGetGamepad(UserInfos[i].nbGp);

        if (p_padinfo)
        {
            p_padinfo->type = (p_gp && p_gp->support & GPDDEV_SUPPORT_RUMBLE) ?
                PDD_DEVTYPE_CONTROLLER|PDD_DEVTYPE_VIBRATION :
                PDD_DEVTYPE_CONTROLLER;
        }
    }
}

static int
SetPeripheral(void)
{
    IC_WindowUpdate();

    GamepadUpdate();
    KeyboardUpdate();
    MouseUpdate();

    SetUserInput();

    SetSocPeripheral();

    SetPdsPeripheral();

    return 0;
}

bool
InputUseRawAnalog(void)
{
    return UseRawAnalog;
}

void
IC_InputInit(void)
{
    X2SetsLR     = CnfGetInt(CNF_MAIN_X2LR);
    UseRawAnalog = CnfGetInt(CNF_MAIN_RAWANALOG);

    DgtLROn  = (Sint16) CnfGetInt(CNF_MAIN_DGTLR_ON);
    DgtLROff = (Sint16) CnfGetInt(CNF_MAIN_DGTLR_OFF);

    UserInfos[0].nbGp = CnfGetInt( CNF_USER1_GAMEPD_NB  );
    UserInfos[0].nbKb = CnfGetInt( CNF_USER1_KEYBRD_NB  );
    UserInfos[0].mode = CnfGetInt( CNF_USER1_INPUT_MODE ); 

    UserInfos[1].nbGp = CnfGetInt( CNF_USER2_GAMEPD_NB  );
    UserInfos[1].nbKb = CnfGetInt( CNF_USER2_KEYBRD_NB  );
    UserInfos[1].mode = CnfGetInt( CNF_USER2_INPUT_MODE );

    UserInfos[2].nbGp = CnfGetInt( CNF_USER3_GAMEPD_NB  );
    UserInfos[2].nbKb = CnfGetInt( CNF_USER3_KEYBRD_NB  );
    UserInfos[2].mode = CnfGetInt( CNF_USER3_INPUT_MODE );

    UserInfos[3].nbGp = CnfGetInt( CNF_USER4_GAMEPD_NB  );
    UserInfos[3].nbKb = CnfGetInt( CNF_USER4_KEYBRD_NB  );
    UserInfos[3].mode = CnfGetInt( CNF_USER4_INPUT_MODE ); 

    GamepadInit();
    KeyboardInit();
    MouseInit();

    /** The rest of UpdateControllers needs to run, as the mod loader hooks the retn
        op at the end to run OnInput. By NOP'ing the entire function up until the
        return, we maintain compatibility with the mod loader **/
    WriteNOP(0x0077E780, 0x0077E897);

    /** Fix cart controls being *0.5 **/
    WriteNOP(0x0061F5E2, 0x0061F5E8);
    WriteNOP(0x0061E4FC, 0x0061E502);
    static const f64 mulf = 1.0;
    WritePointer(0x0061E04E, &mulf);

    WriteCall(UpdateControllers, SetPeripheral);
}
