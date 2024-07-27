/*
*   SA2 Input Controls - '/ic_feature.h'
*
*   Description:
*     SA2 Input Controls feature check header.
*
*   Contributors:
*     - Shaddatic
*/
#ifndef H_IC_FEATURE
#define H_IC_FEATURE

/************************/
/*  Prototypes          */
/************************/
/****** Features ********************************************************************/
/*
*   Description:
*     If Input Controls is set to use raw analog values, rather than values from the
*   emulated Dreamcast controller.
*
*   Returns:
*     If the feature is enabled
*/
bool    ICF_UseRawAnalog( void );
/*
*   Description:
*     If the camera is set to invert the X axis of the right analog stick input from
*   vanilla.
*
*   Returns:
*     If the feature is enabled
*/
bool    ICF_CamInvertX2( void );

#endif/*H_IC_FEATURE*/
