/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core */
#include <sa2b/memutil.h>   /* WriteJump, WriteCall, WriteNoOP */
#include <sa2b/memory.h>    /* MemCopy  */

/****** SoC *************************************************************************/
#include <sa2b/soc/input.h>

/****** System **********************************************************************/
#include <sa2b/shinobi/sg_pad.h>

/****** Game ************************************************************************/
#include <sa2b/sonic/input.h>

/****** Std *************************************************************************/
#include <stdio.h>
#include <math.h>

/****** Input Controls **************************************************************/
#include <ic_core.h>
#include <ic_window.h>
#include <ic_config.h>
#include <ic_socmagic.h>

/****** Self ************************************************************************/
#include <ic_input.h>               /* self                                         */
#include <ic_input/inpt_internal.h> /* internal                                     */

/************************/
/*  Constants           */
/************************/
#define XInputGetCapabilities   ___XInputGetCapabilities
#define XInputGetState          ___XInputGetState
#define XInputSetState          ___XInputSetState

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
static eINPUT_METHOD    UserInputMode[4];
static eGAMEPAD_NUM     UserGamepad[4];
static eKEYBOARD_NUM    UserKeyboard[4];

static USER_INPUT   UserInput[4];

/************************/
/*  Game Functions      */
/************************/
#define UpdateRawXInput             FuncPtr(int, __cdecl, (void), 0x00425700)
#define UpdateControllers           FuncPtr(int, __cdecl, (void), 0x0077E780)

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

    return GamepadGetValid(nb_gp);
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

        const uint32_t btn_old = p_input->on; // old button for button calculations

        /** Clearing **/
        *p_input = (USER_INPUT){0};

        /** Gamepad **/
        bool gp_valid = false; // store valid if the user is in 'SWITCH' mode

        const eGAMEPAD_NUM nb_gp = UserGamepad[i];

        if (nb_gp != GAMEPAD_NONE)
        {
            gp_valid = GamepadGetValid(nb_gp);

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
        const uint32_t btn_new = p_input->on;

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

    result |= ( ubtn & USRBTN_L          ? PDD_DGT_TL : 0 );
    result |= ( ubtn & USRBTN_R          ? PDD_DGT_TR : 0 );

    return result;
}

static Sint16
UserToPdsStick(f64 mag)
{
    return !mag ? 0 : (Sint16)(mag > 0.0 ?
        mag * 127.99999999999999 : //  0~127
        mag * 128.99999999999997); // -128~0
}

static Uint16
UserToPdsTrigger(f64 mag)
{
    return !mag ? 0 : (Uint16)(mag * 255.9999999999999);
}

static void
SetPdsPeripheral(void)
{
    for (int i = 0; i < ARYLEN(PeripheralData); ++i)
    {
        PDS_PERIPHERAL*   const p_pad   = &PeripheralData[i];
        const USER_INPUT* const p_input = &UserInput[i];

        p_pad->support = PDD_DEV_SUPPORT_TC|PDD_DEV_SUPPORT_TB|PDD_DEV_SUPPORT_TA|PDD_DEV_SUPPORT_ST|PDD_DEV_SUPPORT_KU|PDD_DEV_SUPPORT_KD|PDD_DEV_SUPPORT_KL|PDD_DEV_SUPPORT_KR|PDD_DEV_SUPPORT_TZ|PDD_DEV_SUPPORT_TY|PDD_DEV_SUPPORT_TX|PDD_DEV_SUPPORT_TD|PDD_DEV_SUPPORT_AR|PDD_DEV_SUPPORT_AL|PDD_DEV_SUPPORT_AX1|PDD_DEV_SUPPORT_AY1|PDD_DEV_SUPPORT_AX2;

        const uint32_t btn_on = UserToDreamcastButton(p_input->on);

        p_pad->on  =  btn_on;
        p_pad->off = ~btn_on;

        p_pad->press   = UserToDreamcastButton(p_input->press);
        p_pad->release = UserToDreamcastButton(p_input->release);

        p_pad->x1 = UserToPdsStick(p_input->x1);
        p_pad->y1 = UserToPdsStick(p_input->y1);

        p_pad->x2 = UserToPdsStick(p_input->x2);
        p_pad->y2 = UserToPdsStick(p_input->y2);

        p_pad->r = UserToPdsTrigger(p_input->r);
        p_pad->l = UserToPdsTrigger(p_input->l);

        p_pad->info->type = PDD_DEVTYPE_VIBRATION|PDD_DEVTYPE_CONTROLLER;
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

static bool UseRawAnalog;

bool
InputUseRawAnalog(void)
{
    return UseRawAnalog;
}

void
IC_InputInit(void)
{
    UseRawAnalog = CnfGetInt(CNF_COMPAT_ANALOG);

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
    WriteNoOP(0x0077E780, 0x0077E897);

    /** Fix cart controls being *0.5 **/
    WriteNoOP(0x0061F5E2, 0x0061F5E8);
    WriteNoOP(0x0061E4FC, 0x0061E502);
    static const f64 mulf = 1.0;
    WritePointer(0x0061E04E, &mulf);

    WriteCall(UpdateControllers, SetPeripheral);
}
