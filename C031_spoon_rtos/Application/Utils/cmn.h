/*******************************************************************************
** @file:       cmn.h
** @author:
** @version:    V1.0.0
** @time:       11:28 AM Tuesday, June 30, 2020
** @brief:
** @revision:
**              - version 1.0.1: <date> <name>
**              <discribe the change>
*******************************************************************************/
#ifndef cmn_h__
#define cmn_h__
/******************************************************************************
*                               INCLUDES
******************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "uart_console.h"
#include "main.h"
#include "mDelay.h"
#include <string.h>

/******************************************************************************
*                               DEFINES
******************************************************************************/
#define DEBUG_EN                (1)
#define TEST_MODE               (0)
#define TEST_FUNC               (0)
//#define TEST_GPS
//#define TEST_POWER
//#define DEBUG_STM
//#define FILTER_GPS_BASED_ON_GSA
//#define FILTER_GPS_BASED_ON_ANGLE
//#define LOG_TEST_GPS
#define USE_EXTERNAL_XTAL32768
#define ANGLE_INIT (-2)

/*******************************************************************************
*                               COMMON DEFINE
*******************************************************************************/

#ifndef BOOL
#define BOOL  uint8_t
#define TRUE  (1)
#define FALSE (0)
#endif

#ifndef LREP
#define LREP                                        debug_printf
#define LREP_AT                                     debug_printf
#define LREP_GPS_CMD                                debug_off
#define DATA_ToPC                                   debug_off//data_printf
#define LREP_GPS                                    debug_off//dataGPS_printf
#define LREP_NORMAL									normal_printf

#define SEND_DATA_TO_PC                             debug_printf
#define SEND_DATA_TO_MASTER 						debug_printf
#define LREP_PRINTF                                 debug_printf//debug_off  //debug_printf
#endif

#define ASSERT_VOID(con)                            { if(!(con)) {LREP("ASSERT file %s line %d\r\n", __FILE__, __LINE__); return;} }
#define ASSERT_NONVOID(con, ret)                    { if(!(con)) {LREP("ASSERT file %s line %d\r\n", __FILE__, __LINE__); return ret;} }

#ifndef ASSERT
#define ASSERT(con)                                 { if(!(con)) {LREP("\r\nASSERT file %s line %d\r\n", __FILE__, __LINE__); } }
#define ASSERT_VALUE(con, val)                      { if(!(con)) {LREP("\r\nASSERT file %s line %d, val = %d\r\n", __FILE__, __LINE__, val); } }
#endif
	
#define ASSERT_WARN(con)                            { if(!(con)) {LREP("\r\nASSERT file %s line %d.", __FILE__, __LINE__); } }

#ifndef NULL
#define NULL (0)
#endif


/* define size of AT commands*/
#define AT_CMD_SIZE                  200
#define AT_RES_SIZE                  300
#define URL_PATH_SIZE                200

/* Define state of led */
#define LED_ON                                      GPIO_PIN_SET
#define LED_OFF                                     GPIO_PIN_RESET
#define LED_R_ON                                    GPIO_PIN_RESET
#define LED_R_OFF                                   GPIO_PIN_SET
#define LED_G_ON                                    GPIO_PIN_RESET
#define LED_G_OFF                                   GPIO_PIN_SET
#define LED_B_ON                                    GPIO_PIN_RESET
#define LED_B_OFF                                   GPIO_PIN_SET

/* Time wakeup */
#define TIME_CYCLE_RTC_WAKEUP           20
#ifdef USE_EXTERNAL_XTAL32768
#define TIME_CYCLE_RTC_WAKEUP_LOAD      TIME_CYCLE_RTC_WAKEUP - 1
#else
#define TIME_CYCLE_RTC_WAKEUP_LOAD      ((TIME_CYCLE_RTC_WAKEUP * 40000 / 32768) - 1)
#endif

/* Delay bettween AT commands*/
#define delayAtCommand()                HAL_Delay(5)

/* Wait time */
#define WAIT_TIME                       1000000
#define TIME_WAIT_NOTICE_OF_NEW_SMS     (200000)
#define TIME_WAIT_READ_SMS              (20000)
#define DATA_PARSER_SIZE                (64) //200

#define BLINK_FOREVER                   (0XFFFFFFFF)

#define SYSTEM_PARAMETER_TEST_BASE_ADDR ((uint32_t)0x08020000)

/* Refer wiki, reject GPS data have DOP > 5*/
/* Source https://en.wikipedia.org/wiki/Dilution_of_precision_(navigation) */   
/*
________________________________________________________________________________________________________________________________________________________________________________________
DOP     |   Value	    |   Rating	Description
--------+---------------+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
1	    |   Ideal	    |   Highest possible confidence level to be used for applications demanding the highest possible precision at all times.
1-2	    |   Excellent	|   At this confidence level, positional measurements are considered accurate enough to meet all but the most sensitive applications.
2-5	    |   Good	    |   Represents a level that marks the minimum appropriate for making accurate decisions. Positional measurements could be used to make reliable in-route navigation suggestions to the user.
5-10	|   Moderate	|   Positional measurements could be used for calculations, but the fix quality could still be improved. A more open view of the sky is recommended.
10-20	|   Fair	    |   Represents a low confidence level. Positional measurements should be discarded or used only to indicate a very rough estimate of the current location.
>20	    |   Poor	    |   At this level, measurements are inaccurate by as much as 300 meters with a 6-meter accurate device (50 DOP � 6 meters) and should be discarded.
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

#define PDOP_MAX                        (5.0)
#define HDOP_MAX                        (5.0)
#define VDOP_MAX                        (5.0)


/* for data debug on PC */
#define HEADER_PC                       "$PC"
#define HEADER_DEVICE                   "$JIG"

/*******************************************************************************    
**                               ENUM DEFINE
*******************************************************************************/
typedef enum
{
    RET_OK       = 0x00U,
    RET_ERROR    = 0x01U,
    RET_BUSY     = 0x02U,
    RET_TIMEOUT  = 0x03U,
    RET_FAIL,
    RET_DONOTHING,
    RET_INVALID_PARAM,
    RET_CONTINUE,
}status_t;



typedef enum{
    VEHICLE_STOP = 0x30,
	VEHICLE_PARKING,
	VEHICLE_RUNNING,
	VEHICLE_ERROR
}vehicle_status_t;



enum
{
    LOW_LEVEL = 0,
    NORMAL_LEVEL,
    HIGH_LEVEL,
};



enum{
    LED_MODE_ON = 1,
    LED_MODE_OFF,
    LED_MODE_BLINK,
};

typedef void (*led_fun_control_ptr)(uint8_t*);
/*******************************************************************************
**                               FUNCTION POINTER
*******************************************************************************/


/*******************************************************************************
**                               DATA STRUCT DEFINE
*******************************************************************************/


typedef struct system_dianostic_ {
	volatile uint8_t          initialDone;
    volatile uint8_t          flashIsError;


	volatile uint32_t         byteCount;
	volatile uint32_t         countTimer;
	volatile uint32_t         countTimerPWM;
	volatile uint32_t         countTimerCycle;

	volatile uint32_t         countTimerAngle;
	volatile uint16_t 		  angleOffset;
	volatile uint16_t		  countSpeed;
	volatile uint16_t		  angularVelocity;
	volatile uint8_t 		  status;
	volatile int16_t 		  angle;
	volatile uint32_t 		  stepSpeed;
	volatile uint32_t 		  maxSpeedNo;
	volatile uint32_t 		  step;

	volatile uint8_t spoonInt1;
	volatile uint8_t spoonInt2;
	volatile uint8_t spoonInt3;
	volatile uint8_t spoonInt4;

	volatile uint8_t userInt1;
	volatile uint8_t userInt2;
	volatile uint8_t userInt3;
	volatile uint8_t userInt4;

	volatile uint8_t processStart;
	volatile uint8_t enableTest;

} sSysDiagnostic;


typedef struct led_object_ {
    volatile uint32_t          u32TickStart;
    volatile uint32_t          u32TimeOn;
    volatile uint32_t          u32TimeOff;
    volatile uint8_t           isRunning;
    volatile uint8_t           isFirstControl;
    volatile uint8_t           state;
    volatile uint8_t           mode;
    volatile uint32_t          numBlink;
    led_fun_control_ptr        funcOn;
    led_fun_control_ptr        funcOff;
} sLedObject;

typedef struct sys_object_test_ {
    volatile uint8_t bma;
    volatile uint8_t detect12VdcFailing;
    volatile uint8_t detect12VdcRising;
    volatile uint8_t u8FlagTestResult;
} sSys_Object_test;

typedef struct sys_object_test_to_save_ {
    volatile uint8_t u8FlagTestResult;
    volatile uint8_t u8FlagTestResultCRC;
} sSys_Object_test_to_save;



#endif // cmn_h__
