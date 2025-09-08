/*
*   SA2 Input Controls - '/icapi_sdl.h'
* 
*   Description:
*     The 'SDL' API module of Input Controls is for communicating with, or getting
*   communications from, the mounted SDL2 library.
* 
*   Version History:
*     - v1.0.0.0        : Version 0, initial release
*     - v1.1.0.0        : Version 1, API module completely depricated and non-functioning
* 
*   Availability:
*     - Before Init     : No
*     - At Init         : Partial, only prior to module version 1
*     - After Init      : Partial, only prior to module version 1
* 
*   Notice:
*     The API headers are to be included in your own projects. You have full rights to use,
*   distribute, and modify the API headers as you see fit.
*/
#ifndef H_ICAPI_SDL
#define H_ICAPI_SDL

/********************************/
/*  Opaque Types                */
/********************************/
/****** SDL Types *******************************************************************************/
typedef union SDL_Event             SDL_Event; /* sdl2 headers                                  */

/********************************/
/*  API Structures              */
/********************************/
/****** SDL API *********************************************************************************/
/*
*   [[ DEPRICATED : Version 1 ]]
* 
*   Deprication Info:
*     - state           : non-functioning/all 0's
*     - reason          : SASDL dependancy mod is now the correct way to access SDL2
*     - alternative     : SASDL's API
*/
typedef struct
{
    uint32_t version;               /* structure version                                        */

    /****** Version >= 0 ********************************************************************/

    /**** SDL API Version ***********************************/

    struct
    {
        uint8_t release;            /* release version part                     ['1' in v1.2.3] */
        uint8_t major;              /* major version part                       ['2' in v1.2.3] */
        uint8_t minor;              /* minor version part                       ['3' in v1.2.3] */
        uint8_t padding;            /* unused padding                                           */
    }
    sdlver;

    /**** Event Handler *************************************/
    /*
    *   Description:
    *     Register an SDL_Event handler function. Once registered, your function will recieve 
    *   all polled SDL events. Be aware that not all SDL event types can be polled as not every
    *   SDL system is in use.
    * 
    *   Parameters:
    *     - fnHandler   : function to handle incoming SDL events
    */
    void (__cdecl* RegisterEventHandler)( void(__cdecl* fnHandler)(const SDL_Event*) );

    /**** DLL Library ***************************************/
    /*
    *   Description:
    *     Get the DLL HMODULE for the mounted SDL library.
    */
    void* (__cdecl* GetHandle)( void );
    /*
    *   Description:
    *     Get a DLL export from the mounted SDL library.
    * 
    *   Parameters:
    *     pcExName      : name of DLL export
    *     
    *   Returns:
    *     Pointer to the DLL export; or 'nullptr' if nothing is exported under that name.
    */
    void* (__cdecl* GetExport)( const char* pcExName );
}
ICAPI_SDL;

#endif/*H_ICAPI_SDL*/
