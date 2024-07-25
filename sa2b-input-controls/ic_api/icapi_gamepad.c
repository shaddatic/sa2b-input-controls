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
#define ICAPI_GAMEPAD_VER      (0)

/************************/
/*  File Data           */
/************************/
/****** API Struct ******************************************************************/
const ICAPI_GAMEPAD icapi_gamepad =
{
    .version = ICAPI_GAMEPAD_VER,

    .GetGamepad = (void*) GamepadGetGamepad,

    .Valid = (void*) GamepadValid,

    .SetVibration        = (void*) GamepadSetVibration,
    .SetTriggerVibration = (void*) GamepadSetTriggerVibration,
};
