#include <sa2b/core.h>
#include <sa2b/memutil.h>
#include <sa2b/memory.h>

#include <sa2b/ninja/ninja.h>

#include <sa2b/sonic/input.h>
#include <sa2b/sonic/camera.h>
#include <sa2b/sonic/debug.h>

#include <ic_core.h>
#include <ic_input.h>
#include <ic_config.h>

static Angle
CameraGetAnalog(ADJUSTLEVEL* const pParam, Angle rotAng)
{
    const int nb_cam = cameraNumber;
    
    f32 l, r, x2;

    if (InputUseRawAnalog() && nb_cam < NB_USER)
    {
        const USER_INPUT* const p_user = UserGetInput(nb_cam);
        const bool in_state = (nb_cam > 1 || ucInputStatusForEachPlayer[nb_cam] == 1);

        if (ucInputStatus && in_state)
        {
            l  = p_user->l;
            r  = p_user->r;
            x2 = p_user->x2;
        }
        else
            l = r = x2 = 0.0f;
    }
    else
    {
        l  = NORM_PDS_TRIG( perG[nb_cam].l  );
        r  = NORM_PDS_TRIG( perG[nb_cam].r  );
        x2 = NORM_PDS_DIR(  perG[nb_cam].x2 );
    }

    CAMADJUSTWK_KNUCKLES* const p_work = (CAMADJUSTWK_KNUCKLES*)pParam->work;

    p_work->bTurning = false;

    /* triggers */
    const f32 lmr = l - r;

    if (lmr)
    {
        rotAng += (Angle) nearbyint(lmr * 546.0f);

        p_work->turn_ang = rotAng;
        p_work->bTurning = true;

        SetAdjustMode(nb_cam, 0);
    }

    /* right analog stick */
    if (x2)
    {
        rotAng += (Angle) nearbyint(-x2 * 546.0); 

        p_work->turn_ang = rotAng;
        p_work->bTurning = true;

        SetAdjustMode(nb_cam, 0);
    }

    return rotAng;
}

#pragma optimize("", off)
__declspec(naked)
static void
___CameraGetAnalog(void)
{
    __asm
    {
        push eax
        push ecx
        push edx

        push esi
        push ebx

        call CameraGetAnalog
        mov esi, eax

        pop ebx     // ebx
        add esp, 4  // esi

        pop edx
        pop ecx
        pop eax
        retn
    }
}
#pragma optimize("", on)

static int
CheckCamInput(const int nbPer)
{
    if (InputUseRawAnalog() && nbPer < NB_USER)
    {
        const USER_INPUT* const p_user = UserGetInput(nbPer);
        const bool in_state = (nbPer > 1 || ucInputStatusForEachPlayer[nbPer] == 1);

        if (ucInputStatus && in_state)
        {
            return (p_user->l || p_user->r || p_user->x2);
        }
        else
            return false;
    }
    else
        return (perG[nbPer].l || perG[nbPer].r || perG[nbPer].x2);
}

__declspec(naked)
static void
___CheckCamInput(void)
{
    __asm
    {
        push eax
        call CheckCamInput
        test eax, eax
        pop eax
        retn
    }
}

void
IC_CameraInit(void)
{
    if (CnfGetInt(CNF_MAIN_CAMANALOG))
    {
        /* CameraKnukles */
        WriteNoOP(0x004F4D37, 0x004F4DB4);
        WriteCall(0x004F4D37, ___CameraGetAnalog);

        /* CameraKnuklesL */
        WriteNoOP(0x004F42A8, 0x004F431F);
        WriteCall(0x004F42A8, ___CameraGetAnalog);

        /* Sub-Init Free Look */
        WriteNoOP(0x004EE4DA, 0x004EE4F4);
        WriteCall(0x004EE4DA, ___CheckCamInput);

        /* Init Free Look */
        WriteNoOP(0x004EE440, 0x004EE45A);
        WriteCall(0x004EE440, ___CheckCamInput);

        /* Idk */
        WriteNoOP(0x004EDBF3, 0x004EDC0D);
        WriteCall(0x004EDBF3, ___CheckCamInput);
    }
}
