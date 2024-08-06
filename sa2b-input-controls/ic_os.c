/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Ninja ***********************************************************************/
#include <sa2b/ninja/njcommon.h> /* ninja common                                    */

/****** Windows *********************************************************************/
#include <Windows.h>        /* ugh                                                  */

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_window.h>      /* WND_SendMessage                                      */

/****** Self ************************************************************************/
#include <ic_os.h>          /* self                                                 */

/************************/
/*  Typedefs            */
/************************/
/****** Wnd Proc Function ***********************************************************/
typedef LRESULT(__stdcall fnWndProc)(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

/************************/
/*  Game Data           */
/************************/
/****** Window Handle ***************************************************************/
#define MainWindowHandle        DATA_REF(HWND, 0x01933EA8)

/************************/
/*  File Data           */
/************************/
/****** Og WndProc Function *********************************************************/
static fnWndProc*   WndProcFunc; /* original window proc function                   */

/************************/
/*  Source              */
/************************/
/****** Static **********************************************************************/
static LRESULT __stdcall
WndProcInputControls(const HWND hWnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
    /** Collect Window Messages from the OS then call the Mod Loader WndProc **/

    WND_SendMessage( uMsg, wParam, lParam );

    return WndProcFunc(hWnd, uMsg, wParam, lParam);
}

/****** Extern **********************************************************************/
void
OS_GetGameWindowSize(INT_POINT2* const pOutSize)
{
    RECT rect;
    GetClientRect(MainWindowHandle, &rect);

    pOutSize->x = rect.right;
    pOutSize->y = rect.bottom;
}

void
OS_ShowCursor(const bool bShow)
{
    if (bShow)
        while (ShowCursor(true) < 0);
    else
        while (ShowCursor(false) >= 0);
}

bool
OS_CenterCursorOnGameWindow(INT_POINT2* const pOptInOutPos)
{
    RECT rect;
    GetWindowRect(MainWindowHandle, &rect);

    INT_POINT2 reset_pos;

    reset_pos.x = (rect.left + ((rect.right  - rect.left) / 2));
    reset_pos.y = (rect.top  + ((rect.bottom - rect.top ) / 2));

    if (pOptInOutPos)
    {
        if ((reset_pos.x != pOptInOutPos->x || reset_pos.y != pOptInOutPos->y))
        {
            SetCursorPos(reset_pos.x, reset_pos.y);
            *pOptInOutPos = reset_pos;

            return true;
        }

        return false;
    }
    else
    {
        SetCursorPos(reset_pos.x, reset_pos.y);

        return true;
    }
}

void
OS_GetCursorPos(INT_POINT2* const pOutPos)
{
    GetCursorPos((POINT*)pOutPos);
}

void
OS_GetCursorPosOnGameWindow(INT_POINT2* const pOutPos)
{
    GetCursorPos((POINT*)pOutPos);
    ScreenToClient(MainWindowHandle, (POINT*)pOutPos);
}

bool
OS_GetKeyboardState(uint8_t* const pOutStates)
{
    return GetKeyboardState(pOutStates);
}

/****** Init ************************************************************************/
void
OS_Init(void)
{
    /** Get the current WndProc function so we can replace it, but still
        call back to it.
        Yes, there's technically a chance this can return 'nullptr'.
        However, since the Mod Loader sets this, the minimum ML version
        check will ensure it never is **/

    WndProcFunc = (void*) GetWindowLongW(MainWindowHandle, GWL_WNDPROC);
    SetWindowLongW(MainWindowHandle, GWL_WNDPROC, (LONG)WndProcInputControls);
}
