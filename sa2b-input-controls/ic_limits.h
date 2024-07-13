/*
*   SA2 Input Controls - '/ic_limits.h'
*
*   Description:
*     Limits definition header
*
*   Contributors:
*     - Shaddatic
*/
#ifndef H_IC_LIMITS
#define H_IC_LIMITS

/************************/
/*  Constants           */
/************************/
/****** PDS Pad *********************************************************************/
/****** Analog Stick ********************************************************/
#define PDSLIM_XY_MIN   (-128)  /* 100% up/left                             */
#define PDSLIM_XY_MAX   (+128)  /* 100% down/right/down                     */

/****** Analog Trigger ******************************************************/
#define PDSLIM_LR_MIN   (+  0)  /* at rest                                  */
#define PDSLIM_LR_MAX   (+255)  /* 100% down                                */

/****** User Input ******************************************************************/
/****** Analog Stick ********************************************************/
#define USRLIM_XY_MIN   (-1.f)  /* 100% up/left                             */
#define USRLIM_XY_MAX   (+1.f)  /* 100% down/right/down                     */

/****** Analog Trigger ******************************************************/
#define USRLIM_LR_MIN   (+0.f)  /* at rest                                  */
#define USRLIM_LR_MAX   (+1.f)  /* 100% down                                */

#endif/*H_IC_LIMITS*/
