/*
*   SA2 Input Controls - '/icapi_config.h'
*
*   Description:
*     The 'Config' API module of Input Controls is for reading its config settings file
*   directly. It's not recommended for longterm use outside of very specific circumstances, as
*   config definitions change frequently.
*     If you do use it, restrict it to certain versions of Input Controls to ensure it works
*   correctly.
* 
*   Version History:
*     - v1.0.0.0        : Version 0, initial release
* 
*   Availability:
*     - Before Init     : No
*     - At Init         : Yes
*     - After Init      : No
* 
*   Notice:
*     The API headers are to be included in your own projects. You have full rights to use,
*   distribute, and modify the API headers as you see fit.
*/
#ifndef H_ICAPI_CONFIG
#define H_ICAPI_CONFIG

/********************************/
/*  API Structures              */
/********************************/
/****** Feature API *****************************************************************************/
typedef struct
{
    uint32_t version;               /* structure version                                        */

    /****** Version >= 0 ********************************************************************/

    /**** Get Config ****************************************/
    /*
    *   Description:
    *     Read user-settings inside Input Controls' configuration file. It's important to
    *   remember that strings are stored in UTF-8 format.
    *
    *   Parameters:
    *     - puSect      : config setting section
    *     - puKey       : config setting key
    *     - def         : default config value
    *
    *   Returns:
    *     The value of the config entry; or 'def' if no entry exists.
    */
    int32_t  (__cdecl* GetInt)(    const c8* puSect, const c8* puKey, int32_t   def );
    bool     (__cdecl* GetBool)(   const c8* puSect, const c8* puKey, bool      def );
    f64      (__cdecl* GetFloat)(  const c8* puSect, const c8* puKey, f64       def );
    const c8*(__cdecl* GetString)( const c8* puSect, const c8* puKey, const c8* def );
}
ICAPI_CONFIG;

#endif/*H_ICAPI_CONFIG*/
