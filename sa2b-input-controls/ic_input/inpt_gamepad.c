/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Mod Loader ******************************************************************/
#include <sa2b/modloader.h> /* ML_DisplayDebugStringF                               */

/****** Extern **********************************************************************/
#include <sa2b/extern/xinput.h> /* XInput                                           */

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

/****** Self ************************************************************************/
#include <ic_input/inpt_internal.h> /* internal                                     */

/************************/
/*  Constants           */
/************************/
#define XInputGetCapabilities   ___XInputGetCapabilities
#define XInputGetState          ___XInputGetState
#define XInputSetState          ___XInputSetState

#define DBG_AXIS_LEFT           (0)
#define DBG_AXIS_RIGHT          (1)

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

    uint8_t trigLimit;

    bool useVib;

    bool valid;
}
GAMEPAD;

/************************/
/*  File Variables      */
/************************/
static xinput_state XIState[4];
static GAMEPAD      Gamepad[4];

static bool         GamepadDbgAxis;

/************************/
/*  Source              */
/************************/
void*
GamepadXInputGet(const eGAMEPAD_NUM nbGp)
{
    if (nbGp == GAMEPAD_NONE)
        return nullptr;

    return &XIState[nbGp];
}

bool
GamepadGetValid(const eGAMEPAD_NUM nbGp)
{
    if (nbGp == GAMEPAD_NONE)
        return false;

    return Gamepad[nbGp].valid;
}

void
GamepadUpdate(void)
{
    for (int i = 0; i < ARYLEN(Gamepad); ++i)
    {
        Gamepad[i].valid = !XInputGetState(i, &XIState[i]);
    }
}

bool
GamepadVibSet(const eGAMEPAD_NUM nbGp, const f32 spdL, const f32 spdR)
{
    if (nbGp == GAMEPAD_NONE)
        return false;

    const GAMEPAD* const p_gp = &Gamepad[nbGp];

    if (!p_gp->useVib)
        return false;

    const f32 adj_l = p_gp->vibPwrL * spdL;
    const f32 adj_r = p_gp->vibPwrR * spdR;

    return XInputSetState(nbGp, &(xinput_vibration) {
        .spdL = (u16)nearbyint(adj_l * 65'535.0),
        .spdR = (u16)nearbyint(adj_r * 65'535.0)
    }) == 0;
}

bool
GamepadVibStop(const eGAMEPAD_NUM nbGp)
{
    if (nbGp == GAMEPAD_NONE)
        return false;

    return XInputSetState(nbGp, &(xinput_vibration) { .spdL = 0, .spdR = 0 }) == 0;
}

/****** Static **********************************************************************/
static Sint16
UserToPdsStick(f64 mag)
{
    return !mag ? 0 : (Sint16)(mag > 0.0 ?
        mag * 127.99999999999999 : //  0~127
        mag * 128.99999999999997); // -128~0
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

#define NORMALISE_XISTICK(mag)      (f32)((f32)mag/(mag>0?32767.0f:32768.0f))

/****** User Input ******************************************************************/
void
GamepadSetUserInput(const int nbGamepad, USER_INPUT* const pUserInput)
{
    const GAMEPAD*        const p_gp = &Gamepad[nbGamepad];
    const xinput_gamepad* const p_xi = &XIState[nbGamepad].Gamepad;

    pUserInput->on = p_xi->button;

    pUserInput->on |= ( p_xi->LT > p_gp->trigLimit ? USRBTN_L : 0 );
    pUserInput->on |= ( p_xi->RT > p_gp->trigLimit ? USRBTN_R : 0 );

    f32 x1, y1;
    f32 x2, y2;

    /** Left stick **/
    {
        f32 x = NORMALISE_XISTICK(p_xi->LSX);
        f32 y = NORMALISE_XISTICK(p_xi->LSY);

        const f32 idz = p_gp->StickL.idz;
        const f32 odz = p_gp->StickL.odz;

        p_gp->dzMode == DZ_MD_CIRCULAR ?
            CalcCircularDeadzone(&x, &y, idz, odz) :
            CalcSquareDeadzone(  &x, &y, idz, odz);

        x1 = CLAMP(x, -1.0f, 1.0f);
        y1 = CLAMP(y, -1.0f, 1.0f);
    }

    /** Right stick **/
    {
        f32 x = NORMALISE_XISTICK(p_xi->RSX);
        f32 y = NORMALISE_XISTICK(p_xi->RSY);

        const f32 idz = p_gp->StickR.idz;
        const f32 odz = p_gp->StickR.odz;

        p_gp->dzMode == DZ_MD_CIRCULAR ?
            CalcCircularDeadzone(&x, &y, idz, odz) :
            CalcSquareDeadzone(  &x, &y, idz, odz);

        x2 = CLAMP(x, -1.0f, 1.0f);
        y2 = CLAMP(y, -1.0f, 1.0f);
    }

    if (GamepadDbgAxis)
        DebugAxes(nbGamepad, x1, y1, x2, y2);

    pUserInput->x1 =  x1;
    pUserInput->y1 = -y1; // XInput is inverted

    pUserInput->x2 =  x2;
    pUserInput->y2 = -y2; // XInput is inverted

    pUserInput->l = (f32)p_xi->LT / 255.0f;
    pUserInput->r = (f32)p_xi->RT / 255.0f;
}

/****** Init ************************************************************************/
void
GamepadInit(void)
{
    /** Get Gamepad sections info **/
    for (int i = 0; i < ARYLEN(Gamepad); ++i)
    {
        char buf[8];

        snprintf(buf, sizeof(buf), "gp%i", i);

        Gamepad[i].dzMode =     (u8)  CnfGetInt(     CNFV_GAMEPD_DZ_MODE( buf ) );
        Gamepad[i].StickL.idz = (f32) CnfGetPercent( CNFV_GAMEPD_LS_IDZ(  buf ) );
        Gamepad[i].StickL.odz = (f32) CnfGetPercent( CNFV_GAMEPD_LS_ODZ(  buf ) );
        Gamepad[i].StickR.idz = (f32) CnfGetPercent( CNFV_GAMEPD_RS_IDZ(  buf ) );
        Gamepad[i].StickR.odz = (f32) CnfGetPercent( CNFV_GAMEPD_RS_ODZ(  buf ) );
        Gamepad[i].vibPwrL =    (f32) CnfGetPercent( CNFV_GAMEPD_VIB_L(   buf ) );
        Gamepad[i].vibPwrR =    (f32) CnfGetPercent( CNFV_GAMEPD_VIB_R(   buf ) );
        Gamepad[i].useVib  =          CnfGetInt(     CNFV_GAMEPD_VIB(     buf ) );
        Gamepad[i].trigLimit =  (u8)( CnfGetPercent( CNFV_GAMEPD_TR_LIM(  buf ) ) * 255.0 );
    }

    /** Get debug info **/
    GamepadDbgAxis = CnfGetInt(CNF_DEBUG_AXIS);
}
