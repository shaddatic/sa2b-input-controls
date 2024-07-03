#include <sa2b/core.h>
#include <sa2b/config.h>

#include <ic_config.h>

static config* ConfigPtr;

void
CnfInit(void)
{
    ConfigPtr = ConfigOpen2(GetModPath(), "config.ini");
}

void
CnfEnd(void)
{
    ConfigClose(ConfigPtr);
}


s32
CnfGetInt(const CONFIG_INT* const pCnf)
{
    return CnfDirectGetInt(pCnf->sect, pCnf->key, pCnf->def);
}

f64
CnfGetFloat(const CONFIG_FLOAT* const pCnf)
{
    return CnfDirectGetFloat(pCnf->sect, pCnf->key, (f64)pCnf->def);
}

bool
CnfGetBool(const CONFIG_BOOL* const pCnf)
{
    return CnfDirectGetBool(pCnf->sect, pCnf->key, pCnf->def);
}

const utf8*
CnfGetString(const CONFIG_STRING* const pCnf)
{
    return CnfDirectGetString(pCnf->sect, pCnf->key, pCnf->def);
}


s32
CnfDirectGetInt(const char* const section, const char* const key, const s32 def)
{
    return ConfigGetInt(ConfigPtr, section, key, def);
}

f64
CnfDirectGetFloat(const char* const section, const char* const key, const f64 def)
{
    return ConfigGetFloat(ConfigPtr, section, key, def);
}

bool
CnfDirectGetBool(const char* const section, const char* const key, const bool def)
{
    return ConfigGetBool(ConfigPtr, section, key, def);
}

const utf8*
CnfDirectGetString(const char* const section, const char* const key, const utf8* const def)
{
    return ConfigGetString(ConfigPtr, section, key, def);
}


f64
CnfGetPercent(const CONFIG_INT* const pCnf)
{
    return CnfDirectGetPercent(pCnf->sect, pCnf->key, pCnf->def);
}

f64
CnfDirectGetPercent(const char* const section, const char* const key, const s32 def)
{
    return ConfigGetFloat(ConfigPtr, section, key, (f64)def) * 0.01;
}
