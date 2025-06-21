/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <samt/core.h>      /* core                                                 */
#include <samt/modloader.h> /* modloader                                            */
#include <samt/modinfo.h>   /* mods                                                 */

/****** Util ************************************************************************/
#include <samt/util/dllexport.h> /* EXPORT_DLL                                      */

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */

/****** Self ************************************************************************/
#include <ic_api.h>                /* self                                          */
#include <ic_api/icapi_internal.h> /* internal                                      */

/************************/
/*  Constants           */
/************************/
/****** Version *********************************************************************/
#define ICAPI_CORE_VER      (0) /* API version                                      */

/************************/
/*  Typedefs            */
/************************/
/****** API Init User-Function ******************************************************/
typedef void(__cdecl IC_INIT)(const ICAPI_CORE*, const char*, const HelperFunctions*);

/************************/
/*  File Data           */
/************************/
/****** API Struct ******************************************************************/
EXPORT_DLL
const ICAPI_CORE icapi_core =
{
    .version = ICAPI_CORE_VER,

    .modver = { 1,0,1,0 },

    .pApiFeature  = &icapi_feature,
    .pApiConfig   = &icapi_config,
    .pApiUser     = &icapi_user,
    .pApiGamepad  = &icapi_gamepad,
    .pApiKeyboard = &icapi_keyboard,
    .pApiMouse    = &icapi_mouse,
    .pApiWindow   = &icapi_window,
    .pApiSdl      = &icapi_sdl,
};

/************************/
/*  Source              */
/************************/
/****** Static **********************************************************************/
static void
ApiCallByFuncName(const char* const cExName)
{
    const size_t nb_mod = miGetModCount();

    for (size_t i = 0; i < nb_mod; ++i)
    {
        const ml_modinfo* const p_mi = miGetInfoByIndex(i);

        IC_INIT* const p_init = miGetExport(p_mi, cExName);

        if (p_init)
            p_init(&icapi_core, p_mi->puPath, mtGetHelperFunctions());
    }
}

/****** Init/End ********************************************************************/
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
