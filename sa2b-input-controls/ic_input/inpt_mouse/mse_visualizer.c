/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Ninja ***********************************************************************/
#include <sa2b/ninja/ninja.h> /* ninja                                              */

/****** Core Toolkit ****************************************************************/
#include <sa2b/sonic/task.h> /* CreateElementalTask                                 */

/****** Core Toolkit ****************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_config.h>      /* CNF_Get##                                            */

/****** Self ************************************************************************/
#include <ic_input/inpt_mouse/mse_internal.h> /* parent                             */

/************************/
/*  Constants           */
/************************/
/****** Fade Timer ******************************************************************/
#define VISUTASK_TIMERMAX           (30) /* visualizer fade time in frames          */

/****** Create Task Wait Timer ******************************************************/
#define VISUTASK_WAITTIMER          (10) /* visualizer create wait timer            */

/****** Size Constants **************************************************************/
#define VISUTASK_POINT_SIZE         (1.5f)  /* point mode point size                */
#define VISUTASK_POINT_DISTANCE     (16.0f) /* point mode max distance              */
#define VISUTASK_POINT_LINESIZE     (0.4f)  /* point mode line thickness            */

/************************/
/*  Enums               */
/************************/
/****** Size Constants **************************************************************/
typedef enum
{
    VISU_MD_DISABLED,       /* visualizer disabled                                  */
    VISU_MD_POINT,          /* use point visualizer                                 */
    VISU_MD_ARROW,          /* use arrow visulizer                                  */
}
eVISU_MODE;

/************************/
/*  File Variables      */
/************************/
/****** User-Settings ***************************************************************/
static eVISU_MODE MouseVisuMode;  /* visualizer mode                                */
static f32        MouseVisuScale; /* visualizer scale                               */
static f32        MouseVisuAlpha; /* visualizer alpha                               */

/****** Task Variables **************************************************************/
static TASK* MouseVisuTaskP;     /* task pointer                                    */
static s32   MouseVisuTaskTimer; /* task display timer                              */

/****** Cursor Position *************************************************************/
static NJS_POINT2 VisualizerPos; /* position of the cursor                          */

/****** Create Task Wait Timer ******************************************************/
static s32 MouseVisuWaitTimer; /* visualizer create wait timer                      */

/************************/
/*  Source              */
/************************/
/****** Static **********************************************************************/
static void
MouseVisuTaskDestructor(TASK* const tp)
{
    MouseVisuWaitTimer = VISUTASK_WAITTIMER;

    MouseVisuTaskP = nullptr;
}

static void
TranslateAndScalePoints(NJS_POINT2* const pPts)
{
    for (int i = 0; i < 4; ++i)
    {
        pPts[i].x *= MouseVisuScale;
        pPts[i].y *= MouseVisuScale;

        pPts[i].x += 320.f;
        pPts[i].y += 240.f;
    }
}

static void
PutPoints(NJS_POINT2* const pPtsDst, const NJS_POINT2* const pPtsSrc)
{
    for (int i = 0; i < 4; ++i)
    {
        pPtsDst[i] = pPtsSrc[i];
    }
}

static void
CalcPoints(NJS_POINT2* const pPts)
{
    for (int i = 0; i < 4; ++i)
    {
        NJS_POINT3 calc = { .x = pPts[i].x, .y = pPts[i].y, .z = 0.f };

        njCalcPoint(NULL, &calc, &calc);

        pPts[i].x = calc.x;
        pPts[i].y = calc.y;
    }
}

static void
PointDisplayer(const Angle ang, const f32 mag, const f32 alpha)
{
    static const NJS_POINT2 s_LinePts[4] = {
        { +VISUTASK_POINT_LINESIZE, +1.0f },
        { +VISUTASK_POINT_LINESIZE, -0.0f },
        { -VISUTASK_POINT_LINESIZE, +1.0f },
        { -VISUTASK_POINT_LINESIZE, -0.0f }
    };

    static const NJS_POINT2 s_PointPts[4] = {
        { +VISUTASK_POINT_SIZE, +VISUTASK_POINT_SIZE },
        { +VISUTASK_POINT_SIZE, -VISUTASK_POINT_SIZE },
        { -VISUTASK_POINT_SIZE, +VISUTASK_POINT_SIZE },
        { -VISUTASK_POINT_SIZE, -VISUTASK_POINT_SIZE }
    };

    njPushMatrixEx();

    const f32 mag_inv = 1.f - mag;

    const u8 alpha_8 = (u8)(0xC0 * alpha);
    const u8 inv_col = (u8)(0xFF * mag_inv);
    const u8 mag_col = (u8)(0xFF * mag);

    const u8 bool_col = (u8)(mag_inv ? 0xFF : 0x00);
    const u8 bool_col_inv = (u8)(bool_col ? 0x00 : 0xFF);

    {
        njUnitMatrix(NULL);
        njRotateZ(NULL, -ang);

        NJS_POINT2 pt2[4];

        PutPoints(pt2, s_LinePts);

        pt2[0].y = (mag) * VISUTASK_POINT_DISTANCE;
        pt2[2].y = (mag) * VISUTASK_POINT_DISTANCE;

        CalcPoints(pt2);

        TranslateAndScalePoints(pt2);

        NJS_COLOR col[4] = {
            { .argb = { .a = alpha_8, .r = 0xFF, .g = inv_col, .b = inv_col } },
            { .argb = { .a = alpha_8, .r = 0xFF, .g = bool_col, .b = bool_col } },
            { .argb = { .a = alpha_8, .r = 0xFF, .g = inv_col, .b = inv_col } },
            { .argb = { .a = alpha_8, .r = 0xFF, .g = bool_col, .b = bool_col } },
        };

        NJS_POINT2COL poly = {
            .p = pt2,
            .col = col,
            .tex = nullptr,
            .num = 4,
        };

        njDrawPolygon2D(&poly, 4, -1.f, NJD_FILL | NJD_TRANSPARENT);
    }

    {
        NJS_POINT2 pt2[4];

        PutPoints(pt2, s_PointPts);

        for (int i = 0; i < ARYLEN(pt2); ++i)
        {
            pt2[i].x += (f32)VisualizerPos.x * VISUTASK_POINT_DISTANCE;
            pt2[i].y += (f32)VisualizerPos.y * VISUTASK_POINT_DISTANCE;
        }

        TranslateAndScalePoints(pt2);

        NJS_COLOR col[4] = {
            { .argb = { .a = alpha_8, .r = 0xFF, .g = 0x00, .b = 0x00 } },
            { .argb = { .a = alpha_8, .r = bool_col_inv, .g = bool_col, .b = 0x00 } },
            { .argb = { .a = alpha_8, .r = bool_col_inv, .g = bool_col, .b = bool_col } },
            { .argb = { .a = alpha_8, .r = 0xFF, .g = 0x00, .b = bool_col } },
        };

        NJS_POINT2COL poly = {
            .p = pt2,
            .col = col,
            .tex = nullptr,
            .num = 4,
        };

        njDrawPolygon2D(&poly, 4, -1.f, NJD_FILL | NJD_TRANSPARENT);
    }

    njPopMatrixEx();
}

static void
ArrowDisplayer(const Angle ang, const f32 mag, const f32 alpha)
{
    static const NJS_POINT2 s_ArrowPts[4] = {
        { +5.f, -5.f },
        { +0.f, -3.f },
        { +0.f, +8.f },
        { -5.f, -5.f },
    };

    njPushMatrixEx();

    {
        njUnitMatrix(NULL);
        njRotateZ(NULL, -ang);

        NJS_POINT2 pt2[4];

        PutPoints(pt2, s_ArrowPts);
        CalcPoints(pt2);

        const f32 mag_inv = 1.0f - mag;

        const u8 alpha_8 = (u8)(0xC0 * alpha);
        const u8 inv_col = (u8)(0xFF * mag_inv);
        const u8 mag_col = (u8)(0xFF * mag);

        const u8 bool_col = (u8)(mag_inv ? 0xFF : 0x00);
        const u8 bool_col_inv = (u8)(bool_col ? 0x00 : 0xFF);

        NJS_COLOR col[4] = {
            { .argb = { .a = alpha_8, .r = bool_col_inv, .g = bool_col, .b = 0x00 } },
            { .argb = { .a = alpha_8, .r = mag_col, .g = inv_col, .b = inv_col } },
            { .argb = { .a = alpha_8, .r = 0xFF, .g = 0x00, .b = 0x00 } },
            { .argb = { .a = alpha_8, .r = bool_col_inv, .g = 0x00, .b = bool_col } },
        };

        NJS_POINT2COL poly = {
            .p = pt2,
            .col = col,
            .tex = nullptr,
            .num = 4,
        };

        TranslateAndScalePoints(pt2);

        njDrawPolygon2D(&poly, 4, -1.f, NJD_FILL | NJD_TRANSPARENT);
    }

    njPopMatrixEx();
}

static void
MouseVisuTaskDisplayer(TASK* const tp)
{
    if (!MouseVisuTaskTimer)
        return;

    const f32 alpha = ((f32)MouseVisuTaskTimer / (f32)VISUTASK_TIMERMAX) * MouseVisuAlpha;

    const f32 x = VisualizerPos.x;
    const f32 y = VisualizerPos.y;

    const Angle ang = njArcTan2(x, y);

    const f32 mag_raw = sqrtf((x*x)+(y*y));

    const f32 mag = CLAMP(mag_raw, 0.f, 1.f);

    switch (MouseVisuMode) {
    case VISU_MD_POINT:
        PointDisplayer(ang, mag, alpha);
        break;

    case VISU_MD_ARROW:
        ArrowDisplayer(ang, mag, alpha);
        break;
    }
}

static void
MouseVisuTaskExecutor(TASK* const tp)
{
    if (MouseVisuTaskTimer > 0)
        --MouseVisuTaskTimer;
}

static void
MouseVisuTaskCreate(void)
{
    MouseVisuTaskTimer = VISUTASK_TIMERMAX;

    if (MouseVisuTaskP || MouseVisuMode == VISU_MD_DISABLED)
        return;

    TASK* const tp = CreateElementalTask(TELE_NUL, LEV_1, MouseVisuTaskExecutor, "MouseVisuTaskExecutor");

    tp->dest      = MouseVisuTaskDestructor;
    tp->disp_last = MouseVisuTaskDisplayer;

    MouseVisuTaskP = tp;
}

/****** Extern **********************************************************************/
void
MouseSetVisualizerInfo(const f32 x, const f32 y)
{
    /** If we don't do this, the visualizer task can get stuck in the first couple
        frames of the game. I wish there was a better way to handle this, but this
        is fine. **/
    if (MouseVisuWaitTimer != 0)
    {
        --MouseVisuWaitTimer;
        return;
    }

    if (!x && !y)
        return;

    VisualizerPos.x = x;
    VisualizerPos.y = y;

    MouseVisuTaskCreate();
}

/****** Init ************************************************************************/
void
MouseVisualizerInit(void)
{
    MouseVisuMode =        CnfGetInt(     CNF_EMUANALOG_VISU_MODE  );
    MouseVisuAlpha = (f32) CnfGetPercent( CNF_EMUANALOG_VISU_ALPHA );
    MouseVisuScale = (f32) CnfGetFloat(   CNF_EMUANALOG_VISU_SCALE );

    MouseVisuWaitTimer = VISUTASK_WAITTIMER;
}
