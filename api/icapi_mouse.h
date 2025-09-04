/*
*   SA2 Input Controls - '/icapi_mouse.h'
*
*   Description:
*     The 'Mouse' API module of Input Controls is for getting the current state and input
*   directly from the user mouse and mouse cursor.
* 
*   Notes:
*     - All returned pointers are constant for the liftime of the program.
* 
*   Version History:
*     - v1.0.0.0        : Version 0, initial release
* 
*   Availability:
*     - At Init         : Yes (input values will be null'd)
*     - After Init      : Yes
* 
*   Notice:
*     The API headers are to be included in your own projects. You have full rights to use,
*   distribute, and modify the API headers as you see fit.
*/
#ifndef H_ICAPI_MOUSE
#define H_ICAPI_MOUSE

/********************************/
/*  Constants                   */
/********************************/
/****** Mouse Buttons ***************************************************************************/
#define MSEBTN_LEFT                 (1<<0) /* left click                                        */
#define MSEBTN_RIGHT                (1<<1) /* right click                                       */
#define MSEBTN_MIDDLE               (1<<2) /* mouse wheel button                                */
#define MSEBTN_X1                   (1<<3) /* side/eXtra button 1                               */
#define MSEBTN_X2                   (1<<4) /* side/eXtra button 2                               */

/********************************/
/*  Enums                       */
/********************************/
/****** Mouse State *****************************************************************************/
typedef enum
{
    IC_MOUSE_MD_FREE,               /* mouse free to move                                       */
    IC_MOUSE_MD_CAPTURED,           /* mouse captured to game window                            */
}
eIC_MOUSE_MODE;

/********************************/
/*  Structures                  */
/********************************/
/****** Mouse Input *****************************************************************************/
typedef struct
{
    uint8_t                   down; /* mouse buttons currently pressed                 [MSEBTN] */
    uint8_t                  press; /* mouse buttons pressed this moment               [MSEBTN] */
    uint8_t                release; /* mouse buttons released this moment              [MSEBTN] */

    f32                     wheelx; /* mouse wheel X rotation, in notches                       */
    f32                     wheely; /* mouse wheel Y rotation, in notches                       */

    NJS_POINT2I                vec; /* mouse movement vector, in pixels                         */
    NJS_POINT2I                pos; /* mouse position on the window surface, in pixels          */
}
IC_MOUSE;

/********************************/
/*  API Structures              */
/********************************/
/****** Mouse API *******************************************************************************/
typedef struct
{
    uint32_t version;               /* structure version                                        */

    /****** Version >= 0 ********************************************************************/

    /**** Get Mouse *****************************************/
    /*
    *   Description:
    *      Get the mouse info structure.
    * 
    *   Returns:
    *     The mouse struct pointer
    */
    const IC_MOUSE* (__cdecl* GetMouse)( void );

    /**** Mouse Mode ****************************************/
    /*
    *   Description:
    *     Get the current mouse cursor mode. If 'Capture' or 'Free' are called on this frame,
    *   the mouse mode and state will only be updated on the next frame.
    * 
    *   Returns:
    *     The current mouse mode
    */
    eIC_MOUSE_MODE (__cdecl* GetMode)( void );
    /*
    *   Description:
    *     Set the mouse cursor to be captured to the game window. If the mouse is currently
    *   captured, this will do nothing.
    */
    void (__cdecl* Capture)( void );
    /*
    *   Description:
    *     Set the mouse cursor to be freed from the game window. If the mouse is currently
    *   freed, this will do nothing.
    */
    void (__cdecl* Free)( void );

    /**** Cursor Display ************************************/
    /*
    *   Description:
    *     Manually hide the mouse cursor from the game window. This is automatically done when
    *   capturing the mouse.
    */
    void (__cdecl* Hide)( void );
    /*
    *   Description:
    *     Manually show the mouse cursor from the game window. This is automatically done when
    *   freeing the mouse.
    */
    void (__cdecl* Show)( void );
}
ICAPI_MOUSE;

#endif/*H_ICAPI_MOUSE*/
