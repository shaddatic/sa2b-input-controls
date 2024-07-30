/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Self ************************************************************************/
#include <ic_core.h>    /* core                                                     */

/****** Self ************************************************************************/
#include <ic_api/icapi_internal.h>  /* self                                         */
#include <ic_input.h>               /* input                                        */

/************************/
/*  Constants           */
/************************/
/****** Version *********************************************************************/
#define ICAPI_MOUSE_VER      (0)

/************************/
/*  File Data           */
/************************/
/****** API Struct ******************************************************************/
const ICAPI_MOUSE icapi_mouse =
{
    .version = ICAPI_MOUSE_VER,

    .GetMouse = MouseGetMouse,

    .GetMode  = (void*) MouseGetMode,

    .Capture  = MouseCapture,
    .Free     = MouseFree,

    .Hide     = MouseHide,
    .Show     = MouseShow,
};
