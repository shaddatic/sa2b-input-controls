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
/****** Init **********************************************************************/
/*
*   Description:
*     Init Mouse Visualizer sub-module
*/
void    MouseVisualizerInit( void );

/****** Set Info ******************************************************************/
/*
*   Description:
*     Set visualizer X, Y analog stick info
*
*   Parameters:
*     - x       : x position of the emulated analog stick (-1~1)
*     - y       : y position of the emulated analog stick (-1~1)
*/
void    MouseSetVisualizerInfo( f32 x, f32 y );

#endif/*H_IC_INPUT_MOUSE_INTERNAL*/
