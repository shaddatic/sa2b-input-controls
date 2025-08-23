/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <samt/core.h>      /* core                                                 */
#include <samt/memory.h>    /* MemAlloc, MemFree, mReAlloc                          */
#include <samt/string.h>    /* StringSize                                           */
#include <samt/dll.h>       /* DLL_Mount2, DLL_GetExportList                        */
#include <samt/msgbox.h>    /* msgerror                                             */
#include <samt/file.h>      /* mtfileexist                                          */

/****** Utility *********************************************************************/
#include <samt/util/dllexport.h> /* dllexport                                       */

/****** Simple DirectMedia Layer ****************************************************/
#include <SDL2/SDL.h>       /* core                                                 */

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_input.h>       /* event handler                                        */

/****** C Sdl ***********************************************************************/
#include <stdio.h>          /* std in out                                           */

/****** Self ************************************************************************/
#include <ic_sdl2.h>        /* self                                                 */

/************************/
/*  Constants           */
/************************/
/****** Event Handler ***************************************************************/
#define HANDLER_CHUNK_SIZE      (8)

/************************/
/*  Macros              */
/************************/
/****** SDL Function Exports ********************************************************/
#define SDL_FUNC_PTR(retn, name, para)      static retn(SDLCALL* ___##name)para

#define SDL_EXPORT(name)                    { &___##name, "SDL_"#name }

/************************/
/*  File Data           */
/************************/
/****** DLL Handle ******************************************************************/
static mt_dllhandle* SdlHandle; /* SDL DLL handle                                   */

/****** Function Pointers ***********************************************************/
SDL_FUNC_PTR(int                , Init                              , (int)                                            );
SDL_FUNC_PTR(void               , Quit                              , (void)                                           );
SDL_FUNC_PTR(int                , PollEvent                         , (SDL_Event*)                                     );
SDL_FUNC_PTR(SDL_GameController*, GameControllerOpen                , (int)                                            );
SDL_FUNC_PTR(void               , GameControllerClose               , (SDL_GameController*)                            );
SDL_FUNC_PTR(SDL_bool           , IsGameController                  , (int)                                            );
SDL_FUNC_PTR(Sint16             , GameControllerGetAxis             , (SDL_GameController*, int)                       );
SDL_FUNC_PTR(SDL_bool           , GameControllerHasButton           , (SDL_GameController*, SDL_GameControllerButton)  );
SDL_FUNC_PTR(Uint8              , GameControllerGetButton           , (SDL_GameController*, SDL_GameControllerButton)  );
SDL_FUNC_PTR(SDL_bool           , GameControllerHasRumble           , (SDL_GameController*)                            );
SDL_FUNC_PTR(SDL_bool           , GameControllerHasRumbleTriggers   , (SDL_GameController*)                            );
SDL_FUNC_PTR(int                , GameControllerRumble              , (SDL_GameController*, Uint16, Uint16, Uint32)    );
SDL_FUNC_PTR(int                , GameControllerRumbleTriggers      , (SDL_GameController*, Uint16, Uint16, Uint32)    );
SDL_FUNC_PTR(int                , NumJoysticks                      , (void)                                           );
SDL_FUNC_PTR(const char*        , GameControllerName                , (SDL_GameController*)                            );
SDL_FUNC_PTR(int                , GameControllerAddMappingsFromRW   , (SDL_RWops*, int)                                );
SDL_FUNC_PTR(SDL_RWops*         , RWFromFile                        , (const char*, const char*)                       );
SDL_FUNC_PTR(const char*        , GetError                          , (void)                                           );
SDL_FUNC_PTR(char*              , GameControllerMapping             , (SDL_GameController*)                            );
SDL_FUNC_PTR(void               , free                              , (void*)                                          );

/****** Export List *****************************************************************/
static mt_dllexport SdlExports[] =
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
    SDL_EXPORT(GetError),
    SDL_EXPORT(GameControllerMapping),
    SDL_EXPORT(free),
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

int SDLCALL
SDL_GameControllerAddMappingsFromRW(SDL_RWops* const rw, const int freerw)
{
    return ___GameControllerAddMappingsFromRW(rw, freerw);
}

SDL_RWops* SDLCALL
SDL_RWFromFile(const char* const file, const char* const mode)
{
    return ___RWFromFile(file, mode);
}

const char* SDLCALL
SDL_GetError(void)
{
    return ___GetError();
}

char* SDLCALL
SDL_GameControllerMapping(SDL_GameController* gamecontroller)
{
    return ___GameControllerMapping(gamecontroller);
}

void SDLCALL
SDL_free(void *mem)
{
    ___free(mem);
}

/****** Static **********************************************************************/
static c8*
GetMappingFilePath(const c8* puPath)
{
    const size_t sz_buf = mtStrLength(puPath, STR_NOMAX) + sizeof("./gamecontrollerdb.txt");

    c8* const pu_buf = mtAlloc(c8, sz_buf);

    mtStrCopy(pu_buf, "./gamecontrollerdb.txt", sz_buf);

    if ( mtFileExists(pu_buf) )
    {
        return pu_buf;
    }

    mtStrFormat(pu_buf, sz_buf, "./%s/%s", puPath, "gamecontrollerdb.txt");

    if ( mtFileExists(pu_buf) )
    {
        return pu_buf;
    }

    // no file found, free memory return nullptr
    mtFree(pu_buf);

    return nullptr;
}

/****** Extern **********************************************************************/
void*
ICSDL_GetHandle(void)
{
    return SdlHandle;
}

void*
ICSDL_GetExport(const char* const cExName)
{
    if ( !SdlHandle )
    {
        return nullptr;
    }

    return mtDllGetExport(SdlHandle, cExName);
}

void
ICSDL_RegisterEventHandler(void (__cdecl* fnEvHandler)(const SDL_Event*))
{
    return;
}

EXPORT_DLL
int32_t
SASDLAPI_Init(const SASDLAPI* pApi, const c8* puPath, const void* pHelpFuncs, usize ixMod)
{
    // register event handler
    pApi->RegisterEventHandler( &GamepadEventHandler );

    // get dll handle
    mt_dllhandle* p_sdlhdl = pApi->GetHandle();

    mtDllGetExportList(p_sdlhdl, SdlExports, ARYLEN(SdlExports));

    // get player mapping file
    c8* const pu_buf = GetMappingFilePath(puPath);

    if ( pu_buf )
    {
        if ( SDL_GameControllerAddMappingsFromFile(pu_buf) == -1 )
        {
            OutputFormat("IC INFO: Error parsing 'gamecontrollerdb.txt' (ERR: %s)!", SDL_GetError());
        }
        else
        {
            OutputString("IC INFO: Successfully loaded 'gamecontrollerdb.txt' file!");
        }

        mtMemFree(pu_buf);
    }
    else
    {
        OutputString("IC INFO: 'gamecontrollerdb.txt' file not found!");
    }

    // set handle
    SdlHandle = p_sdlhdl;

    // return gamecontroller flag
    return SDL_INIT_GAMECONTROLLER;
}
