/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */

/****** Self ************************************************************************/
#include <ic_api/icapi_internal.h> /* self                                          */
#include <ic_input.h>              /* input                                         */

/************************/
/*  Constants           */
/************************/
/****** Version *********************************************************************/
#define ICAPI_GAMEPAD_VER       (0) /* API version                                  */

/************************/
/*  File Data           */
/************************/
/****** API Struct ******************************************************************/
const ICAPI_GAMEPAD icapi_gamepad =
{
    .version = ICAPI_GAMEPAD_VER,

    .GetGamepad = GamepadGetGamepad,

    .Valid = GamepadValid,

    .SetVibration        = GamepadSetVibration,
    .SetTriggerVibration = GamepadSetTriggerVibration,
};
