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
#include <ic_window.h>  /* external self                                            */

EXTERN_START

/************************/
/*  Prototypes          */
/************************/
/****** Init ************************************************************************/
void    WND_MessageInit( void );

/****** Init ************************************************************************/
void    WND_MsgKillFocus( void );
void    WND_MsgSetFocus( void );

EXTERN_END

#endif/*H_IC_WINDOW_INTERNAL*/
