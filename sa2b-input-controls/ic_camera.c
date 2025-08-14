/********************************/
/*  Includes                    */
/********************************/
/****** Core Toolkit ****************************************************************************/
#include <samt/core.h>              /* core                                                     */
#include <samt/writeop.h>           /* WriteJump, WriteCall, WriteNOP                           */
#include <samt/memory.h>            /* MemCopy                                                  */

/****** Ninja ***********************************************************************************/
#include <samt/ninja/ninja.h>       /* ninja                                                    */

/****** Game ************************************************************************************/
#include <samt/sonic/input.h>       /* per, perG                                                */
#include <samt/sonic/camera.h>      /* sa2 camera                                               */

/****** Input Controls **************************************************************************/
#include <ic_core.h>                /* core                                                     */
#include <ic_input.h>               /* input                                                    */
#include <ic_feature.h>             /* ICF_UseRawAnalog                                         */

/****** Config **********************************************************************************/
#include <cnf.h>                    /* CnfGet##                                                 */

/****** Self ************************************************************************************/
#include <ic_camera.h>              /* self                                                     */

/********************************/
/*  File Data                   */
/********************************/
/****** Invert **********************************************************************************/
static bool CameraInvX2;            /* invert right stick camera input                          */

/********************************/
/*  Source                      */
/********************************/
/****** Static **********************************************************************************/
static Angle
CameraGetAnalog(ADJUSTLEVEL* const pParam, Angle rotAng)
{
    const int nb_cam = cameraNumber;
    
    f32 l, r, x2;

    if (ICF_UseRawAnalog() && nb_cam < NB_IC_USER)
    {
        const bool in_state = (nb_cam > 1 || ucInputStatusForEachPlayer[nb_cam] == 1);

        if (ucInputStatus && in_state)
        {
            const IC_USER* const p_user = UserGetInput(nb_cam);

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

    /** Invert the stick if setting enabled **/
    if (CameraInvX2) x2 = -x2;

    CAMADJUSTWK_KNUCKLES* const p_work = (CAMADJUSTWK_KNUCKLES*)pParam->work;

    p_work->bTurning = false;

    /* triggers */
    if (l || r)
    {
        const f32 lmr = l - r;

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
    if (ICF_UseRawAnalog() && nbPer < NB_IC_USER)
    {
        const IC_USER* const p_user = UserGetInput(nbPer);
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

/****** Chao Camera *****************************************************************************/
static void
ChaoCameraAnalog(void)
{
    const IC_USER* p_user = UserGetInput(IC_USER_1);

    const f32 inpt_xz = (p_user->r - p_user->l) + ( CameraInvX2 ? -p_user->x2 : p_user->x2 );

    njRotateY(NULL, (Angle)( inpt_xz * 512.f ));
}

/****** Extern **********************************************************************************/
bool
ICF_CamInvertX2(void)
{
    return CameraInvX2;
}

/****** Init ************************************************************************************/
void
IC_CameraInit(void)
{
    if (CNF_GetInt(CNF_CAMERA_ANALOG))
    {
        /* CameraKnukles */
        WriteNOP( 0x004F4D37, 0x004F4DB4);
        WriteCall(0x004F4D37, ___CameraGetAnalog);

        /* CameraKnuklesL */
        WriteNOP( 0x004F42A8, 0x004F431F);
        WriteCall(0x004F42A8, ___CameraGetAnalog);

        /* Sub-Init Free Look */
        WriteNOP( 0x004EE4DA, 0x004EE4F4);
        WriteCall(0x004EE4DA, ___CheckCamInput);

        /* Init Free Look */
        WriteNOP( 0x004EE440, 0x004EE45A);
        WriteCall(0x004EE440, ___CheckCamInput);

        /* Idk */
        WriteNOP( 0x004EDBF3, 0x004EDC0D);
        WriteCall(0x004EDBF3, ___CheckCamInput);

        /* Chao */
        WriteNOP(0x0057CE0A, 0x0057CE14); // if ( perG[0].on & BTN_L )
        WriteNOP(0x0057CE36, 0x0057CE38); // ^
        WriteNOP(0x0057CE8E, 0x0057CEED); // entire R trigger code
        WriteCall(0x0057CE62, ChaoCameraAnalog); // _RotateY
    }

    CameraInvX2 = CNF_GetInt(CNF_CAMERA_LRINV);
}
