/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

#include <sa2b/sonic/display.h>

#include <ic_core.h>
#include <ic_os.h>
#include <ic_input.h>

#include <ic_window.h>

/************************/
/*  File Data           */
/************************/
static bool         FocusState;

static NJS_POINT2I  WindowSize;
static NJS_POINT2I  GameResolution;

/************************/
/*  Source              */
/************************/
bool
WindowInFocus(void)
{
    return FocusState;
}

void
WindowGetSize(NJS_POINT2I* const pOutSize)
{
    *pOutSize = WindowSize;
}

void
WindowGetGameResolution(NJS_POINT2I* const pOutRes)
{
    *pOutRes = GameResolution;
}

void
IC_WindowUpdate(void)
{
    INT_POINT2 size;

    OS_GetGameWindowSize(&size);

    WindowSize.x = (Sint16) size.x;
    WindowSize.y = (Sint16) size.y;

    GameResolution.x = (Sint16) DisplayResolutionX;
    GameResolution.y = (Sint16) DisplayResolutionY;
}

void
OSMSG_KillFocus(void)
{
    FocusState = false;
}

void
OSMSG_SetFocus(void)
{
    FocusState = true;
}

void
IC_WindowInit(void)
{
    FocusState = true;
}
