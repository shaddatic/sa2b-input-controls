/*
*   SA2 Input Controls - '/ic_api.h'
*
*   Description:
*     API module header
*
*   Contributors:
*     - Shaddatic
*/
#ifndef H_IC_API
#define H_IC_API

EXTERN_START

/********************************/
/*  Enums                       */
/********************************/
/****** User Functions **************************************************************************/
typedef enum
{
    IC_UF_INIT,             /* main func                                                        */
    IC_UF_EARLY,            /* early func                                                       */
    IC_UF_LATE,             /* late func                                                        */

    IC_UF_OLD_INIT,         /* old main func                                                    */
    IC_UF_OLD_EARLY,        /* old early func                                                   */

    IC_NB_UF,               /* enum count                                                       */
}
IC_USERFUNC;

/********************************/
/*  Prototypes                  */
/********************************/
/****** Init ************************************************************************************/
/*
*   Description:
*     Call user exported functions.
*
*   Parameters:
*     uf        : user function type
*/
void    ICAPI_CallUserFuncs( IC_USERFUNC uf );

EXTERN_END

#endif/*H_IC_API*/
