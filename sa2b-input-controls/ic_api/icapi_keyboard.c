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
#define ICAPI_KEYBOARD_VER      (0)

/************************/
/*  File Data           */
/************************/
/****** API Struct ******************************************************************/
const ICAPI_KEYBOARD icapi_keyboard =
{
    .version = ICAPI_KEYBOARD_VER,

    .Poll       = KeyboardPoll,
    .Down       = KeyboardDown,
    .Press      = KeyboardPress,
    .Release    = KeyboardRelease,
    .CapsLock   = KeyboardCapsLock,
    .ScrollLock = KeyboardScrollLock,
    .NumLock    = KeyboardNumLock,
};
