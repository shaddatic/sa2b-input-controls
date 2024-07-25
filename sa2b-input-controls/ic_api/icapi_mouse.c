/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Self ************************************************************************/
#include <ic_core.h>    /* core                                                     */
#include <ic_input.h>   /* input                                                    */

/****** Self ************************************************************************/
#include <ic_api/icapi_internal.h>  /* self                                         */

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

    .GetMouse = (void*) MouseGetMouse,

    .GetMode  = (void*) MouseGetMode,

    .Capture  = (void*) MouseCapture,
    .Free     = (void*) MouseFree,
};
