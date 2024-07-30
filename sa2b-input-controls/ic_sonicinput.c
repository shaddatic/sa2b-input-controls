/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */
#include <sa2b/writeop.h>   /* WriteJump                                            */

/****** System **********************************************************************/
#include <sa2b/shinobi/sg_pad.h>

/****** Ninja ***********************************************************************/
#include <sa2b/ninja/ninja.h>

/****** Game ************************************************************************/
#include <sa2b/sonic/input.h>
#include <sa2b/sonic/camera.h>

/****** Std *************************************************************************/
#include <math.h>

/****** Input Controls **************************************************************/
#include <ic_core.h>
#include <ic_config.h>
#include <ic_input.h>
#include <ic_feature.h> /*  */

/****** Self ************************************************************************/
#include <ic_sonicinput.h>  /* self                                                 */

/************************/
/*  Constants           */
/************************/
/****** Sonic Deadzone **************************************************************/
/** Convert PDS_PERIPHERAL analog unit (-128~127) to raw analog unit (-1~1) **/
#define PDS_TO_RAW(a)           ((f64)(a)/127.0)

#define SONIC_DZ                PDS_TO_RAW(12)
#define SONIC_DZ_ADJ            (1.0/(1.0-SONIC_DZ))

/************************/
/*  Game Data           */
/************************/
#define AnalogThrust        DATA_ARY(bool, 0x01DEFDA0, [8])

/************************/
/*  File Variables      */
/************************/
static bool DreamcastMode;

/************************/
/*  Source              */
/************************/
static void
CalcDreamcastDeadzone(f32* const pX, f32* const pY)
{
    /** This is supposed to be bad code, before you wonder. This is just how
        Dreamcast was lol, and a similar bug was in SA1 **/

    f64 x = *pX;
    f64 y = *pY;

    if (x > SONIC_DZ || x < -SONIC_DZ || y > SONIC_DZ || y < -SONIC_DZ)
    {
        x += x < 0.0 ? SONIC_DZ : -SONIC_DZ;
        y += y < 0.0 ? SONIC_DZ : -SONIC_DZ;

        x *= SONIC_DZ_ADJ; // Make sure it still adds up to 1
        y *= SONIC_DZ_ADJ;
    }
    else
        x = y = 0.f;

    *pX = (f32)x;
    *pY = (f32)y;
}

static void
GetUserRawAnalog(const eUSER_NUM nbUser, f32* const pX, f32* const pY)
{
    const IC_USER* const p_user = UserGetUser(nbUser);

    *pX = p_user->x1;
    *pY = p_user->y1;
}

static void
NewGetPlayersInputData(void)
{
    for (int i = 0; i < ARYLEN(input_dataG); ++i)
    {
        SONIC_INPUT* const p_sin = &input_dataG[i];

        const f32 last_stroke = p_sin->stroke;

        const bool input_status = (i > 1 || ucInputStatusForEachPlayer[i] == 1);

        if (ucInputStatus == 1 && input_status)
        {
            f32 x1, y1;

            if (ICF_UseRawAnalog() && i < NB_USER)
            {
                GetUserRawAnalog(i, &x1, &y1);
            }
            else
            {
                x1 = NORM_PDS_DIR(perG[i].x1);
                y1 = NORM_PDS_DIR(perG[i].y1);
            }

            if (DreamcastMode)
                CalcDreamcastDeadzone(&x1, &y1);

            Angle angy = njArcTan2(y1, x1);

            if (camera_twp) // Rotate to camera
            {
                if (i >= cameraCount)
                    angy -= cameraControlWork[0]->ang.y;
                else
                    angy -= cameraControlWork[i]->ang.y;
            }

            const f64 xy2 = (x1*x1)+(y1*y1);
            const f64 mag = sqrt(xy2) * xy2; // add a sensitivity ramp

            p_sin->stroke = (f32) MIN(mag, 1.0);
            p_sin->angle  = angy;
        }
        else
            *p_sin = (SONIC_INPUT){0};

        AnalogThrust[i] = p_sin->stroke >= 0.5f && last_stroke <= 0.5f;
    }
}


static void
NewGetPlayersInputData_FH(void)
{
    /** Same as the above function, but player 2's input_data is set by player 1's
        raw USER_INPUT controller. This is done specifically for the Final Hazard
        boss fight, as Shadow is considered player 2 **/

    for (int i = 0; i < ARYLEN(input_dataG); ++i)
    {
        SONIC_INPUT* const p_sin  = &input_dataG[i];

        const f32 last_stroke = p_sin->stroke;

        const bool input_status = (i > 1 || ucInputStatusForEachPlayer[i] == 1);

        if (ucInputStatus == 1 && input_status)
        {
            f32 x1, y1;

            if (ICF_UseRawAnalog() && i < NB_USER)
            {
                GetUserRawAnalog((i < 2) ? 0 : i, &x1, &y1);
            }
            else
            {
                x1 = NORM_PDS_DIR(perG[i].x1);
                y1 = NORM_PDS_DIR(perG[i].y1);
            }

            if (DreamcastMode)
                CalcDreamcastDeadzone(&x1, &y1);

            Angle angy = njArcTan2(y1, x1);

            if (camera_twp) // Rotate to camera
            {
                if (i >= cameraCount)
                    angy -= cameraControlWork[0]->ang.y;
                else
                    angy -= cameraControlWork[i]->ang.y;
            }

            const f64 xy2 = (x1*x1)+(y1*y1);
            const f64 mag = sqrt(xy2) * xy2; // add a sensitivity ramp

            p_sin->stroke = (f32) MIN(mag, 1.0);
            p_sin->angle  = angy;
        }
        else
            *p_sin = (SONIC_INPUT){0};

        AnalogThrust[i] = p_sin->stroke >= 0.5f && last_stroke <= 0.5f;
    }
}

void
IC_SonicInputInit(void)
{
    DreamcastMode = CnfGetInt(CNF_MISC_DCDRIFT);

    WriteJump(0x00441EF0, NewGetPlayersInputData);

    /** FinalHazard re-call for 1P->2P control **/
    WriteCall(0x0049E580, NewGetPlayersInputData_FH);
}
