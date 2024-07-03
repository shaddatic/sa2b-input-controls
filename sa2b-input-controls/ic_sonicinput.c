/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */
#include <sa2b/memutil.h>   /* WriteJump */

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
#include <ic_input.h>

/************************/
/*  Game Data           */
/************************/
#define AnalogThrust        DataAry(bool, 0x01DEFDA0, [8])

/************************/
/*  Source              */
/************************/
static void
NewGetPlayersInputData(void)
{
    for (int i = 0; i < ARYLEN(input_dataG); ++i)
    {
        SONIC_INPUT* const p_sin  = &input_dataG[i];

        const f32 last_stroke = p_sin->stroke;

        const bool input_status = (i > 1 || ucInputStatusForEachPlayer[i] == 1);

        if (ucInputStatus == 1 && input_status)
        {
            f32 x1, y1;

            if (InputUseRawAnalog() && i < NB_USER)
            {
                if (i < 4)
                {
                    const USER_INPUT* const p_user = UserGetInput(i);

                    x1 = p_user->x1;
                    y1 = p_user->y1;
                }
                else
                    x1 = y1 = 0.0f;
            }
            else
            {
                x1 = NORM_PDS_DIR(perG[i].x1);
                y1 = NORM_PDS_DIR(perG[i].y1);
            }

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
        {
            p_sin->stroke = 0.0f;
            p_sin->angle  = 0;
        }

        AnalogThrust[i] = p_sin->stroke >= 0.5f && last_stroke <= 0.5f;
    }
}

/** Same as the above function, but player 2's input_data is set by player 1's
    raw USER_INPUT controller. This is done specifically for the Final Hazard
    boss fight, as Shadow is considered player 2 **/
static void
NewGetPlayersInputData_FH(void)
{
    for (int i = 0; i < ARYLEN(input_dataG); ++i)
    {
        SONIC_INPUT* const p_sin  = &input_dataG[i];

        const f32 last_stroke = p_sin->stroke;

        const bool input_status = (i > 1 || ucInputStatusForEachPlayer[i] == 1);

        if (ucInputStatus == 1 && input_status)
        {
            f32 x1, y1;

            if (InputUseRawAnalog() && i < NB_USER)
            {
                if (i < 4)
                {
                    const USER_INPUT* const p_user = i <= 1 ? UserGetInput(0) : UserGetInput(i);

                    x1 = p_user->x1;
                    y1 = p_user->y1;
                }
                else
                    x1 = y1 = 0.0f;
            }
            else
            {
                x1 = NORM_PDS_DIR(perG[i].x1);
                y1 = NORM_PDS_DIR(perG[i].y1);
            }

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
        {
            p_sin->stroke = 0.0f;
            p_sin->angle  = 0;
        }

        AnalogThrust[i] = p_sin->stroke >= 0.5f && last_stroke <= 0.5f;
    }
}

void
IC_SonicInputInit(void)
{
    WriteJump(0x00441EF0, NewGetPlayersInputData);
    WriteCall(0x0049E580, NewGetPlayersInputData_FH);
}
