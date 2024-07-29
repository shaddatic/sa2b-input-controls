/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */
#include <sa2b/memory.h>    /* mReAlloc                                             */

/****** Game ************************************************************************/
#include <sa2b/sonic/display.h>

/****** Input Controls **************************************************************/
#include <ic_core.h>
#include <ic_os.h>
#include <ic_input.h>

/****** Self ************************************************************************/
#include <ic_window.h>
#include <ic_window/icwnd_internal.h>

/************************/
/*  Structures          */
/************************/
/****** Event Handler ***************************************************************/
typedef struct
{
    void (__cdecl* func)( uint32_t msg, uint32_t wParam, int32_t lParam );
}
WNDMSG_HANDLER;

/************************/
/*  File Data           */
/************************/
static bool         FocusState;

static NJS_POINT2I  WindowSize;
static NJS_POINT2I  GameResolution;

static WNDMSG_HANDLER* MsgHandlerListP;
static size_t          MsgHandlerListNum;

/************************/
/*  Source              */
/************************/
bool
WND_InFocus(void)
{
    return FocusState;
}

void
WND_GetSize(NJS_POINT2I* const pOutSize)
{
    *pOutSize = WindowSize;
}

void
WND_GetGameResolution(NJS_POINT2I* const pOutRes)
{
    *pOutRes = GameResolution;
}

void
WND_Update(void)
{
    INT_POINT2 size;

    OS_GetGameWindowSize(&size);

    WindowSize.x = (Sint16) size.x;
    WindowSize.y = (Sint16) size.y;

    GameResolution.x = (Sint16) DisplayResolutionX;
    GameResolution.y = (Sint16) DisplayResolutionY;
}

void
WND_RegisterMessageHandler(void (__cdecl* fnMsgHandler)(uint32_t msg, uint32_t wParam, int32_t lParam))
{
    if (!fnMsgHandler)
        return;

    const size_t nb = MsgHandlerListNum;

    WNDMSG_HANDLER* p_hdl = MsgHandlerListP;

    p_hdl = mReAlloc(WNDMSG_HANDLER, p_hdl, nb+1);

    p_hdl[nb].func = fnMsgHandler;

    MsgHandlerListP = p_hdl;
    MsgHandlerListNum = nb+1;
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

void
WND_Init(void)
{
    FocusState = true;

    WND_MessageInit();
}
