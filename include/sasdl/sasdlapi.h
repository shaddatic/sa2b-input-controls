/*
*   Sonic Adventure SDL Loader - '/sasdlapi.h'
*
*   Description:
*     Main header for SASDL's external API modules. Check the 'readme.md' file for instructions
*   on proper usage.
*     The API headers are to be included in your own projects. You have full rights to use,
*   distribute, and modify the API headers as you see fit.
* 
*   Version History:
*     - v1.0.0.0        : Version 0, initial release
* 
*   Availability:
*     - SASDLAPI_Init   : Yes
*     - After Init      : Yes
*/
#ifndef H_SASDLAPI
#define H_SASDLAPI

/********************************/
/*  Opaque Types                */
/********************************/
/****** SDL *************************************************************************************/
typedef struct _SDL_GameController  SDL_GameController; /* SDL headers                          */
typedef union  SDL_Event            SDL_Event;          /* SDL headers                          */

/********************************/
/*  Macros                      */
/********************************/
/****** Mod Version *****************************************************************************/
/*
*   Description:
*     Macro for checking the current SASDL version against a given version.
* 
*   Examples:
*     - if ( SASDL_CHECKVER(pApi, 1,4,1,0) ) // check if version is >= v1.4.1.0
*
*   Parameters:
*     - api             : core api pointer
*     - rel             : release part value
*     - maj             : major part value 
*     - sem             : semi-major part value 
*     - min             : minor part value 
* 
*   Returns:
*     'true' if the current version is '>=' the given version; or 'false' if not.
*/
#define SASDL_CHECKVER(api, rel, maj, sem, min)                                             \
    ( (int)(api)->modver.release   > (rel) || ( (int)(api)->modver.release   == (rel) &&    \
    ( (int)(api)->modver.major     > (maj) || ( (int)(api)->modver.major     == (maj) &&    \
    ( (int)(api)->modver.semimajor > (sem) || ( (int)(api)->modver.semimajor == (sem) &&    \
    ( (int)(api)->modver.minor     > (min) || ( (int)(api)->modver.minor     == (min) ))))))) )

/********************************/
/*  Enums                       */
/********************************/
/****** Event Return ****************************************************************************/
typedef enum
{
    SASDL_EVRET_CONTINUE,           /* continue event loop                            [default] */
    SASDL_EVRET_BREAK,              /* break loop, no other mods will receive this event        */
}
SASDL_EVRET;

/********************************/
/*  Structures                  */
/********************************/
/****** API *************************************************************************************/
typedef struct
{
    uint32_t version;               /* structure version                                        */

    /****** Version >= 0 ************************************************************************/

    /**** Mod Version ***************************************/

    struct                          /* sasdl mod version                                        */
    {
        uint8_t release;            /* release version part           (eg. the '1' in v1.2.3.4) */
        uint8_t major;              /* major version part             (eg. the '2' in v1.2.3.4) */
        uint8_t semimajor;          /* semi-major version part        (eg. the '3' in v1.2.3.4) */
        uint8_t minor;              /* minor version part             (eg. the '4' in v1.2.3.4) */
    }
    modver;

    struct                          /* sdl library version                                      */
    {
        uint8_t release;            /* release version part             (eg. the '1' in v1.2.3) */
        uint8_t major;              /* major version part               (eg. the '2' in v1.2.3) */
        uint8_t minor;              /* minor version part               (eg. the '3' in v1.2.3) */
        uint8_t padding;            /* unused padding                                           */
    }
    sdlver;

    /**** SDL DLL Library ***********************************/
    /*
    *   Description:
    *     Get the DLL Windows 'HMODULE' for the mounted SDL library.
    */
    void* (__cdecl* GetHandle)( void );

    /**** SDL Events ****************************************/
    /*
    *   Description:
    *     Register an SDL_Event handler function. Once registered, your function
    *   will recieve all polled SDL events. Be aware that not all SDL event
    *   types can be polled as not every SDL system is in use unless specified.
    * 
    *   Parameters:
    *     - fnHandler   : function to handle incoming SDL events
    */
    void (__cdecl* RegisterEventHandler)( SASDL_EVRET(__cdecl* fnHandler)(const SDL_Event*) );
}
SASDLAPI;

#endif/*H_SASDLAPI*/
