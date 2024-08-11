/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */
#include <sa2b/modloader.h> /* modloader                                            */
#include <sa2b/modinfo.h>   /* mods                                                 */

/****** Util ************************************************************************/
#include <sa2b/util/dllexport.h> /* EXPORT_DLL                                      */

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
ICAPI_CORE icapi_core =
{
    .version = ICAPI_CORE_VER,

    .ic_version = { 1,0,0,1 },
};

/************************/
/*  Source              */
/************************/
/****** Static **********************************************************************/
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

/****** Init/End ********************************************************************/
void
ICAPI_Init(void)
{
    icapi_core.pApiConfig = &icapi_config,
    icapi_core.pApiSdl    = &icapi_sdl;

    ApiCallByFuncName("IC_EarlyInit");
}

void
ICAPI_End(void)
{
    icapi_core.pApiFeature = &icapi_feature;
    icapi_core.pApiUser     = &icapi_user;
    icapi_core.pApiGamepad  = &icapi_gamepad;
    icapi_core.pApiKeyboard = &icapi_keyboard;
    icapi_core.pApiMouse    = &icapi_mouse;
    icapi_core.pApiWindow   = &icapi_window;

    ApiCallByFuncName("IC_Init");

    icapi_core.pApiConfig = nullptr;
}
