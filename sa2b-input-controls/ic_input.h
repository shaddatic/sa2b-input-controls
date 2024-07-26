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
/*  Macro               */
/************************/
/* Normalize PDS Directional -128~128 to -1~1 */
#define NORM_PDS_DIR(a)     ((f32)(a)/(f32)PDSLIM_XY_MAX)

/* Normalize PDS Trigger 0~255 to 0~1 */
#define NORM_PDS_TRIG(a)    ((f32)(a)/(f32)PDSLIM_LR_MAX)

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

/************************/
/*  Enums               */
/************************/
typedef enum
{
    USER_1,
    USER_2,
    USER_3,
    USER_4,

    NB_USER,
}
eUSER_NUM;

typedef enum
{
    INPUT_MD_NONE = -1,

    INPUT_MD_KEYBOARD,
    INPUT_MD_GAMEPAD,
    INPUT_MD_SWITCH,
    INPUT_MD_BOTH,
}
eINPUT_MODE;

typedef enum
{
    GAMEPAD_NONE = -1,

    GAMEPAD_1,
    GAMEPAD_2,
    GAMEPAD_3,
    GAMEPAD_4,

    NB_GAMEPAD,
}
eGAMEPAD_NUM;

typedef enum
{
    KEYBOARD_NONE = -1,

    KEYBOARD_1,
    KEYBOARD_2,
    KEYBOARD_3,
    KEYBOARD_4,

    NB_KEYBOARD,
}
eKEYBOARD_NUM;

typedef enum
{
    DZ_MD_CIRCULAR,
    DZ_MD_SQUARE,
}
eDZ_MODE;

typedef enum
{
    CURSOR_FREE,        /* cursor is unlocked from the center of the game window    */
    CURSOR_CAPTURED,    /* cursor is locked to the center of the game window        */
}
eCURSOR_STATE;

/************************/
/*  Structures          */
/************************/
typedef struct
{
    uint32_t down;
    uint32_t press;
    uint32_t release;

    f32 l, r;

    f32 x1, y1;
    f32 x2, y2;
}
USER_INPUT;

typedef struct
{
    void*               pSdlGp;
    int                 id;

    const char*         name;

    u32                 support;

    u32                 down;

    s16                 l, r;

    s16                 x1, y1;
    s16                 x2, y2;
}
GAMEPAD;

typedef struct
{
    uint8_t down;
    uint8_t press;
    uint8_t release;

    f32     wheel;

    NJS_POINT2I vec;
    NJS_POINT2I pos;
}
MOUSE;

/************************/
/*  Functions           */
/************************/
/****** Init ************************************************************************/
/*
*   Description:
*     Init Input module, and its sub-modules
*/
void    IC_InputInit(void);

/****** User Input ******************************************************************/
/*
*   Description:
*     Get the raw USER_INPUT for a specified user
*/
const USER_INPUT* UserGetInput( eUSER_NUM nbUser );
/*
*   Description:
*     Get the input mode for a specified user
*/
eINPUT_MODE UserGetInputMode( eUSER_NUM nbUser );
/*
*   Description:
*     Get the gamepad number for a specified user
*/
eGAMEPAD_NUM UserGetGamepadNum( eUSER_NUM nbUser );
/*
*   Description:
*     Get the keyboard layout number for a specified user
*/
eKEYBOARD_NUM UserGetKeyboardNum( eUSER_NUM nbUser );

/****** Gamepad *********************************************************************/
const GAMEPAD* GamepadGetGamepad( eGAMEPAD_NUM nbGp );

/*
*   Description:
*     If the specified gamepad is on and available.
*/
bool    GamepadValid( eGAMEPAD_NUM nbGp );
/*
*   Description:
*     Set vibration for a specified gamepad.
*/
bool    GamepadSetVibration( eGAMEPAD_NUM nbGp, f32 spdLo, f32 spdHi );
/*
*   Description:
*     Set vibration for a specified gamepad.
*/
bool    GamepadSetTriggerVibration( eGAMEPAD_NUM nbGp, f32 spdL, f32 spdR );

/****** Keyboard ********************************************************************/
u8      KeyboardPoll( void ); /* poll most recent key press                         */

bool    KeyboardDown(    u8 key );
bool    KeyboardPress(   u8 key );
bool    KeyboardRelease( u8 key );

bool    KeyboardCapsLock(   void );
bool    KeyboardScrollLock( void );
bool    KeyboardNumLock(    void );

/****** Mouse ***********************************************************************/
/*
*   Description:
*     Get the MOUSE input structure
*/
const MOUSE* MouseGetMouse( void );
/*
*   Description:
*     Get the mouse mode
*/
eCURSOR_STATE MouseGetMode( void );
/*
*   Description:
*     Get the mouse mode
*/
void    MouseCapture( void );
/*
*   Description:
*     Get the mouse mode
*/
void    MouseFree( void );

EXTERN_END

#endif/*H_IC_INPUT*/
