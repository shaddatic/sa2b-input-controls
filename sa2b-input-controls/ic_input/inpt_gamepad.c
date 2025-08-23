/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <samt/core.h>      /* core                                                 */
#include <samt/string.h>    /* mtstrformat                                          */

/****** Mod Loader ******************************************************************/
#include <samt/modloader.h> /* ML_DisplayDebugStringF                               */

/****** Ninja ***********************************************************************/
#include <samt/ninja/ninja.h> /* ninja                                              */

/****** Game ************************************************************************/
#include <samt/sonic/debug.h> /* OutputFormat                                       */

/****** Simple DirectMedia Layer ****************************************************/
#include <SDL2/SDL.h>       /* core                                                 */

/****** Std *************************************************************************/
#include <stdio.h>          /* snprintf                                             */
#include <math.h>           /* nearintf                                             */

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_sdl2.h>        /* ICSDL_RegisterEventHandler                           */

/****** Config **********************************************************************/
#include <cnf.h>            /* CnfGet##                                             */

/****** Self ************************************************************************/
#include <ic_input/inpt_internal.h> /* parent                                       */

/************************/
/*  Constants           */
/************************/
/****** SDL GameController Index ****************************************************/
#define GPD_SDLIDX_NONE     (-1) /* gamepad slot is not linked to an open gamepad   */

/************************/
/*  Macros              */
/************************/
/****** Normalize Gamepad Analogs ***************************************************/
#define NORM_GPD_DIR(mag)   ((f64)mag/(mag>0?(f64)GPDLIM_XY_MAX:(f64)-GPDLIM_XY_MIN))
#define NORM_GPD_TRIG(mag)  ((f64)mag/(f64)(SDL_JOYSTICK_AXIS_MAX))

/************************/
/*  Enums               */
/************************/
/****** Deadzone Modes **************************************************************/
typedef enum
{
    DZ_MD_CIRCULAR,         /* use circular deadzone calculations                   */
    DZ_MD_SQUARE,           /* use square deadzone calculations                     */
}
eDZ_MODE;

/************************/
/*  Structures          */
/************************/
/****** Deadzone Settings ***********************************************************/
typedef struct
{
    f32 idz;                /* inner deadzone                                       */
    f32 odz;                /* outer deadzone                                       */
}
DEADZONE;

/****** Gamepad Settings ************************************************************/
typedef struct
{
    DEADZONE StickL;        /* left stick deadzone                                  */
    DEADZONE StickR;        /* right stick deadzone                                 */
    f32      vibStr;        /* vibration strength                                   */
    u8       dzMode;        /* deadzone mode                                        */
}
GAMEPAD_SETTINGS;

/************************/
/*  File Data           */
/************************/
/****** Gamepad Settings ************************************************************/
static GAMEPAD_SETTINGS GpSettings[4]; /* gamepad settings                          */

/****** Gamepad Slots ***************************************************************/
static IC_GAMEPAD Gamepads[4]; /* physical gamepad data                             */

/****** Debug ***********************************************************************/
static bool GamepadDbgAxis; /* display gamepad debug menu                           */

/************************/
/*  Source              */
/************************/
/****** Static **********************************************************************/
static void
GetGamepadInfo(SDL_GameController* pSdlGp, char* pcOutInfo, usize szOutInfo)
{
    /** There's probably an easier way to get the GUID, but this works just fine lol **/

    pcOutInfo[0] = '\0';

    char* const pc_guid = SDL_GameControllerMapping(pSdlGp);

    for ( char* pc_scan = pc_guid; ; ++pc_scan )
    {
        /** Scan for the first comma, and end the string there. That will give us
            the GUID of the controller for printing. **/
        if ( *pc_scan == ',' )
        {
            *pc_scan = '\0';
            break;
        }
    }

    mtStrFormat(pcOutInfo, szOutInfo, "GUID(%s), NAME(%s)", pc_guid, SDL_GameControllerName(pSdlGp));

    SDL_free(pc_guid);
}

static void
ResetGamepadStruct(IC_GAMEPAD* const pGp)
{
    *pGp = (IC_GAMEPAD){ .id = GPD_SDLIDX_NONE };
}

static void
OpenGamepad(const int joy)
{
    if ( !SDL_IsGameController(joy) )
    {
        return;
    }

    for (int i = 0; i < ARYLEN(Gamepads); ++i)
    {
        const char* pc_opendebug;

        IC_GAMEPAD* const p_gp = &Gamepads[i];

        /** If ID matches, reset device **/
        if ( p_gp->id == joy )
        {
            SDL_GameControllerClose(p_gp->pgp);
            ResetGamepadStruct(p_gp);

            pc_opendebug = "IC INFO: Controller reset [%s]";
            goto OPEN;
        }

        if ( !p_gp->pgp )
        {
            pc_opendebug = "IC INFO: Controller connected [%s]";

        OPEN:
            SDL_GameController* const p_sdlgc = SDL_GameControllerOpen(joy);

            if (!p_sdlgc)
            {
                break;
            }

            // debug print
            {
                char c_buf[128];
                GetGamepadInfo(p_sdlgc, c_buf, ARYLEN(c_buf));

                OutputFormat( pc_opendebug, c_buf );
            }

            p_gp->pgp = p_sdlgc;
            p_gp->id  = joy;

            p_gp->name = SDL_GameControllerName(p_sdlgc);

            for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; ++i)
            {
                if (SDL_GameControllerHasButton(p_sdlgc, i))
                {
                    p_gp->support |= (1<<i);
                }
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

        if ( p_gp->id == joy )
        {
            // debug print
            {
                char c_buf[128];
                GetGamepadInfo(p_gp->pgp, c_buf, ARYLEN(c_buf));

                OutputFormat( "IC INFO: Controller disconnected [%s]", c_buf );
            }

            SDL_GameControllerClose(p_gp->pgp);
            ResetGamepadStruct(p_gp);
        }
    }
}

static void
DebugAxes(const int nbGp, const f32 x1, const f32 y1, const f32 x2, const f32 y2)
{
    mlDebugSetColor(0xFFFFFFFF);
    mlDebugSetScale(8);

    const f32 m1 = sqrtf((x1*x1)+(y1*y1));
    const f32 m2 = sqrtf((x2*x2)+(y2*y2));

    mlDebugPrint(NJM_LOCATION(1, (nbGp*3)+1), "GAMEPAD %i: %s", nbGp, Gamepads[nbGp].name);

    mlDebugPrint(NJM_LOCATION(2, (nbGp*3)+2), "LS: X %+f, Y %+f, M %+f", x1, y1, m1);
    mlDebugPrint(NJM_LOCATION(2, (nbGp*3)+3), "RS: X %+f, Y %+f, M %+f", x2, y2, m2);
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
    if ( idz <= 0.f && odz >= 1.f )
    {
        return;
    }

    *pX = (f32) CalcLinearDeadzone(*pX, idz, odz);
    *pY = (f32) CalcLinearDeadzone(*pY, idz, odz);
}

static void
CalcCircularDeadzone(f32* const pX, f32* const pY, const f32 idz, const f32 odz)
{
    if ( idz <= 0.f && odz >= 1.f )
    {
        return;
    }

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

/****** Extern **********************************************************************/
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
    const GAMEPAD_SETTINGS* const p_usr = &GpSettings[nbGp];

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
    const GAMEPAD_SETTINGS* const p_usr = &GpSettings[nbGp];

    const f32 str = p_usr->vibStr;

    const Sint16 l = (Sint16)( ( CLAMP(spdL, 0.f, 1.f) * 65535.f ) * str );
    const Sint16 r = (Sint16)( ( CLAMP(spdR, 0.f, 1.f) * 65535.f ) * str );

    return SDL_GameControllerRumbleTriggers(p_gpd->pgp, l, r, 0xFFFFFFFF);
}

bool
GamepadSetUserInput(const eIC_GAMEPAD_NUM nbGp, INPUT_OUT* const pOutInput)
{
    if (nbGp == IC_GAMEPAD_NONE || !GamepadValid(nbGp))
        return false;

    const GAMEPAD_SETTINGS* const p_usrgp = &GpSettings[nbGp];
    const IC_GAMEPAD*       const p_gp    = &Gamepads[nbGp];

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
GamepadInputPoll(void)
{
    return; // polling is done inside of SDL_GetEvent
}

void
GamepadInputExec(void)
{
    for ( int ix_gp = 0; ix_gp < ARYLEN(Gamepads); ++ix_gp )
    {
        if ( !GamepadValid(ix_gp) )
        {
            continue;
        }

        IC_GAMEPAD*         const p_gp    = &Gamepads[ix_gp];
        SDL_GameController* const p_sdlgc = p_gp->pgp;

        const uint32_t old_down = p_gp->down;

        uint32_t new_down = 0;

        for ( int ix_btn = 0; ix_btn < SDL_CONTROLLER_BUTTON_MAX; ++ix_btn )
        {
            if ( SDL_GameControllerGetButton(p_sdlgc, ix_btn) )
            {
                new_down |= (1<<ix_btn);
            }
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

/****** Event Handler ***************************************************************/
SASDL_EVRET
GamepadEventHandler(const SDL_Event* pEvent)
{
    switch ( pEvent->type )
    {
        case SDL_JOYDEVICEADDED:
        {
            OpenGamepad(pEvent->cdevice.which);
            return SASDL_EVRET_BREAK;
        }
        case SDL_JOYDEVICEREMOVED:
        {
            CloseGamepad(pEvent->cdevice.which);
            return SASDL_EVRET_BREAK;
        }
    }

    return SASDL_EVRET_CONTINUE;
}

/****** Init ************************************************************************/
void
GamepadInit(void)
{
    /** Get Gamepad sections info **/
    for ( int ix_gp = 0; ix_gp < ARYLEN(GpSettings); ++ix_gp )
    {
        c8 ubuf[8];

        snprintf(ubuf, ARYLEN(ubuf), "gp%i", ix_gp);

        GpSettings[ix_gp].dzMode =     (u8)  CNF_GetInt(     CNFV_GAMEPD_DZ_MODE( ubuf ) );
        GpSettings[ix_gp].StickL.idz = (f32) CNF_GetPercent( CNFV_GAMEPD_LS_IDZ(  ubuf ) );
        GpSettings[ix_gp].StickL.odz = (f32) CNF_GetPercent( CNFV_GAMEPD_LS_ODZ(  ubuf ) );
        GpSettings[ix_gp].StickR.idz = (f32) CNF_GetPercent( CNFV_GAMEPD_RS_IDZ(  ubuf ) );
        GpSettings[ix_gp].StickR.odz = (f32) CNF_GetPercent( CNFV_GAMEPD_RS_ODZ(  ubuf ) );
        GpSettings[ix_gp].vibStr  =    (f32) CNF_GetPercent( CNFV_GAMEPD_VIB_STR( ubuf ) );
    }

    /** Get debug info **/
    GamepadDbgAxis = CNF_GetInt(CNF_DEBUG_AXIS);

    for ( int ix_gp = 0; ix_gp < ARYLEN(Gamepads); ++ix_gp )
    {
        IC_GAMEPAD* const p_gp = &Gamepads[ix_gp];

        ResetGamepadStruct(p_gp);
    }

    ICSDL_RegisterEventHandler( GamepadEventHandler );
}
