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
SDL_FUNC_PTR(Uint8              , GameControllerGetButton        , (SDL_GameController*, SDL_GameControllerButton));
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
int SDLCALL
SDL_Init(Uint32 flags)
{
    return ___Init(flags);
}

void SDLCALL
SDL_Quit(void)
{
    ___Quit();
}

int SDLCALL
SDL_PollEvent(SDL_Event* const event)
{
    return ___PollEvent(event);
}

SDL_GameController* SDLCALL
SDL_GameControllerOpen(const int joystick_index)
{
    return ___GameControllerOpen(joystick_index);
}

void SDLCALL
SDL_GameControllerClose(SDL_GameController* const gamecontroller)
{
    ___GameControllerClose(gamecontroller);
}

int SDLCALL
SDL_IsGameController(const int joystick_index)
{
    return ___IsGameController(joystick_index);
}

Sint16 SDLCALL
SDL_GameControllerGetAxis(SDL_GameController* const gamecontroller, const SDL_GameControllerAxis axis)
{
    return ___GameControllerGetAxis(gamecontroller, axis);
}

SDL_bool SDLCALL
SDL_GameControllerHasButton(SDL_GameController* const gamecontroller, const SDL_GameControllerButton button)
{
    return ___GameControllerHasButton(gamecontroller, button);
}

Uint8 SDLCALL
SDL_GameControllerGetButton(SDL_GameController* const gamecontroller, const SDL_GameControllerButton button)
{
    return ___GameControllerGetButton(gamecontroller, button);
}

SDL_bool SDLCALL
SDL_GameControllerHasRumble(SDL_GameController* const gamecontroller)
{
    return ___GameControllerHasRumble(gamecontroller);
}

SDL_bool SDLCALL
SDL_GameControllerHasRumbleTriggers(SDL_GameController* const gamecontroller)
{
    return ___GameControllerHasRumbleTriggers(gamecontroller);
}

int SDLCALL
SDL_GameControllerRumble(SDL_GameController* const gamecontroller, const Uint16 low_frequency_rumble, const Uint16 high_frequency_rumble, const Uint32 duration_ms)
{
    return ___GameControllerRumble(gamecontroller, low_frequency_rumble, high_frequency_rumble, duration_ms);
}

SDL_bool SDLCALL
SDL_GameControllerRumbleTriggers(SDL_GameController* const pGp, const Uint16 left_rumble, const Uint16 right_rumble, const Uint32 duration_ms)
{
    return ___GameControllerRumbleTriggers(pGp, left_rumble, right_rumble, duration_ms);
}

int SDLCALL
SDL_NumJoysticks(void)
{
    return ___NumJoysticks();
}

const char* SDLCALL
SDL_GameControllerName(SDL_GameController* const gamecontroller)
{
    return ___GameControllerName(gamecontroller);
}

/****** Static **********************************************************************/
static utf8*
GetMappingFilePath(void)
{
    const size_t sz_buf = StringSize(GetModPath(), STR_NOMAX) + 21;

    utf8* const pu_buf = MemAlloc(sz_buf);

    snprintf(pu_buf, sz_buf, "%s/%s", GetModPath(), "gamecontrollerdb.txt");

    return pu_buf;
}

/****** API *************************************************************************/
void*
ICSDL_GetHandle(void)
{
    return SdlHandle;
}

void*
ICSDL_GetExport(const char* const cExName)
{
    return DLL_GetExport(SdlHandle, cExName);
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

    SDL_Init( SDL_INIT_GAMECONTROLLER );

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

    SDL_Quit();

    DLL_Unmount(SdlHandle);
}
