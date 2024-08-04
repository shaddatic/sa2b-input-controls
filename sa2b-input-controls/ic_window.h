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
void    WND_Init( void );

/****** Update **********************************************************************/
/*
*   Description:
*     Update Window module. To be called before USER_INPUT is calculated
*/
void    WND_Update( void );

/****** Info ************************************************************************/
/*
*   Description:
*     Get if the game window is in focus
*/
bool    WND_InFocus( void );
/*
*   Description:
*     Get the window size in physical pixels
*/
void    WND_GetSize( NJS_POINT2I* pOutSize );

/****** Window Message **************************************************************/
/*
*   Description:
*     Register a window message handler function
*
*   Parameters:
*     fnHandler : function pointer to handle incomming window messages
*/
void    WND_RegisterMessageHandler( void(__cdecl* fnHandler)(uint32_t msg, uint32_t wParam, int32_t lParam) );

/****** Coordinates *****************************************************************/
/*
*   Description:
*     Translate window surface coordinates to the game's 640x480 coordinates.
*
*   Parameters:
*     pIn       : pointer to surface coordinates to translate
*     pOut      : return pointer for game screen coordinates
*/
void    WND_TranslateSurfaceToGameCoords( const NJS_POINT2I* pIn, NJS_POINT2* pOut );

/****** Internal Message ************************************************************/
/*
*   Description:
*     Send a window message to the message handlers
*
*   Parameters:
*     msg       : message type
*     wParam    : wParam
*     lParam    : lParam
*/
void    WND_SendMessage(uint32_t msg, uint32_t wParam, int32_t lParam);

EXTERN_END

#endif/*H_IC_WINDOW*/
