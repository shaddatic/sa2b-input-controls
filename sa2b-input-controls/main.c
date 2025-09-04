/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <samt/core.h>      /* core                                                 */
#include <samt/init.h>      /* init                                                 */
#include <samt/msgbox.h>    /* message                                              */
#include <samt/modinfo.h>   /* getinfo                                              */

/****** Mod Loader ******************************************************************/
#include <samt/modloader.h> /* mod loader                                           */

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_input.h>       /* input                                                */
#include <ic_socmagic.h>    /* soc/magic                                            */
#include <ic_sonicinput.h>  /* sonic input                                          */
#include <ic_os.h>          /* operating system                                     */
#include <ic_vibtask.h>     /* vibtask                                              */
#include <ic_window.h>      /* window                                               */
#include <ic_camera.h>      /* camera                                               */
#include <ic_api.h>         /* input controls API                                   */
#include <ic_misc.h>        /* misc                                                 */
#include <ic_feature.h>     /* feature                                              */

/****** Config **********************************************************************/
#include <cnf.h>            /* CnfGet##                                             */

/************************/
/*  Source              */
/************************/
/****** DLL Export ******************************************************************/
EXPORT_DLL
void __cdecl
Init(const c8* puPath, const ml_helpfuncs* pHelpFuncs, usize ixMod)
{
    mtSystemInit(puPath, pHelpFuncs, ixMod);

    if ( !miCheckSupport() )
    {
        mtMsgError(
            "Input Controls : Mod Loader Version",

            "Input Controls can't operate safely on the currently installed version of the SA2 Mod Loader.\n"
            "Please update the Mod Loader to a newer version!\n\n"

            "Input Controls will now abort the init process."
        );
        return;
    }

    if ( !miGetInfoByID("sasdl") )
    {
        mtMsgError(
            "Input Controls : SA SDL Dependancy",

            "Input Controls requires the \"SA SDL Loader\" dependancy mod to work, but you don't have it installed.\n\n"

            "Before v1.1, Input Controls loaded its own SDL library but has now switched to using a common dependancy mod.\n\n"

            "Input Controls will now abort the init process."
        );
        return;
    }

    ICAPI_CallUserFuncs( IC_UF_EARLY );

    CNF_Init();

    ICF_Init();
    IC_InputGetConfig();

    ICAPI_CallUserFuncs( IC_UF_INIT );
    ICAPI_CallUserFuncs( IC_UF_OLD_EARLY );

    IC_InputInit();
    IC_CameraInit();
    IC_VibTaskInit();
    IC_SocMagicInit();
    IC_SonicInputInit();
    OS_Init();
    WND_Init();
    IC_MiscInit();

    ICAPI_CallUserFuncs( IC_UF_OLD_INIT );

    CNF_End();

    ICAPI_CallUserFuncs( IC_UF_LATE );
}
