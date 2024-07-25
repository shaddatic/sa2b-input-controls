/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */
#include <sa2b/writeop.h>   /* WriteJump                                            */

/****** Game ************************************************************************/
#include <sa2b/sonic/task.h>    /* TASK                                             */
#include <sa2b/sonic/game.h>    /* ssStageNumber                                    */
#define SAMT_INCL_FUNCPTRS
#include <sa2b/sonic/output.h>  /* VibTask                                          */
#undef  SAMT_INCL_FUNCPTRS

/****** Chao ************************************************************************/
#include <sa2b/sonic/chao/al_stage.h>   /* AL_GetStageNumber                        */

/****** Core Toolkit ****************************************************************/
#include <ic_core.h>    /* core                                                     */
#include <ic_input.h>   /* GP_VibSet                                                */

/************************/
/*  Game Variables      */
/************************/
#define now_saving          DATA_REF(int8_t , 0x0174AFD4)

/************************/
/*  Source              */
/************************/
static void
NewVibTask(TASK* const tp)
{
    VIBPARAM* const vpp = GET_VIBPARAM(tp);
    VIBWK*    const vwp = GET_VIBWK(tp);

    if (now_saving == 1 || (ssStageNumber == 90 && AL_GetStageNumber() != AL_GetNextStageNumber() && AL_GetStageNumber() != -1))
    {
        DestroyTask(tp);
    }
    else
    {
        const int32_t timer = vwp->timer;

        if (timer == 1)
        {
            const f32 pwr = (f32)vpp->power / 7.0f;

            const int nb_gp = UserGetGamepadNum(vpp->unit);

            GamepadSetVibration(nb_gp, pwr, pwr);
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
NewVibTaskDest(TASK* const tp)
{
    VIBPARAM* const vpp = GET_VIBPARAM(tp);
    VIBWK*    const vwp = GET_VIBWK(tp);

    if (VibTaskP[vpp->unit] == tp) // I feel like this will always be true
    {
        VibTaskP[vpp->unit] = nullptr;

        if (vpp->flag != 3)
        {
            GamepadSetVibration(UserGetGamepadNum(vpp->unit), -1.f, -1.f);
        }
        else
            GamepadSetVibration(UserGetGamepadNum(vpp->unit), -1.f, -1.f);
    }
}

static void
NewVibStop(void)
{
    for (eUSER_NUM i = 0; i < NB_USER; ++i)
    {
        const int nb_gp = UserGetGamepadNum(i);

        GamepadSetVibration((eGAMEPAD_NUM)nb_gp, -1.f, -1.f);
    }
}

void
IC_VibTaskInit(void)
{
    WriteJump(VibTask_p    , NewVibTask);
    WriteJump(VibTaskDest_p, NewVibTaskDest);

    WriteJump(VibStop_p, NewVibStop);
}
