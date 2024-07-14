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
#include <ic_api.h>

/************************/
/*  Constants           */
/************************/
/****** Mod Loader Version Mins *****************************************************/
#define MLVER_API_MIN       (9) /* minimum mod loader version for API               */
#define MLVER_MIN           (8) /* minimum mod loader version                       */

/************************/
/*  Source              */
/************************/
EXPORT_DLL
void __cdecl
Init(const char* path, const HelperFunctions* pHelperFunctions)
{
    SAMT_Init(path, pHelperFunctions);

    bool can_api = true;

    {
        const int ml_ver = ML_GetVersion();

        if (ml_ver < MLVER_MIN)
        {
        UserErrorMessageBox("Input Controls : Mod Loader Version",
                "Input Controls can't operate safely on the currently installed version of the SA2 Mod Loader.\n"
                "Please update the Mod Loader to a newer version!\n\n"
                "Input Controls will now abort the init process."
        );

            return;
        }
        else if (ml_ver < MLVER_API_MIN)
        {
            /** Just print a warning to the console **/
            OutputString("Input Controls : WARNING\n"
                "The installed mod loader version doesn't support features required for the Input Controls API."
                "The API will be disabled to prevent crashing. However, it is recommended you update to the latest "
                "Mod Loader version, as older versions will not be actively supported");

        can_api = false;
    }
    }

    CnfInit();

    if (can_api)
        ICAPI_Init();

    IC_InputInit();
    IC_CameraInit();
    IC_VibTaskInit();
    IC_SocMagicInit();
    IC_SonicInputInit();
    OS_Init();
    IC_WindowInit();

    if (can_api)
        ICAPI_End();

    CnfEnd();
}

EXPORT_DLL
ModInfo SA2ModInfo = { ML_VERSION };
