/*
*   SA2 Input Controls - '/icapi_gamepad.h'
*
*   Description:
*     The 'Gamepad' API module of Input Controls is for getting the current state and input
*   directly from the user gamepads.
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
#ifndef H_ICAPI_GAMEPAD
#define H_ICAPI_GAMEPAD

/********************************/
/*  Opaque Types                */
/********************************/
/****** SDL *************************************************************************************/
typedef struct _SDL_GameController  SDL_GameController; /* SDL headers                          */

/********************************/
/*  Constants                   */
/********************************/
/****** User Buttons ****************************************************************************/
#define USRBTN_A                    (1<< 0) /* button a                                         */
#define USRBTN_B                    (1<< 1) /* button b                                         */
#define USRBTN_X                    (1<< 2) /* button x                                         */
#define USRBTN_Y                    (1<< 3) /* button y                                         */
#define USRBTN_START                (1<< 4) /* button start                                     */
#define USRBTN_BACK                 (1<< 5) /* button back                                      */
#define USRBTN_ZL                   (1<< 6) /* button zl                               (lb, l1) */
#define USRBTN_ZR                   (1<< 7) /* button zr                            (z, rb, r1) */
#define USRBTN_LS                   (1<< 8) /* button ls                                   (l3) */
#define USRBTN_RS                   (1<< 9) /* button rs                                   (r3) */
#define USRBTN_DPAD_UP              (1<<10) /* directional pad, up                              */
#define USRBTN_DPAD_DOWN            (1<<11) /* directional pad, down                            */
#define USRBTN_DPAD_LEFT            (1<<12) /* directional pad, left                            */
#define USRBTN_DPAD_RIGHT           (1<<13) /* directional pad, right                           */

/****** Raw Gamepad Buttons *********************************************************************/
#define GPDBTN_A                    (1<< 0) /* button a                                         */
#define GPDBTN_B                    (1<< 1) /* button b                                         */
#define GPDBTN_X                    (1<< 2) /* button x                                         */
#define GPDBTN_Y                    (1<< 3) /* button y                                         */
#define GPDBTN_BACK                 (1<< 4) /* button back                                      */
#define GPDBTN_GUIDE                (1<< 5) /* xb/ps button, ns: home button                    */
#define GPDBTN_START                (1<< 6) /* button start                                     */
#define GPDBTN_LS                   (1<< 7) /* button ls                                   (l3) */
#define GPDBTN_RS                   (1<< 8) /* button rs                                   (r3) */
#define GPDBTN_ZL                   (1<< 9) /* button zl                               (lb, l1) */
#define GPDBTN_ZR                   (1<<10) /* button zr                            (z, rb, r1) */
#define GPDBTN_DPAD_UP              (1<<11) /* directional pad, up                              */
#define GPDBTN_DPAD_DOWN            (1<<12) /* directional pad, down                            */
#define GPDBTN_DPAD_LEFT            (1<<13) /* directional pad, left                            */
#define GPDBTN_DPAD_RIGHT           (1<<14) /* directional pad, right                           */
#define GPDBTN_MISC                 (1<<15) /* xb: share, ps: mic, ns: capture                  */
#define GPDBTN_XBPADDLE_UR          (1<<16) /* xb paddle: upper right              (from front) */
#define GPDBTN_XBPADDLE_UL          (1<<17) /* xb paddle: upper left               (from front) */
#define GPDBTN_XBPADDLE_LR          (1<<18) /* xb paddle: lower right              (from front) */
#define GPDBTN_XBPADDLE_LL          (1<<19) /* xb paddle: lower left               (from front) */
#define GPDBTN_TOUCHPAD             (1<<20) /* ps 4/5 touchpad button                           */

/****** Support *********************************************************************************/
#define GPDDEV_SUPPORT_A            (1<< 0) /* button a                                         */
#define GPDDEV_SUPPORT_B            (1<< 1) /* button b                                         */
#define GPDDEV_SUPPORT_X            (1<< 2) /* button x                                         */
#define GPDDEV_SUPPORT_Y            (1<< 3) /* button y                                         */
#define GPDDEV_SUPPORT_BACK         (1<< 4) /* button back                                      */
#define GPDDEV_SUPPORT_GUIDE        (1<< 5) /* xb/ps button, ns: home button                    */
#define GPDDEV_SUPPORT_START        (1<< 6) /* button start                                     */
#define GPDDEV_SUPPORT_LS           (1<< 7) /* button ls                                   (l3) */
#define GPDDEV_SUPPORT_RS           (1<< 8) /* button rs                                   (r3) */
#define GPDDEV_SUPPORT_ZL           (1<< 9) /* button zl                               (lb, l1) */
#define GPDDEV_SUPPORT_ZR           (1<<10) /* button zr                            (z, rb, r1) */
#define GPDDEV_SUPPORT_DPAD_UP      (1<<11) /* directional pad, up                              */
#define GPDDEV_SUPPORT_DPAD_DOWN    (1<<12) /* directional pad, down                            */
#define GPDDEV_SUPPORT_DPAD_LEFT    (1<<13) /* directional pad, left                            */
#define GPDDEV_SUPPORT_DPAD_RIGHT   (1<<14) /* directional pad, right                           */
#define GPDDEV_SUPPORT_MISC         (1<<15) /* xb: share, ps: mic, ns: capture                  */
#define GPDDEV_SUPPORT_XBPADDLE_UR  (1<<16) /* xb paddle: upper right              (from front) */
#define GPDDEV_SUPPORT_XBPADDLE_UL  (1<<17) /* xb paddle: upper left               (from front) */
#define GPDDEV_SUPPORT_XBPADDLE_LR  (1<<18) /* xb paddle: lower right              (from front) */
#define GPDDEV_SUPPORT_XBPADDLE_LL  (1<<19) /* xb paddle: lower left               (from front) */
#define GPDDEV_SUPPORT_TOUCHPAD     (1<<20) /* ps 4/5 touchpad button                           */
#define GPDDEV_SUPPORT_RUMBLETRIGG  (1<<30) /* supports rumble triggers                         */
#define GPDDEV_SUPPORT_RUMBLE       (1<<31) /* supports rumble                                  */

/********************************/
/*  Structures                  */
/********************************/
/****** Gamepad Structure ***********************************************************************/
typedef struct
{
    SDL_GameController*        pgp; /* SDL gamepad pointer                                      */
    int32_t                     id; /* SDL gamepad/joystick index                               */

    const char*               name; /* gamepad name                                             */

    uint32_t               support; /* gamepad supported features                      [GPDDEV] */

    uint32_t                  down; /* buttons currently pressed                       [GPDBTN] */
    uint32_t                 press; /* buttons pressed this moment                     [GPDBTN] */
    uint32_t               release; /* buttons released this moment                    [GPDBTN] */

    int16_t                   l, r; /* analog l/r triggers                            [0~32767] */

    int16_t                 x1, y1; /* left analog stick                         [-32768~32767] */
    int16_t                 x2, y2; /* right analog stick                        [-32768~32767] */
}
IC_GAMEPAD;

/********************************/
/*  API Structures              */
/********************************/
/****** Gamepad API *****************************************************************************/
typedef struct
{
    uint32_t version;               /* structure version                                        */

    /****** Version >= 0 ********************************************************************/

    /**** Raw Gamepad ***************************************/
    /*
    *   Description:
    *     Get raw gamepad input and attributes.
    * 
    *   Parameters:
    *     - nbGp        : gamepad number to get
    * 
    *   Returns:
    *     A Gamepad struct pointer; or 'nullptr' if 'nbGp' is GAMEPAD_NONE.
    */
    const IC_GAMEPAD* (__cdecl* GetGamepad)( eIC_GAMEPAD_NUM nbGp );

    /**** Valid/Open ***************************************/
    /*
    *   Description:
    *     Check if the given gamepad is active and linked to an open, physical game controller
    *   device.
    * 
    *   Notes:
    *     - This may change over the course of the program, as gamepads are connected and
    *       disconnected.
    *     - During Init, no controllers are set as connected.
    * 
    *   Parameters:
    *     - nbGp        : gamepad number to get state of
    * 
    *   Returns:
    *     'true' if the given gamepad is valid; or 'false' if not.
    */
    bool (__cdecl* Valid)( eIC_GAMEPAD_NUM nbGp );

    /**** Vibration *****************************************/
    /*
    *   Description:
    *     Set the vibration motors on a gamepad. You can check support by searching for the 
    *   'GPDDEV_SUPPORT_RUMBLE' flag in the structure's 'support' member.
    * 
    *   Parameters:
    *     - nbGp        : gamepad number to set vibration for
    *     - freqLo      : speed of the low frequency motor                                [0~1]
    *     - freqHi      : speed of the high frequency motor                               [0~1]
    * 
    *   Returns:
    *     'true' if successful; or 'false' if not.
    */
    bool (__cdecl* SetVibration)( eIC_GAMEPAD_NUM nbGp, f32 freqLo, f32 freqHi );
    /*
    *   Description:
    *     Set the trigger vibration motors on a gamepad. Only applicable to Xbox One and higher
    *   controllers. You can check support by searching for the 'GPDDEV_SUPPORT_RUMBLE_TRIGGER'
    *   flag in the structure's 'support' member.
    * 
    *   Parameters:
    *     - nbGp        : gamepad number to set vibration for
    *     - freqL       : speed of the left trigger motor                                 [0~1]
    *     - freqR       : speed of the right trigger motor                                [0~1]
    * 
    *   Returns:
    *     'true' if successful; or 'false' if not.
    */
    bool (__cdecl* SetTriggerVibration)( eIC_GAMEPAD_NUM nbGp, f32 freqL, f32 freqR );
}
ICAPI_GAMEPAD;

#endif/*H_ICAPI_GAMEPAD*/
