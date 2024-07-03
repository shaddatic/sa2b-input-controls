/*
*   Sonic Adventure Mod Tools (SA2B) - '/sonic/seqctrl.h'
*
*   ~~ Under Construction ~~
*
*   Description:
*     
*
*   Contributors:
*     - SEGA - Sonic Team,
*     - Shaddatic
*
*   Only for use with Sonic Adventure 2 for PC
*/
#ifndef H_SA2B_SEQCTRL
#define H_SA2B_SEQCTRL

EXTERN_START

/************************/
/*  Enums               */
/************************/
enum /* ucCurSequence */
{
    SEQ_NO_NONE,

    SEQ_NO_STORY_HERO,
    SEQ_NO_STORY_DARK,
    SEQ_NO_STORY_LAST,

    SEQ_NO_INTRO,

    SEQ_NO_DEMO_SONIC,
    SEQ_NO_DEMO_PREVIEW,
    SEQ_NO_DEMO_TAILS,
    SEQ_NO_DEMO_KNUCKLES,
    SEQ_NO_DEMO_SHADOW,
    SEQ_NO_DEMO_EGGMAN,
    SEQ_NO_DEMO_ROUGE,

    SEQ_NO_BOSS_HERO,
    SEQ_NO_BOSS_DARK,
    SEQ_NO_BOSS_ALL,

    SEQ_NO_INTRO_DC,
};

enum
{
    SEQ_TYPE_EVENT,
    SEQ_TYPE_STAGE,
    SEQ_TYPE_TITLE,
    SEQ_TYPE_ENDING,
};

/************************/
/*  Structures          */
/************************/
typedef struct
{
    uint8_t type;
    uint8_t player_num;
    uint8_t stage_num;
    int16_t event_num[4];
}
SEQ_SECTION;

/************************/
/*  Data                */
/************************/
/****** Sequence Section Lists ******************************************************/
#define seqSectionStoryHero         DataAry(SEQ_SECTION, 0x0173A148, [46])
#define seqSectionStoryDark         DataAry(SEQ_SECTION, 0x0173A370, [44])
#define seqSectionStoryLast         DataAry(SEQ_SECTION, 0x0173A580, [ 8])
#define seqSectionIntroGC           DataAry(SEQ_SECTION, 0x0173A5E0, [ 2])
#define seqSectionDemoSonic         DataAry(SEQ_SECTION, 0x01730074, [ 3])
#define seqSectionDemoPreview       DataAry(SEQ_SECTION, 0x0173089C, [ 3])
#define seqSectionDemoTails         DataAry(SEQ_SECTION, 0x0173A5F8, [ 2])
#define seqSectionDemoKnuckles      DataAry(SEQ_SECTION, 0x01730AC4, [ 3])
#define seqSectionDemoShadow        DataAry(SEQ_SECTION, 0x017312EC, [ 2])
#define seqSectionDemoEggman        DataAry(SEQ_SECTION, 0x0173A610, [ 2])
#define seqSectionDemoRouge         DataAry(SEQ_SECTION, 0x017318CC, [ 3])
#define seqSectionBossHero          DataAry(SEQ_SECTION, 0x0173A628, [ 9])
#define seqSectionBossDark          DataAry(SEQ_SECTION, 0x0173A698, [ 9])
#define seqSectionBossAll           DataAry(SEQ_SECTION, 0x0173A708, [19])
#define seqSectionIntroDC           DataAry(SEQ_SECTION, 0x0173A7EC, [ 2])

/****** Sequence State **************************************************************/
#define ucNextSectionNo             DataRef(uint8_t    , 0x01DEB31E)
#define ucCurSectionNo              DataRef(uint8_t    , 0x01DEB31F)
#define ucCurSequenceNo             DataRef(uint8_t    , 0x01DEB320)

/************************/
/*  Functions           */
/************************/
/****** Sequence Control ************************************************************/
/*
*   Description:
*     Takes control the game state 
*/
void    SeqCtrl( void );

/****** Sequence ********************************************************************/


/************************/
/*  Function Ptrs       */
/************************/
#ifdef SAMT_INCL_FUNCPTRS
/****** Function Pointers ***********************************************************/
#   define SeqCtrl_p                FuncPtr(void, __cdecl, (void), 0x004586A0)

/****** User-Function Pointers ******************************************************/


#endif/*SAMT_INCL_FUNCPTRS*/

EXTERN_END

#endif/*H_SA2B_SEQCTRL*/
