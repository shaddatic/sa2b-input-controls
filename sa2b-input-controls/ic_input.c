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
/*  Enums               */
/************************/
typedef enum
{
    INPUT_KEYBOARD,
    INPUT_GAMEPAD,
    INPUT_SWITCH,
    INPUT_BOTH,
}
eINPUT_METHOD;

/************************/
/*  File Variables      */
/************************/
static bool UseRawAnalog;

static eINPUT_METHOD    UserInputMode[4];
static eGAMEPAD_NUM     UserGamepad[4];
static eKEYBOARD_NUM    UserKeyboard[4];

static USER_INPUT   UserInput[4];

/************************/
/*  Game Functions      */
/************************/
#define UpdateRawXInput             FUNC_PTR(int, __cdecl, (void), 0x00425700)
#define UpdateControllers           FUNC_PTR(int, __cdecl, (void), 0x0077E780)

/************************/
/*  Source              */
/************************/
eGAMEPAD_NUM
UserGetGamepadNum(const eUSER_NUM nbUser)
{
    return UserGamepad[nbUser];
}

bool
UserGetGamepadValid(const eUSER_NUM nbUser)
{
    const eGAMEPAD_NUM nb_gp = UserGetGamepadNum(nbUser);

    return GamepadValid(nb_gp);
}

bool
UserVibSet(const eUSER_NUM nbUser, const f32 spdL, const f32 spdR)
{
    const eGAMEPAD_NUM nb_gp = UserGetGamepadNum(nbUser);

    return GamepadVibSet(nb_gp, spdL, spdR);
}

bool
UserVibStop(const eUSER_NUM nbUser)
{
    const eGAMEPAD_NUM nb_gp = UserGetGamepadNum(nbUser);

    return GamepadVibStop(nb_gp);
}

eKEYBOARD_NUM
UserGetKeyboardNum(const eUSER_NUM nbUser)
{
    return UserKeyboard[nbUser];
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
        USER_INPUT* const p_input = &UserInput[i];

        const uint32_t btn_old = p_input->down; // old button for button calculations

        /** Clearing **/
        *p_input = (USER_INPUT){0};

        /** Gamepad **/
        bool gp_valid = false; // store valid if the user is in 'SWITCH' mode

        const eGAMEPAD_NUM nb_gp = UserGamepad[i];

        if (nb_gp != GAMEPAD_NONE)
        {
            gp_valid = GamepadValid(nb_gp);

            if (UserInputMode[i] != INPUT_KEYBOARD && gp_valid) 
            {
                GamepadSetUserInput(nb_gp, p_input);
            }
        }

        /** Keyboard **/
        const eKEYBOARD_NUM nb_kb = UserKeyboard[i];

        if (nb_kb != KEYBOARD_NONE)
        {
            if (UserInputMode[i] != INPUT_GAMEPAD && (UserInputMode[i] != INPUT_SWITCH || !gp_valid))
            {
                KeyboardSetUserInput(nb_kb, p_input);
            }
        }

        /** Set button **/
        const uint32_t btn_new = p_input->down;

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
        if (!GamepadValid(UserGamepad[i]) && UserKeyboard[i] == KEYBOARD_NONE)
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
        }

        /** Get button inputs **/
        {
            const u32 old_on = p_pad->on;

            /** Calculate emulated trigger buttons **/
            const u32 trig_on = ( (old_on & PDD_DGT_TL) ? (p_pad->l > 160 ? PDD_DGT_TL : 0) : (p_pad->l >= 192 ? PDD_DGT_TL : 0) ) |
                                ( (old_on & PDD_DGT_TR) ? (p_pad->r > 160 ? PDD_DGT_TR : 0) : (p_pad->r >= 192 ? PDD_DGT_TR : 0) );

            const u32 btn_on = UserToDreamcastButton(p_input->down) | trig_on;

            p_pad->on  =  btn_on;
            p_pad->off = ~btn_on;

            p_pad->press   = btn_on & ~old_on;
            p_pad->release = old_on & ~btn_on;
        }

        PDS_PERIPHERALINFO* const p_padinfo = p_pad->info;

        if (p_padinfo)
            p_padinfo->type = GamepadVibValid(UserGamepad[i]) ? PDD_DEVTYPE_CONTROLLER|PDD_DEVTYPE_VIBRATION : PDD_DEVTYPE_CONTROLLER;
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
    UseRawAnalog = CnfGetInt(CNF_MAIN_RAWANALOG);

    UserGamepad[0]   = CnfGetInt( CNF_USER1_GAMEPD_NB  );
    UserKeyboard[0]  = CnfGetInt( CNF_USER1_KEYBRD_NB  );
    UserInputMode[0] = CnfGetInt( CNF_USER1_INPUT_MODE );

    UserGamepad[1]   = CnfGetInt( CNF_USER2_GAMEPD_NB  );
    UserKeyboard[1]  = CnfGetInt( CNF_USER2_KEYBRD_NB  );
    UserInputMode[1] = CnfGetInt( CNF_USER2_INPUT_MODE );

    UserGamepad[2]   = CnfGetInt( CNF_USER3_GAMEPD_NB  );
    UserKeyboard[2]  = CnfGetInt( CNF_USER3_KEYBRD_NB  );
    UserInputMode[2] = CnfGetInt( CNF_USER3_INPUT_MODE );

    UserGamepad[3]   = CnfGetInt( CNF_USER4_GAMEPD_NB  );
    UserKeyboard[3]  = CnfGetInt( CNF_USER4_KEYBRD_NB  );
    UserInputMode[3] = CnfGetInt( CNF_USER4_INPUT_MODE );

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
