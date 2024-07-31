/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */

/****** Ninja ***********************************************************************/
#include <sa2b/ninja/njcommon.h>    /* ninja common                                 */

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
    MOUSE_SUB_NONE,        /* no sub state                                         */
    MOUSE_SUB_FREEING,     /* cursor is to be freed next input execution           */
    MOUSE_SUB_CAPTURING,   /* cursor is to be captured, if possible, next exec     */
}
eMOUSE_SUBSTATE;

typedef enum
{
    EMU_MD_RAWVEC,
    EMU_MD_CLICKDRAG,
}
eEMU_MODE;

/************************/
/*  File Variables      */
/************************/
static IC_MOUSE Mouse;                      /* mouse struct                         */

static eIC_MOUSE_MODE   MouseState;         /* cursor state                         */
static eMOUSE_SUBSTATE  MouseSubState;      /* cursor sub-state                     */

static INT_POINT2    CursorPos;
static INT_POINT2    CursorPosLast;         /* last position of the cursor          */

static f32           WheelBufferX;          /* os message wheel buffer X            */
static f32           WheelBufferY;          /* os message wheel buffer Y            */

static eIC_KEYBOARD_NUM MouseEmuKbIndex;    /* analog emulation keyboard num        */
static eEMU_STICK       MouseEmuStickIndex; /* analog emulation stick num           */
static int32_t          MouseEmuMode;       /* analog emulation input mode          */
static f32              MouseEmuSensitivity;/* analog emulation sensitivity         */

static NJS_POINT2I   MouseEmuDragVector;    /* click & drag vector                  */
static int32_t       MouseEmuDragMax;       /* click & drag max vector              */

static int8_t        MouseEmuClickKey;      /* click & drag 'click' key             */

/************************/
/*  Source              */
/************************/
void
MouseUpdate(void)
{
    Mouse = (IC_MOUSE){0};

    if (!WND_InFocus()) 
    {
        /** If the cursor is currently captured, set the sub-state to 'capturing'
            so it can be re-captured next exec. **/
        if (MouseState == IC_MOUSE_MD_CAPTURED)
        {
            MouseSubState = MOUSE_SUB_CAPTURING;
            OS_ShowCursor(true);
        }

        return;
    }

    /** on button **/
    {
        uint8_t button = 0;

        button |= ( KeyboardDown(KEY_M_LCLICK) ? MSEBTN_LEFT   : 0 );
        button |= ( KeyboardDown(KEY_M_RCLICK) ? MSEBTN_RIGHT  : 0 );
        button |= ( KeyboardDown(KEY_M_MCLICK) ? MSEBTN_MIDDLE : 0 );
        button |= ( KeyboardDown(KEY_M_X1)     ? MSEBTN_X1     : 0 );
        button |= ( KeyboardDown(KEY_M_X2)     ? MSEBTN_X2     : 0 );

        Mouse.down = button;
    }

    /** press button **/
    {
        uint8_t button = 0;

        button |= ( KeyboardPress(KEY_M_LCLICK) ? MSEBTN_LEFT   : 0 );
        button |= ( KeyboardPress(KEY_M_RCLICK) ? MSEBTN_RIGHT  : 0 );
        button |= ( KeyboardPress(KEY_M_MCLICK) ? MSEBTN_MIDDLE : 0 );
        button |= ( KeyboardPress(KEY_M_X1)     ? MSEBTN_X1     : 0 );
        button |= ( KeyboardPress(KEY_M_X2)     ? MSEBTN_X2     : 0 );

        Mouse.press = button;
    }

    /** release button **/
    {
        uint8_t button = 0;

        button |= ( KeyboardRelease(KEY_M_LCLICK) ? MSEBTN_LEFT   : 0 );
        button |= ( KeyboardRelease(KEY_M_RCLICK) ? MSEBTN_RIGHT  : 0 );
        button |= ( KeyboardRelease(KEY_M_MCLICK) ? MSEBTN_MIDDLE : 0 );
        button |= ( KeyboardRelease(KEY_M_X1)     ? MSEBTN_X1     : 0 );
        button |= ( KeyboardRelease(KEY_M_X2)     ? MSEBTN_X2     : 0 );

        Mouse.release = button;
    }

    INT_POINT2 cursor_pos;
    OS_GetCursorPos(&cursor_pos);

    /** Handle vector stuff early, as if the cursor is captured this
        frame it would cause non-user input spikes **/
    Mouse.vec.x = (Sint16)(cursor_pos.x - CursorPosLast.x);
    Mouse.vec.y = (Sint16)(cursor_pos.y - CursorPosLast.y);

    /** Handle the cursor sub-state **/
    switch (MouseSubState) {
    case MOUSE_SUB_FREEING:
        MouseState = IC_MOUSE_MD_FREE;
        OS_ShowCursor(true);
        break;

    case MOUSE_SUB_CAPTURING:
        MouseState = IC_MOUSE_MD_CAPTURED;
        OS_ShowCursor(false);
        break;
    }

    /** If the cursor is currently captured, force cursor to the center of the game
        window and get that new position. If the cursor moved, also re-hide it **/
    if (MouseState == IC_MOUSE_MD_CAPTURED)
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

    Mouse.wheelx = WheelBufferX;
    WheelBufferX = 0.0f;

    Mouse.wheely = WheelBufferY;
    WheelBufferY = 0.0f;

    MouseSubState = MOUSE_SUB_NONE;
}

bool
MouseGetEmulatedAnalog(const eIC_KEYBOARD_NUM nbKb, const eEMU_STICK nbAnalog, f32* const pOutX, f32* const pOutY)
{
    if (nbKb != MouseEmuKbIndex || nbAnalog != MouseEmuStickIndex)
        return false;

    NJS_POINT2I* p_vec;

    if (MouseEmuMode == EMU_MD_CLICKDRAG)
    {
        p_vec = &MouseEmuDragVector;

        if (KeyboardDown(MouseEmuClickKey))
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
MouseMsgWheelX(const f32 wheel)
{
    WheelBufferX += wheel;
}

void
MouseMsgWheelY(const f32 wheel)
{
    WheelBufferY += wheel;
}

const IC_MOUSE*
MouseGetMouse(void)
{
    return &Mouse;
}

eIC_MOUSE_MODE
MouseGetMode(void)
{
    return MouseState;
}

void
MouseCapture(void)
{
    MouseSubState = MOUSE_SUB_CAPTURING;
}

void
MouseFree(void)
{
    MouseSubState = MOUSE_SUB_FREEING;
}

void
MouseHide(void)
{
    OS_ShowCursor(true);
}

void
MouseShow(void)
{
    OS_ShowCursor(false);
}

void
MouseInit(void)
{
    MouseEmuKbIndex = CnfGetInt(CNF_EMUANALOG_KEYBRD);

    if (MouseEmuKbIndex != IC_KEYBOARD_NONE)
    {
        MouseEmuStickIndex  =     CnfGetInt(CNF_EMUANALOG_STICK);
        MouseEmuMode        =     CnfGetInt(CNF_EMUANALOG_MODE);
        MouseEmuClickKey    = (u8)CnfGetInt(CNF_EMUANALOG_CLICK);

        MouseEmuSensitivity = (f32)(CnfGetPercent(CNF_EMUANALOG_SENSITIVITY) * 0.01);

        MouseEmuDragMax = (s32)(1.0f/MouseEmuSensitivity)+1;

        MouseVisualizerInit();

        MouseCapture();
    }

    MouseWindowMsgInit();
}
