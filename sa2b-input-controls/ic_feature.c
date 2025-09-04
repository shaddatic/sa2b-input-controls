/********************************/
/*  Includes                    */
/********************************/
/****** Core Toolkit ****************************************************************************/
#include <samt/core.h>              /* core                                                     */

/****** Game ************************************************************************************/
#include <samt/sonic/input.h>       /* per, perG                                                */
#include <samt/sonic/camera.h>      /* sa2 camera                                               */

/****** Input Controls **************************************************************************/
#include <ic_core.h>                /* core                                                     */
#include <ic_input.h>               /* input                                                    */

/****** Config **********************************************************************************/
#include <cnf.h>                    /* CnfGet##                                                 */

/****** Self ************************************************************************************/
#include <ic_feature.h>             /* self                                                     */

/********************************/
/*  Variables                   */
/********************************/
/****** Features ********************************************************************************/
static bool UseRawAnalog;
static bool CamInvertX2;
static bool MouseAnalogEmu;
static bool CamInvertLR;

/********************************/
/*  Source                      */
/********************************/
/****** Features ********************************************************************************/
bool
ICF_UseRawAnalog(void)
{
    return UseRawAnalog;
}

bool
ICF_CamInvertX2(void)
{
    return CamInvertX2;
}

bool
ICF_MouseAnalogEmulation(void)
{
    return MouseAnalogEmu != IC_KEYBOARD_NONE;
}

bool
ICF_CamInvertLR(void)
{
    return CamInvertLR;
}

/****** Init ************************************************************************************/
void
ICF_Init(void)
{
    UseRawAnalog   = CNF_GetInt( CNF_MAIN_RAWANALOG );
    CamInvertX2    = CNF_GetInt( CNF_CAMERA_INV_X2 );
    MouseAnalogEmu = CNF_GetInt( CNF_EMUANALOG_KEYBRD );
    CamInvertLR    = CNF_GetInt( CNF_CAMERA_INV_LR );
}
