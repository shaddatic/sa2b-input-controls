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

/****** Simple DirectMedia Layer ****************************************************/
#include <SDL2/SDL.h>       /* core                                                 */

/****** Std *************************************************************************/
#include <stdio.h>  /* snprintf                                                     */
#include <math.h>   /* nearintf                                                     */

/****** Input Controls **************************************************************/
#include <ic_core.h>    /* core                                                     */
#include <ic_config.h>  /* CnfGet___                                                */
#include <ic_sdl2.h>    /* ICSDL_RegisterEventHandler                               */

/****** Self ************************************************************************/
#include <ic_input/inpt_internal.h> /* internal                                     */

/************************/
/*  Constants           */
/************************/
#define GPD_SDLIDX_NONE     (-1)

/************************/
/*  File Macro          */
/************************/
#define NORM_GPD_DIR(mag)   ((f64)mag/(mag>0?(f64)GPDLIM_XY_MAX:(f64)-GPDLIM_XY_MIN))
#define NORM_GPD_TRIG(mag)  ((f64)mag/(f64)(SDL_JOYSTICK_AXIS_MAX))

/************************/
/*  Enums               */
/************************/
typedef enum
{
    DZ_MD_CIRCULAR,
    DZ_MD_SQUARE,
}
eDZ_MODE;

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
    f32      vibStr;
    u8       dzMode;
}
USER_GAMEPAD;

/************************/
/*  File Data           */
/************************/
static USER_GAMEPAD UserGamepads[4]; /* user gamepad settings                        */

static IC_GAMEPAD Gamepads[4];          /* physical gamepad data                        */

static bool GamepadDbgAxis;

/************************/
/*  Source              */
/************************/
static void
ResetGamepadStruct(IC_GAMEPAD* const pGp)
{
    *pGp = (IC_GAMEPAD){ .id = GPD_SDLIDX_NONE };
}

static void
OpenGamepad(const int joy)
{
    if (!SDL_IsGameController(joy))
        return;

    for (int i = 0; i < ARYLEN(Gamepads); ++i)
    {
        IC_GAMEPAD* const p_gp = &Gamepads[i];

        /** If ID matches, reset device **/
        if (p_gp->id == joy)
        {
            SDL_GameControllerClose(p_gp->pgp);
            ResetGamepadStruct(p_gp);
            goto OPEN;
        }

        if (!p_gp->pgp)
        {
        OPEN:
            SDL_GameController* const p_sdlgc = SDL_GameControllerOpen(joy);

            if (!p_sdlgc)
                break;

            p_gp->pgp = p_sdlgc;
            p_gp->id  = joy;

            p_gp->name = SDL_GameControllerName(p_sdlgc);

            for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
            {
                if (SDL_GameControllerHasButton(p_sdlgc, i))
                    p_gp->support |= (1<<i);
            }

            p_gp->support |= SDL_GameControllerHasRumbleTriggers(p_sdlgc) ? GPDDEV_SUPPORT_RUMBLE_TRIGGER : 0;
            p_gp->support |= SDL_GameControllerHasRumble(p_sdlgc)         ? GPDDEV_SUPPORT_RUMBLE         : 0;
            break;
        }
    }
}

static void
CloseGamepad(const int joy)
{
    for (int i = 0; i < ARYLEN(Gamepads); ++i)
    {
        IC_GAMEPAD* const p_gp = &Gamepads[i];

        if (p_gp->id == joy)
        {
            SDL_GameControllerClose(p_gp->pgp);
            ResetGamepadStruct(p_gp);
        }
    }
}

static void
GamepadEventHandler(const SDL_Event* pEvent)
{
    switch (pEvent->type) {
    case SDL_JOYDEVICEADDED:
        OpenGamepad(pEvent->cdevice.which);
        break;

    case SDL_JOYDEVICEREMOVED:
        CloseGamepad(pEvent->cdevice.which);
        break;
    }
}

const IC_GAMEPAD*
GamepadGetGamepad(const eIC_GAMEPAD_NUM nbGp)
{
    if (nbGp == IC_GAMEPAD_NONE)
        return false;

    return &Gamepads[nbGp];
}

bool
GamepadValid(const eIC_GAMEPAD_NUM nbGp)
{
    if (nbGp == IC_GAMEPAD_NONE)
        return false;

    return Gamepads[nbGp].pgp;
}

bool
GamepadSetVibration(const eIC_GAMEPAD_NUM nbGp, const f32 spdLo, const f32 spdHi)
{
    if (nbGp == IC_GAMEPAD_NONE)
        return false;

    const IC_GAMEPAD*   const p_gpd = &Gamepads[nbGp];
    const USER_GAMEPAD* const p_usr = &UserGamepads[nbGp];

    const f32 str = p_usr->vibStr;

    const Sint16 lo = (Sint16)( ( CLAMP(spdLo, 0.f, 1.f) * 65535.f ) * str );
    const Sint16 hi = (Sint16)( ( CLAMP(spdHi, 0.f, 1.f) * 65535.f ) * str );

    return SDL_GameControllerRumble(p_gpd->pgp, lo, hi, 0xFFFFFFFF);
}

bool
GamepadSetTriggerVibration(const eIC_GAMEPAD_NUM nbGp, const f32 spdL, const f32 spdR)
{
    if (nbGp == IC_GAMEPAD_NONE)
        return false;

    const IC_GAMEPAD*   const p_gpd = &Gamepads[nbGp];
    const USER_GAMEPAD* const p_usr = &UserGamepads[nbGp];

    const f32 str = p_usr->vibStr;

    const Sint16 l = (Sint16)( ( CLAMP(spdL, 0.f, 1.f) * 65535.f ) * str );
    const Sint16 r = (Sint16)( ( CLAMP(spdR, 0.f, 1.f) * 65535.f ) * str );

    return SDL_GameControllerRumbleTriggers(p_gpd->pgp, l, r, 0xFFFFFFFF);
}

static void
DebugAxes(const int nbGp, const f32 x1, const f32 y1, const f32 x2, const f32 y2)
{
    char buf[64];

    ML_SetDebugFontColor(0xFFFFFFFF);
    ML_SetDebugFontScale(12.f);

    const f32 m1 = sqrtf((x1*x1)+(y1*y1));
    const f32 m2 = sqrtf((x2*x2)+(y2*y2));

    ML_DisplayDebugStringF(NJM_LOCATION(1, (nbGp*3)+1), buf, 64, "GAMEPAD %i: %s", nbGp, Gamepads[nbGp].name);

    ML_DisplayDebugStringF(NJM_LOCATION(2, (nbGp*3)+2), buf, 64, "LS: X %+f, Y %+f, M %+f", x1, y1, m1);
    ML_DisplayDebugStringF(NJM_LOCATION(2, (nbGp*3)+3), buf, 64, "RS: X %+f, Y %+f, M %+f", x2, y2, m2);
}

static f64
CalcLinearDeadzone(const f32 input, const f32 idz, const f32 odz)
{
    f64 out = input;

    if (fabs(out) >= idz)
    {
        out -= out > 0.0 ? idz : -idz;

        out *= (1.0 / (odz - idz));
    }
    else
        out = 0.0;

    return out;
}

static void
CalcSquareDeadzone(f32* const pX, f32* const pY, const f32 idz, const f32 odz)
{
    *pX = (f32) CalcLinearDeadzone(*pX, idz, odz);
    *pY = (f32) CalcLinearDeadzone(*pY, idz, odz);
}

static void
CalcCircularDeadzone(f32* const pX, f32* const pY, const f32 idz, const f32 odz)
{
    const f32 x = *pX;
    const f32 y = *pY;

    const f64 dz_mag = CalcLinearDeadzone( sqrtf((x*x) + (y*y)), idz, odz );

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
GamepadSetUserInput(const eIC_GAMEPAD_NUM nbGp, INPUT_OUT* const pOutInput)
{
    if (nbGp == IC_GAMEPAD_NONE || !GamepadValid(nbGp))
        return false;

    const USER_GAMEPAD* const p_usrgp = &UserGamepads[nbGp];
    const IC_GAMEPAD*   const p_gp    = &Gamepads[nbGp];

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
    for (int i = 0; i < ARYLEN(Gamepads); ++i)
    {
        if (!GamepadValid(i))
            continue;

        IC_GAMEPAD*         const p_gp    = &Gamepads[i];
        SDL_GameController* const p_sdlgc = p_gp->pgp;

        const uint32_t old_down = p_gp->down;

        uint32_t new_down = 0;

        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
        {
            if (SDL_GameControllerGetButton(p_sdlgc, i))
                new_down |= (1<<i);
        }

        p_gp->down = new_down;

        p_gp->press   = new_down & ~old_down;
        p_gp->release = old_down & ~new_down;

        p_gp->x1 = SDL_GameControllerGetAxis(p_sdlgc, SDL_CONTROLLER_AXIS_LEFTX);
        p_gp->y1 = SDL_GameControllerGetAxis(p_sdlgc, SDL_CONTROLLER_AXIS_LEFTY);
        p_gp->x2 = SDL_GameControllerGetAxis(p_sdlgc, SDL_CONTROLLER_AXIS_RIGHTX);
        p_gp->y2 = SDL_GameControllerGetAxis(p_sdlgc, SDL_CONTROLLER_AXIS_RIGHTY);

        p_gp->l = SDL_GameControllerGetAxis(p_sdlgc, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
        p_gp->r = SDL_GameControllerGetAxis(p_sdlgc, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
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
        UserGamepads[i].vibStr  =    (f32) CnfGetPercent( CNFV_GAMEPD_VIB_STR( buf ) );
    }

    /** Get debug info **/
    GamepadDbgAxis = CnfGetInt(CNF_DEBUG_AXIS);

    for (int i = 0; i < ARYLEN(Gamepads); ++i)
    {
        IC_GAMEPAD* const p_gp = &Gamepads[i];

        ResetGamepadStruct(p_gp);
    }

    ICSDL_RegisterEventHandler( GamepadEventHandler );
}
