/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Self ************************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_sdl2.h>        /* input                                                */

/****** Self ************************************************************************/
#include <ic_api/icapi_internal.h> /* self                                          */

/************************/
/*  Constants           */
/************************/
/****** Version *********************************************************************/
#define ICAPI_SDL_VER           (0) /* API version                                  */

/************************/
/*  File Data           */
/************************/
/****** API Struct ******************************************************************/
const ICAPI_SDL icapi_sdl =
{
    .version = ICAPI_SDL_VER,

    .sdl_version = { 2,30,5 },

    .RegisterEventHandler = ICSDL_RegisterEventHandler,

    .GetHandle = ICSDL_GetHandle,
    .GetExport = ICSDL_GetExport,
};
