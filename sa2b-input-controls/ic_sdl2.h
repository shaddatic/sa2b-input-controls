/*
*   SA2 Input Controls - '/ic_sdl2.h'
*
*   Description:
*     Simple DirectMedia Layer 2 module
*
*   Contributors:
*     - Shaddatic
*/
#ifndef H_IC_SDL2
#define H_IC_SDL2

EXTERN_START

/************************/
/*  Prototypes          */
/************************/
/****** Module Init *****************************************************************/
bool    ICSDL_Init( void );
void    ICSDL_Exit( void );

/****** Input Controls API **********************************************************/
void*   ICSDL_GetHandle( void );
void*   ICSDL_GetExport( const char* cExName );

EXTERN_END

#endif/*H_IC_SDL2*/
