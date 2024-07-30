/************************/
/*  Includes            */
/************************/
/****** Core Toolkit ****************************************************************/
#include <sa2b/core.h>      /* core                                                 */
#include <sa2b/writeop.h>   /* WriteCall, WriteJump, WriteNOP                       */

/****** SoC *************************************************************************/
#define SAMT_INCL_FUNCPTRS
#include <sa2b/soc/input.h> /* SOCInput, SOCController                              */
#undef  SAMT_INCL_FUNCPTRS

/****** Input Controls **************************************************************/
#include <ic_core.h>        /* core                                                 */
#include <ic_input.h>       /* input module                                         */

/****** Self ************************************************************************/
#include <ic_socmagic.h>    /* self                                                 */

/************************/
/*  File Data           */
/************************/
/****** SoC Input *******************************************************************/
static SOCInput* SocInputGlobalP;   /* constant global SOCInput pointer             */

/************************/
/*  Source              */
/************************/
static SOCInput* __cdecl
NewGetSocInput(void)
{
    return SocInputGlobalP;
}

static SOCController* __stdcall
NewGetController(SOCInput* pSocInput, int nbGp)
{
    return nullptr;
}

static int __cdecl
NewSocGetActiveIndex(void)
{
    return -1;
}

static void
IC_SetVib(const int pno, const int mode, const int l, const int r)
{
    const eIC_GAMEPAD_NUM nb_gp = UserGetUser((eIC_USER_NUM)pno)->gp;

    if (mode == 1)
    {
        const f32 f_l = (f32)l / 32767.0f;
        const f32 f_r = (f32)r / 32767.0f;

        GamepadSetVibration(nb_gp, f_l, f_r);
    }
    else
        GamepadSetVibration(nb_gp, -1.f, -1.f);
}

__declspec(naked)
static void
___SetVib(void)
{
    __asm
    {
        push [esp+0Ch]
        push [esp+0Ch]
        push [esp+0Ch]
        push eax

        call IC_SetVib

        add esp, 16
        retn
    }
}

static SOCController* __stdcall
VibFixMenu1(SOCInput* pSocInput, int nbGp)
{
    GamepadSetVibration(UserGetUser((eIC_USER_NUM)nbGp)->gp, -1.f, -1.f);

    return nullptr;
}

static SOCController* __stdcall
VibFixMenu2(SOCInput* pSocInput, int nbGp)
{
    GamepadSetVibration(UserGetUser((eIC_USER_NUM)nbGp)->gp, 1.f, 1.f);

    return nullptr;
}

static uint32_t
UserToSocButton(uint32_t ubtn)
{
    uint32_t result = 0;

    result |= ( ubtn & USRBTN_DPAD_UP    ? SOCBTN_DPAD_UP    : 0 );
    result |= ( ubtn & USRBTN_DPAD_DOWN  ? SOCBTN_DPAD_DOWN  : 0 );
    result |= ( ubtn & USRBTN_DPAD_LEFT  ? SOCBTN_DPAD_LEFT  : 0 );
    result |= ( ubtn & USRBTN_DPAD_RIGHT ? SOCBTN_DPAD_RIGHT : 0 );

    result |= ( ubtn & USRBTN_A          ? SOCBTN_A : 0 );
    result |= ( ubtn & USRBTN_B          ? SOCBTN_B : 0 );
    result |= ( ubtn & USRBTN_X          ? SOCBTN_X : 0 );
    result |= ( ubtn & USRBTN_Y          ? SOCBTN_Y : 0 );

    result |= ( ubtn & USRBTN_START      ? SOCBTN_START : 0 );

    return result;
}

static int32_t
UserToSocStick(f64 mag)
{
    return !mag ? 0 : (int32_t)(mag * 127.99999999999999);
}

static int32_t
UserToSocTrigger(f64 mag)
{
    return !mag ? 0 : (int32_t)(mag * 255.9999999999999);
}

EXTERN
void
SetSocPeripheral(void)
{
    for (int i = 0; i < ARYLEN(SocPeripheral); ++i)
    {
        SocPeripheralLast[i] = SocPeripheral[i]; // copy

        SOC_PERIPHERAL* const p_socper = &SocPeripheral[i];
        const IC_USER*  const p_user   = UserGetUser((eIC_USER_NUM)i);

        p_socper->button = UserToSocButton(p_user->down);

        const f32 lr = p_user->r - p_user->l;

        if (lr)
            p_socper->button |= ( lr > 0 ? SOCBTN_R : SOCBTN_L );

        p_socper->x1 = UserToSocStick( p_user->x1);
        p_socper->y1 = UserToSocStick(-p_user->y1);

        p_socper->x2 = UserToSocStick( p_user->x2);
        p_socper->y2 = UserToSocStick(-p_user->y2);

        p_socper->r = UserToSocTrigger(p_user->r);
        p_socper->l = UserToSocTrigger(p_user->l);
    }
}

EXTERN
void
IC_SocMagicInit(void)
{
    /** Call the destructors for SOCInput **/
    {
        SOCInput* const p_socin = SOC_GetInput();

        p_socin->m_pInput->vft->Destructor(p_socin->m_pInput, 1);
        p_socin->m_pInput = nullptr;

        for (int i = 0; i < ARYLEN(p_socin->m_pController); ++i)
        {
            SOCController* const p_cont = p_socin->m_pController[i];

            if (p_cont)
                p_cont->vft->Destructor(p_cont, 1);

            p_socin->m_pController[i] = nullptr;
        }

        SocInputGlobalP = p_socin;
    }

    /** We don't want the Magic Singleton code to run when the SOCInput is fetched,
        so instead we replace the function with our own which just returns the
        pointer. By this point, SOCInput already exists so this is fine to do **/
    WriteJump(SOC_GetInput_p     , NewGetSocInput);

    /** If this function isn't killed, the game will crash whenever the controller
        index is searched for, and that wouldn't be great **/
    WriteJump(SOC_GetController_p, NewGetController);

    /** This function crashes too, although we're still not very sure what it does **/
    WriteJump(0x00425460         , NewSocGetActiveIndex);

    /** Make sure this function is still operational **/
    WriteJump(0x00425A40, ___SetVib);       // Replace SOC_SetVib

    /** Disable SoC input warning messages **/
    WriteNOP(0x0040A92D, 0x0040A943);       /* Disables SoC input change warning message   */
    WriteNOP(0x0040A9E7, 0x0040A9FD);       /* Also for the warning message, but secondary */

    /** Menu fixes **/
    WriteCall(0x0066D9CA, VibFixMenu1);     /* Fix menu directly calling SOCController->VibStop */
    WriteCall(0x0066E32F, VibFixMenu2);     /* Fix menu directly calling SOCController->VibSet  */

    /** Main Menu Exit crash **/
    WriteNOP(0x0040173A, 0x00401749);
}
