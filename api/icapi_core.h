/*
*   SA2 Input Controls - '/icapi_core.h'
* 
*   Description:
*     The 'Core' API is the central hub for accessing all other API modules. You can get it
*   through one of the mod export functions as an argument.
* 
*   Version History:
*     - v1.0.0.0        : Version 0, initial release
* 
*   Availability:
*     - At Init         : Yes
*     - After Init      : Yes
* 
*   Notice:
*     The API headers are to be included in your own projects. You have full rights to use,
*   distribute, and modify the API headers as you see fit.
*/
#ifndef H_ICAPI_CORE
#define H_ICAPI_CORE

/********************************/
/*  API Structures              */
/********************************/
/****** Core API ********************************************************************************/
typedef struct
{
    uint32_t version;               /* structure version                                        */

    /****** Version >= 0 ********************************************************************/

    /**** Mod Version ***************************************/

    struct
    {
        uint8_t release;                    /* release version part           ('1' in v1.2.3.4) */
        uint8_t major;                      /* major version part             ('2' in v1.2.3.4) */
        uint8_t semimajor;                  /* semi-major version part        ('3' in v1.2.3.4) */
        uint8_t minor;                      /* minor version part             ('4' in v1.2.3.4) */
    }
    modver;

    /**** API Modules ***************************************/

    const ICAPI_FEATURE*  pApiFeature;      /* feature API                                      */
    const ICAPI_CONFIG*   pApiConfig;       /* config API                                       */

    const ICAPI_USER*     pApiUser;         /* input controls user API                          */
    const ICAPI_GAMEPAD*  pApiGamepad;      /* gamepad API                                      */
    const ICAPI_KEYBOARD* pApiKeyboard;     /* keyboard API                                     */
    const ICAPI_MOUSE*    pApiMouse;        /* mouse API                                        */
    const ICAPI_WINDOW*   pApiWindow;       /* game window API                                  */
    const ICAPI_SDL*      pApiSdl;          /* SDL API                             [DEPRICATED] */
}
ICAPI, ICAPI_CORE;

#endif/*H_ICAPI_CORE*/
