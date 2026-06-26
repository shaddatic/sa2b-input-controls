/********************************/
/*  Includes                    */
/********************************/
/****** Core Toolkit ****************************************************************************/
#include <samt/core.h>              /* core                                                     */
#include <samt/modloader.h>         /* modloader                                                */
#include <samt/modinfo.h>           /* mods                                                     */
#include <samt/msgbox.h>            /* message box                                              */
#include <samt/string.h>            /* string                                                   */

/****** Util ************************************************************************************/
#include <samt/util/dllexport.h>    /* EXPORT_DLL                                               */

/****** Input Controls **************************************************************************/
#include <ic_core.h>                /* core                                                     */

/****** Self ************************************************************************************/
#include <ic_api.h>                 /* self                                                     */
#include <ic_api/icapi_internal.h>  /* internal                                                 */

/********************************/
/*  Constants                   */
/********************************/
/****** Version *********************************************************************************/
#define ICAPI_CORE_VER      (0)     /* API version                                              */

/********************************/
/*  Typedefs                    */
/********************************/
/****** API Init User-Function ******************************************************************/
typedef i32(__cdecl IC_INIT)(const ICAPI*, const char*, const HelperFunctions*, isize);

/********************************/
/*  File Data                   */
/********************************/
/****** API Struct ******************************************************************************/
EXPORT_DLL
const ICAPI icapi_core =
{
    .version = ICAPI_CORE_VER,

    .modver = { 1,1,0,1 },

    .pApiFeature  = &icapi_feature,
    .pApiConfig   = &icapi_config,
    .pApiUser     = &icapi_user,
    .pApiGamepad  = &icapi_gamepad,
    .pApiKeyboard = &icapi_keyboard,
    .pApiMouse    = &icapi_mouse,
    .pApiWindow   = &icapi_window,
    .pApiSdl      = &icapi_sdl,
};

/********************************/
/*  Source                      */
/********************************/
/****** Init/End ********************************************************************************/
void
ICAPI_CallUserFuncs(const IC_USERFUNC uf)
{
    /****** Static Vars *********************************************************************/

    static const c7* const UserFuncNames[IC_NB_UF] =
    {
        [IC_UF_INIT]  = "ICAPI_Init",
        [IC_UF_EARLY] = "ICAPI_Early",
        [IC_UF_LATE]  = "ICAPI_Late",

        [IC_UF_OLD_INIT]  = "IC_Init",
        [IC_UF_OLD_EARLY] = "IC_EarlyInit",
    };

    /****** Start ***************************************************************************/

    const c7* const pc_uf = UserFuncNames[uf];

    const isize nb_mod = miGetModCount();

    for ( isize i = 0; i < nb_mod; ++i )
    {
        const ml_modinfo* const p_mi = miGetInfoByIndex(i);

        IC_INIT* const p_init = miGetExport(p_mi, pc_uf);

        if ( !p_init )
        {
            continue;
        }

        const i32 ret = p_init(&icapi_core, p_mi->puPath, mtGetHelperFunctions(), i);

        if ( uf < IC_UF_OLD_INIT && ret != 0 )
        {
            c8 buf[96];

            mtStrFormat(buf, ARYLEN(buf), "ICAPI : Unexpected Return Value (%s)", p_mi->puName);

            mtMsgWarning(buf,
                "Return value from user-exported function was != 0. The function is either malformed or made for a later version of Input Controls!");
        }
    }
}
