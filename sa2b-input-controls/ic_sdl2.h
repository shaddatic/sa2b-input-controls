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
/*  Opaque Types        */
/************************/
/****** Event Handling ***************************************************************/
typedef union SDL_Event     SDL_Event;

/************************/
/*  Prototypes          */
/************************/
/****** Event Handling ***************************************************************/
/*
*   Description:
*     Register an SDL event handler function
*/
void    ICSDL_RegisterEventHandler( void(__cdecl* fnEvHandler)(const SDL_Event*) );

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
