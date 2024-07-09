/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */
#include <sa2b/memory.h>    /* MemCopy, MemSet                                      */

/****** Mod Loader ******************************************************************/
#include <sa2b/modloader.h> /* ML_DisplayDebugStringF                               */

/****** Ninja ***********************************************************************/
#include <sa2b/ninja/njdebug.h> /* NJM_LOCATION                                     */

/****** Std *************************************************************************/
#include <stdio.h>  /* snprintf                                                     */

/****** Game ************************************************************************/
#include <sa2b/sonic/debug.h>   /* OutputInt                                        */

/****** Input Controls **************************************************************/
#include <ic_core.h>    /* core                                                     */
#include <ic_config.h>  /* CnfGet___                                                */
#include <ic_window.h>
#include <ic_os.h>

/****** Self ************************************************************************/
#include <ic_input/inpt_internal.h>

/************************/
/*  Constants           */
/************************/
#define LEN_KEY_BITMAP      (256/8)

/************************/
/*  Typedefs            */
/************************/
typedef uint8_t     KEY_MAP[256];
typedef uint8_t     KEY_BITMAP[LEN_KEY_BITMAP];

/************************/
/*  Structures          */
/************************/
typedef struct
{
    KEY_BITMAP down;
    KEY_BITMAP press;
    KEY_BITMAP release;

    bool capslock : 1;
    bool numblock : 1;
    bool scrllock : 1;
}
KEYBOARD;

typedef struct
{
    uint8_t up;
    uint8_t down;
    uint8_t left;
    uint8_t right;
}
DIR_KEYS;

typedef struct
{
    DIR_KEYS StickL;
    DIR_KEYS StickR;

    uint8_t btn_a;
    uint8_t btn_b;
    uint8_t btn_x;
    uint8_t btn_y;

    uint8_t btn_l;
    uint8_t btn_r;

    uint8_t btn_start;
    uint8_t btn_back;

    uint8_t btn_zl;
    uint8_t btn_zr;

    uint8_t btn_ls;
    uint8_t btn_rs;

    DIR_KEYS DPad;
}
USER_KEYS;

/************************/
/*  File Variables      */
/************************/
static bool     KeyboardDebugPoll;

static KEYBOARD Keyboard;

static USER_KEYS KeyboardLayout[4] = /* default options */
{
    /** SA2 P1 default **/
    {
        .StickL    = { .up = 'W', .down = 'S', .left = 'A', .right = 'D' },
        .StickR    = { .up = KEY_NUM_8, .down = KEY_NUM_2, .left = KEY_NUM_4, .right = KEY_NUM_6  },
        .btn_a     = 'J',
        .btn_b     = 'K',
        .btn_x     = 'U',
        .btn_y     = 'I',
        .btn_l     = 'Q',
        .btn_r     = 'E',
        .btn_start = KEY_RETURN,
        .btn_back  = KEY_BACKSPACE,
        .btn_zl    = 'U',
        .btn_zr    = 'O',
        .btn_ls    = KEY_NONE,
        .btn_rs    = KEY_NONE,
        .DPad      = { .up = KEY_ARROW_UP, .down = KEY_ARROW_DOWN, .left = KEY_ARROW_LEFT, .right = KEY_ARROW_RIGHT },
    },

    /** SADX default **/
    {
        .StickL    = { .up = KEY_ARROW_UP, .down = KEY_ARROW_DOWN, .left = KEY_ARROW_LEFT, .right = KEY_ARROW_RIGHT },
        .StickR    = { .up = 'I'         , .down = 'M'           , .left = 'J'           , .right = 'L'             },
        .btn_a     = 'X',
        .btn_b     = 'Z',
        .btn_x     = 'A',
        .btn_y     = 'S',
        .btn_l     = 'Q',
        .btn_r     = 'W',
        .btn_start = KEY_RETURN,
        .btn_back  = 'V',
        .btn_zl    = 'C',
        .btn_zr    = 'B',
        .btn_ls    = KEY_NONE,
        .btn_rs    = KEY_NONE,
        .DPad      = { .up = KEY_NUM_8, .down = KEY_NUM_2, .left = KEY_NUM_4, .right = KEY_NUM_6 },
    },

    /** Shaddatic Default **/
    {
        .StickL    = { .up = 'W', .down = 'S', .left = 'A', .right = 'D' },
        .StickR    = { .up = KEY_NUM_8, .down = KEY_NUM_2, .left = KEY_NUM_4, .right = KEY_NUM_6  },
        .btn_a     = KEY_SPACE,
        .btn_b     = 'C',
        .btn_x     = 'X',
        .btn_y     = 'Z',
        .btn_l     = 'Q',
        .btn_r     = 'E',
        .btn_start = KEY_ESCAPE,
        .btn_back  = KEY_TILDE,
        .btn_zl    = '2',
        .btn_zr    = '3',
        .btn_ls    = KEY_NONE,
        .btn_rs    = KEY_NONE,
        .DPad      = { .up = KEY_ARROW_UP, .down = KEY_ARROW_DOWN, .left = KEY_ARROW_LEFT, .right = KEY_ARROW_RIGHT },
    },

    /** None Default **/
    {
        .StickL    = { .up = KEY_NONE, .down = KEY_NONE, .left = KEY_NONE, .right = KEY_NONE },
        .StickR    = { .up = KEY_NONE, .down = KEY_NONE, .left = KEY_NONE, .right = KEY_NONE },
        .btn_a     = KEY_NONE,
        .btn_b     = KEY_NONE,
        .btn_x     = KEY_NONE,
        .btn_y     = KEY_NONE,
        .btn_l     = KEY_NONE,
        .btn_r     = KEY_NONE,
        .btn_start = KEY_NONE,
        .btn_back  = KEY_NONE,
        .btn_zl    = KEY_NONE,
        .btn_zr    = KEY_NONE,
        .btn_ls    = KEY_NONE,
        .btn_rs    = KEY_NONE,
        .DPad      = { .up = KEY_NONE, .down = KEY_NONE, .left = KEY_NONE, .right = KEY_NONE },
    },


};

/************************/
/*  Source              */
/************************/
u8
KeyboardPoll(void)
{
    /** Loops in reverse to ensure KEY_SHIFT and KEY_CONTROL 
        are never returned, as Poll should return specific key **/
    for (int i = LEN_KEY_BITMAP-1; i >= 0; --i)
    {
        uint8_t currbit = 0x80;

        for (int j = 7; j >= 0; --j, currbit >>= 1)
        {
            if (Keyboard.press[i] & currbit)
                return (uint8_t)((i * 8) + j);
        }
    }

    return KEY_NONE;
}

bool
KeyboardDown(const u8 key)
{
    const int idx = (key >> 3); // div by 8
    const int bit = (key &  7); // mod by 8

    return (Keyboard.down[idx] >> bit) & 0x1;
}

bool
KeyboardPress(const u8 key)
{
    const int idx = (key >> 3); // div by 8
    const int bit = (key &  7); // mod by 8

    return (Keyboard.press[idx] >> bit) & 0x1;
}

bool
KeyboardRelease(const u8 key)
{
    const int idx = (key >> 3); // div by 8
    const int bit = (key &  7); // mod by 8

    return (Keyboard.release[idx] >> bit) & 0x1;
}

bool
KeyboardCapsLocked(void)
{
    return Keyboard.capslock;
}

bool
KeyboardScrollLocked(void)
{
    return Keyboard.scrllock;
}

bool
KeyboardNumLocked(void)
{
    return Keyboard.numblock;
}

static void
DebugPoll(void)
{
    static int s_LastDbgPoll;

    uint8_t poll = KeyboardPoll();

    if (poll == KEY_NONE)
        poll = s_LastDbgPoll;
    else
        s_LastDbgPoll = poll;

    ML_SetDebugFontColor(0xFFFFFFFF);
    ML_SetDebugFontScale(12.f);

    char buf[64];

    ML_DisplayDebugStringF(NJM_LOCATION(1, 1), buf, 64, "LAST KEY: %i", poll);
}

void
KeyboardUpdate(void)
{
    KEY_MAP key_states;

    KEY_BITMAP old_key;

    MemCopy(old_key, Keyboard.down, sizeof(KEY_BITMAP));

    MemSet(Keyboard.down   , 0x0, sizeof(KEY_BITMAP));
    MemSet(Keyboard.press  , 0x0, sizeof(KEY_BITMAP));
    MemSet(Keyboard.release, 0x0, sizeof(KEY_BITMAP));

    if (!WindowInFocus() || !OS_GetKeyboardState(key_states))
        return;

    Keyboard.capslock = key_states[KEY_CAPSLOCK]    & KEY_TOGGLED;
    Keyboard.numblock = key_states[KEY_NUMLOCK]     & KEY_TOGGLED;
    Keyboard.scrllock = key_states[KEY_SCROLLLOCK]  & KEY_TOGGLED;

    /** Translate KEY_MAP to a KEY_BITMAP for storage **/
    for (int nb_bitmap = 0, nb_state = 0; nb_bitmap < LEN_KEY_BITMAP; ++nb_bitmap)
    {
        uint8_t* const p_on      = &Keyboard.down[nb_bitmap];
        uint8_t* const p_press   = &Keyboard.press[nb_bitmap];
        uint8_t* const p_release = &Keyboard.release[nb_bitmap];

        uint8_t cur_bit = 0b0000'0001;

        for (int j = 0; j < BITSIN(u8); ++j, ++nb_state, cur_bit <<= 1)
        {
            if (key_states[nb_state] & KEY_DOWN)
                *p_on |= cur_bit;
        }

        *p_press   = (*p_on) & ~old_key[nb_bitmap];
        *p_release = old_key[nb_bitmap] & ~(*p_on);
    }

    if (KeyboardDebugPoll)
        DebugPoll();
}

static f32
GetStickAxis(const uint8_t keyPos, const uint8_t keyNeg)
{
    return (KeyboardDown(keyPos) ? 1.0f : 0.0f) - (KeyboardDown(keyNeg) ? 1.0f : 0.0f);
}

void
KeyboardSetUserInput(const int nbKb, USER_INPUT* const pUserInput)
{
    const USER_KEYS* const p_kbl = &KeyboardLayout[nbKb];

    /** Left analog stick **/
    {
        const f32 in_x = pUserInput->x1;
        const f32 in_y = pUserInput->y1;

        f32 x = 0, y = 0;

        MouseGetEmulatedAnalog(nbKb, EMU_STICK_LEFT, &x, &y);

        const f32 dgt_x = GetStickAxis(p_kbl->StickL.right, p_kbl->StickL.left);
        const f32 dgt_y = GetStickAxis(p_kbl->StickL.down , p_kbl->StickL.up  );

        if (dgt_x) x = dgt_x;
        if (dgt_y) y = dgt_y;

        /** Ensure the largest player input is the only one registered **/
        if (ABS(x) > ABS(in_x)) pUserInput->x1 = x;
        if (ABS(y) > ABS(in_y)) pUserInput->y1 = y;
    }
    /** Right analog stick **/
    {
        const f32 in_x = pUserInput->x2;
        const f32 in_y = pUserInput->y2;

        f32 x = 0, y = 0;

        MouseGetEmulatedAnalog(nbKb, EMU_STICK_RIGHT, &x, &y);

        const f32 dgt_x = GetStickAxis(p_kbl->StickR.right, p_kbl->StickR.left);
        const f32 dgt_y = GetStickAxis(p_kbl->StickR.down , p_kbl->StickR.up  );

        if (dgt_x) x = dgt_x;
        if (dgt_y) y = dgt_y;

        /** Ensure the largest player input is the only one registered **/
        if (ABS(x) > ABS(in_x)) pUserInput->x2 = x;
        if (ABS(y) > ABS(in_y)) pUserInput->y2 = y;
    }

    uint32_t btn = 0;

    btn |= ( KeyboardDown(p_kbl->btn_a) ? USRBTN_A : 0 );  /* Button A */
    btn |= ( KeyboardDown(p_kbl->btn_b) ? USRBTN_B : 0 );  /* Button B */
    btn |= ( KeyboardDown(p_kbl->btn_x) ? USRBTN_X : 0 );  /* Button X */
    btn |= ( KeyboardDown(p_kbl->btn_y) ? USRBTN_Y : 0 );  /* Button Y */

    btn |= ( KeyboardDown(p_kbl->btn_start) ? USRBTN_START : 0 );  /* Button Start/Pause */
    btn |= ( KeyboardDown(p_kbl->btn_back ) ? USRBTN_BACK  : 0 );  /* Button Back */

    btn |= ( KeyboardDown(p_kbl->btn_zl) ? USRBTN_ZL : 0 );  /* Button ZL */
    btn |= ( KeyboardDown(p_kbl->btn_zr) ? USRBTN_ZR : 0 );  /* Button ZR */

    btn |= ( KeyboardDown(p_kbl->btn_l) ? USRBTN_L : 0 );  /* Button L */
    btn |= ( KeyboardDown(p_kbl->btn_r) ? USRBTN_R : 0 );  /* Button R */

    btn |= ( KeyboardDown(p_kbl->DPad.up   ) ? USRBTN_DPAD_UP    : 0 );  /* D-Pad Up */
    btn |= ( KeyboardDown(p_kbl->DPad.down ) ? USRBTN_DPAD_DOWN  : 0 );  /* D-Pad Down */
    btn |= ( KeyboardDown(p_kbl->DPad.left ) ? USRBTN_DPAD_LEFT  : 0 );  /* D-Pad Left */
    btn |= ( KeyboardDown(p_kbl->DPad.right) ? USRBTN_DPAD_RIGHT : 0 );  /* D-Pad Right */

    /** Ensure this doesn't unpress buttons held by the gamepad **/
    pUserInput->down |= btn;

    if (btn & USRBTN_L)
        pUserInput->l = 1.0f;  /* Analog L */

    if (btn & USRBTN_R)
        pUserInput->r = 1.0f;  /* Analog R */
}

void
KeyboardInit(void)
{
    /** Get Gamepad sections info **/
    for (int i = 0; i < ARYLEN(KeyboardLayout); ++i)
    {
        USER_KEYS* const p_kbl = &KeyboardLayout[i];

        char buf[8];

        snprintf(buf, sizeof(buf), "kb%i", i);

        p_kbl->StickL.up    = CnfGetInt( CNFV_KEYBRD_LS_UP(    buf, p_kbl->StickL.up    ) );
        p_kbl->StickL.down  = CnfGetInt( CNFV_KEYBRD_LS_DOWN(  buf, p_kbl->StickL.down  ) );
        p_kbl->StickL.left  = CnfGetInt( CNFV_KEYBRD_LS_LEFT(  buf, p_kbl->StickL.left  ) );
        p_kbl->StickL.right = CnfGetInt( CNFV_KEYBRD_LS_RIGHT( buf, p_kbl->StickL.right ) );

        p_kbl->StickR.up    = CnfGetInt( CNFV_KEYBRD_RS_UP(    buf, p_kbl->StickR.up    ) );
        p_kbl->StickR.down  = CnfGetInt( CNFV_KEYBRD_RS_DOWN(  buf, p_kbl->StickR.down  ) );
        p_kbl->StickR.left  = CnfGetInt( CNFV_KEYBRD_RS_LEFT(  buf, p_kbl->StickR.left  ) );
        p_kbl->StickR.right = CnfGetInt( CNFV_KEYBRD_RS_RIGHT( buf, p_kbl->StickR.right ) );

        p_kbl->btn_a = CnfGetInt( CNFV_KEYBRD_BTN_A( buf, p_kbl->btn_a ) );
        p_kbl->btn_b = CnfGetInt( CNFV_KEYBRD_BTN_B( buf, p_kbl->btn_b ) );
        p_kbl->btn_x = CnfGetInt( CNFV_KEYBRD_BTN_X( buf, p_kbl->btn_x ) );
        p_kbl->btn_y = CnfGetInt( CNFV_KEYBRD_BTN_Y( buf, p_kbl->btn_y ) );

        p_kbl->btn_l = CnfGetInt( CNFV_KEYBRD_BTN_L( buf, p_kbl->btn_l) );
        p_kbl->btn_r = CnfGetInt( CNFV_KEYBRD_BTN_R( buf, p_kbl->btn_r) );

        p_kbl->btn_start = CnfGetInt( CNFV_KEYBRD_BTN_START( buf, p_kbl->btn_start) );
        p_kbl->btn_back  = CnfGetInt( CNFV_KEYBRD_BTN_BACK(  buf, p_kbl->btn_back ) );

        p_kbl->btn_zl = CnfGetInt( CNFV_KEYBRD_BTN_ZL( buf, p_kbl->btn_zl ) );
        p_kbl->btn_zr = CnfGetInt( CNFV_KEYBRD_BTN_ZR( buf, p_kbl->btn_zr ) );

        p_kbl->DPad.up    = CnfGetInt( CNFV_KEYBRD_DIR_UP(    buf, p_kbl->DPad.up    ) );
        p_kbl->DPad.down  = CnfGetInt( CNFV_KEYBRD_DIR_DOWN(  buf, p_kbl->DPad.down  ) );
        p_kbl->DPad.left  = CnfGetInt( CNFV_KEYBRD_DIR_LEFT(  buf, p_kbl->DPad.left  ) );
        p_kbl->DPad.right = CnfGetInt( CNFV_KEYBRD_DIR_RIGHT( buf, p_kbl->DPad.right ) );
    }

    KeyboardDebugPoll = CnfGetInt(CNF_DEBUG_KEYPOLL);
}
