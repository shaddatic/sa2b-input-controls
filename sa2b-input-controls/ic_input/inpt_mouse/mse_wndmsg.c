/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Windows *********************************************************************/
#include <Windows.h>        /* windows, ew                                          */

/****** Windows *********************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_window.h>      /* WND_RegisterMessageHandler                           */

/****** Self ************************************************************************/
#include <ic_input/inpt_mouse/mse_internal.h> /* parent                             */

/************************/
/*  Source              */
/************************/
/****** Static **********************************************************************/
static void
MouseMessageHandler(uint32_t msg, uint32_t wParam, int32_t lParam)
{
    /** Converts the raw mouse wheel message into a float, then adds that to the wheel
        buffer. Each notch is 120, or 'WHEEL_DELTA', so we divide by that. The buffer is
        collected and emptied later by the input/mouse sub-module. **/

    s16 raw_wheel = GET_WHEEL_DELTA_WPARAM( wParam );
    f32 f_wheel = (f32)raw_wheel / (f32)WHEEL_DELTA;

    switch (msg) {
    case WM_MOUSEWHEEL:
        MouseMsgWheelY(f_wheel);
        break;

    case WM_MOUSEHWHEEL:
        MouseMsgWheelX(f_wheel);
        break;
    }
}

/****** Init ************************************************************************/
void
MouseWindowMsgInit(void)
{
    WND_RegisterMessageHandler( MouseMessageHandler );
}
