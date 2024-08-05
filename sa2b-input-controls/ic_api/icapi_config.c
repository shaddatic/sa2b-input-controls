/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Self ************************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_config.h>      /* config                                               */

/****** Self ************************************************************************/
#include <ic_api/icapi_internal.h> /* self                                          */

/************************/
/*  Constants           */
/************************/
/****** Version *********************************************************************/
#define ICAPI_CONFIG_VER    (0) /* API version                                      */

/************************/
/*  File Data           */
/************************/
/****** API Struct ******************************************************************/
const ICAPI_CONFIG icapi_config =
{
    .version = ICAPI_CONFIG_VER,

    .GetInt    = CnfDirectGetInt,
    .GetFloat  = CnfDirectGetFloat,
    .GetBool   = CnfDirectGetBool,
    .GetString = CnfDirectGetString,

    .GetPercent = CnfDirectGetPercent,
};
