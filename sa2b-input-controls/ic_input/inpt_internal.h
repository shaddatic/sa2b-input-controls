/*
*   SA2 Input Controls - '/ic_input/inpt_internal.h'
*
*   Description:
*     Keyboard module header
*
*   Contributors:
*     - Shaddatic
*/
#ifndef H_IC_INPUT_INTERNAL
#define H_IC_INPUT_INTERNAL

/************************/
/*  External Headers    */
/************************/
/****** Input Controls **************************************************************/
#include <ic_input.h>   /* external self                                            */

EXTERN_START

/************************/
/*  Enums               */
/************************/
/****** Emulated Analog *************************************************************/
typedef enum
{
    EMU_STICK_LEFT,
    EMU_STICK_RIGHT,
}
eEMU_STICK;

/************************/
/*  Structures          */
/************************/
/****** Internal Input Buffer *******************************************************/
typedef struct
{
    u32 down;
    f32 l , r;
    f32 x1, y1;
    f32 x2, y2;
}
INPUT_OUT;

/************************/
/*  Prototypes          */
/************************/
/****** Update **********************************************************************/
/*
*   Description:
*     Update sub-modules.
*/
void    GamepadUpdate(  void );
void    KeyboardUpdate( void );
void    MouseUpdate(    void );

/****** Set User Input **************************************************************/
/*
*   Description:
*     Get USER_INPUT from physical device, using user-settings for said device.
*/
bool    GamepadSetUserInput(  eGAMEPAD_NUM  nbGp, INPUT_OUT* pOutInput );
bool    KeyboardSetUserInput( eKEYBOARD_NUM nbKb, INPUT_OUT* pOutInput );

/****** Input Init ******************************************************************/
/*
*   Description:
*     Init sub-modules.
*/
void    GamepadInit(  void );
void    KeyboardInit( void );
void    MouseInit(    void );

/****** Emulated Analog *************************************************************/
/*
*   Description:
*     Get mouse emulated analog stick values. If the setting is disabled, or not
*   enabled for the specified analog stick, the output floats will be untouched.
*
*   Parameters:
*     - nbKb     : keyboard index
*     - nbAnalog : analog stick index to emulate
*     - pOutX    : return pointer for x value
*     - pOutY    : return pointer for y value
*
*   Returns:
*     If 'pOutX' and 'Y' were modified
*/
bool    MouseGetEmulatedAnalog( eKEYBOARD_NUM nbKb, eEMU_STICK nbAnalog, f32* pOutX, f32* pOutY );

EXTERN_END

#endif/*H_IC_INPUT_INTERNAL*/
