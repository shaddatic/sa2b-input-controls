/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */
#include <sa2b/memory.h>    /* MemAlloc, MemFree                                    */
#include <sa2b/string.h>    /* StringSize                                           */
#include <sa2b/dll.h>       /* DLL_Mount2, DLL_GetExportList                        */
#include <sa2b/user.h>      /* UserErrorMessageBox                                  */

/****** Simple DirectMedia Layer ****************************************************/
#include <SDL2/SDL.h>       /* core                                                 */

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */

/****** C Sdl ***********************************************************************/
#include <stdio.h>          /* std in out                                           */

/****** Self ************************************************************************/
#include <ic_sdl2.h>        /* self                                                 */

/************************/
/*  Macros              */
/************************/
/****** SDL Function Exports ********************************************************/
#define SDL_FUNC_PTR(retn, name, para)      static retn(SDLCALL* ___##name)para

#define SDL_EXPORT(name)                    { &___##name, "SDL_"#name }

#define SDL2_GameControllerAddMappingsFromFile(file)    ___GameControllerAddMappingsFromRW( ___RWFromFile(file, "rb"), 1 )

/************************/
/*  File Data           */
/************************/
/****** DLL Handle ******************************************************************/
static dll_handle* SdlHandle;

/****** Function Pointers ***********************************************************/
SDL_FUNC_PTR(int                , Init                           , (int)                                          );
SDL_FUNC_PTR(void               , Quit                           , (void)                                         );
SDL_FUNC_PTR(int                , PollEvent                      , (SDL_Event*)                                   );
SDL_FUNC_PTR(SDL_GameController*, GameControllerOpen             , (int)                                          );
SDL_FUNC_PTR(void               , GameControllerClose            , (SDL_GameController*)                          );
SDL_FUNC_PTR(SDL_bool           , IsGameController               , (int)                                          );
SDL_FUNC_PTR(Sint16             , GameControllerGetAxis          , (SDL_GameController*, int)                     );
SDL_FUNC_PTR(SDL_bool           , GameControllerHasButton        , (SDL_GameController*, SDL_GameControllerButton));
SDL_FUNC_PTR(SDL_bool           , GameControllerGetButton        , (SDL_GameController*, SDL_GameControllerButton));
SDL_FUNC_PTR(SDL_bool           , GameControllerHasRumble        , (SDL_GameController*)                          );
SDL_FUNC_PTR(SDL_bool           , GameControllerHasRumbleTriggers, (SDL_GameController*)                          );
SDL_FUNC_PTR(int                , GameControllerRumble           , (SDL_GameController*, Uint16, Uint16, Uint32)  );
SDL_FUNC_PTR(int                , GameControllerRumbleTriggers   , (SDL_GameController*, Uint16, Uint16, Uint32)  );
SDL_FUNC_PTR(int                , NumJoysticks                   , (void)                                         );
SDL_FUNC_PTR(const char*        , GameControllerName             , (SDL_GameController*)                          );
SDL_FUNC_PTR(int                , GameControllerAddMappingsFromRW, (SDL_RWops*, int)                              );
SDL_FUNC_PTR(SDL_RWops*         , RWFromFile                     , (const char*, const char*)                     );

/****** Export List *****************************************************************/
static const dll_export SdlExports[] =
{
    SDL_EXPORT(Init),
    SDL_EXPORT(Quit),
    SDL_EXPORT(PollEvent),
    SDL_EXPORT(GameControllerOpen),
    SDL_EXPORT(GameControllerClose),
    SDL_EXPORT(IsGameController),
    SDL_EXPORT(GameControllerGetAxis),
    SDL_EXPORT(GameControllerHasButton),
    SDL_EXPORT(GameControllerGetButton),
    SDL_EXPORT(GameControllerHasRumble),
    SDL_EXPORT(GameControllerHasRumbleTriggers),
    SDL_EXPORT(GameControllerRumble),
    SDL_EXPORT(GameControllerRumbleTriggers),
    SDL_EXPORT(NumJoysticks),
    SDL_EXPORT(GameControllerName),
    SDL_EXPORT(GameControllerAddMappingsFromRW),
    SDL_EXPORT(RWFromFile),
};

/************************/
/*  Source              */
/************************/
/****** SDL Exports *****************************************************************/
int
SDL2_Init(u32 flag)
{
    return ___Init(flag);
}

void
SDL2_Quit(void)
{
    ___Quit();
}

int
SDL2_PollEvent(SDL_Event* const pOutEvent)
{
    return ___PollEvent(pOutEvent);
}

SDL_GameController*
SDL2_GameControllerOpen(const int nbGp)
{
    return ___GameControllerOpen(nbGp);
}

void
SDL2_GameControllerClose(SDL_GameController* const pGp)
{
    ___GameControllerClose(pGp);
}

int
SDL2_IsGameController(const int nbGp)
{
    return ___IsGameController(nbGp);
}

Sint16
SDL2_GameControllerGetAxis(SDL_GameController* pGp, SDL_GameControllerAxis axis)
{
    return ___GameControllerGetAxis(pGp, axis);
}

SDL_bool
SDL2_GameControllerHasButton(SDL_GameController* pGp, SDL_GameControllerButton btn)
{
    return ___GameControllerHasButton(pGp, btn);
}

SDL_bool
SDL2_GameControllerGetButton(SDL_GameController* pGp, SDL_GameControllerButton btn)
{
    return ___GameControllerGetButton(pGp, btn);
}

SDL_bool
SDL2_GameControllerHasRumble(SDL_GameController* pGp)
{
    return ___GameControllerHasRumble(pGp);
}

SDL_bool
SDL2_GameControllerHasRumbleTriggers(SDL_GameController* pGp)
{
    return ___GameControllerHasRumbleTriggers(pGp);
}

int
SDL2_GameControllerRumble(SDL_GameController* pGp, Uint16 freqLo, Uint16 freqHi, Uint32 msDur)
{
    return ___GameControllerRumble(pGp, freqLo, freqHi, msDur);
}

SDL_bool
SDL2_GameControllerRumbleTriggers(SDL_GameController* pGp, Uint16 freqL, Uint16 freqR, Uint32 msDur)
{
    return ___GameControllerRumbleTriggers(pGp, freqL, freqR, msDur);
}

int
SDL2_NumJoysticks(void)
{
    return ___NumJoysticks();
}

const char*
SDL2_GameControllerName(SDL_GameController* pGp)
{
    return ___GameControllerName(pGp);
}

static utf8*
GetMappingFilePath(void)
{
    const size_t sz_buf = StringSize(GetModPath(), STR_NOMAX) + 21;

    utf8* const pu_buf = MemAlloc(sz_buf);

    snprintf(pu_buf, sz_buf, "%s/%s", GetModPath(), "gamecontrollerdb.txt");

    return pu_buf;
}

/****** Init ************************************************************************/
bool
ICSDL_Init(void)
{
    dll_handle* const p_hdl = DLL_Mount2(GetModPath(), "lib/SDL2.dll");

    if (!p_hdl)
    {
        UserErrorMessageBox("Input Controls : SDL2 Critical Error",
            "The SDL2 library could not be mounted! This is likely because '/lib/SDL2.dll' is missing from the Input Controls mod folder.\n"
            "Input Controls cannot function without SDL, the init process will now be aborted!"
        );
        return false;
    }

    DLL_GetExportList(p_hdl, SdlExports, ARYLEN(SdlExports));

    SDL2_Init( SDL_INIT_GAMECONTROLLER );

    utf8* const pu_buf = GetMappingFilePath();
    
    SDL2_GameControllerAddMappingsFromFile(pu_buf);

    MemFree(pu_buf);

    SdlHandle = p_hdl;

    return true;
}

void
ICSDL_Exit(void)
{
    if (!SdlHandle)
        return;

    SDL2_Quit();

    DLL_Unmount(SdlHandle);
}
