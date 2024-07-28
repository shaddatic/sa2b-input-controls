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
#define ICAPI_USER_VER      (0)

/************************/
/*  File Data           */
/************************/
/****** API Struct ******************************************************************/
const ICAPI_USER icapi_user =
{
    .version = ICAPI_USER_VER,

    .GetInput = (void*) UserGetInput,

    .GetGamepadNum  = (void*) UserGetGamepadNum,
    .GetKeyboardNum = (void*) UserGetKeyboardNum,
};
