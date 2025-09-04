/*
*   SA2 Input Controls - '/icapi.h'
*
*   Description:
*     Main header for Input Controls' external API modules. Check the 'readme.md' file for
*   instructions on proper usage.
* 
*   Notice:
*     The API headers are to be included in your own projects. You have full rights to use,
*   distribute, and modify the API headers as you see fit.
*/
#ifndef H_ICAPI
#define H_ICAPI

/********************************/
/*  Types                       */
/********************************/
/****** Base Types ******************************************************************************/
typedef float                       f32; /* 4 byte real number                                  */
typedef double                      f64; /* 8 byte real number                                  */
typedef char                        c8;  /* supports UTF-8 strings                              */

/********************************/
/*  Macros                      */
/********************************/
/****** IC Version ******************************************************************************/
/*
*   Description:
*     Macro for checking the current Input Controls version against a set version.
* 
*   Examples:
*     - if ( !ICD_CHECKVER(pApiCore, 1,3,3,0) ) // check if IC version too low
*
*   Parameters:
*     - api         : core api pointer
*     - rel         : minimum release part value
*     - maj         : minimum major part value 
*     - sem         : minimum semi-major part value 
*     - min         : minimum minor part value 
* 
*   Returns:
*     'true' if the current version is >= the set version; or 'false' if not.
*/
#define ICD_CHECKVER(api, rel, maj, sem, min) \
    ( (int)(api)->modver.release   > (rel) || ( (int)(api)->modver.release   == (rel) && \
    ( (int)(api)->modver.major     > (maj) || ( (int)(api)->modver.major     == (maj) && \
    ( (int)(api)->modver.semimajor > (sem) || ( (int)(api)->modver.semimajor == (sem) && \
    ( (int)(api)->modver.minor     > (min) || ( (int)(api)->modver.minor     == (min) ))))))) )

/********************************/
/*  Enums                       */
/********************************/
/****** User Index ******************************************************************************/
typedef enum
{
    IC_USER_1,                      /* player 1                                                 */
    IC_USER_2,                      /* player 2                                                 */
    IC_USER_3,                      /* player 3                                                 */
    IC_USER_4,                      /* player 4                                                 */

    NB_IC_USER,                     /* enum count                                               */
}
eIC_USER_NUM;

/****** Gamepad Index ***************************************************************************/
typedef enum
{
    IC_GAMEPAD_NONE = -1,           /* no gamepad                                               */

    IC_GAMEPAD_1,                   /* gamepad 1                                                */
    IC_GAMEPAD_2,                   /* gamepad 2                                                */
    IC_GAMEPAD_3,                   /* gamepad 3                                                */
    IC_GAMEPAD_4,                   /* gamepad 4                                                */

    NB_IC_GAMEPAD,                  /* enum count                                               */
}
eIC_GAMEPAD_NUM;

/****** Keyboard Layout *************************************************************************/
typedef enum
{
    IC_KEYBOARD_NONE = -1,          /* no keyboard layout                                       */

    IC_KEYBOARD_1,                  /* keyboard layout 1                                        */
    IC_KEYBOARD_2,                  /* keyboard layout 2                                        */
    IC_KEYBOARD_3,                  /* keyboard layout 3                                        */
    IC_KEYBOARD_4,                  /* keyboard layout 4                                        */

    NB_IC_KEYBOARD,                 /* enum count                                               */
}
eIC_KEYBOARD_NUM;

/********************************/
/*  Internal Includes           */
/********************************/
/****** API Modules *****************************************************************************/
#include "icapi_feature.h"          /* feature api module                                       */
#include "icapi_config.h"           /* config api module                                        */
#include "icapi_user.h"             /* user api module                                          */
#include "icapi_gamepad.h"          /* gamepad api module                                       */
#include "icapi_keyboard.h"         /* keyboard api module                                      */
#include "icapi_mouse.h"            /* mouse api module                                         */
#include "icapi_window.h"           /* window api module                                        */
#include "icapi_sdl.h"              /* sdl2 api module                             [DEPRICATED] */

/****** Core API ********************************************************************************/
#include "icapi_core.h"             /* api core                                                 */

#endif/*H_ICAPI*/
