/*
*   SA2 Input Controls - '/inputcont_api.h'
*
*   Description:
*     External header for interfacing with Input Controls' mod API. For instructions
*   on usage, read the 'readme.md' file.
*
*   Contributors:
*     - Shaddatic
*
*   Feel free to import this file into your project
*/
#ifndef H_IC_EXTERN_API
#define H_IC_EXTERN_API

/************************/
/*  Typedefs            */
/************************/
/****** Base Types ******************************************************************/
typedef float               f32;    /* 4 byte real number                           */
typedef double              f64;    /* 8 byte real number                           */

/****** String Formats **************************************************************/
typedef char                utf8;   /* supports UTF-8 strings                       */

/************************/
/*  Constants           */
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

/****** Support *********************************************************************/
#define GPDDEV_SUPPORT_A              (1<< 0) /* button a                           */
#define GPDDEV_SUPPORT_B              (1<< 1) /* button b                           */
#define GPDDEV_SUPPORT_X              (1<< 2) /* button x                           */
#define GPDDEV_SUPPORT_Y              (1<< 3) /* button y                           */
#define GPDDEV_SUPPORT_BACK           (1<< 4) /* button back                        */
#define GPDDEV_SUPPORT_GUIDE          (1<< 5) /* xb/ps button, ns: home button      */
#define GPDDEV_SUPPORT_START          (1<< 6) /* button start                       */
#define GPDDEV_SUPPORT_LS             (1<< 7) /* button ls                     (l3) */
#define GPDDEV_SUPPORT_RS             (1<< 8) /* button rs                     (r3) */
#define GPDDEV_SUPPORT_ZL             (1<< 9) /* button zl                 (lb, l1) */
#define GPDDEV_SUPPORT_ZR             (1<<10) /* button zr              (z, rb, r1) */
#define GPDDEV_SUPPORT_DPAD_UP        (1<<11) /* directional pad, up                */
#define GPDDEV_SUPPORT_DPAD_DOWN      (1<<12) /* directional pad, down              */
#define GPDDEV_SUPPORT_DPAD_LEFT      (1<<13) /* directional pad, left              */
#define GPDDEV_SUPPORT_DPAD_RIGHT     (1<<14) /* directional pad, right             */
#define GPDDEV_SUPPORT_MISC           (1<<15) /* xb: share, ps: mic, ns: capture    */
#define GPDDEV_SUPPORT_XBPADDLE_UR    (1<<16) /* xb paddle: upper right (fr. front) */
#define GPDDEV_SUPPORT_XBPADDLE_UL    (1<<17) /* xb paddle: upper left  (fr. front) */
#define GPDDEV_SUPPORT_XBPADDLE_LR    (1<<18) /* xb paddle: lower right (fr. front) */
#define GPDDEV_SUPPORT_XBPADDLE_LL    (1<<19) /* xb paddle: lower left  (fr. front) */
#define GPDDEV_SUPPORT_TOUCHPAD       (1<<20) /* ps 4/5 touchpad button             */
#define GPDDEV_SUPPORT_RUMBLE_TRIGGER (1<<30) /* supports rumble triggers           */
#define GPDDEV_SUPPORT_RUMBLE         (1<<31) /* supports rumble                    */

/****** Keyboard Keys ***************************************************************/
/****** Main Keys ***********************************************************/
#define KEY_BACKSPACE       (0x08) /* backspace key                         */
#define KEY_TAB             (0x09) /* tab key                               */
#define KEY_CLEAR           (0x0C) /* clear key         (only some layouts) */
#define KEY_RETURN          (0x0D) /* return/enter key                      */
#define KEY_PAUSE           (0x13) /* pause key                             */
#define KEY_ESCAPE          (0x1B) /* 'esc' key                             */
#define KEY_SPACE           (0x20) /* space key                             */
#define KEY_PAGE_UP         (0x21) /* page up key                           */
#define KEY_PAGE_DOWN       (0x22) /* page down key                         */
#define KEY_END             (0x23) /* end key                               */
#define KEY_HOME            (0x24) /* home key                              */
#define KEY_ARROW_LEFT      (0x25) /* arrow left key                        */
#define KEY_ARROW_UP        (0x26) /* arrow up key                          */
#define KEY_ARROW_RIGHT     (0x27) /* arrow right key                       */
#define KEY_ARROW_DOWN      (0x28) /* arrow down key                        */
#define KEY_PRINTSCREEN     (0x2C) /* printscreen key                       */
#define KEY_INSERT          (0x2D) /* insert key                            */
#define KEY_DELETE          (0x2E) /* delete key                            */
#define KEY_HELP            (0x2F) /* help key          (only some layouts) */
#define KEY_SEMICOLON       (0xBA) /* ';' key                               */
#define KEY_EQUALS          (0xBB) /* '=' key                               */
#define KEY_COMMA           (0xBC) /* ',' key                               */
#define KEY_SUBTRACT        (0xBD) /* '-' key                               */
#define KEY_PERIOD          (0xBE) /* '.' key                               */
#define KEY_FORWARDSLASH    (0xBF) /* '/' key                               */
#define KEY_TILDE           (0xC0) /* '~' key                               */
#define KEY_SQUAREBRACKET_L (0xDB) /* '[' key                               */
#define KEY_BACKSLASH       (0xDC) /* '\' key                               */
#define KEY_SQUAREBRACKET_R (0xDD) /* ']' key                               */
#define KEY_QUOTE           (0xDE) /* ''' key                               */
/****** Numbers ('0' <-> '9') ***********************************************/
/****** Letters ('A' <-> 'Z') ***********************************************/
/****** Super/Windows *******************************************************/
#define KEY_SUPER_L         (0x5B) /* left super/windows key                */
#define KEY_SUPER_R         (0x5C) /* right super/windows key               */
/****** Numpad Keys *********************************************************/
#define KEY_NUM_0           (0x60) /* numpad 0           (requires numlock) */
#define KEY_NUM_1           (0x61) /* numpad 1           (requires numlock) */
#define KEY_NUM_2           (0x62) /* numpad 2           (requires numlock) */
#define KEY_NUM_3           (0x63) /* numpad 3           (requires numlock) */
#define KEY_NUM_4           (0x64) /* numpad 4           (requires numlock) */
#define KEY_NUM_5           (0x65) /* numpad 5           (requires numlock) */
#define KEY_NUM_6           (0x66) /* numpad 6           (requires numlock) */
#define KEY_NUM_7           (0x67) /* numpad 7           (requires numlock) */
#define KEY_NUM_8           (0x68) /* numpad 8           (requires numlock) */
#define KEY_NUM_9           (0x69) /* numpad 9           (requires numlock) */
#define KEY_NUM_MULTIPLY    (0x6A) /* numpad '*'                            */
#define KEY_NUM_PLUS        (0x6B) /* numpad '+'                            */
#define KEY_NUM_SEPARATOR   (0x6C) /* numpad ',' or '.' (only some layouts) */
#define KEY_NUM_SUBTRACT    (0x6D) /* numpad '-'                            */
#define KEY_NUM_DECIMAL     (0x6E) /* numpad '.'         (requires numlock) */
#define KEY_NUM_DIVIDE      (0x6F) /* numpad '/'                            */
/****** Function Keys *******************************************************/
#define KEY_F1              (0x70) /* function key 1                        */
#define KEY_F2              (0x71) /* function key 2                        */
#define KEY_F3              (0x72) /* function key 3                        */
#define KEY_F4              (0x73) /* function key 4                        */
#define KEY_F5              (0x74) /* function key 5                        */
#define KEY_F6              (0x75) /* function key 6                        */
#define KEY_F7              (0x76) /* function key 7                        */
#define KEY_F8              (0x77) /* function key 8                        */
#define KEY_F9              (0x78) /* function key 9                        */
#define KEY_F10             (0x79) /* function key 10                       */
#define KEY_F11             (0x7A) /* function key 11                       */
#define KEY_F12             (0x7B) /* function key 12                       */
#define KEY_F13             (0x7C) /* function key 13                       */
#define KEY_F14             (0x7D) /* function key 14                       */
#define KEY_F15             (0x7E) /* function key 15                       */
#define KEY_F16             (0x7F) /* function key 16                       */
#define KEY_F17             (0x80) /* function key 17                       */
#define KEY_F18             (0x81) /* function key 18                       */
#define KEY_F19             (0x82) /* function key 19                       */
#define KEY_F20             (0x83) /* function key 20                       */
#define KEY_F21             (0x84) /* function key 21                       */
#define KEY_F22             (0x85) /* function key 22                       */
#define KEY_F23             (0x86) /* function key 23                       */
#define KEY_F24             (0x87) /* function key 24                       */
/****** 'Lock' Keys *********************************************************/
#define KEY_CAPSLOCK        (0x14) /* capital lock                          */
#define KEY_NUMLOCK         (0x90) /* number lock                           */
#define KEY_SCROLLLOCK      (0x91) /* scroll lock                           */
/****** Modifier Keys *******************************************************/
#define KEY_SHIFT_L         (0xA0) /* left shift key                        */
#define KEY_SHIFT_R         (0xA1) /* right shift key                       */
#define KEY_CONTROL_L       (0xA2) /* left control key                      */
#define KEY_CONTROL_R       (0xA3) /* right control key                     */
#define KEY_ALT_L           (0xA4) /* left alt key                          */
#define KEY_ALT_R           (0xA5) /* right alt key                         */
/****** L or R **************************************************************/
#define KEY_SHIFT           (0x10) /* any shift key                         */
#define KEY_CONTROL         (0x11) /* any control key                       */
/****** Other ***************************************************************/
#define KEY_CTRLBREAK       (0x03) /* ctrl+pause/break                      */
/****** Mouse 'Keys' ********************************************************/
#define KEY_M_LCLICK        (0x01) /* mouse left button                     */
#define KEY_M_RCLICK        (0x02) /* mouse right button                    */
#define KEY_M_MCLICK        (0x04) /* mouse middle button                   */
#define KEY_M_X1            (0x05) /* mouse side/eXtra button 1             */
#define KEY_M_X2            (0x06) /* mouse side/eXtra button 2             */
/****** No Key **************************************************************/
#define KEY_NONE            (0xFF) /* no key                                */

/****** Mouse Buttons ***************************************************************/
#define MSEBTN_LEFT         (1<<0) /* left click                                    */
#define MSEBTN_RIGHT        (1<<1) /* right click                                   */
#define MSEBTN_MIDDLE       (1<<2) /* mouse wheel button                            */
#define MSEBTN_X1           (1<<3) /* side/eXtra button 1                           */
#define MSEBTN_X2           (1<<4) /* side/eXtra button 2                           */

/************************/
/*  Enums               */
/************************/
/****** ################## **********************************************************/
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
    IC_INPUT_MD_NONE = -1,

    IC_INPUT_MD_KEYBOARD,
    IC_INPUT_MD_GAMEPAD,
    IC_INPUT_MD_SWITCH,
    IC_INPUT_MD_BOTH,
}
eIC_INPUT_MODE;

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
    IC_MOUSE_MD_FREE,
    IC_MOUSE_MD_CAPTURED,
}
eIC_MOUSE_MODE;

/************************/
/*  Structures          */
/************************/
/****** ################## **********************************************************/
typedef void                IC_USER_INPUT;
typedef void                IC_MOUSE;
typedef void                IC_GAMEPAD;

/************************/
/*  Config API          */
/************************/
/*
*   
*/
typedef struct
{
    /****** Version >= 0 ************************************************************/
    uint32_t version;

    /**** Get Config ********************************************************/
    /*
    *   Description:
    *     Read user-settings inside Input Controls' configuration file. It's
    *   important to remember that strings are stored in UTF-8 format.
    *
    *   Parameters:
    *     - uSect   : config setting section
    *     - uKey    : config setting key
    *     - def     : default config value
    *
    *   Returns:
    *     The value of the config entry, or 'def' if no entry exists
    */
    int32_t    (__cdecl* GetInt)(    const utf8* uSect, const utf8* uKey, int32_t     def );
    bool       (__cdecl* GetBool)(   const utf8* uSect, const utf8* uKey, bool        def );
    f64        (__cdecl* GetFloat)(  const utf8* uSect, const utf8* uKey, f64         def );
    const utf8*(__cdecl* GetString)( const utf8* uSect, const utf8* uKey, const utf8* def );

    /**** Get Config (IC Specific) ******************************************/
    /*
    *   Description:
    *     Get a percentage config option, with '0' representing 0% and '1'
    *   representing 100%.
    *
    *   Parameters:
    *     - uSect   : config setting section
    *     - uKey    : config setting key
    *     - def     : default percentage value (0~100)
    *
    *   Returns:
    *     The percentage of the config entry or 'def' if no entry exists (0~1)
    */
    f64 (__cdecl* GetPercent)( const utf8* uSect, const utf8* uKey, int32_t def );
}
ICAPI_CONFIG;

/************************/
/*  Feature API         */
/************************/
/*
*   
*/
typedef struct
{
    /****** Version >= 0 ************************************************************/
    uint32_t version;

    /**** Features **********************************************************/
    /*
    *   Description:
    *     If Input Controls is set to use raw analog values, rather than
    *   values from the emulated Dreamcast controller.
    *
    *   Returns:
    *     If the feature is enabled
    */
    bool (__cdecl* UseRawAnalog)( void );
    /*
    *   Description:
    *     If the camera is set to invert the X axis of the right analog
    *   stick input from vanilla. When inverted, it better matches regular
    *   camera input from other games.
    *
    *   Returns:
    *     If the feature is enabled
    */
    bool (__cdecl* CamInvertX2)( void );
}
ICAPI_FEATURE;

/************************/
/*  User Input API      */
/************************/
/*
*   
*/
typedef struct
{
    /****** Version >= 0 ************************************************************/
    uint32_t version;

    /**** Raw Input *********************************************************/
    /*
    *   Description:
    *     Get the raw user input. The pointer will be constant for the
    *   lifetime of the program.
    *
    *   Parameters:
    *     - nbUser  : user number to get raw input of
    *
    *   Returns:
    *     A pointer to the specified user's raw input structure
    */
    const IC_USER_INPUT* (__cdecl* GetInput)( eIC_USER_NUM nbUser );

    /**** Input Settings ****************************************************/
    /*
    *   Description:
    *
    *   Parameters:
    *     - nbUser  : user number to get raw input of
    *
    *   Returns:
    *     The user's set input mode
    */
    eIC_INPUT_MODE (__cdecl* GetInputMode)( eIC_USER_NUM nbUser );
    /*
    *   Description:
    *
    *   Parameters:
    *     - nbUser  : user number to get raw input of
    *
    *   Returns:
    *     The user's set gamepad number
    */
    eIC_GAMEPAD_NUM (__cdecl* GetGamepadNum)( eIC_USER_NUM nbUser );
    /*
    *   Description:
    *
    *   Parameters:
    *     - nbUser  : user number to get raw input of
    *
    *   Returns:
    *     The user's set keyboard layout number
    */
    eIC_KEYBOARD_NUM (__cdecl* GetKeyboardNum)( eIC_USER_NUM nbUser );
}
ICAPI_USER;

/************************/
/*  Gamepad API         */
/************************/
/*
*   
*/
typedef struct
{
    /****** Version >= 0 ************************************************************/
    uint32_t version;

    /**** Raw Gamepad *******************************************************/
    /*
    *   Description:
    *     Get raw gamepad input and attributes. The pointer will be constant
    *   for the lifetime of the program.
    * 
    *   Parameters:
    *     - nbGp    : gamepad number to get
    * 
    *   Returns:
    *     Gamepad struct pointer, or nullptr if 'nbGp' is GAMEPAD_NONE.
    */
    const IC_GAMEPAD* (__cdecl* GetGamepad)( eIC_GAMEPAD_NUM nbGp );

    /**** Valid/Open *******************************************************/
    /*
    *   Description:
    *     Check if the given gamepad is active and linked to an open, 
    *   physical device.
    * 
    *   Parameters:
    *     - nbGp    : gamepad number to get state of
    * 
    *   Returns:
    *     If the given gamepad is valid.
    */
    bool (__cdecl* Valid)( eIC_GAMEPAD_NUM nbGp );

    /**** Vibration *********************************************************/
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
    *     If successful
    */
    bool (__cdecl* SetVibration)( eIC_GAMEPAD_NUM nbGp, f32 freqLo, f32 freqHi );
    /*
    *   Description:
    *     Set the trigger vibration motors on a gamepad. Only applicable to
    *   Xbox One and higher controllers. You can check support by searching
    *   for the GPDDEV_SUPPORT_RUMBLE_TRIGGER flag in 'support'.
    * 
    *   Parameters:
    *     - nbGp    : gamepad number to set vibration for
    *     - freqL   : speed of the left trigger motor (0~1)
    *     - freqR   : speed of the right trigger motor (0~1)
    * 
    *   Returns:
    *     If successful
    */
    bool (__cdecl* SetTriggerVibration)( eIC_GAMEPAD_NUM nbGp, f32 freqL, f32 freqR );
}
ICAPI_GAMEPAD;

/************************/
/*  Keyboard API        */
/************************/
/*
*   
*/
typedef struct
{
    /****** Version >= 0 ************************************************************/
    uint32_t version;

    /**** Poll Key **********************************************************/
    /*
    *   Description:
    *     Get the most recent key press from the keyboard. If multiple keys are
    *   pressed at the same time, the highest key index will be returned.
    * 
    *   Returns:
    *     Most recent key index
    */
    uint8_t (__cdecl* Poll)( void );

    /**** Key State *********************************************************/
    /*
    *   Description:
    *     Check if 'key' is currently being held down.
    *   
    *   Parameters:
    *     - key     : key index to check
    * 
    *   Returns:
    *     If 'key' is down
    */
    bool (__cdecl* Down)( uint8_t key );
    /*
    *   Description:
    *     Check if 'key' was pressed now.
    *   
    *   Parameters:
    *     - key     : key index to check
    * 
    *   Returns:
    *     If 'key' was pressed this frame
    */
    bool (__cdecl* Press)( uint8_t key );
    /*
    *   Description:
    *     Check if 'key' was released now.
    *   
    *   Parameters:
    *     - key     : key index to check
    * 
    *   Returns:
    *     If 'key' was released this frame
    */
    bool (__cdecl* Release)( uint8_t key );

    /**** Modifier State ****************************************************/
    /*
    *   Description:
    *     Check if Caps Lock modifier is currently active.
    * 
    *   Returns:
    *     If CapsLock key is toggled
    */
    bool (__cdecl* CapsLock)( void );
    /*
    *   Description:
    *     Check if Scroll Lock modifier is currently active.
    * 
    *   Returns:
    *     If ScrollLock key is toggled
    */
    bool (__cdecl* ScrollLock)( void );
    /*
    *   Description:
    *     Check if Number Lock modifier is currently active.
    * 
    *   Returns:
    *     If NumLock key is toggled
    */
    bool (__cdecl* NumLock)( void );
}
ICAPI_KEYBOARD;

/************************/
/*  Mouse API           */
/************************/
/*
* 
*/
typedef struct
{
    /****** Version >= 0 ************************************************************/
    uint32_t version;

    /**** Get Mouse *********************************************************/
    /*
    *   Description:
    *      Get the mouse info structure. The pointer will be constant for
    *   the lifetime of the program.
    * 
    *   Returns:
    *     Mouse struct pointer
    */
    const IC_MOUSE* (__cdecl* GetMouse)( void );

    /**** Mouse Mode ********************************************************/
    /*
    *   Description:
    *     
    *   Returns:
    *     Current mouse mode
    */
    eIC_MOUSE_MODE (__cdecl* GetMode)( void );
    /*
    *   Description:
    */
    void (__cdecl* Capture)( void );
    /*
    *   Description:
    */
    void (__cdecl* Free)( void );

    /**** Cursor Display ****************************************************/
    /*
    *   Description:
    *     Manually hide the mouse cursor from the game window. This is
    *   automatically done when capturing the mouse.
    */
    void (__cdecl* Hide)( void );
    /*
    *   Description:
    *     Manually show the mouse cursor from the game window. This is
    *   automatically done when freeing the mouse.
    */
    void (__cdecl* Show)( void );
}
ICAPI_MOUSE;

/************************/
/*  Window API          */
/************************/
/*
* 
*/
typedef struct
{
    /****** Version >= 0 ************************************************************/
    uint32_t version;

    /**** Focus *************************************************************/
    /*
    *   Description:
    *     Check if the main game window is in focus.
    * 
    *   Returns:
    *     Window focus state
    */
    bool (__cdecl* InFocus)( void );

    /**** Parameters ********************************************************/
    /*
    *   Description:
    *     Get the size of the game window in physical pixels.
    * 
    *   Parameters:
    *     - pOutSize : return pointer for window size
    */
    void (__cdecl* GetSize)( NJS_POINT2I* pOutSize );
    /*
    *   Description:
    *     Get the game's internal rendering resolution.
    * 
    *   Parameters:
    *     - pOutRes : return pointer for game resolution
    */
    void (__cdecl* GetGameResolution)( NJS_POINT2I* pOutRes );
}
ICAPI_WINDOW;

/************************/
/*  SDL API             */
/************************/
/*
* 
*/
typedef struct
{
    uint8_t release, major, minor, ___reserved; 
}
SDL_VERSION;

typedef struct
{
    /****** Version >= 0 ************************************************************/
    uint32_t version;

    /**** SDL API Version ***************************************************/
    SDL_VERSION sdl_version;

    /**** DLL Library *******************************************************/
    /*
    *   Description:
    *     Get HMODULE for mounted SDL library.
    *     
    *   Returns:
    *     HMODULE for mounted SDL DLL file.
    */
    void* (__cdecl* GetHandle)( void );
    /*
    *   Description:
    *     Get a DLL export from the mounted SDL library.
    * 
    *   Parameters:
    *     cExName   : name of DLL export
    *     
    *   Returns:
    *     Pointer to the DLL export, or nullptr if nothing is exported under
    *   that name.
    */
    void* (__cdecl* GetExport)( const char* cExName );
}
ICAPI_SDL;

/************************/
/*  Core API            */
/************************/
/*
*   The Input Controls 'Core' API is the central hub for all the other API modules.
*   It can be accessed through one of the Input Controls mod function exports as a
*   parameter, or directly by searching Input Controls for the 'icapi_core' dllexport.
*/
typedef struct
{
    uint8_t release, major, minor, hotfix;
}
IC_VERSION;

typedef struct
{
    /****** Version >= 0 ************************************************************/
    uint32_t version;

    /**** Mod Version *******************************************************/
    IC_VERSION ic_version;

    /**** APIs **************************************************************/
    const ICAPI_FEATURE*    pApiFeature;

    const ICAPI_CONFIG*     pApiConfig;

    const ICAPI_USER*       pApiUser;
    const ICAPI_GAMEPAD*    pApiGamepad;
    const ICAPI_KEYBOARD*   pApiKeyboard;
    const ICAPI_MOUSE*      pApiMouse;
    const ICAPI_WINDOW*     pApiWindow;
    const ICAPI_SDL*        pApiSdl;
}
ICAPI_CORE;

#endif/*H_IC_EXTERN_API*/
