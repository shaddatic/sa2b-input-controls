/********************************/
/*  Includes                    */
/********************************/
/****** Core Toolkit ****************************************************************************/
#include <samt/core.h>              /* core                                                     */
#include <samt/writeop.h>           /* WriteJump, WriteCall, WriteNoOP                          */

/****** Ninja ***********************************************************************************/
#include <samt/ninja/ninja.h>       /* task                                                     */

/****** Game ************************************************************************************/
#include <samt/sonic/task.h>        /* task                                                     */

/****** Input Controls **************************************************************************/
#include <ic_core.h>                /* core                                                     */

/****** Config **********************************************************************************/
#include <cnf.h>                    /* CnfGet##                                                 */

/****** Self ************************************************************************************/
#include <ic_misc.h>                /* self                                                     */

/********************************/
/*  Game Defs                   */
/********************************/
/****** Cart Work *******************************************************************************/
#define GET_CARTWK(_tp)     ((CARTWK*)_tp->mwp)

typedef struct
{
    taskwk tw;
    s32 cartmode;
    NJS_CNK_OBJECT* pObject;
    NJS_CNK_OBJECT* pFarObject;
    NJS_TEXLIST*    pTexlist;
    int headlights;
    b32 is_player;
    int character;
    int player_num;
    void* ptr1;
    char gap4[20];
    Angle angSlide;
    char gap5[132];
    Angle3 ang3;
    Angle3 ang2;
    char gap6[412];
    void* texp;
    int address_u;
    int address_v;
    int tes5;
    int min_filter;
    int mag_filter;
    int bank;
    __int32 stencil_data[1];
    void* stencil_tex;
    char gap7[28];
}
CARTWK;

/****** Cart Variables **************************************************************************/
#define CartForceNoInput        DATA_REF(s32, 0x01D96F40)
#define CartNoInputTimer        DATA_REF(s32, 0x01AEDEC8)

/********************************/
/*  Source                      */
/********************************/
/****** Static **********************************************************************************/
void __fastcall
CartNoPlayerInputFix(task* tp)
{
    taskwk* twp = tp->twp;
    CARTWK* cwp = GET_CARTWK(tp);

    twp->pos.z -= 10.f;

    const s32 timer = CartNoInputTimer;

    CartNoInputTimer = timer + 1;

    if ( timer > 120 )
    {
        CartForceNoInput = 0;
        CartNoInputTimer = 0;

        cwp->cartmode = 0;
    }
}

/****** Init ****************************************************************************/
void
IC_MiscInit(void)
{
    WriteJump(0x0061E990, CartNoPlayerInputFix);
}
