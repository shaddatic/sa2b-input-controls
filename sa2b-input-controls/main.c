/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */
#include <sa2b/init.h>      /* init                                                 */
#include <sa2b/user.h>      /* user                                                 */

/****** Mod Loader ******************************************************************/
#include <sa2b/modloader.h> /* mod loader                                           */

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_config.h>      /* config                                               */
#include <ic_input.h>       /* input                                                */
#include <ic_socmagic.h>    /* soc/magic                                            */
#include <ic_sonicinput.h>  /* sonic input                                          */
#include <ic_os.h>          /* operating system                                     */
#include <ic_vibtask.h>     /* vibtask                                              */
#include <ic_window.h>      /* window                                               */
#include <ic_camera.h>      /* sonic input                                          */
#include <ic_sonicinput.h>  /* sonic input                                          */

/************************/
/*  Constants           */
/************************/
/** Minimum mod loader version **/
#define MLVER_MIN   (9)

EXPORT_DLL
void __cdecl
Init(const char* path, const HelperFunctions* pHelperFunctions)
{
    SAMT_Init(path, pHelperFunctions);

    bool can_api = true;

    if (ML_GetVersion() < MLVER_MIN)
    {
        UserErrorMessageBox("Input Controls : Mod Loader Version",
            "SA2 Input Controls cannot fully operate on this Mod Loader version.\n"
            "Some features will be disabled to prevent crashing. However, it is recommended "
            "you update to the latest Mod Loader version, as older versions will not be supported"
        );
        can_api = false;
    }

    CnfInit();

    IC_InputInit();
    IC_CameraInit();
    IC_VibTaskInit();
    IC_SocMagicInit();
    IC_SonicInputInit();
    OS_Init();
    IC_WindowInit();

    CnfEnd();
}

EXPORT_DLL
ModInfo SA2ModInfo = { ML_VERSION };
