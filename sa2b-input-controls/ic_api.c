/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */
#include <sa2b/modloader.h> /* modloader                                            */
#include <sa2b/modinfo.h>   /* mods                                                 */

/****** Util ************************************************************************/
#include <sa2b/util/dllexport.h>    /* EXPORT_DLL                                   */

/****** Input Controls **************************************************************/
#include <ic_core.h>    /* core                                                     */

/****** Self ************************************************************************/
#include <ic_api.h>                 /* self                                         */
#include <ic_api/icapi_internal.h>  /* internal                                     */

/************************/
/*  Constants           */
/************************/
#define ICAPI_CORE_VER      (0)

/************************/
/*  File Data           */
/************************/
EXPORT_DLL
const ICAPI_CORE icapi_core =
{
    .version = ICAPI_CORE_VER,

    .ic_version = { 1,0,0,0 },

    .pApiUser     = &icapi_user,
    .pApiGamepad  = &icapi_gamepad,
    .pApiKeyboard = &icapi_keyboard,
    .pApiMouse    = &icapi_mouse,
    .pApiWindow   = nullptr,
    .pApiSdl      = &icapi_sdl,
};

/************************/
/*  Typedefs            */
/************************/
typedef void(__cdecl IC_INIT)(const ICAPI_CORE*, const char*, const HelperFunctions*);

/************************/
/*  Source              */
/************************/
static void
ApiCallByFuncName(const char* const cExName)
{
    const size_t nb_mod = MI_GetTotalNumber();

    for (size_t i = 0; i < nb_mod; ++i)
    {
        const mod_info* const p_mi = MI_GetInfoByPosition(i);

        IC_INIT* const p_init = MI_GetExport(p_mi, cExName);

        if (p_init)
            p_init(&icapi_core, p_mi->cPath, ML_GetHelperFunctions());
    }
}

void
ICAPI_Init(void)
{
    ApiCallByFuncName("IC_EarlyInit");
}

void
ICAPI_End(void)
{
    ApiCallByFuncName("IC_Init");
}
