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

static INT_POINT2   WindowSize;
static INT_POINT2   GameResolution;

/************************/
/*  Source              */
/************************/
bool
WindowInFocus(void)
{
    return FocusState;
}

void
IC_WindowUpdate(void)
{
    OS_GetGameWindowSize(&WindowSize);

    GameResolution.x = (int32_t) DisplayResolutionX;
    GameResolution.y = (int32_t) DisplayResolutionY;
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
