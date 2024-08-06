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
#include <sa2b/ninja/njcommon.h>    /* NJS_POINT2I                                  */

/****** Input Controls **************************************************************/
#include <ic_limits.h>              /* limits                                       */

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
/*  User Input          */
/************************/
/****** User Buttons ****************************************************************/
#define USRBTN_A                    (1<< 0) /* button a                             */
#define USRBTN_B                    (1<< 1) /* button b                             */
#define USRBTN_X                    (1<< 2) /* button x                             */
#define USRBTN_Y                    (1<< 3) /* button y                             */
#define USRBTN_START                (1<< 4) /* button start                         */
#define USRBTN_BACK                 (1<< 5) /* button back                          */
#define USRBTN_ZL                   (1<< 6) /* button zl                   (lb, l1) */
#define USRBTN_ZR                   (1<< 7) /* button zr                (z, rb, r1) */
#define USRBTN_LS                   (1<< 8) /* button ls                       (l3) */
#define USRBTN_RS                   (1<< 9) /* button rs                       (r3) */
#define USRBTN_DPAD_UP              (1<<10) /* directional pad, up                  */
#define USRBTN_DPAD_DOWN            (1<<11) /* directional pad, down                */
#define USRBTN_DPAD_LEFT            (1<<12) /* directional pad, left                */
#define USRBTN_DPAD_RIGHT           (1<<13) /* directional pad, right               */

/****** Raw Gamepad Buttons *********************************************************/
#define GPDBTN_A                    (1<< 0) /* button a                             */
#define GPDBTN_B                    (1<< 1) /* button b                             */
#define GPDBTN_X                    (1<< 2) /* button x                             */
#define GPDBTN_Y                    (1<< 3) /* button y                             */
#define GPDBTN_BACK                 (1<< 4) /* button back                          */
#define GPDBTN_GUIDE                (1<< 5) /* xb/ps button, ns: home button        */
#define GPDBTN_START                (1<< 6) /* button start                         */
#define GPDBTN_LS                   (1<< 7) /* button ls                       (l3) */
#define GPDBTN_RS                   (1<< 8) /* button rs                       (r3) */
#define GPDBTN_ZL                   (1<< 9) /* button zl                   (lb, l1) */
#define GPDBTN_ZR                   (1<<10) /* button zr                (z, rb, r1) */
#define GPDBTN_DPAD_UP              (1<<11) /* directional pad, up                  */
#define GPDBTN_DPAD_DOWN            (1<<12) /* directional pad, down                */
#define GPDBTN_DPAD_LEFT            (1<<13) /* directional pad, left                */
#define GPDBTN_DPAD_RIGHT           (1<<14) /* directional pad, right               */
#define GPDBTN_MISC                 (1<<15) /* xb: share, ps: mic, ns: capture      */
#define GPDBTN_XBPADDLE_UR          (1<<16) /* xb paddle: upper right  (from front) */
#define GPDBTN_XBPADDLE_UL          (1<<17) /* xb paddle: upper left   (from front) */
#define GPDBTN_XBPADDLE_LR          (1<<18) /* xb paddle: lower right  (from front) */
#define GPDBTN_XBPADDLE_LL          (1<<19) /* xb paddle: lower left   (from front) */
#define GPDBTN_TOUCHPAD             (1<<20) /* ps 4/5 touchpad button               */

/************************/
/*  Gamepad Flags       */
/************************/
/****** Support *********************************************************************/
#define GPDDEV_SUPPORT_A                (1<< 0) /* button a                         */
#define GPDDEV_SUPPORT_B                (1<< 1) /* button b                         */
#define GPDDEV_SUPPORT_X                (1<< 2) /* button x                         */
#define GPDDEV_SUPPORT_Y                (1<< 3) /* button y                         */
#define GPDDEV_SUPPORT_BACK             (1<< 4) /* button back                      */
#define GPDDEV_SUPPORT_GUIDE            (1<< 5) /* xb/ps button, ns: home button    */
#define GPDDEV_SUPPORT_START            (1<< 6) /* button start                     */
#define GPDDEV_SUPPORT_LS               (1<< 7) /* button ls                   (l3) */
#define GPDDEV_SUPPORT_RS               (1<< 8) /* button rs                   (r3) */
#define GPDDEV_SUPPORT_ZL               (1<< 9) /* button zl               (lb, l1) */
#define GPDDEV_SUPPORT_ZR               (1<<10) /* button zr            (z, rb, r1) */
#define GPDDEV_SUPPORT_DPAD_UP          (1<<11) /* directional pad, up              */
#define GPDDEV_SUPPORT_DPAD_DOWN        (1<<12) /* directional pad, down            */
#define GPDDEV_SUPPORT_DPAD_LEFT        (1<<13) /* directional pad, left            */
#define GPDDEV_SUPPORT_DPAD_RIGHT       (1<<14) /* directional pad, right           */
#define GPDDEV_SUPPORT_MISC             (1<<15) /* xb: share, ps: mic, ns: capture  */
#define GPDDEV_SUPPORT_XBPADDLE_UR      (1<<16) /* xb paddle: upper right   (front) */
#define GPDDEV_SUPPORT_XBPADDLE_UL      (1<<17) /* xb paddle: upper left    (front) */
#define GPDDEV_SUPPORT_XBPADDLE_LR      (1<<18) /* xb paddle: lower right   (front) */
#define GPDDEV_SUPPORT_XBPADDLE_LL      (1<<19) /* xb paddle: lower left    (front) */
#define GPDDEV_SUPPORT_TOUCHPAD         (1<<20) /* ps 4/5 touchpad button           */
#define GPDDEV_SUPPORT_RUMBLE_TRIGGER   (1<<30) /* supports rumble triggers         */
#define GPDDEV_SUPPORT_RUMBLE           (1<<31) /* supports rumble                  */

/************************/
/*  Mouse Input         */
/************************/
/****** Mouse Buttons ***************************************************************/
#define MSEBTN_LEFT                     (1<<0)  /* left click                       */
#define MSEBTN_RIGHT                    (1<<1)  /* right click                      */
#define MSEBTN_MIDDLE                   (1<<2)  /* mouse wheel button               */
#define MSEBTN_X1                       (1<<3)  /* side/eXtra button 1              */
#define MSEBTN_X2                       (1<<4)  /* side/eXtra button 2              */

/************************/
/*  Keyboard Keys       */
/************************/
/****** Main Keys *******************************************************************/
#define KEY_BACKSPACE       (0x08)  /* backspace key                                */
#define KEY_TAB             (0x09)  /* tab key                                      */
#define KEY_CLEAR           (0x0C)  /* clear key                (only some layouts) */
#define KEY_RETURN          (0x0D)  /* return/enter key                             */
#define KEY_PAUSE           (0x13)  /* pause key                                    */
#define KEY_ESCAPE          (0x1B)  /* 'esc' key                                    */
#define KEY_SPACE           (0x20)  /* space key                                    */
#define KEY_PAGE_UP         (0x21)  /* page up key                                  */
#define KEY_PAGE_DOWN       (0x22)  /* page down key                                */
#define KEY_END             (0x23)  /* end key                                      */
#define KEY_HOME            (0x24)  /* home key                                     */
#define KEY_ARROW_LEFT      (0x25)  /* arrow left key                               */
#define KEY_ARROW_UP        (0x26)  /* arrow up key                                 */
#define KEY_ARROW_RIGHT     (0x27)  /* arrow right key                              */
#define KEY_ARROW_DOWN      (0x28)  /* arrow down key                               */
#define KEY_PRINTSCREEN     (0x2C)  /* printscreen key                              */
#define KEY_INSERT          (0x2D)  /* insert key                                   */
#define KEY_DELETE          (0x2E)  /* delete key                                   */
#define KEY_HELP            (0x2F)  /* help key                 (only some layouts) */
#define KEY_SEMICOLON       (0xBA)  /* ';' key                                      */
#define KEY_EQUALS          (0xBB)  /* '=' key                                      */
#define KEY_COMMA           (0xBC)  /* ',' key                                      */
#define KEY_SUBTRACT        (0xBD)  /* '-' key                                      */
#define KEY_PERIOD          (0xBE)  /* '.' key                                      */
#define KEY_FORWARDSLASH    (0xBF)  /* '/' key                                      */
#define KEY_TILDE           (0xC0)  /* '~' key                                      */
#define KEY_SQUAREBRACKET_L (0xDB)  /* '[' key                                      */
#define KEY_BACKSLASH       (0xDC)  /* '\' key                                      */
#define KEY_SQUAREBRACKET_R (0xDD)  /* ']' key                                      */
#define KEY_QUOTE           (0xDE)  /* ''' key                                      */
/****** Numbers ('0' <-> '9') *******************************************************/
/****** Letters ('A' <-> 'Z') *******************************************************/
/****** Super/Windows ***************************************************************/
#define KEY_SUPER_L         (0x5B)  /* left super/windows key                       */
#define KEY_SUPER_R         (0x5C)  /* right super/windows key                      */
/****** Numpad Keys *****************************************************************/
#define KEY_NUM_0           (0x60)  /* numpad 0                  (requires numlock) */
#define KEY_NUM_1           (0x61)  /* numpad 1                  (requires numlock) */
#define KEY_NUM_2           (0x62)  /* numpad 2                  (requires numlock) */
#define KEY_NUM_3           (0x63)  /* numpad 3                  (requires numlock) */
#define KEY_NUM_4           (0x64)  /* numpad 4                  (requires numlock) */
#define KEY_NUM_5           (0x65)  /* numpad 5                  (requires numlock) */
#define KEY_NUM_6           (0x66)  /* numpad 6                  (requires numlock) */
#define KEY_NUM_7           (0x67)  /* numpad 7                  (requires numlock) */
#define KEY_NUM_8           (0x68)  /* numpad 8                  (requires numlock) */
#define KEY_NUM_9           (0x69)  /* numpad 9                  (requires numlock) */
#define KEY_NUM_MULTIPLY    (0x6A)  /* numpad '*'                                   */
#define KEY_NUM_PLUS        (0x6B)  /* numpad '+'                                   */
#define KEY_NUM_SEPARATOR   (0x6C)  /* numpad ',' or '.'        (only some layouts) */
#define KEY_NUM_SUBTRACT    (0x6D)  /* numpad '-'                                   */
#define KEY_NUM_DECIMAL     (0x6E)  /* numpad '.'                (requires numlock) */
#define KEY_NUM_DIVIDE      (0x6F)  /* numpad '/'                                   */
/****** Function Keys ***************************************************************/
#define KEY_F1              (0x70)  /* function key 1                               */
#define KEY_F2              (0x71)  /* function key 2                               */
#define KEY_F3              (0x72)  /* function key 3                               */
#define KEY_F4              (0x73)  /* function key 4                               */
#define KEY_F5              (0x74)  /* function key 5                               */
#define KEY_F6              (0x75)  /* function key 6                               */
#define KEY_F7              (0x76)  /* function key 7                               */
#define KEY_F8              (0x77)  /* function key 8                               */
#define KEY_F9              (0x78)  /* function key 9                               */
#define KEY_F10             (0x79)  /* function key 10                              */
#define KEY_F11             (0x7A)  /* function key 11                              */
#define KEY_F12             (0x7B)  /* function key 12                              */
#define KEY_F13             (0x7C)  /* function key 13                              */
#define KEY_F14             (0x7D)  /* function key 14                              */
#define KEY_F15             (0x7E)  /* function key 15                              */
#define KEY_F16             (0x7F)  /* function key 16                              */
#define KEY_F17             (0x80)  /* function key 17                              */
#define KEY_F18             (0x81)  /* function key 18                              */
#define KEY_F19             (0x82)  /* function key 19                              */
#define KEY_F20             (0x83)  /* function key 20                              */
#define KEY_F21             (0x84)  /* function key 21                              */
#define KEY_F22             (0x85)  /* function key 22                              */
#define KEY_F23             (0x86)  /* function key 23                              */
#define KEY_F24             (0x87)  /* function key 24                              */
/****** 'Lock' Keys *****************************************************************/
#define KEY_CAPSLOCK        (0x14)  /* capital lock                                 */
#define KEY_NUMLOCK         (0x90)  /* number lock                                  */
#define KEY_SCROLLLOCK      (0x91)  /* scroll lock                                  */
/****** Modifier Keys ***************************************************************/
#define KEY_SHIFT_L         (0xA0)  /* left shift key                               */
#define KEY_SHIFT_R         (0xA1)  /* right shift key                              */
#define KEY_CONTROL_L       (0xA2)  /* left control key                             */
#define KEY_CONTROL_R       (0xA3)  /* right control key                            */
#define KEY_ALT_L           (0xA4)  /* left alt key                                 */
#define KEY_ALT_R           (0xA5)  /* right alt key                                */
/****** L or R **********************************************************************/
#define KEY_SHIFT           (0x10)  /* any shift key                                */
#define KEY_CONTROL         (0x11)  /* any control key                              */
/****** Other ***********************************************************************/
#define KEY_CTRLBREAK       (0x03)  /* ctrl+pause/break                             */
/****** Mouse 'Keys' ****************************************************************/
#define KEY_M_LCLICK        (0x01)  /* mouse left button                            */
#define KEY_M_RCLICK        (0x02)  /* mouse right button                           */
#define KEY_M_MCLICK        (0x04)  /* mouse middle button                          */
#define KEY_M_X1            (0x05)  /* mouse side/eXtra button 1                    */
#define KEY_M_X2            (0x06)  /* mouse side/eXtra button 2                    */
/****** No Key **********************************************************************/
#define KEY_NONE            (0xFF)  /* no key                                       */

#ifndef H_IC_EXTERN_API

/************************/
/*  Enums               */
/************************/
typedef enum
{
    IC_USER_1,
    IC_USER_2,
    IC_USER_3,
    IC_USER_4,

    NB_IC_USER,
}
eIC_USER_NUM;

typedef enum
{
    IC_GAMEPAD_NONE = -1,

    IC_GAMEPAD_1,
    IC_GAMEPAD_2,
    IC_GAMEPAD_3,
    IC_GAMEPAD_4,

    NB_IC_GAMEPAD,
}
eIC_GAMEPAD_NUM;

typedef enum
{
    IC_KEYBOARD_NONE = -1,

    IC_KEYBOARD_1,
    IC_KEYBOARD_2,
    IC_KEYBOARD_3,
    IC_KEYBOARD_4,

    NB_IC_KEYBOARD,
}
eIC_KEYBOARD_NUM;

typedef enum
{
    IC_MOUSE_MD_FREE,     /* cursor is unlocked from the center of the game window  */
    IC_MOUSE_MD_CAPTURED, /* cursor is locked to the center of the game window      */
}
eIC_MOUSE_MODE;

/************************/
/*  Structures          */
/************************/
typedef struct
{
    uint32_t       down; /* user input buttons currently pressed           (USRBTN) */
    uint32_t      press; /* user input buttons pressed this moment         (USRBTN) */
    uint32_t    release; /* user input buttons released this moment        (USRBTN) */

    f32            l, r; /* analog l/r triggers                            (0.f~1.f) */

    f32          x1, y1; /* left analog stick                             (-1.f~1.f) */
    f32          x2, y2; /* right analog stick                            (-1.f~1.f) */
}
IC_USER;

typedef struct
{
    SDL_GameController* pgp; /* SDL gamepad pointer                                 */
    int32_t              id; /* SDL gamepad/joystick index                          */

    const char*        name; /* gamepad name                                        */

    uint32_t        support; /* gamepad supported features                          */

    uint32_t           down; /* currently pressed buttons                 (GPDBTN)  */
    uint32_t          press; /* currently pressed buttons                 (GPDBTN)  */
    uint32_t        release; /* currently pressed buttons                 (GPDBTN)  */
       
    int16_t            l, r; /* analog l/r triggers                       (0~32767) */

    int16_t          x1, y1; /* left analog stick                    (-32768~32767) */
    int16_t          x2, y2; /* right analog stick                   (-32768~32767) */
}
IC_GAMEPAD;

typedef struct
{
    uint8_t        down; /* mouse buttons currently pressed                (MSEBTN) */
    uint8_t       press; /* mouse buttons pressed this moment              (MSEBTN) */
    uint8_t     release; /* mouse buttons released this moment             (MSEBTN) */

    f32          wheelx; /* mouse wheel X rotation, in notches                      */
    f32          wheely; /* mouse wheel Y rotation, in notches                      */

    NJS_POINT2I     vec; /* mouse movement vector, in pixels                        */
    NJS_POINT2I     pos; /* mouse position on the game surface, in pixels           */
}
IC_MOUSE;

#endif/*H_IC_EXTERN_API*/

/************************/
/*  Prototypes          */
/************************/
/****** Init ************************************************************************/
/*
*   Description:
*     Init Input module, and its sub-modules
*/
void    IC_InputInit( void );

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
const IC_USER* UserGetInput( eIC_USER_NUM nbUser );

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
