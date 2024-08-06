/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */
#include <sa2b/memory.h>    /* mReAlloc                                             */
#include <sa2b/modloader.h> /* ML_GetVersion                                        */

/****** Game ************************************************************************/
#include <sa2b/sonic/display.h> /* DisplayResolutionX/Y                             */

/****** Utility *********************************************************************/
#include <sa2b/util/ml_settings.h> /* ml_settings                                   */

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_os.h>          /* OS_GetGameWindowSize                                 */

/****** Self ************************************************************************/
#include <ic_window.h>                /* self                                       */
#include <ic_window/icwnd_internal.h> /* internal                                   */

/************************/
/*  Constants           */
/************************/
/****** Message Handler *************************************************************/
#define HANDLER_CHUNK_SIZE      (8)

/****** Aspect Ratio ****************************************************************/
#define AR_4BY3                 (640.f/480.f) /* 4:3 aspect ratio                   */

/************************/
/*  Structures          */
/************************/
/****** Message Handler *************************************************************/
typedef struct
{
    void (__cdecl* func)( uint32_t msg, uint32_t wParam, int32_t lParam );
}
WNDMSG_HANDLER;

/************************/
/*  File Data           */
/************************/
/****** Focus ***********************************************************************/
static bool FocusState;     /* window focus state                                   */

/****** Window Surface **************************************************************/
static NJS_POINT2I SurfaceSize; /* window surface size                              */

/****** Focus ***********************************************************************/
static bool ScreenStretched; /* game screen stretched                               */

/****** Message Handler *************************************************************/
static WNDMSG_HANDLER* MsgHandlerListP;   /* handler list pointer                   */
static size_t          MsgHandlerListNum; /* handler list count                     */

/************************/
/*  Source              */
/************************/
/****** Extern **********************************************************************/
bool
WND_InFocus(void)
{
    return FocusState;
}

void
WND_GetSize(NJS_POINT2I* const pOutSize)
{
    *pOutSize = SurfaceSize;
}

void
WND_TranslateSurfaceToGameCoords(const NJS_POINT2I* const pIn, NJS_POINT2* const pOut)
{
    const f32 x_norm = (f32)pIn->x / (f32)SurfaceSize.x;
    const f32 y_norm = (f32)pIn->y / (f32)SurfaceSize.y;

    if (ScreenStretched)
    {
        const f32 res_ratio = DisplayResolutionX / DisplayResolutionY;

        const f32 crd_adj  = (res_ratio / AR_4BY3);
        const f32 crd_shft = (crd_adj - 1.f) * 0.5f;

        pOut->x = ((x_norm * crd_adj) - crd_shft) * 640.f;
        pOut->y = y_norm * 480.f;
    }
    else /* not stretched */
    {
        const f32 res_ratio = DisplayResolutionX / DisplayResolutionY;
        const f32 wnd_ratio = (f32)SurfaceSize.x / (f32)SurfaceSize.y;

        if (res_ratio > wnd_ratio)
        {
            /** if the game is wider than the window **/

            const f32 crd_adj_x  = (res_ratio / AR_4BY3);
            const f32 crd_shft_x = (crd_adj_x - 1.f) * 0.5f;

            const f32 crd_adj_y  = (res_ratio / wnd_ratio);
            const f32 crd_shft_y = (crd_adj_y - 1.f) * 0.5f;

            pOut->x = ((x_norm * crd_adj_x) - crd_shft_x) * 640.f;
            pOut->y = ((y_norm * crd_adj_y) - crd_shft_y) * 480.f;
        }
        else /* window is wider than game */
        {
            const f32 crd_adj  = (wnd_ratio / AR_4BY3);
            const f32 crd_shft = (crd_adj - 1.f) * 0.5f;

            pOut->x = ((x_norm * crd_adj) - crd_shft) * 640.f;
            pOut->y = y_norm * 480.f;
        }
    }
}

void
WND_Update(void)
{
    INT_POINT2 size;

    OS_GetGameWindowSize(&size);

    SurfaceSize.x = (Sint16) size.x;
    SurfaceSize.y = (Sint16) size.y;
}

void
WND_RegisterMessageHandler(void(__cdecl*fnMsgHandler)(uint32_t msg, uint32_t wParam, int32_t lParam))
{
    if (!fnMsgHandler)
        return;

    const size_t nb_hdl = MsgHandlerListNum;

    WNDMSG_HANDLER* p_hdl = MsgHandlerListP;

    if ( !(nb_hdl % HANDLER_CHUNK_SIZE) )
    {
        p_hdl = mReAlloc(WNDMSG_HANDLER, p_hdl, ( nb_hdl + HANDLER_CHUNK_SIZE ));

        MsgHandlerListP = p_hdl;
    }
    
    p_hdl[nb_hdl].func = fnMsgHandler;

    MsgHandlerListNum = nb_hdl+1;
}

void
WND_SendMessage(uint32_t msg, uint32_t wParam, int32_t lParam)
{
    const size_t nb_hdl         = MsgHandlerListNum;
    const WNDMSG_HANDLER* p_hdl = MsgHandlerListP;

    for (size_t i = 0; i < nb_hdl; ++i, ++p_hdl)
    {
        p_hdl->func( msg, wParam, lParam );
    }
}

void
WND_MsgKillFocus(void)
{
    FocusState = false;
}

void
WND_MsgSetFocus(void)
{
    FocusState = true;
}

/****** Init ************************************************************************/
void
WND_Init(void)
{
    FocusState = true;

    WND_MessageInit();

    const ml_settings* const p_mls = ML_GetSettings();

    if (!p_mls)
        return;

    ScreenStretched = (ML_GetVersion() < 14) ? !p_mls->fixedAspectRatio : p_mls->screenStretch;
}
