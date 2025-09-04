/*
*   SA2 Input Controls - '/ic_input.h'
*
*   Description:
*     Input module header
*
*   Contributors:
*     - Shaddatic
*/
#ifndef H_IC_INPUT
#define H_IC_INPUT

/************************/
/*  External Headers    */
/************************/
/****** Ninja ***********************************************************************/
#include <samt/ninja/njcommon.h>    /* NJS_POINT2I                                  */

/****** Input Controls **************************************************************/
#include <ic_limits.h>              /* limits                                       */

/****** ICAPI ***********************************************************************/
#include <icapi.h>                  /* external api                                 */

EXTERN_START

/************************/
/*  Opaque Types        */
/************************/
/****** SDL *************************************************************************/
typedef struct _SDL_GameController  SDL_GameController;

/************************/
/*  Macro               */
/************************/
/****** Normalize PDS Analog ********************************************************/
#define NORM_PDS_DIR(a)     ((f32)(a)/(f32)PDSLIM_XY_MAX) /* PDS directional        */
#define NORM_PDS_TRIG(a)    ((f32)(a)/(f32)PDSLIM_LR_MAX) /* PDS trigger            */

/************************/
/*  Prototypes          */
/************************/
/****** Init ************************************************************************/
/*
*   Description:
*     Get and load config settings for input.
*/
void    IC_InputGetConfig( void );
/*
*   Description:
*     Init Input module, and its sub-modules
*/
void    IC_InputInit( void );

/****** Event Handlers **************************************************************/
/*
*   Description:
*     Gamepad SDL Event handler for SASDL's API.
*
*   Parameters:
*     - pEvent      : SDL event structure
*
*   Returns:
*     'BREAK' if event handled; or 'CONTINUE' if not.
*/
SASDL_EVRET GamepadEventHandler( const SDL_Event* pEvent );

/****** User Input ******************************************************************/
/**** Raw Input *****************************************************************/
/*
*   Description:
*     Get the raw input of a user. The pointer will be constant for the
*   lifetime of the program.
*
*   Parameters:
*     - nbUser  : user number
*
*   Returns:
*     The pointer to the user's raw input structure
*/
const IC_USER_INPUT* UserGetInput( eIC_USER_NUM nbUser );

/**** Device Number *************************************************************/
/*
*   Description:
*     Get the gamepad number of a user. The index will be constant for the
*   lifetime of the program.
*
*   Parameters:
*     - nbUser  : user number
*
*   Returns:
*     The user's gamepad number
*/
eIC_GAMEPAD_NUM UserGetGamepadNum( const eIC_USER_NUM nbUser );
/*
*   Description:
*     Get the keyboard layout number of a given user. The index will be
*   constant for the lifetime of the program.
*
*   Parameters:
*     - nbUser  : user number
*
*   Returns:
*     The user's keyboard layout number
*/
eIC_KEYBOARD_NUM UserGetKeyboardNum( const eIC_USER_NUM nbUser );

/****** Callback ********************************************************************/
/*
*   Description:
*     Set the user input callback function.
*
*   Notes:
*     - This will be called just after the user input structure is executed.
*     - Only 1 mod can callback at a time
*
*   Parameters:
*     - callback    : callback function to call                      [opt: nullptr]
*/
void    UserSetCallback( void(__cdecl* callback)(int ixUser, IC_USER_INPUT* pUser) );

/****** Gamepad *********************************************************************/
/**** Get ************************************************************************/
/*
*   Description:
*     Get raw gamepad input and attributes. The pointer will be constant for
*   the lifetime of the program.
* 
*   Parameters:
*     - nbGp    : gamepad number to get
* 
*   Returns:
*     A Gamepad struct pointer, or 'nullptr' if 'nbGp' is GAMEPAD_NONE.
*/
const IC_GAMEPAD* GamepadGetGamepad( eIC_GAMEPAD_NUM nbGp );

/**** Valid *********************************************************************/
/*
*   Description:
*     Check if the given gamepad is active and linked to an open, physical
*   game controller device.
* 
*   Parameters:
*     - nbGp    : gamepad number to get state of
* 
*   Returns:
*     'true' if the given gamepad is valid, or 'false' if not
*/
bool    GamepadValid( eIC_GAMEPAD_NUM nbGp );

/**** Feedback ******************************************************************/
/*
*   Description:
*     Set the vibration motors on a gamepad. You can check support by
*   searching for the GPDDEV_SUPPORT_RUMBLE flag in 'support'.
* 
*   Parameters:
*     - nbGp    : gamepad number to set vibration for
*     - freqLo  : speed of the low frequency motor (0~1)
*     - freqHi  : speed of the high frequency motor (0~1)
* 
*   Returns:
*     'true' if successful, or 'false' if not
*/
bool    GamepadSetVibration( eIC_GAMEPAD_NUM nbGp, f32 spdLo, f32 spdHi );
/*
*   Description:
*     Set the trigger vibration motors on a gamepad. Only applicable to Xbox
*   One and higher controllers. You can check support by searching for the
*   GPDDEV_SUPPORT_RUMBLE_TRIGGER flag in 'support'.
* 
*   Parameters:
*     - nbGp    : gamepad number to set vibration for
*     - freqL   : speed of the left trigger motor (0~1)
*     - freqR   : speed of the right trigger motor (0~1)
* 
*   Returns:
*     'true' if successful, or 'false' if not
*/
bool    GamepadSetTriggerVibration( eIC_GAMEPAD_NUM nbGp, f32 spdL, f32 spdR );

/****** Keyboard ********************************************************************/
/**** Poll **********************************************************************/
/*
*   Description:
*     Get a key that was pressed this moment. If multiple keys are pressed
*   at the same time, the highest key index will be returned.
* 
*   Returns:
*     The index of a key pressed this moment, or 'KEY_NONE' if no keys have
*   been pressed
*/
u8      KeyboardPoll( void );

/**** Key State *****************************************************************/
/*
*   Description:
*     Check if a key is currently being held down.
*   
*   Parameters:
*     - key     : key to get state of
* 
*   Returns:
*     'true' if they key is down, or 'false' if it is not
*/
bool    KeyboardDown( u8 key );
/*
*   Description:
*     Check if a key was pressed this moment.
*   
*   Parameters:
*     - key     : key to get state of
* 
*   Returns:
*     'true' if they key was pressed, or 'false' if it was not
*/
bool    KeyboardPress( u8 key );
/*
*   Description:
*     Check if a key was released this moment.
*   
*   Parameters:
*     - key     : key to get state of
* 
*   Returns:
*     'true' if they key was released, or 'false' if it was not
*/
bool    KeyboardRelease( u8 key );

/**** Modifier State ************************************************************/
/*
*   Description:
*     Check if Caps Lock modifier is currently active.
* 
*   Returns:
*     'true' if caps lock is active, or 'false' if it is not
*/
bool    KeyboardCapsLock( void );
/*
*   Description:
*     Check if Scroll Lock modifier is currently active.
* 
*   Returns:
*     'true' if scroll lock is active, or 'false' if it is not
*/
bool    KeyboardScrollLock( void );
/*
*   Description:
*     Check if Number Lock modifier is currently active.
* 
*   Returns:
*     'true' if number lock is active, or 'false' if it is not
*/
bool    KeyboardNumLock( void );

/****** Mouse ***********************************************************************/
/**** Get Mouse *****************************************************************/
/*
*   Description:
*      Get the mouse info structure. The pointer will be constant for the
*   lifetime of the program.
* 
*   Returns:
*     The mouse struct pointer
*/
const IC_MOUSE* MouseGetMouse( void );

/**** Mouse Mode ****************************************************************/
/*
*   Description:
*     Get the current mouse cursor mode. If 'Capture' or 'Free' are called
*   on this frame, the mouse mode and state will only be updated on the
*   next frame.
* 
*   Returns:
*     The current mouse mode
*/
eIC_MOUSE_MODE MouseGetMode( void );
/*
*   Description:
*     Set the mouse cursor to be captured to the game window. If the mouse
*   is currently captured, this will do nothing.
*/
void    MouseCapture( void );
/*
*   Description:
*     Set the mouse cursor to be freed from the game window. If the mouse
*   is currently freed, this will do nothing.
*/
void    MouseFree( void );

/**** Cursor Display ************************************************************/
/*
*   Description:
*     Manually hide the mouse cursor from the game window. This is
*   automatically done when capturing the mouse.
*/
void    MouseHide( void );
/*
*   Description:
*     Manually show the mouse cursor from the game window. This is
*   automatically done when freeing the mouse.
*/
void    MouseShow( void );

EXTERN_END

#endif/*H_IC_INPUT*/
