/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Mod Loader ******************************************************************/
#include <sa2b/modloader.h> /* ML_DisplayDebugStringF                               */

/****** Ninja ***********************************************************************/
#include <sa2b/ninja/ninja.h>   /* Ninja                                            */

/****** Game ************************************************************************/
#include <sa2b/sonic/debug.h>   /* OutputFormat                                     */

/****** Std *************************************************************************/
#include <stdio.h>  /* snprintf                                                     */
#include <math.h>   /* nearintf                                                     */

/****** Input Controls **************************************************************/
#include <ic_core.h>    /* core                                                     */
#include <ic_config.h>  /* CnfGet___                                                */
#include <ic_sdl2.h>    /* SDL2                                                     */

/****** Self ************************************************************************/
#include <ic_input/inpt_internal.h> /* internal                                     */

/************************/
/*  File Macro          */
/************************/
#define NORM_GPD_DIR(mag)   ((f64)mag/(mag>0?(f64)GPDLIM_XY_MAX:(f64)-GPDLIM_XY_MIN))
#define NORM_GPD_TRIG(mag)  ((f64)mag/(f64)(SDL_JOYSTICK_AXIS_MAX))

/************************/
/*  Structures          */
/************************/
typedef struct
{
    f32 idz;    /* inner deadzone                                                   */
    f32 odz;    /* outer deadzone                                                   */
}
DEADZONE;

typedef struct
{
    DEADZONE StickL;
    DEADZONE StickR;

    f32 vibPwrL;
    f32 vibPwrR;

    uint8_t dzMode;

    bool useVib;
}
USER_GAMEPAD;

#define GPDDEV_SUPPORT_RUMBLE_TRIGGER   (1<<30)
#define GPDDEV_SUPPORT_RUMBLE           (1<<31)

typedef struct
{
    SDL_GameController* pSdlGp;
    int                 id;

    u32                 support;

    u32                 down;

    int16_t             x1, y1;
    int16_t             x2, y2;
    int16_t             l , r;
}
GAMEPAD;

/************************/
/*  File Data           */
/************************/
static USER_GAMEPAD UserGamepads[4]; /* user gamepad settings                        */

static GAMEPAD Gamepads[4];          /* physical gamepad data                        */

static bool GamepadDbgAxis;

/************************/
/*  Source              */
/************************/
static void
ResetGamepadStruct(GAMEPAD* const pGp)
{
    *pGp = (GAMEPAD){ .id = -1 };
}

static void
OpenGamepad(const int id)
{
    if (!SDL2_IsGameController(id))
        return;

    for (int i = 0; i < ARYLEN(Gamepads); ++i)
    {
        GAMEPAD* const p_gp = &Gamepads[i];

        /** If ID matches, reset device **/
        if (p_gp->id == id)
        {
            SDL2_GameControllerClose(p_gp->pSdlGp);
            goto OPEN;
        }

        if (!p_gp->pSdlGp)
        {
        OPEN:
            p_gp->pSdlGp = SDL2_GameControllerOpen(id);
            p_gp->id     = id;

            SDL_GameController* const p_sdlgc = p_gp->pSdlGp;

            for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
            {
                if (SDL2_GameControllerHasButton(p_sdlgc, i))
                    p_gp->support |= (1<<i);
            }

            p_gp->support |= SDL2_GameControllerHasRumbleTriggers(p_sdlgc) ? GPDDEV_SUPPORT_RUMBLE_TRIGGER : 0;
            p_gp->support |= SDL2_GameControllerHasRumble(p_sdlgc)         ? GPDDEV_SUPPORT_RUMBLE         : 0;
            break;
        }
    }
}

static void
CloseGamepad(const int id)
{
    for (int i = 0; i < ARYLEN(Gamepads); ++i)
    {
        GAMEPAD* const p_gp = &Gamepads[i];

        if (p_gp->id == id)
        {
            SDL2_GameControllerClose(p_gp->pSdlGp);
            ResetGamepadStruct(p_gp);
        }
    }
}

static void
HandleSdlEvents(void)
{
    SDL_Event ev;

    while (SDL2_PollEvent(&ev))
    {
        switch (ev.type) {
        case SDL_JOYDEVICEADDED:
            OpenGamepad(ev.cdevice.which);
            break;

        case SDL_JOYDEVICEREMOVED:
            CloseGamepad(ev.cdevice.which);
            break;

        default:
            break;
        }
    }
}

bool
GamepadValid(const eGAMEPAD_NUM nbGp)
{
    if (nbGp == GAMEPAD_NONE)
        return false;

    return Gamepads[nbGp].pSdlGp;
}

bool
GamepadVibValid(const eGAMEPAD_NUM nbGp)
{
    if (nbGp == GAMEPAD_NONE)
        return false;

    return Gamepads[nbGp].support & GPDDEV_SUPPORT_RUMBLE;
}

bool
GamepadVibSet(const eGAMEPAD_NUM nbGp, const f32 spdL, const f32 spdR)
{
    if (nbGp == GAMEPAD_NONE)
        return false;

    const GAMEPAD* const p_gp = &Gamepads[nbGp];

    return SDL2_GameControllerRumble(p_gp->pSdlGp, (Sint16)(spdL*65535.f), (Sint16)(spdR*65535.f), 0xFFFFFFFF);
}

bool
GamepadVibStop(const eGAMEPAD_NUM nbGp)
{
    return GamepadVibSet(nbGp, 0.f, 0.f);
}

static void
DebugAxes(const int nbGp, const f32 x1, const f32 y1, const f32 x2, const f32 y2)
{
    char buf[64];

    ML_SetDebugFontColor(0xFFFFFFFF);
    ML_SetDebugFontScale(12.f);

    const f32 m1 = sqrtf((x1*x1)+(y1*y1));
    const f32 m2 = sqrtf((x2*x2)+(y2*y2));

    ML_DisplayDebugStringF(NJM_LOCATION(1, (nbGp*3)+1), buf, 64, "GAMEPAD %i:", nbGp);

    ML_DisplayDebugStringF(NJM_LOCATION(2, (nbGp*3)+2), buf, 64, "LS: X %+f, Y %+f, M %+f", x1, y1, m1);
    ML_DisplayDebugStringF(NJM_LOCATION(2, (nbGp*3)+3), buf, 64, "RS: X %+f, Y %+f, M %+f", x2, y2, m2);
}

static f32
CalcLinearDeadzone(const f32 input, const f32 idz, const f32 odz)
{
    f32 out = input;

    if (fabsf(out) >= idz)
    {
        out -= out > 0.0f ? idz : -idz;

        out *= (1.0f / (odz - idz));
    }
    else
        out = 0.0;

    return out;
}

static void
CalcSquareDeadzone(f32* const pX, f32* const pY, const f32 idz, const f32 odz)
{
    *pX = CalcLinearDeadzone(*pX, idz, odz);
    *pY = CalcLinearDeadzone(*pY, idz, odz);
}

static void
CalcCircularDeadzone(f32* const pX, f32* const pY, const f32 idz, const f32 odz)
{
    const f32 x = *pX;
    const f32 y = *pY;

    const f64 dz_mag = CalcLinearDeadzone( sqrtf((x * x) + (y * y)), idz, odz );

    const f64 angf = atan2(x, y);

    *pX = (f32)(sin(angf) * dz_mag);
    *pY = (f32)(cos(angf) * dz_mag);
}

static u32
GamepadToUserButton(u32 gpbtn)
{
    u32 btn = 0;

    btn |= ( gpbtn & GPDBTN_A ? USRBTN_A : 0 );
    btn |= ( gpbtn & GPDBTN_B ? USRBTN_B : 0 );
    btn |= ( gpbtn & GPDBTN_X ? USRBTN_X : 0 );
    btn |= ( gpbtn & GPDBTN_Y ? USRBTN_Y : 0 );

    btn |= ( gpbtn & GPDBTN_START ? USRBTN_START : 0 );
    btn |= ( gpbtn & GPDBTN_BACK  ? USRBTN_BACK  : 0 );

    btn |= ( gpbtn & GPDBTN_ZL ? USRBTN_ZL : 0 );
    btn |= ( gpbtn & GPDBTN_ZR ? USRBTN_ZR : 0 );

    btn |= ( gpbtn & GPDBTN_LS ? USRBTN_LS : 0 );
    btn |= ( gpbtn & GPDBTN_RS ? USRBTN_RS : 0 );

    btn |= ( gpbtn & GPDBTN_DPAD_UP    ? USRBTN_DPAD_UP : 0 );
    btn |= ( gpbtn & GPDBTN_DPAD_DOWN  ? USRBTN_DPAD_DOWN : 0 );
    btn |= ( gpbtn & GPDBTN_DPAD_LEFT  ? USRBTN_DPAD_LEFT : 0 );
    btn |= ( gpbtn & GPDBTN_DPAD_RIGHT ? USRBTN_DPAD_RIGHT : 0 );

    return btn;
}

bool
GamepadSetUserInput(const eGAMEPAD_NUM nbGp, INPUT_OUT* const pOutInput)
{
    if (nbGp == GAMEPAD_NONE || !GamepadValid(nbGp))
        return false;

    const USER_GAMEPAD*       const p_usrgp = &UserGamepads[nbGp];
    const GAMEPAD*            const p_gp    = &Gamepads[nbGp];

    pOutInput->down = GamepadToUserButton(p_gp->down);

    f32 x1, y1;
    f32 x2, y2;

    /** Left stick **/
    {
        f32 x = (f32) NORM_GPD_DIR( p_gp->x1 );
        f32 y = (f32) NORM_GPD_DIR( p_gp->y1 );

        const f32 idz = p_usrgp->StickL.idz;
        const f32 odz = p_usrgp->StickL.odz;

        p_usrgp->dzMode == DZ_MD_CIRCULAR ?
            CalcCircularDeadzone(&x, &y, idz, odz) :
            CalcSquareDeadzone(  &x, &y, idz, odz);

        x1 = CLAMP(x, -1.0f, 1.0f);
        y1 = CLAMP(y, -1.0f, 1.0f);
    }

    /** Right stick **/
    {
        f32 x = (f32) NORM_GPD_DIR( p_gp->x2 );
        f32 y = (f32) NORM_GPD_DIR( p_gp->y2 );

        const f32 idz = p_usrgp->StickR.idz;
        const f32 odz = p_usrgp->StickR.odz;

        p_usrgp->dzMode == DZ_MD_CIRCULAR ?
            CalcCircularDeadzone(&x, &y, idz, odz) :
            CalcSquareDeadzone(  &x, &y, idz, odz);

        x2 = CLAMP(x, -1.0f, 1.0f);
        y2 = CLAMP(y, -1.0f, 1.0f);
    }

    if (GamepadDbgAxis)
        DebugAxes(nbGp, x1, y1, x2, y2);

    pOutInput->x1 = x1;
    pOutInput->y1 = y1;

    pOutInput->x2 = x2;
    pOutInput->y2 = y2;

    pOutInput->l = (f32) NORM_GPD_TRIG( p_gp->l );
    pOutInput->r = (f32) NORM_GPD_TRIG( p_gp->r );

    return true;
}

void
GamepadUpdate(void)
{
    HandleSdlEvents();

    for (int i = 0; i < ARYLEN(Gamepads); ++i)
    {
        if (!GamepadValid(i))
            continue;

        GAMEPAD*            const p_gp    = &Gamepads[i];
        SDL_GameController* const p_sdlgc = p_gp->pSdlGp;

        p_gp->down = 0;

        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
        {
            if (SDL2_GameControllerGetButton(p_sdlgc, i))
                p_gp->down |= (1<<i);
        }

        p_gp->x1 = SDL2_GameControllerGetAxis(p_sdlgc, SDL_CONTROLLER_AXIS_LEFTX);
        p_gp->y1 = SDL2_GameControllerGetAxis(p_sdlgc, SDL_CONTROLLER_AXIS_LEFTY);
        p_gp->x2 = SDL2_GameControllerGetAxis(p_sdlgc, SDL_CONTROLLER_AXIS_RIGHTX);
        p_gp->y2 = SDL2_GameControllerGetAxis(p_sdlgc, SDL_CONTROLLER_AXIS_RIGHTY);

        p_gp->l = SDL2_GameControllerGetAxis(p_sdlgc, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
        p_gp->r = SDL2_GameControllerGetAxis(p_sdlgc, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
    }
}

void
GamepadInit(void)
{
    /** Get Gamepad sections info **/
    for (int i = 0; i < ARYLEN(UserGamepads); ++i)
    {
        char buf[8];

        snprintf(buf, sizeof(buf), "gp%i", i);

        UserGamepads[i].dzMode =     (u8)  CnfGetInt(     CNFV_GAMEPD_DZ_MODE( buf ) );
        UserGamepads[i].StickL.idz = (f32) CnfGetPercent( CNFV_GAMEPD_LS_IDZ(  buf ) );
        UserGamepads[i].StickL.odz = (f32) CnfGetPercent( CNFV_GAMEPD_LS_ODZ(  buf ) );
        UserGamepads[i].StickR.idz = (f32) CnfGetPercent( CNFV_GAMEPD_RS_IDZ(  buf ) );
        UserGamepads[i].StickR.odz = (f32) CnfGetPercent( CNFV_GAMEPD_RS_ODZ(  buf ) );
        UserGamepads[i].vibPwrL =    (f32) CnfGetPercent( CNFV_GAMEPD_VIB_L(   buf ) );
        UserGamepads[i].vibPwrR =    (f32) CnfGetPercent( CNFV_GAMEPD_VIB_R(   buf ) );
        UserGamepads[i].useVib  =          CnfGetInt(     CNFV_GAMEPD_VIB(     buf ) );
    }

    /** Get debug info **/
    GamepadDbgAxis = CnfGetInt(CNF_DEBUG_AXIS);

    for (int i = 0; i < ARYLEN(Gamepads); ++i)
    {
        GAMEPAD* const p_gp = &Gamepads[i];

        ResetGamepadStruct(p_gp);
    }
}
