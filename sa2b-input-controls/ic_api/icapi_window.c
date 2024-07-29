/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Self ************************************************************************/
#include <ic_core.h>    /* core                                                     */
#include <ic_window.h>  /* input                                                    */

/****** Self ************************************************************************/
#include <ic_api/icapi_internal.h>  /* self                                         */

/************************/
/*  Constants           */
/************************/
/****** Version *********************************************************************/
#define ICAPI_WINDOW_VER      (0)

/************************/
/*  File Data           */
/************************/
/****** API Struct ******************************************************************/
const ICAPI_WINDOW icapi_window =
{
    .version = ICAPI_WINDOW_VER,

    .InFocus = WND_InFocus,

    .GetSize           = WND_GetSize,
    .GetGameResolution = WND_GetGameResolution,
};
