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

/************************/
/*  External Headers    */
/************************/
/****** Ninja ***********************************************************************/
#include <sa2b/ninja/njcommon.h> /* NJS_POINT2I                                     */

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
/*
*   Description:
*     Get the window size in physical pixels
*/
void    WindowGetSize( NJS_POINT2I* pOutSize );
/*
*   Description:
*     Get the game's internal rendering resolution
*/
void    WindowGetGameResolution( NJS_POINT2I* pOutRes );

EXTERN_END

#endif/*H_IC_WINDOW*/
