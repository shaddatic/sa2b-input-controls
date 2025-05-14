/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <samt/core.h>      /* core                                                 */
#include <samt/writeop.h>   /* WriteJump                                            */

/****** Game ************************************************************************/
#include <samt/sonic/task.h>   /* TASK                                              */
#include <samt/sonic/game.h>   /* ssStageNumber                                     */
#define SAMT_INCL_FUNCPTRS
#include <samt/sonic/output.h> /* VibTask                                           */
#undef  SAMT_INCL_FUNCPTRS

/****** Chao ************************************************************************/
#include <samt/sonic/chao/al_stage.h> /* AL_GetStageNumber                          */

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_input.h>       /* GP_VibSet                                            */

/************************/
/*  Game Variables      */
/************************/
/****** Now Saving ******************************************************************/
#define now_saving          DATA_REF(int8_t , 0x0174AFD4)

/************************/
/*  Source              */
/************************/
/****** Static **********************************************************************/
static void
NewVibTask(task* const tp)
{
    VIBPARAM* const vpp = GET_VIBPARAM(tp);
    VIBWK*    const vwp = GET_VIBWK(tp);

    const eCHAO_STAGE_NUMBER c_stg = AL_GetStageNumber();
    const eCHAO_STAGE_NUMBER n_stg = AL_GetNextStageNumber();

    if (now_saving == 1 || (ssStageNumber == 90 && c_stg != n_stg && c_stg != -1))
    {
        DestroyTask(tp);
    }
    else
    {
        const int32_t timer = vwp->timer;

        if (timer == 1)
        {
            const f32 pwr = (f32)vpp->power / 7.0f;

            GamepadSetVibration( UserGetGamepadNum(vpp->unit), pwr, pwr );
        }

        if (timer <= vpp->maxTime)
        {
            ++vwp->timer;
        }
        else
            FreeTask(tp);
    }
}

static void
NewVibTaskDest(task* const tp)
{
    VIBPARAM* const vpp = GET_VIBPARAM(tp);
    VIBWK*    const vwp = GET_VIBWK(tp);

    if (VibTaskP[vpp->unit] == tp) // I feel like this will always be true
    {
        VibTaskP[vpp->unit] = nullptr;

        if (vpp->flag != 3)
        {
            GamepadSetVibration( UserGetGamepadNum(vpp->unit), -1.f, -1.f );
        }
        else
            GamepadSetVibration( UserGetGamepadNum(vpp->unit), -1.f, -1.f );
    }
}

static void
NewVibStop(void)
{
    for (eIC_USER_NUM i = 0; i < NB_IC_USER; ++i)
    {
        GamepadSetVibration( UserGetGamepadNum(i), -1.f, -1.f );
    }
}

/****** Init ************************************************************************/
void
IC_VibTaskInit(void)
{
    WriteJump(VibTask_p    , NewVibTask);
    WriteJump(VibTaskDest_p, NewVibTaskDest);
    WriteJump(VibStop_p    , NewVibStop);
}
