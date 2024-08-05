/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */
#include <sa2b/config.h>    /* ConfigOpen2, ConfigClose, ConfigGet##                */

/****** Self ************************************************************************/
#include <cnf.h>            /* self                                                 */

/************************/
/*  File Data           */
/************************/
/****** Config Pointer **************************************************************/
static config* ConfigP;     /* config pointer                                       */

/****** Config Set Flag *************************************************************/
static bool ConfigSet;      /* config has had a value changed                       */

/************************/
/*  Includes            */
/************************/
/****** Get *************************************************************************/
s32
CNF_GetInt(const CONFIG_INT* const pCnf)
{
    return CNF_DirectGetInt(pCnf->sect, pCnf->key, pCnf->def);
}

f64
CNF_GetFloat(const CONFIG_FLOAT* const pCnf)
{
    return CNF_DirectGetFloat(pCnf->sect, pCnf->key, (f64)pCnf->def);
}

bool
CNF_GetBool(const CONFIG_BOOL* const pCnf)
{
    return CNF_DirectGetBool(pCnf->sect, pCnf->key, pCnf->def);
}

const utf8*
CNF_GetString(const CONFIG_STRING* const pCnf)
{
    return CNF_DirectGetString(pCnf->sect, pCnf->key, pCnf->def);
}

/****** Set *************************************************************************/
void
CNF_SetInt(const CONFIG_INT* const pCnf, s32 val)
{
    CNF_DirectSetInt(pCnf->sect, pCnf->key, val);
}

void
CNF_SetFloat(const CONFIG_FLOAT* const pCnf, f64 val)
{
    CNF_DirectSetFloat(pCnf->sect, pCnf->key, val);
}

void
CNF_SetBool(const CONFIG_BOOL* const pCnf, bool val)
{
    CNF_DirectSetBool(pCnf->sect, pCnf->key, val);
}

void
CNF_SetString(const CONFIG_STRING* const pCnf, const utf8* val)
{
    CNF_DirectSetString(pCnf->sect, pCnf->key, val);
}

/****** Direct Get ******************************************************************/
s32
CNF_DirectGetInt(const char* const section, const char* const key, const s32 def)
{
    return ConfigGetInt(ConfigP, section, key, def);
}

f64
CNF_DirectGetFloat(const char* const section, const char* const key, const f64 def)
{
    return ConfigGetFloat(ConfigP, section, key, def);
}

bool
CNF_DirectGetBool(const char* const section, const char* const key, const bool def)
{
    return ConfigGetBool(ConfigP, section, key, def);
}

const utf8*
CNF_DirectGetString(const char* const section, const char* const key, const utf8* const def)
{
    return ConfigGetString(ConfigP, section, key, def);
}

/****** Direct Set ******************************************************************/
void
CNF_DirectSetInt(const char* const section, const char* const key, const s32 val)
{
    ConfigSet = true;

    ConfigSetInt(ConfigP, section, key, val);
}

void
CNF_DirectSetFloat(const char* const section, const char* const key, const f64 val)
{
    ConfigSet = true;

    ConfigSetFloat(ConfigP, section, key, val);
}

void
CNF_DirectSetBool(const char* const section, const char* const key, const bool val)
{
    ConfigSet = true;

    ConfigSetBool(ConfigP, section, key, val);
}

void
CNF_DirectSetString(const char* const section, const char* const key, const utf8* const val)
{
    ConfigSet = true;

    ConfigSetString(ConfigP, section, key, val);
}

/****** Extensions ******************************************************************/
f64
CNF_GetPercent(const CONFIG_INT* const pCnf)
{
    return CNF_DirectGetPercent(pCnf->sect, pCnf->key, pCnf->def);
}

void
CNF_SetPercent(const CONFIG_INT* const pCnf, s32 val)
{
    CNF_DirectSetPercent(pCnf->sect, pCnf->key, val);
}

f64
CNF_DirectGetPercent(const char* const section, const char* const key, const s32 def)
{
    return CNF_DirectGetFloat(section, key, (f64)def) * 0.01;
}

void
CNF_DirectSetPercent(const char* const section, const char* const key, const s32 val)
{
    CNF_DirectSetFloat(section, key, (f64)val);
}

/****** Init/End ********************************************************************/
void
CNF_Init(void)
{
    ConfigP = ConfigOpen2(GetModPath(), "config.ini");
}

void
CNF_End(void)
{
    if (ConfigSet)
        ConfigSave(ConfigP);

    ConfigClose(ConfigP);
}

void
CNF_Save(void)
{
    ConfigSave(ConfigP);
}
