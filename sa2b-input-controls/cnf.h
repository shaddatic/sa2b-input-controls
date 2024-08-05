/*
*   SA2 Input Controls - '/ic_config.h'
*
*   Description:
*     Input Controls Config module.
*
*   Contributors:
*   -   Shaddatic
*/
#ifndef H_IC_CONFIG
#define H_IC_CONFIG

/************************/
/*  Internal Headers    */
/************************/
/****** Config Setting Definitions **************************************************/
#include <cnf/cnf_def.h>    /* config def                                           */

EXTERN_START

/************************/
/*  Structures          */
/************************/
/****** Config Int Entry ************************************************************/
typedef struct
{
    const utf8* sect;       /* config entry section                                 */
    const utf8* key;        /* config entry key                                     */
    const s32   def;        /* config entry default value                           */
}
CONFIG_INT;

/****** Config Bool Entry ***********************************************************/
typedef struct
{
    const utf8* sect;       /* config entry section                                 */
    const utf8* key;        /* config entry key                                     */
    const bool  def;        /* config entry default value                           */
}
CONFIG_BOOL;

/****** Config Float Entry **********************************************************/
typedef struct
{
    const utf8* sect;       /* config entry section                                 */
    const utf8* key;        /* config entry key                                     */
    const f32   def;        /* config entry default value                           */
}
CONFIG_FLOAT;

/****** Config String Entry *********************************************************/
typedef struct
{
    const utf8* sect;       /* config entry section                                 */
    const utf8* key;        /* config entry key                                     */
    const utf8* def;        /* config entry default value                           */
}
CONFIG_STRING;

/************************/
/*  Prototypes          */
/************************/
/****** Init/End ********************************************************************/
/*
*   Description
*     Init Config module.
*/
void    CNF_Init( void );
/*
*   Description
*     End Config module.
*/
void    CNF_End( void );

/****** Save ************************************************************************/
/*
*   Description
*     Save Config file.
*/
void    CNF_Save( void );

/****** Get *************************************************************************/
/*
*   Description:
*     Get a config setting via config definition struct.
*
*   Parameters:
*     - pCnfDef : pointer to a config setting definition
*
*   Returns:
*     The user setting if it exists. If not, the default value
*/
s32         CNF_GetInt(     const CONFIG_INT*    pCnfDef );
f64         CNF_GetFloat(   const CONFIG_FLOAT*  pCnfDef );
bool        CNF_GetBool(    const CONFIG_BOOL*   pCnfDef );
const utf8* CNF_GetString(  const CONFIG_STRING* pCnfDef );

/****** Set *************************************************************************/
/*
*   Description:
*     Set a config setting via config definition struct.
*
*   Parameters:
*     - pCnfDef : pointer to a config setting definition
*     - val     : value to set
*/
void    CNF_SetInt(     const CONFIG_INT*    pCnfDef, s32         val );
void    CNF_SetFloat(   const CONFIG_FLOAT*  pCnfDef, f64         val );
void    CNF_SetBool(    const CONFIG_BOOL*   pCnfDef, bool        val );
void    CNF_SetString(  const CONFIG_STRING* pCnfDef, const utf8* val );

/****** Direct Get ******************************************************************/
/*
*   Description:
*     Get a config setting via direct config parameters.
*
*   Parameters:
*     - section : section to search in
*     - key     : key to find
*     - def     : default value
*
*   Returns:
*     The user setting if it exists. If not, the default value
*/
s32         CNF_DirectGetInt(     const char* uSect, const char* uKey, s32          def );
f64         CNF_DirectGetFloat(   const char* uSect, const char* uKey, f64          def );
bool        CNF_DirectGetBool(    const char* uSect, const char* uKey, bool         def );
const utf8* CNF_DirectGetString(  const char* uSect, const char* uKey, const utf8*  def );

/****** Direct Set ******************************************************************/
/*
*   Description:
*     Set a config setting via direct config parameters.
*
*   Parameters:
*     - section : section to search in
*     - key     : key to find
*     - val     : value to set
*/
void    CNF_DirectSetInt(     const char* uSect, const char* uKey, s32          val );
void    CNF_DirectSetFloat(   const char* uSect, const char* uKey, f64          val );
void    CNF_DirectSetBool(    const char* uSect, const char* uKey, bool         val );
void    CNF_DirectSetString(  const char* uSect, const char* uKey, const utf8*  val );

/****** Extensions ******************************************************************/
/*
*   Description:
*     Get a config percentage setting via config definition struct.
*
*   Parameters:
*     - pCnfDef : pointer to a config setting definition
*
*   Returns:
*     The user setting if it exists. If not, the default value
*/
f64         CNF_GetPercent( const CONFIG_INT* pCnfDef );
/*
*   Description:
*     Get a config setting via direct config parameters.
*
*   Parameters:
*     - section : section to search in
*     - key     : key to find
*     - def     : default value
*
*   Returns:
*     The user setting if it exists. If not, the default value
*/
f64         CNF_DirectGetPercent( const char* uSect, const char* uKey, s32 def );
/*
*   Description:
*     Get a config percentage setting via config definition struct.
*
*   Parameters:
*     - pCnfDef : pointer to a config setting definition
*     - val     : value to set
*
*   Returns:
*     The user setting if it exists. If not, the default value
*/
void    CNF_SetPercent( const CONFIG_INT* pCnfDef, s32 val );
/*
*   Description:
*     Get a config setting via direct config parameters.
*
*   Parameters:
*     - section : section to search in
*     - key     : key to find
*     - val     : value to set
*
*   Returns:
*     The user setting if it exists. If not, the default value
*/
void    CNF_DirectSetPercent( const char* uSect, const char* uKey, s32 val );

EXTERN_END

#endif/*H_IC_CONFIG*/
