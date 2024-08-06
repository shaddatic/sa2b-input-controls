/*
*   SA2 Input Controls - '/ic_os.h'
*
*   Description:
*     Operating system calls module
*
*   Contributors:
*     - Shaddatic
*/
#ifndef H_IC_OS
#define H_IC_OS

EXTERN_START

/************************/
/*  Constants           */
/************************/
/****** Key States ******************************************************************/
#define KEY_TOGGLED         (0b0000'0001) /* key modifier is toggled                */
#define KEY_DOWN            (0b1000'0000) /* key is down                            */

/************************/
/*  Structures          */
/************************/
/****** Integer Point 2 *************************************************************/
typedef struct
{
    int32_t x, y;           /* coordinates                                          */
}
INT_POINT2;

/************************/
/*  Prototypes          */
/************************/
/****** Init ************************************************************************/
/*
*   Description:
*     Init module
*/
void    OS_Init( void );

/****** Window **********************************************************************/
/*
*   Description:
*     Get the size of the game window in pixels, not including the trims and title
*   bar.
*
*   Parameters:
*     - pOutSize : return pointer
*/
void    OS_GetGameWindowSize( INT_POINT2* pOutSize );

/****** Cursor **********************************************************************/
/*
*   Description:
*     Sets the mouse cursor position to the center of the game window. Optionally,
*   the current position of the cursor can be checked to avoid OS calls and the new
*   position will be returned.
*
*   Parameters:
*     - pOptInOutSize : optional pointer to mouse postion for check and return
*
*   Returns:
*     If an operating system call was used to center the mouse.
*/
bool    OS_CenterCursorOnGameWindow( INT_POINT2* pOptInOutPos );
/*
*   Description:
*     Sets the cursor to the givin visibility state.
*
*   Parameters:
*     - bShow   : state to set the cursor visibility to
*/
void    OS_ShowCursor( bool bShow );
/*
*   Description:
*     Gets the current global cursor position.
*
*   Parameters:
*     - pOutPos : return pointer
*/
void    OS_GetCursorPos( INT_POINT2* pOutPos );
/*
*   Description:
*     Gets the current cursor position relative to the game window.
*
*   Parameters:
*     - pOutPos : return pointer
*/
void    OS_GetCursorPosOnGameWindow( INT_POINT2* pOutPos );

/****** Keyboard **********************************************************************/
/*
*   Description:
*     Gets the current keyboard state array from the OS.
*
*   Parameters:
*     - pOutStates : return pointer to a 256 long, u8 array
*/
bool    OS_GetKeyboardState( uint8_t* pOutStates );

EXTERN_END

#endif/*H_IC_OS*/
