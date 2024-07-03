/*
*   SA2 Input Controls - '/ic_window.h'
*
*   Description:
*     Game window module
*
*   Contributors:
*     - Shaddatic
*/
#ifndef H_IC_WINDOW
#define H_IC_WINDOW

EXTERN_START

/************************/
/*  Prototypes          */
/************************/
/****** Init ************************************************************************/
/*
*   Description:
*     Init Window module
*/
void    IC_WindowInit( void );

/****** Update **********************************************************************/
/*
*   Description:
*     Update Window module. To be called before USER_INPUT is calculated
*/
void    IC_WindowUpdate( void );

/****** Info ************************************************************************/
/*
*   Description:
*     Get if the game window is in focus
*/
bool    WindowInFocus( void );

EXTERN_END

#endif/*H_IC_WINDOW*/
