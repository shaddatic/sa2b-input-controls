/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Input Controls **************************************************************/
#include <sa2b/ninja/njcommon.h>    /* ninja                                        */

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_os.h>          /* operating system                                     */
#include <ic_window.h>      /* window                                               */
#include <ic_config.h>      /* config                                               */

/****** Self ************************************************************************/
#include <ic_input/inpt_internal.h>
#include <ic_input/inpt_mouse/mse_internal.h>

/************************/
/*  Enums               */
/************************/
typedef enum
{
    CURSOR_FREE,        /* cursor is unlocked from the center of the game window    */
    CURSOR_CAPTURED,    /* cursor is locked to the center of the game window        */
}
eCURSOR_STATE;

typedef enum
{
    CURSOR_SUB_NONE,        /* no sub state                                         */
    CURSOR_SUB_FREEING,     /* cursor is to be freed next input execution           */
    CURSOR_SUB_CAPTURING,   /* cursor is to be captured, if possible, next exec     */
}
eCURSOR_SUBSTATE;

typedef enum
{
    EMU_MD_RAWVEC,
    EMU_MD_CLICKDRAG,
}
eEMU_MODE;

/************************/
/*  File Variables      */
/************************/
static MOUSE Mouse;                         /* mouse struct                         */

static eCURSOR_STATE    CursorState;        /* cursor state                         */
static eCURSOR_SUBSTATE CursorSubState;     /* cursor sub-state                     */

static INT_POINT2    CursorPos;
static INT_POINT2    CursorPosLast;         /* last position of the cursor          */

static f32           WheelBuffer;           /* os message wheel buffer              */

static eKEYBOARD_NUM MouseEmuKbIndex;       /* analog emulation keyboard num        */
static eEMU_STICK    MouseEmuStickIndex;    /* analog emulation stick num           */
static int32_t       MouseEmuMode;          /* analog emulation input mode          */
static f32           MouseEmuSensitivity;   /* analog emulation sensitivity         */

static NJS_POINT2I   MouseEmuDragVector;    /* click & drag vector                  */
static int32_t       MouseEmuDragMax;       /* click & drag max vector              */

static int32_t       MouseEmuClickKey;      /* click & drag 'click' key             */

/************************/
/*  Source              */
/************************/
void
MouseCapture(void)
{
    CursorSubState = CURSOR_SUB_CAPTURING;
}

void
MouseFree(void)
{
    CursorSubState = CURSOR_SUB_FREEING;
}

void
MouseUpdate(void)
{
    Mouse = (MOUSE){0};

    if (!WindowInFocus()) 
    {
        /** If the cursor is currently captured, set the sub-state to 'capturing'
            so it can be re-captured next exec. **/
        if (CursorState == CURSOR_CAPTURED)
        {
            CursorSubState = CURSOR_SUB_CAPTURING;
            OS_ShowCursor(true);
        }

        return;
    }

    /** on button **/
    {
        uint8_t button = 0;

        button |= ( KeyboardDown(MOUSE_LCLICK) ? MSEBTN_LEFT   : 0 );
        button |= ( KeyboardDown(MOUSE_RCLICK) ? MSEBTN_RIGHT  : 0 );
        button |= ( KeyboardDown(MOUSE_MCLICK) ? MSEBTN_MIDDLE : 0 );
        button |= ( KeyboardDown(MOUSE_X1)     ? MSEBTN_X1     : 0 );
        button |= ( KeyboardDown(MOUSE_X2)     ? MSEBTN_X2     : 0 );

        Mouse.down = button;
    }

    /** press button **/
    {
        uint8_t button = 0;

        button |= ( KeyboardPress(MOUSE_LCLICK) ? MSEBTN_LEFT   : 0 );
        button |= ( KeyboardPress(MOUSE_RCLICK) ? MSEBTN_RIGHT  : 0 );
        button |= ( KeyboardPress(MOUSE_MCLICK) ? MSEBTN_MIDDLE : 0 );
        button |= ( KeyboardPress(MOUSE_X1)     ? MSEBTN_X1     : 0 );
        button |= ( KeyboardPress(MOUSE_X2)     ? MSEBTN_X2     : 0 );

        Mouse.press = button;
    }

    /** release button **/
    {
        uint8_t button = 0;

        button |= ( KeyboardRelease(MOUSE_LCLICK) ? MSEBTN_LEFT   : 0 );
        button |= ( KeyboardRelease(MOUSE_RCLICK) ? MSEBTN_RIGHT  : 0 );
        button |= ( KeyboardRelease(MOUSE_MCLICK) ? MSEBTN_MIDDLE : 0 );
        button |= ( KeyboardRelease(MOUSE_X1)     ? MSEBTN_X1     : 0 );
        button |= ( KeyboardRelease(MOUSE_X2)     ? MSEBTN_X2     : 0 );

        Mouse.release = button;
    }

    INT_POINT2 cursor_pos;
    OS_GetCursorPos(&cursor_pos);

    /** Handle vector stuff early, as if the cursor is captured this
        frame it would cause non-user input spikes **/
    Mouse.vec.x = (Sint16)(cursor_pos.x - CursorPosLast.x);
    Mouse.vec.y = (Sint16)(cursor_pos.y - CursorPosLast.y);

    /** Handle the cursor sub-state **/
    switch (CursorSubState) {
    case CURSOR_SUB_FREEING:
        CursorState = CURSOR_FREE;
        OS_ShowCursor(true);
        break;

    case CURSOR_SUB_CAPTURING:
        CursorState = CURSOR_CAPTURED;
        OS_ShowCursor(false);
        break;
    }

    /** If the cursor is currently captured, force cursor to the center of the game
        window and get that new position. If the cursor moved, also re-hide it **/
    if (CursorState == CURSOR_CAPTURED)
    {
        if (OS_CenterCursorOnGameWindow(&cursor_pos))
        {
            OS_ShowCursor(false);
        }
    }

    /** Set the last global position for use next exec **/
    CursorPosLast = cursor_pos;

    {
        INT_POINT2 cursor_wndpos;
        OS_GetCursorPosOnGameWindow(&cursor_wndpos);

        Mouse.pos.x = (Sint16)cursor_wndpos.x;
        Mouse.pos.y = (Sint16)cursor_wndpos.y; 
    }

    Mouse.wheel = WheelBuffer;
    WheelBuffer = 0.0f;

    CursorSubState = CURSOR_SUB_NONE;
}

bool
MouseGetEmulatedAnalog(const eKEYBOARD_NUM nbKb, const eEMU_STICK nbAnalog, f32* const pOutX, f32* const pOutY)
{
    if (nbKb != MouseEmuKbIndex || nbAnalog != MouseEmuStickIndex)
        return false;

    NJS_POINT2I* p_vec;

    if (MouseEmuMode == EMU_MD_CLICKDRAG)
    {
        p_vec = &MouseEmuDragVector;

        if (KeyboardDown((u8)MouseEmuClickKey))
        {
            const s32 vec_x = MouseEmuDragVector.x + Mouse.vec.x;
            const s32 vec_y = MouseEmuDragVector.y + Mouse.vec.y;

            MouseEmuDragVector.x = CLAMP(vec_x, -MouseEmuDragMax, MouseEmuDragMax);
            MouseEmuDragVector.y = CLAMP(vec_y, -MouseEmuDragMax, MouseEmuDragMax);
        }
        else
            MouseEmuDragVector = (NJS_POINT2I){0};
    }
    else
        p_vec = &Mouse.vec;

    const f32 x = CLAMP((f32)p_vec->x * MouseEmuSensitivity, -1.0f, 1.0f);
    const f32 y = CLAMP((f32)p_vec->y * MouseEmuSensitivity, -1.0f, 1.0f);

    MouseSetVisualizerInfo(x, y);

    *pOutX = x;
    *pOutY = y;

    return true;
}

void
OSMSG_MouseWheel(const f32 wheel)
{
    WheelBuffer += wheel;
}

const MOUSE*
GetMouse(void)
{
    return &Mouse;
}

void
MouseInit(void)
{
    MouseEmuKbIndex     = CnfGetInt(CNF_EMUANALOG_KEYBRD);

    if (MouseEmuKbIndex != KEYBOARD_NONE)
    {
        MouseEmuStickIndex  = CnfGetInt(CNF_EMUANALOG_STICK);
        MouseEmuMode        = CnfGetInt(CNF_EMUANALOG_MODE);
        MouseEmuClickKey    = CnfGetInt(CNF_EMUANALOG_CLICK);

        MouseEmuSensitivity = (f32)(CnfGetPercent(CNF_EMUANALOG_SENSITIVITY) * 0.01);

        MouseEmuDragMax = (s32)(1.0f/MouseEmuSensitivity)+1;

        MouseVisualizerInit();

        MouseCapture();
    }
}
