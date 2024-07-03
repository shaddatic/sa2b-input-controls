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
#include <ic_config/cnf_def.h>

EXTERN_START

/************************/
/*  Structures          */
/************************/
typedef struct
{
    const utf8* sect;
    const utf8* key;
    const s32   def;
}
CONFIG_INT;

typedef struct
{
    const utf8* sect;
    const utf8* key;
    const bool  def;
}
CONFIG_BOOL;

typedef struct
{
    const utf8*     sect;
    const utf8*     key;
    const f32       def;
}
CONFIG_FLOAT;

typedef struct
{
    const utf8* sect;
    const utf8* key;
    const utf8* def;
}
CONFIG_STRING;

/************************/
/*  Functions           */
/************************/
/****** Init ************************************************************************/
/*
*   Description
*     Init Config module.
*/
void    CnfInit( void );
/*
*   Description
*     End Config module.
*/
void    CnfEnd( void );

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
s32         CnfGetInt(     const CONFIG_INT*    pCnfDef );
f64         CnfGetFloat(   const CONFIG_FLOAT*  pCnfDef );
bool        CnfGetBool(    const CONFIG_BOOL*   pCnfDef );
const utf8* CnfGetString(  const CONFIG_STRING* pCnfDef );
f64         CnfGetPercent( const CONFIG_INT*    pCnfDef );

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
s32         CnfDirectGetInt(     const char* section, const char* key, s32          def );
f64         CnfDirectGetFloat(   const char* section, const char* key, f64          def );
bool        CnfDirectGetBool(    const char* section, const char* key, bool         def );
const utf8* CnfDirectGetString(  const char* section, const char* key, const utf8*  def );
f64         CnfDirectGetPercent( const char* section, const char* key, s32          def );

EXTERN_END

#endif/*H_IC_CONFIG*/
