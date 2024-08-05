/*
*   SA2 Input Controls - '/ic_window/icwnd_internal.h'
*
*   Description:
*     Internal window header
*
*   Contributors:
*     - Shaddatic
*/
#ifndef H_IC_WINDOW_INTERNAL
#define H_IC_WINDOW_INTERNAL

/************************/
/*  External Headers    */
/************************/
/****** Input Controls **************************************************************/
#include <ic_window.h>  /* external parent                                          */

EXTERN_START

/************************/
/*  Prototypes          */
/************************/
/****** Init ************************************************************************/
/*
*   Description:
*     Init sub-modules.
*/
void    WND_MessageInit( void );

/****** Messages ********************************************************************/
/*
*   Description:
*     Send window focus messages to parent window module.
*/
void    WND_MsgKillFocus( void );
void    WND_MsgSetFocus( void );

EXTERN_END

#endif/*H_IC_WINDOW_INTERNAL*/
