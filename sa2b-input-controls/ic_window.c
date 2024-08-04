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
/*  Constants           */
/************************/
/****** Message Handler *************************************************************/
#define HANDLER_CHUNK_SIZE      (8)

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
WND_Update(void)
{
    INT_POINT2 size;

    OS_GetGameWindowSize(&size);

    SurfaceSize.x = (Sint16) size.x;
    SurfaceSize.y = (Sint16) size.y;
}

void
WND_RegisterMessageHandler(void (__cdecl* fnMsgHandler)(uint32_t msg, uint32_t wParam, int32_t lParam))
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

void
WND_Init(void)
{
    FocusState = true;

    WND_MessageInit();
}
