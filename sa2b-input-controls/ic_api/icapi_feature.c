/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <samt/core.h>      /* core                                                 */

/****** Self ************************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_feature.h>     /* config                                               */

/****** Self ************************************************************************/
#include <ic_api/icapi_internal.h> /* self                                          */

/************************/
/*  Constants           */
/************************/
/****** Version *********************************************************************/
#define ICAPI_FEATURE_VER   (1) /* API version                                      */

/************************/
/*  File Data           */
/************************/
/****** API Struct ******************************************************************/
const ICAPI_FEATURE icapi_feature =
{
    .version = ICAPI_FEATURE_VER,

    .UseRawAnalog         = ICF_UseRawAnalog,
    .CamInvertX2          = ICF_CamInvertX2,
    .MouseAnalogEmulation = ICF_MouseAnalogEmulation,

    // version 1

    .CamInvertLR          = ICF_CamInvertLR,
};
