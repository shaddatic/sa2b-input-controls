/*
*   SA2 Input Controls - '/icapi_window.h'
*
*   Description:
*     The 'Window' API module of Input Controls is for getting window messages, the current
*   window state, and converting window surface coordinates to game screen coordinates.
* 
*   Version History:
*     - v1.0.0.0        : Version 0, initial release
* 
*   Availability:
*     - Before Init     : Partial, input values will be null
*     - At Init         : Partial, input values will be null
*     - After Init      : Yes
* 
*   Notice:
*     The API headers are to be included in your own projects. You have full rights to use,
*   distribute, and modify the API headers as you see fit.
*/
#ifndef H_ICAPI_WINDOW
#define H_ICAPI_WINDOW

/********************************/
/*  API Structures              */
/********************************/
/****** Window API ******************************************************************************/
typedef struct
{
    uint32_t version;               /* structure version                                        */

    /****** Version >= 0 ********************************************************************/

    /**** Message Handler ***********************************/
    /*
    *   Description:
    *     Register a window message handler function. Once registered, your function will
    *   recieve all window messages sent to the game.
    * 
    *   Parameters:
    *     - fnHandler : function to handle incoming window messages
    */
    void (__cdecl* RegisterMessageHandler)( void(__cdecl* fnHandler)(uint32_t msg, uint32_t wParam, int32_t lParam) );

    /**** Coordinates ***************************************/
    /*
    *   Description:
    *     Translate window surface coordinates, like the mouse position coordinates, to the
    *   game's 640x480 screen coordinates.
    * 
    *   Parameters:
    *     - pIn     : pointer to window surface coordinates
    *     - pOut    : return pointer for game screen coordinates
    */
    void (__cdecl* TranslateSurfaceToGameCoords)( const NJS_POINT2I* pIn, NJS_POINT2* pOut );

    /**** Focus *********************************************/
    /*
    *   Description:
    *     Check the main game window's focus state. If the user has set the game to pause when
    *   out of focus, this will always be true.
    * 
    *   Returns:
    *     'true' if the game window is in focus; or 'false' if it is not.
    */
    bool (__cdecl* InFocus)( void );

    /**** Parameters ****************************************/
    /*
    *   Description:
    *     Get the current size of the game window surface in pixels. Be aware that the user can
    *   resize the window at any time.
    * 
    *   Parameters:
    *     - pOutSize : return pointer for window size
    */
    void (__cdecl* GetSize)( NJS_POINT2I* pOutSize );
}
ICAPI_WINDOW;

#endif/*H_ICAPI_WINDOW*/
