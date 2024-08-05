/*
*   SA2 Input Controls - '/ic_input/inpt_internal.h'
*
*   Description:
*     Keyboard module header
*
*   Contributors:
*     - Shaddatic
*/
#ifndef H_IC_INPUT_MOUSE_INTERNAL
#define H_IC_INPUT_MOUSE_INTERNAL

/************************/
/*  Prototypes          */
/************************/
/****** Init ************************************************************************/
/*
*   Description:
*     Init Mouse Visualizer sub-module
*/
void    MouseVisualizerInit( void );
/*
*   Description:
*     Init Mouse Window Message sub-module.
*/
void    MouseWindowMsgInit( void );

/****** Set Visualizer Info *********************************************************/
/*
*   Description:
*     Set visualizer X, Y analog stick info
*
*   Parameters:
*     - x       : x position of the emulated analog stick (-1~1)
*     - y       : y position of the emulated analog stick (-1~1)
*/
void    MouseSetVisualizerInfo( f32 x, f32 y );

/****** Message Functions ***********************************************************/
/*
*   Description:
*     Send mouse wheel window messages to parent mouse module. Wheel input must be
*   converted to wheel increments from raw values.
*
*   Parameters:
*     - wheel   : wheel increments as a float value
*/
void    MouseMsgWheelX( f32 wheel );
void    MouseMsgWheelY( f32 wheel );

#endif/*H_IC_INPUT_MOUSE_INTERNAL*/
