/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Self ************************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_feature.h>     /* config                                               */

/****** Self ************************************************************************/
#include <ic_api/icapi_internal.h> /* self                                          */

/************************/
/*  Constants           */
/************************/
/****** Version *********************************************************************/
#define ICAPI_FEATURE_VER   (0) /* API version                                      */

/************************/
/*  File Data           */
/************************/
/****** API Struct ******************************************************************/
const ICAPI_FEATURE icapi_feature =
{
    .version = ICAPI_FEATURE_VER,

    .UseRawAnalog = ICF_UseRawAnalog,
    .CamInvertX2  = ICF_CamInvertX2,
};
