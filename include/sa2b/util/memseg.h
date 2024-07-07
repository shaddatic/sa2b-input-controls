/*
*   Sonic Adventure Mod Tools (SA2B) - '/util/memseg.h'
*
*   Description:
*     Utility header defining memory segments and their addresses
*
*   Contributors:
*     - Shaddatic
*
*   Only for use with Sonic Adventure 2 for PC
*/
#ifndef H_UTIL_MEMSEG
#define H_UTIL_MEMSEG

/************************/
/*  Constants           */
/************************/
/****** Address Segments ************************************************************/
#define MEMSEG_STACK        ((void*)0x00400000) /* end of the stack segment         */
#define MEMSEG_TEXT         ((void*)0x00873000) /* end of the text/code segment     */
#define MEMSEG_IMPORT       ((void*)0x0087342C) /* end of the import segment        */
#define MEMSEG_RDATA        ((void*)0x00917000) /* end of the read-only segment     */
#define MEMSEG_DATA         ((void*)0x02775000) /* end of the data segment          */
#define MEMSEG_BIND         ((void*)0x027FB600) /* end of the bind segment          */

#endif/*H_SAMT_MEMSEG*/
