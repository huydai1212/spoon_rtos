
/*******************************************************************************
** @file:      	version_proc.c
** @author:    	HUY DAI
** @version:   	V1.0.0
** @time:      	8:59 AM Tuesday, June 4, 2019

** @brief:		 
** @revision:
**             	- version 1.0.1: <date> <name>
**             	<discribe the change>
*******************************************************************************/

/*******************************************************************************
**                               INCLUDES
*******************************************************************************/
#include "version.h"
/*******************************************************************************
**                       INTERNAL MACRO DEFINITIONS
*******************************************************************************/

/*******************************************************************************
**                      COMMON VARIABLE DEFINITIONS
*******************************************************************************/



/*******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/

/*******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/

/*******************************************************************************
**                           FUNCTION DEFINITIONS
*******************************************************************************/
/*******************************************************************************
** @brief		compare 2 versions
** @param		v1, v2
** @return		if Return value < 0 then it indicates v1 is less than v2.
				if Return value > 0 then it indicates v2 is less than v1.
				if Return value = 0 then it indicates v1 is equal to v2.
** @time		
** @revision	
**             
********************************************************************************/
int8_t version_cmp(sVersion_t v1, sVersion_t v2)
{
    /* compare major part*/
	if(v1.major > v2.major)
    {
        return 1;
    }
    else if(v1.major < v2.major)
    {
        return -1;
    }
    else
    {
    }
    
    /* compare minor part */
    if(v1.minor > v2.minor)
    {
        return 1;
    }
    else if(v1.minor < v2.minor)
    {
        return -1;
    }
    else
    {
    }
    
    /* compare build_indicator part*/
    if(v1.build_indicator > v2.build_indicator)
    {
        return 1;
    }
    else if(v1.build_indicator < v2.build_indicator)
    {
        return -1;
    }
    else
    {
    }
    return 0;
}



/******************************** End of file **********************************/
