/*
*   SA2 Input Controls - '/icapi_keyboard.h'
*
*   Description:
*     The 'Keyboard' API module of Input Controls is for getting the current state and input
*   directly from the user keyboard.
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
#ifndef H_ICAPI_KEYBOARD
#define H_ICAPI_KEYBOARD

/********************************/
/*  API Structures              */
/********************************/
/****** Main Keys *******************************************************************************/
#define KEY_BACKSPACE               (0x08) /* backspace key                                     */
#define KEY_TAB                     (0x09) /* tab key                                           */
#define KEY_CLEAR                   (0x0C) /* clear key                     (only some layouts) */
#define KEY_RETURN                  (0x0D) /* return/enter key                                  */
#define KEY_PAUSE                   (0x13) /* pause key                                         */
#define KEY_ESCAPE                  (0x1B) /* 'esc' key                                         */
#define KEY_SPACE                   (0x20) /* space key                                         */
#define KEY_PAGE_UP                 (0x21) /* page up key                                       */
#define KEY_PAGE_DOWN               (0x22) /* page down key                                     */
#define KEY_END                     (0x23) /* end key                                           */
#define KEY_HOME                    (0x24) /* home key                                          */
#define KEY_ARROW_LEFT              (0x25) /* arrow left key                                    */
#define KEY_ARROW_UP                (0x26) /* arrow up key                                      */
#define KEY_ARROW_RIGHT             (0x27) /* arrow right key                                   */
#define KEY_ARROW_DOWN              (0x28) /* arrow down key                                    */
#define KEY_PRINTSCREEN             (0x2C) /* printscreen key                                   */
#define KEY_INSERT                  (0x2D) /* insert key                                        */
#define KEY_DELETE                  (0x2E) /* delete key                                        */
#define KEY_HELP                    (0x2F) /* help key                      (only some layouts) */
#define KEY_SEMICOLON               (0xBA) /* ';' key                                           */
#define KEY_EQUALS                  (0xBB) /* '=' key                                           */
#define KEY_COMMA                   (0xBC) /* ',' key                                           */
#define KEY_SUBTRACT                (0xBD) /* '-' key                                           */
#define KEY_PERIOD                  (0xBE) /* '.' key                                           */
#define KEY_FORWARDSLASH            (0xBF) /* '/' key                                           */
#define KEY_TILDE                   (0xC0) /* '~' key                                           */
#define KEY_SQUAREBRACKET_L         (0xDB) /* '[' key                                           */
#define KEY_BACKSLASH               (0xDC) /* '\' key                                           */
#define KEY_SQUAREBRACKET_R         (0xDD) /* ']' key                                           */
#define KEY_QUOTE                   (0xDE) /* ''' key                                           */

/****** Numbers ('0' <-> '9') *******************************************************************/
/****** Letters ('A' <-> 'Z') *******************************************************************/
/****** Super/Windows ***************************************************************************/
#define KEY_SUPER_L                 (0x5B) /* left super/windows key                            */
#define KEY_SUPER_R                 (0x5C) /* right super/windows key                           */

/****** Numpad Keys *****************************************************************************/
#define KEY_NUM_0                   (0x60) /* numpad 0                       (requires numlock) */
#define KEY_NUM_1                   (0x61) /* numpad 1                       (requires numlock) */
#define KEY_NUM_2                   (0x62) /* numpad 2                       (requires numlock) */
#define KEY_NUM_3                   (0x63) /* numpad 3                       (requires numlock) */
#define KEY_NUM_4                   (0x64) /* numpad 4                       (requires numlock) */
#define KEY_NUM_5                   (0x65) /* numpad 5                       (requires numlock) */
#define KEY_NUM_6                   (0x66) /* numpad 6                       (requires numlock) */
#define KEY_NUM_7                   (0x67) /* numpad 7                       (requires numlock) */
#define KEY_NUM_8                   (0x68) /* numpad 8                       (requires numlock) */
#define KEY_NUM_9                   (0x69) /* numpad 9                       (requires numlock) */
#define KEY_NUM_MULTIPLY            (0x6A) /* numpad '*'                                        */
#define KEY_NUM_PLUS                (0x6B) /* numpad '+'                                        */
#define KEY_NUM_SEPARATOR           (0x6C) /* numpad ',' or '.'             (only some layouts) */
#define KEY_NUM_SUBTRACT            (0x6D) /* numpad '-'                                        */
#define KEY_NUM_DECIMAL             (0x6E) /* numpad '.'                     (requires numlock) */
#define KEY_NUM_DIVIDE              (0x6F) /* numpad '/'                                        */

/****** Function Keys ***************************************************************************/
#define KEY_F1                      (0x70) /* function key 1                                    */
#define KEY_F2                      (0x71) /* function key 2                                    */
#define KEY_F3                      (0x72) /* function key 3                                    */
#define KEY_F4                      (0x73) /* function key 4                                    */
#define KEY_F5                      (0x74) /* function key 5                                    */
#define KEY_F6                      (0x75) /* function key 6                                    */
#define KEY_F7                      (0x76) /* function key 7                                    */
#define KEY_F8                      (0x77) /* function key 8                                    */
#define KEY_F9                      (0x78) /* function key 9                                    */
#define KEY_F10                     (0x79) /* function key 10                                   */
#define KEY_F11                     (0x7A) /* function key 11                                   */
#define KEY_F12                     (0x7B) /* function key 12                                   */
#define KEY_F13                     (0x7C) /* function key 13                                   */
#define KEY_F14                     (0x7D) /* function key 14                                   */
#define KEY_F15                     (0x7E) /* function key 15                                   */
#define KEY_F16                     (0x7F) /* function key 16                                   */
#define KEY_F17                     (0x80) /* function key 17                                   */
#define KEY_F18                     (0x81) /* function key 18                                   */
#define KEY_F19                     (0x82) /* function key 19                                   */
#define KEY_F20                     (0x83) /* function key 20                                   */
#define KEY_F21                     (0x84) /* function key 21                                   */
#define KEY_F22                     (0x85) /* function key 22                                   */
#define KEY_F23                     (0x86) /* function key 23                                   */
#define KEY_F24                     (0x87) /* function key 24                                   */

/****** 'Lock' Keys *****************************************************************************/
#define KEY_CAPSLOCK                (0x14) /* capital lock                                      */
#define KEY_NUMLOCK                 (0x90) /* number lock                                       */
#define KEY_SCROLLLOCK              (0x91) /* scroll lock                                       */

/****** Modifier Keys ***************************************************************************/
#define KEY_SHIFT_L                 (0xA0) /* left shift key                                    */
#define KEY_SHIFT_R                 (0xA1) /* right shift key                                   */
#define KEY_CONTROL_L               (0xA2) /* left control key                                  */
#define KEY_CONTROL_R               (0xA3) /* right control key                                 */
#define KEY_ALT_L                   (0xA4) /* left alt key                                      */
#define KEY_ALT_R                   (0xA5) /* right alt key                                     */

/****** L or R **********************************************************************************/
#define KEY_SHIFT                   (0x10) /* any shift key                                     */
#define KEY_CONTROL                 (0x11) /* any control key                                   */

/****** Other ***********************************************************************************/
#define KEY_CTRLBREAK               (0x03) /* ctrl+pause/break                                  */

/****** Mouse 'Keys' ****************************************************************************/
#define KEY_M_LCLICK                (0x01) /* mouse left button                                 */
#define KEY_M_RCLICK                (0x02) /* mouse right button                                */
#define KEY_M_MCLICK                (0x04) /* mouse middle button                               */
#define KEY_M_X1                    (0x05) /* mouse side/eXtra button 1                         */
#define KEY_M_X2                    (0x06) /* mouse side/eXtra button 2                         */

/****** No Key **********************************************************************************/
#define KEY_NONE                    (0xFF) /* no key                                            */

/********************************/
/*  API Structures              */
/********************************/
/****** Keyboard API ****************************************************************************/
typedef struct
{
    uint32_t version;               /* structure version                                        */

    /****** Version >= 0 ********************************************************************/

    /**** Poll Key ******************************************/
    /*
    *   Description:
    *     Get a key that was pressed this moment. If multiple keys are pressed at the same
    *   time, the highest key index will be returned.
    * 
    *   Returns:
    *     The index of a key pressed this moment; or 'KEY_NONE' if no keys have been pressed.
    */
    uint8_t (__cdecl* Poll)( void );

    /**** Key State *****************************************/
    /*
    *   Description:
    *     Check if a key is currently being held down.
    *   
    *   Parameters:
    *     - key     : key to get state of
    * 
    *   Returns:
    *     'true' if they key is down; or 'false' if it is not.
    */
    bool (__cdecl* Down)( uint8_t key );
    /*
    *   Description:
    *     Check if a key was pressed this moment.
    *   
    *   Parameters:
    *     - key     : key to get state of
    * 
    *   Returns:
    *     'true' if they key was pressed; or 'false' if it was not.
    */
    bool (__cdecl* Press)( uint8_t key );
    /*
    *   Description:
    *     Check if a key was released this moment.
    *   
    *   Parameters:
    *     - key     : key to get state of
    * 
    *   Returns:
    *     'true' if they key was released; or 'false' if it was not.
    */
    bool (__cdecl* Release)( uint8_t key );

    /**** Modifier State ************************************/
    /*
    *   Description:
    *     Check if Caps Lock modifier is currently active.
    */
    bool (__cdecl* CapsLock)( void );
    /*
    *   Description:
    *     Check if Scroll Lock modifier is currently active.
    */
    bool (__cdecl* ScrollLock)( void );
    /*
    *   Description:
    *     Check if Number Lock modifier is currently active.
    */
    bool (__cdecl* NumLock)( void );
}
ICAPI_KEYBOARD;

#endif/*H_ICAPI_KEYBOARD*/
