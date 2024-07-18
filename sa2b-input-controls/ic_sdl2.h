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

#ifndef IC_SDL2_INITONLY

/************************/
/*  External Headers    */
/************************/
/****** Simple DirectMedia Layer ****************************************************/
#include <SDL2/SDL.h>       /* core                                                 */

#endif/*IC_SDL2_INITONLY*/

EXTERN_START

/************************/
/*  Prototypes          */
/************************/
/****** Module Init *****************************************************************/
void    SDL_InitInit( void );
void    SDL_ExitExit( void );

#ifndef IC_SDL2_INITONLY

/****** Init ************************************************************************/
int     SDL2_Init(u32 flag);

/****** Event ***********************************************************************/
int     SDL2_PollEvent( SDL_Event* pOutEvent );

/****** Gamepad *********************************************************************/
SDL_GameController* SDL2_GameControllerOpen( int nbGp );
void                SDL2_GameControllerClose( SDL_GameController* pGp );

int     SDL2_IsGameController( int nbGp );

Sint16  SDL2_GameControllerGetAxis( SDL_GameController* pGp, SDL_GameControllerAxis axis );

SDL_bool    SDL2_GameControllerHasButton(SDL_GameController* pGp, SDL_GameControllerButton btn);

SDL_bool    SDL2_GameControllerGetButton(SDL_GameController* pGp, SDL_GameControllerButton btn);

SDL_bool    SDL2_GameControllerHasRumble(SDL_GameController* pGp);
SDL_bool    SDL2_GameControllerHasRumbleTriggers(SDL_GameController* pGp);

int     SDL2_GameControllerRumble( SDL_GameController* pGp, Uint16 freqLo, Uint16 freqHi, Uint32 msDur );
int     SDL2_GameControllerRumbleTriggers(SDL_GameController* pGp, Uint16 freqL, Uint16 freqR, Uint32 msDur );

int     SDL2_NumJoysticks( void );

#endif/*IC_SDL2_INITONLY*/

EXTERN_END

#endif/*H_IC_SDL2*/
