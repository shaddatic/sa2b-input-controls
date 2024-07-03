/*
*   SA2 Input Controls - '/ic_socmagic.h'
*
*   Description:
*     SoC/Magic input replacement.
*
*   Contributors:
*     - Shaddatic
*/
#ifndef H_IC_SOCMAGIC
#define H_IC_SOCMAGIC

EXTERN_START

/************************/
/*  Prototypes          */
/************************/
/****** Init ************************************************************************/
/*
*   Description:
*     Init module
*/
void    IC_SocMagicInit( void );
/*
*   Description:
*     Set left over SoC peripheral data using User Input data, as some things use it
*   directly
*/
void    SetSocPeripheral( void );

EXTERN_END

#endif/*H_IC_SOCMAGIC*/
