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
/*
*   Description:
*     Init SDL module
*
*   Returns:
*     If Init was successful
*/
bool    ICSDL_Init( void );
/*
*   Description:
*     SDL Exit function for OnExit
*/
void    ICSDL_Exit( void );

/****** Input Controls API **********************************************************/
/*
*   Description:
*     Get DLL handle for mounted SDL library
*
*   Returns:
*     The 'HMODULE' or 'dll_handle' to the SDL DLL library
*/
void*   ICSDL_GetHandle( void );
/*
*   Description:
*     Get DLL handle for mounted SDL library
*
*   Returns:
*     A pointer to the DLL export, or nullptr if no export is found
*/
void*   ICSDL_GetExport( const char* cExName );

EXTERN_END

#endif/*H_IC_SDL2*/
