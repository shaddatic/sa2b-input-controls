/*
*   SA2 Input Controls - '/ic_windows.h'
*
*   Description:
*     Sadly, includes the 'windows.h' header file.
*
*   Contributors:
*     - Shaddatic
*/
#ifndef H_IC_WINDOWS
#define H_IC_WINDOWS

EXTERN_START

/************************/
/*  Prototypes          */
/************************/
/****** Message Functions ***********************************************************/
/*
*   Description:
*     Handles mouse wheel window messages, input must be converted to wheel
*   increments by dividing raw value by 120, or WHEEL_DELTA.
*
*   Module:
*     Input::Mouse
*
*   Parameters:
*     - wheel   : wheel increments as a float value (RawVal/WHEEL_DELTA)
*/
void    OSMSG_MouseWheel( f32 wheel );
/*
*   Description:
*     Handles KILLFOCUS window messages.
*
*   Module:
*     Window
*/
void    OSMSG_KillFocus( void );
/*
*   Description:
*     Handles SETFOCUS window messages.
*
*   Module:
*     Window
*/
void    OSMSG_SetFocus( void );

EXTERN_END

#endif/*H_IC_WINDOWS*/
