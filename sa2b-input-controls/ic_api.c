/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */
#include <sa2b/modloader.h> /* modloader                                            */
#include <sa2b/mods.h>      /* mods                                                 */

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

    .pApiUser     = nullptr,
    .pApiGamepad  = nullptr,
    .pApiKeyboard = &icapi_keyboard,
    .pApiMouse    = nullptr,
    .pApiWindow   = nullptr,
};

/************************/
/*  Typedefs            */
/************************/
typedef void(__cdecl IC_INIT)(const ICAPI_CORE*, const char*, const HelperFunctions*);

/************************/
/*  Source              */
/************************/
static void
ApiCallByFuncName(const char* name)
{
    const size_t nb_mod = ModGetTotalNumber();

    for (size_t i = 0; i < nb_mod; ++i)
    {
        const mod_handle* mhp = ModGetHandlePosition(i);

        IC_INIT* const init = ModGetExport(mhp, name);

        if (init)
            init(&icapi_core, ModGetPath(mhp), ML_GetHelperFunctions());
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
