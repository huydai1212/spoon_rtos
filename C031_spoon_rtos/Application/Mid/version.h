/*******************************************************************************
** @file:      	version.h
** @author:    	HUY DAI
** @version:   	V1.0.0
** @time:      	6:30 PM Saturday, June 1, 2019
** @brief:		 
** @revision:
**             	- version 1.0.1: <date> <name>
**             	<discribe the change>
*******************************************************************************/

#ifndef _VERSION_H_
#define _VERSION_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
**                               INCLUDES
*******************************************************************************/
#include "stdint.h"

/*******************************************************************************
**                               DEFINES
*******************************************************************************/

/* please choose only one version */       
#define HARDWARE_V2_1_1


/* define version firmware */
typedef struct
{
	uint8_t major;
	uint8_t minor;
	uint32_t build_indicator;
} sVersion_t;

/* device time date */

typedef struct
{
    uint8_t date; // day of month
	uint8_t month;
	uint16_t year;
	uint8_t hour;
	uint8_t minute;
	uint32_t second;
    uint8_t day; // day of week
    uint16_t dayInYear;
    uint8_t daylightSavings;
} sDateTime_t;


/*******************************************************************************
**                     EXTERNAL VARIABLE DECLARATIONS
*******************************************************************************/
extern const char* g_dateTimeStr;
extern const char* g_builder;
extern sDateTime_t g_timeBuild;
extern sVersion_t g_fwVersion;
extern sVersion_t g_hwVersion;
/*******************************************************************************
**                     EXTERNAL FUNCTION DECLARATIONS
*******************************************************************************/
extern int8_t version_cmp(sVersion_t v1, sVersion_t v2);


 
 
 
 
 
 
 
 
 
 
 
 
 
 

#ifdef __cplusplus
}
#endif

#endif

/******************************** End of file *********************************/
