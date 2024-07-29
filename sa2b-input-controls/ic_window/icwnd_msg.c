/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Windows *********************************************************************/
#include <Windows.h>    /* ugh                                                      */

/****** Input Controls **************************************************************/
#include <ic_core.h>

/****** Self ************************************************************************/
#include <ic_window/icwnd_internal.h> /* self                                       */

static void
WindowMessageHandler(uint32_t msg, uint32_t wParam, int32_t lParam)
{
    switch (msg) {
    case WM_KILLFOCUS:
        WND_MsgKillFocus();
        break;

    case WM_SETFOCUS:
        WND_MsgSetFocus();
        break;
    }
}

void
WND_MessageInit(void)
{
    WND_RegisterMessageHandler( WindowMessageHandler );
}
