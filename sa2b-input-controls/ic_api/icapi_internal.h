/*
*   SA2 Input Controls - '/ic_api/icapi_internal.h'
*
*   Description:
*     Internal header for the Input Controls mod API
*
*   Contributors:
*     - Shaddatic
*/
#ifndef H_IC_API_INTERNAL
#define H_IC_API_INTERNAL

/************************/
/*  External Headers    */
/************************/
/****** External API ****************************************************************/
#include <../api/inputcont_api.h>

/************************/
/*  API Structs         */
/************************/
/****** APIs ************************************************************************/
EXTERN const ICAPI_USER     icapi_user;
EXTERN const ICAPI_GAMEPAD  icapi_gamepad;
EXTERN const ICAPI_KEYBOARD icapi_keyboard;
EXTERN const ICAPI_SDL      icapi_sdl;

#endif/*H_IC_API_INTERNAL*/
