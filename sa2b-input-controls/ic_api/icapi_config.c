/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */

/****** Config **********************************************************************/
#include <cnf.h>            /* CnfDirectGet##                                       */

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

    .GetInt    = CNF_DirectGetInt,
    .GetFloat  = CNF_DirectGetFloat,
    .GetBool   = CNF_DirectGetBool,
    .GetString = CNF_DirectGetString,
};
