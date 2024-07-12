/*
*   SA2 Input Controls - '/ic_config/cnf_def.h'
*
*   Description:
*     Defines config options for use with config module.
*
*   Contributors:
*     - Shaddatic
*/
#ifndef H_IC_CONFIG_DEF
#define H_IC_CONFIG_DEF

/************************/
/*  Macro               */
/************************/
/****** Config Option ***************************************************************/
#define CNFO_INT(     sect_, key_, def_ )       &(const CONFIG_INT)     { .sect = sect_, .key = key_, .def = def_ }
#define CNFO_BOOL(    sect_, key_, def_ )       &(const CONFIG_BOOL)    { .sect = sect_, .key = key_, .def = def_ }
#define CNFO_FLOAT(   sect_, key_, def_ )       &(const CONFIG_FLOAT)   { .sect = sect_, .key = key_, .def = def_ }
#define CNFO_STRING(  sect_, key_, def_ )       &(const CONFIG_STRING)  { .sect = sect_, .key = key_, .def = def_ }

/************************/
/*  External Headers    */
/************************/
/****** Input Module ****************************************************************/
#include <ic_input.h>

/************************/
/*  Sections            */
/************************/
/****** Main ************************************************************************/
#define CNFS_MAIN           "main"

/****** Camera **********************************************************************/
#define CNFS_CAMERA         "camera"

/****** User/Player *****************************************************************/
#define CNFS_USER1          "user0"
#define CNFS_USER2          "user1"
#define CNFS_USER3          "user2"
#define CNFS_USER4          "user3"

/****** Gamepad *********************************************************************/
#define CNFS_GAMEPD_1       "gp0"
#define CNFS_GAMEPD_2       "gp1"
#define CNFS_GAMEPD_3       "gp2"
#define CNFS_GAMEPD_4       "gp3"

/****** Keyboard ********************************************************************/
#define CNFS_KEYBRD_1       "kb0"
#define CNFS_KEYBRD_2       "kb1"
#define CNFS_KEYBRD_3       "kb2"
#define CNFS_KEYBRD_4       "kb3"

/****** Analog Emulation ************************************************************/
#define CNFS_EMUANALOG      "emu_analog"

/****** Compatibility ***************************************************************/
#define CNFS_COMPAT         "compatibility"

/****** Misc ***********************************************************************/
#define CNFS_MISC           "misc"

/****** Debug ***********************************************************************/
#define CNFS_DEBUG          "debug"

/************************/
/*  Enums               */
/************************/
/****** Basic Bool ******************************************************************/
typedef enum
{
    CNFE_BOOL_DISABLED,
    CNFE_BOOL_ENABLED
}
CNFE_BOOL;
/************************/
/*  Options             */
/************************/
/**                                   | Type      | Section             | Key                   | Default                              **/
/****** Main ****************************************************************************************************************************/
#define CNF_MAIN_RAWANALOG              CNFO_INT(   CNFS_MAIN           , "raw_analog"          , CNFE_BOOL_ENABLED                     )

/****** Camera **************************************************************************************************************************/
#define CNF_CAMERA_ANALOG               CNFO_INT(   CNFS_CAMERA         , "analog"              , CNFE_BOOL_ENABLED                     )
#define CNF_CAMERA_LRINV                CNFO_INT(   CNFS_CAMERA         , "lr_inv"              , CNFE_BOOL_DISABLED                    )

/****** User ****************************************************************************************************************************/
#define CNF_USER1_GAMEPD_NB             CNFO_INT(   CNFS_USER1          , "gp_nb"               , GAMEPAD_1                             )
#define CNF_USER1_KEYBRD_NB             CNFO_INT(   CNFS_USER1          , "kb_nb"               , KEYBOARD_1                            )
#define CNF_USER1_INPUT_MODE            CNFO_INT(   CNFS_USER1          , "input_md"            , INPUT_SWITCH                          )
#define CNF_USER2_GAMEPD_NB             CNFO_INT(   CNFS_USER2          , "gp_nb"               , GAMEPAD_2                             )
#define CNF_USER2_KEYBRD_NB             CNFO_INT(   CNFS_USER2          , "kb_nb"               , KEYBOARD_NONE                         )
#define CNF_USER2_INPUT_MODE            CNFO_INT(   CNFS_USER2          , "input_md"            , INPUT_SWITCH                          )
#define CNF_USER3_GAMEPD_NB             CNFO_INT(   CNFS_USER3          , "gp_nb"               , GAMEPAD_NONE                          )
#define CNF_USER3_KEYBRD_NB             CNFO_INT(   CNFS_USER3          , "kb_nb"               , KEYBOARD_NONE                         )
#define CNF_USER3_INPUT_MODE            CNFO_INT(   CNFS_USER3          , "input_md"            , INPUT_SWITCH                          )
#define CNF_USER4_GAMEPD_NB             CNFO_INT(   CNFS_USER4          , "gp_nb"               , GAMEPAD_NONE                          )
#define CNF_USER4_KEYBRD_NB             CNFO_INT(   CNFS_USER4          , "kb_nb"               , KEYBOARD_NONE                         )
#define CNF_USER4_INPUT_MODE            CNFO_INT(   CNFS_USER4          , "input_md"            , INPUT_SWITCH                          )

/****** Gamepad *************************************************************************************************************************/
#define CNFV_GAMEPD_DZ_MODE(s)          CNFO_INT(   s                   , "dz_md"               , DZ_MD_CIRCULAR                        )
#define CNFV_GAMEPD_LS_IDZ(s)           CNFO_INT(   s                   , "ls_idz"              , 10                                    )
#define CNFV_GAMEPD_LS_ODZ(s)           CNFO_INT(   s                   , "ls_odz"              , 100                                   )
#define CNFV_GAMEPD_RS_IDZ(s)           CNFO_INT(   s                   , "rs_idz"              , 10                                    )
#define CNFV_GAMEPD_RS_ODZ(s)           CNFO_INT(   s                   , "rs_odz"              , 100                                   )
#define CNFV_GAMEPD_TR_LIM(s)           CNFO_INT(   s                   , "tr_lim"              , 55                                    )
#define CNFV_GAMEPD_VIB(s)              CNFO_INT(   s                   , "vib"                 , CNFE_BOOL_ENABLED                     )
#define CNFV_GAMEPD_VIB_L(s)            CNFO_INT(   s                   , "vib_l"               , 100                                   )
#define CNFV_GAMEPD_VIB_R(s)            CNFO_INT(   s                   , "vib_r"               , 100                                   )

/****** Keyboard Layout *****************************************************************************************************************/
#define CNFV_KEYBRD_LS_UP(s, d)         CNFO_INT(   s                   , "ls_up"               , d                                     )
#define CNFV_KEYBRD_LS_DOWN(s, d)       CNFO_INT(   s                   , "ls_down"             , d                                     )
#define CNFV_KEYBRD_LS_LEFT(s, d)       CNFO_INT(   s                   , "ls_left"             , d                                     )
#define CNFV_KEYBRD_LS_RIGHT(s, d)      CNFO_INT(   s                   , "ls_right"            , d                                     )
#define CNFV_KEYBRD_RS_UP(s, d)         CNFO_INT(   s                   , "rs_up"               , d                                     )
#define CNFV_KEYBRD_RS_DOWN(s, d)       CNFO_INT(   s                   , "rs_down"             , d                                     )
#define CNFV_KEYBRD_RS_LEFT(s, d)       CNFO_INT(   s                   , "rs_left"             , d                                     )
#define CNFV_KEYBRD_RS_RIGHT(s, d)      CNFO_INT(   s                   , "rs_right"            , d                                     )
#define CNFV_KEYBRD_BTN_A(s, d)         CNFO_INT(   s                   , "btn_a"               , d                                     )
#define CNFV_KEYBRD_BTN_B(s, d)         CNFO_INT(   s                   , "btn_b"               , d                                     )
#define CNFV_KEYBRD_BTN_X(s, d)         CNFO_INT(   s                   , "btn_x"               , d                                     )
#define CNFV_KEYBRD_BTN_Y(s, d)         CNFO_INT(   s                   , "btn_y"               , d                                     )
#define CNFV_KEYBRD_BTN_L(s, d)         CNFO_INT(   s                   , "btn_l"               , d                                     )
#define CNFV_KEYBRD_BTN_R(s, d)         CNFO_INT(   s                   , "btn_r"               , d                                     )
#define CNFV_KEYBRD_BTN_START(s, d)     CNFO_INT(   s                   , "btn_start"           , d                                     )
#define CNFV_KEYBRD_BTN_BACK(s, d)      CNFO_INT(   s                   , "btn_back"            , d                                     )
#define CNFV_KEYBRD_BTN_ZL(s, d)        CNFO_INT(   s                   , "btn_zl"              , d                                     )
#define CNFV_KEYBRD_BTN_ZR(s, d)        CNFO_INT(   s                   , "btn_zr"              , d                                     )
#define CNFV_KEYBRD_DIR_UP(s, d)        CNFO_INT(   s                   , "dp_up"               , d                                     )
#define CNFV_KEYBRD_DIR_DOWN(s, d)      CNFO_INT(   s                   , "dp_down"             , d                                     )
#define CNFV_KEYBRD_DIR_LEFT(s, d)      CNFO_INT(   s                   , "dp_left"             , d                                     )
#define CNFV_KEYBRD_DIR_RIGHT(s, d)     CNFO_INT(   s                   , "dp_right"            , d                                     )

/****** Emulated Analog *****************************************************************************************************************/
#define CNF_EMUANALOG_KEYBRD            CNFO_INT(   CNFS_EMUANALOG      , "kb_nb"               , KEYBOARD_NONE                         )
#define CNF_EMUANALOG_STICK             CNFO_INT(   CNFS_EMUANALOG      , "stick"               , 0                                     )
#define CNF_EMUANALOG_MODE              CNFO_INT(   CNFS_EMUANALOG      , "mode"                , 1                                     )
#define CNF_EMUANALOG_CLICK             CNFO_INT(   CNFS_EMUANALOG      , "click"               , KEY_M_LCLICK                          )
#define CNF_EMUANALOG_SENSITIVITY       CNFO_INT(   CNFS_EMUANALOG      , "sensitivity"         , 50                                    )
#define CNF_EMUANALOG_VISU_MODE         CNFO_INT(   CNFS_EMUANALOG      , "visu_mode"           , 1                                     )
#define CNF_EMUANALOG_VISU_ALPHA        CNFO_INT(   CNFS_EMUANALOG      , "visu_alpha"          , 75                                    )
#define CNF_EMUANALOG_VISU_SCALE        CNFO_FLOAT( CNFS_EMUANALOG      , "visu_scale"          , 1                                     )

/****** Misc Section ********************************************************************************************************************/
#define CNF_MISC_DCDRIFT                CNFO_INT(   CNFS_MISC           , "dc_drift"            , CNFE_BOOL_DISABLED                    )

/****** Debug Section *******************************************************************************************************************/
#define CNF_DEBUG_AXIS                  CNFO_INT(   CNFS_DEBUG          , "gp_axis"             , CNFE_BOOL_DISABLED                    )
#define CNF_DEBUG_KEYPOLL               CNFO_INT(   CNFS_DEBUG          , "kb_poll"             , CNFE_BOOL_DISABLED                    )

#endif/*H_IC_CONFIG_DEF*/
