/*
*   SA2 Input Controls - '/icapi_feature.h'
*
*   Description:
*     The 'Feature' API module of Input Controls is for getting the current state of specific
*   mod features.
* 
*   Version History:
*     - v1.0.0.0        : Version 0, initial release
*     - v1.1.0.0        : Version 1, added function for LR camera inversion
* 
*   Availability:
*     - Before Init     : No
*     - At Init         : Yes
*     - After Init      : Yes
* 
*   Notice:
*     The API headers are to be included in your own projects. You have full rights to use,
*   distribute, and modify the API headers as you see fit.
*/
#ifndef H_ICAPI_FEATURE
#define H_ICAPI_FEATURE

/********************************/
/*  API Structures              */
/********************************/
/****** Feature API *****************************************************************************/
typedef struct
{
    uint32_t version;               /* structure version                                        */

    /****** Version >= 0 ********************************************************************/

    /**** Features ******************************************/
    /*
    *   Description:
    *     If Input Controls is set to use raw analog values, rather than values from the
    *   emulated Dreamcast controller.
    *
    *   Returns:
    *     'true' if the feature is enable; or 'false' if not.
    */
    bool (__cdecl* UseRawAnalog)( void );
    /*
    *   Description:
    *     If the camera is set to invert the X axis of the right analog stick input compared to
    *   vanilla. 
    *     When inverted, it better matches regular analog camera input from other games.
    *
    *   Returns:
    *     'true' if the feature is enable; or 'false' if not.
    */
    bool (__cdecl* CamInvertX2)( void );
    /*
    *   Description:
    *     If mouse analog emulation is enabled on any keyboard layout. If you wish to use the
    *   mouse in your own mod, you should make sure this is disabled.
    *
    *   Returns:
    *     'true' if the feature is enable; or 'false' if not.
    */
    bool (__cdecl* MouseAnalogEmulation)( void );

    /****** Version >= 1 ********************************************************************/

    /**** Camera ********************************************/
    /*
    *   Description:
    *     If the camera is set to invert the L and R triggers compared to vanilla.
    *
    *   Returns:
    *     'true' if the feature is enable; or 'false' if not.
    */
    bool (__cdecl* CamInvertLR)( void );
    /*
    *   Description:
    *     If the camera is set to invert the L and R triggers compared to vanilla.
    *
    *   Returns:
    *     'true' if the feature is enable; or 'false' if not.
    */
    bool (__cdecl* CamInvertY2)( void );
}
ICAPI_FEATURE;

#endif/*H_ICAPI_FEATURE*/
