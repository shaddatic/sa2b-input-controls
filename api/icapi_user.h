/*
*   SA2 Input Controls - '/icapi_user.h'
*
*   Description:
*     The 'User' API module of Input Controls is for getting the current player settings, such
*   as which devices they're using as input. 
*     The input structure is the combination of their selected gamepad and keyboard input, 
*   essentially a higher resolution version of the Dreamcast gamepad structure.
* 
*   Notes:
*     - All returned pointers are constant for the liftime of the program.
* 
*   Version History:
*     - v1.0.0.0        : Version 0, initial release
*     - v1.1.0.0        : Version 1, added input callback so mods can directly edit user input
* 
*   Availability:
*     - Before Init     : Partial, input values will be null and player settings will be 'NONE'
*     - At Init         : Partial, input values will be null
*     - After Init      : Yes
* 
*   Notice:
*     The API headers are to be included in your own projects. You have full rights to use,
*   distribute, and modify the API headers as you see fit.
*/
#ifndef H_ICAPI_USER
#define H_ICAPI_USER

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

/********************************/
/*  Structures                  */
/********************************/
/****** Raw User Input **************************************************************************/
typedef struct
{
    uint32_t                  down; /* user input buttons currently pressed            [USRBTN] */
    uint32_t                 press; /* user input buttons pressed this moment          [USRBTN] */
    uint32_t               release; /* user input buttons released this moment         [USRBTN] */

    f32                       l, r; /* analog l/r triggers                            [0.f~1.f] */

    f32                     x1, y1; /* left analog stick                             [-1.f~1.f] */
    f32                     x2, y2; /* right analog stick                            [-1.f~1.f] */
}
IC_USER_INPUT;

/********************************/
/*  API Structures              */
/********************************/
/****** User API ********************************************************************************/
typedef struct
{
    uint32_t version;               /* structure version                                        */

    /****** Version >= 0 ********************************************************************/

    /**** Raw Input *****************************************/
    /*
    *   Description:
    *     Get the raw input of a player, which is the combined input of their selected gamepad
    *   and keyboard layout.
    *
    *   Parameters:
    *     - nbUser      : player number
    *
    *   Returns:
    *     Combined player input structure.
    */
    const IC_USER_INPUT* (__cdecl* GetInput)( eIC_USER_NUM nbUser );

    /**** Device Number *************************************/
    /*
    *   Description:
    *     Get the gamepad number of a player.
    *
    *   Parameters:
    *     - nbUser      : player number
    */
    eIC_GAMEPAD_NUM (__cdecl* GetGamepadNum)( eIC_USER_NUM nbUser );
    /*
    *   Description:
    *     Get the keyboard layout number of a player.
    *
    *   Parameters:
    *     - nbUser      : player number
    */
    eIC_KEYBOARD_NUM (__cdecl* GetKeyboardNum)( eIC_USER_NUM nbUser );

    /****** Version >= 1 ********************************************************************/

    /**** Callback ******************************************/
    /*
    *   Description:
    *     Set the user input callback function, which allows you to edit the player's input at
    *   runtime from the source in a standardized way.
    *
    *   Notes:
    *     - This will be called just after the user input structure is executed.
    *     - Only 1 mod can use the callback at a time
    *
    *   Parameters:
    *     - callback    : callback function to call              [opt: nullptr]
    */
    void (__cdecl* SetCallback)( void(__cdecl* callback)(int ixUser, IC_USER_INPUT* pUser) );
}
ICAPI_USER;

#endif/*H_ICAPI_USER*/
