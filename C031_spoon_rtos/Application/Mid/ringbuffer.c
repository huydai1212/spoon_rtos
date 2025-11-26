/*******************************************************************************
 ** @file:       ringbuffer.h
 ** @author:     HUY DAI
 ** @version:    V1.0.0
 ** @time:       3:11 PM Friday, 2 March, 2018
 ** @brief:		 
 ** @revision:
 **             	- version 1.0.1: <date> <name>
 **             	<discribe the change>
 ******************************************************************************/

/*******************************************************************************
 **                               INCLUDES
 ******************************************************************************/
#include <rb.h>
#include <ringbuffer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmn.h"
#include "uart_console.h"

/*******************************************************************************
**                       INTERNAL MACRO DEFINITIONS
*******************************************************************************/
/*******************************************************************************
**                      COMMON VARIABLE DEFINITIONS
*******************************************************************************/

/*******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/ 

/* ring buffer for gps data raw */
#define             GPS_DATA_RAW_SIZE           256
sRb                 rbGpsRaw;
static uint8_t      rbGpsRawData[GPS_DATA_RAW_SIZE];

/* ring buffer for gps data log */
#define             GPS_LOG_DATA_RAW_SIZE           256
sRb                 rbGpsLog;
//static uint8_t      rbGpsLogData[GPS_LOG_DATA_RAW_SIZE];

/* ring buffer for firmware*/
#define             FW_DATA_RAW_SIZE           256
sRb                 rbFwRaw;
//static uint8_t      rbFwRawData[FW_DATA_RAW_SIZE];

/* ring buffer for gsm*/
#define             GSM_DATA_RAW_SIZE           256
sRb                 rbGsmRaw;
static uint8_t      rbGsmRawData[GSM_DATA_RAW_SIZE];

/* ring burrer for PC data */
#define             PC_DATA_RAW_SIZE            256//384
sRb                 rbPcRaw;
static uint8_t      rbPcRawData[PC_DATA_RAW_SIZE];

/* ring buffer for sms */
#define             SMS_DATA_RAW_SIZE           64
sRb                 rbSmsRaw;
static uint8_t      rbSmsRawData[SMS_DATA_RAW_SIZE];

/* ring buffer for event */
#define             EVENT_RB_SIZE               256
sRb                 rbEvent;
static uint8_t      rbEventData[EVENT_RB_SIZE];

#define             PC_ACK_SIZE                 128
sRb                 rbPCAck;
static uint8_t      rbPCAckData[PC_ACK_SIZE];

/*******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/

/*******************************************************************************
**                           FUNCTION DEFINITIONS
*******************************************************************************/

/*******************************************************************************
** @brief		init data ringbufer
** @param		
** @return		None
** @time		2:01 PM Wednesday, 31 October, 2018
** @revision	
**             
*******************************************************************************/
void rb_Init(void)
{
//    /* ring buffer for gsm */
//    rb_reset(&rbGsmRaw, rbGsmRawData, GSM_DATA_RAW_SIZE, GSM_DATA_RAW_SIZE);
//    /* ring buffer for gps data log */
//    rb_reset(&rbGpsLog, rbGpsLogData, GPS_LOG_DATA_RAW_SIZE, GPS_LOG_DATA_RAW_SIZE);
//    /* ring buffer for gps data raw */
//    rb_reset(&rbGpsRaw, rbGpsRawData, GPS_DATA_RAW_SIZE, GPS_DATA_RAW_SIZE);
//    /* ring buffer for firmware */
//    rb_reset(&rbFwRaw, rbFwRawData, FW_DATA_RAW_SIZE, FW_DATA_RAW_SIZE);
//    /* ring buffer for sms */
//    rb_reset(&rbSmsRaw, rbSmsRawData, SMS_DATA_RAW_SIZE, SMS_DATA_RAW_SIZE);
    /* ring buffer for PC*/
    rbPcReset();
    /* ring buffer for event */
//    rbEventReset();
    /* pc ack */
//    rbPcAckReset();

} 


/*******************************************************************************
** @brief		get data from ring buffer
** @param		*string is poiter of data output
** @return		TRUE or FALSE
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbCoordinatePop(uint8_t * str, uint16_t n)
{
    return rb_pop(&rbGpsLog, str, n);
}

/*******************************************************************************
** @brief		get data from ring buffer
** @param		*string is poiter of data output
** @return		TRUE or FALSE
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbCoordinatePush(uint8_t * str, uint16_t n)
{
    return rb_push(&rbGpsLog, str, n);
}

/*******************************************************************************
** @brief		
** @param		
** @return		number data in ring buffer
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbCoordinateGetCount(void)
{
    return rb_getCount(&rbGpsLog);
}

/*******************************************************************************
** @brief       reset gps ring buffer
** @param
** @return      none
** @time
** @revision
**
*******************************************************************************/
void rbGpsReset(void)
{
    rb_reset(&rbGpsRaw, rbGpsRawData, GPS_DATA_RAW_SIZE, GPS_DATA_RAW_SIZE);
}

/*******************************************************************************
** @brief		get gps data raw
** @param		
** @return		size pop
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbGpsPop(uint8_t * str, uint16_t n)
{
    return rb_pop(&rbGpsRaw, str, n);
}

/*******************************************************************************
** @brief		put gps data raw to ring buffer
** @param		
** @return		TRUE or FALSE
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbGpsPush(uint8_t * str, uint16_t n)
{
    return rb_push(&rbGpsRaw, str, n);
}

/*******************************************************************************
** @brief		get the current number of GPS log in ring buffer
** @param		
** @return		current number of GPS log
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbGpsGetCount(void)
{
    return rb_getCount(&rbGpsRaw);
}

/*******************************************************************************
** @brief       reset GSM ring buffer
** @param
** @return      none
** @time
** @revision
**
*******************************************************************************/
void rbGsmReset(void)
{
    rb_reset(&rbGsmRaw, rbGsmRawData, GSM_DATA_RAW_SIZE, GSM_DATA_RAW_SIZE);
}


/*******************************************************************************
** @brief		get data from ring buffer GSM
** @param		*str is pointer of data input
** @param       n: the number on character
** @return		size of pop
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbGsmPop(uint8_t * str, uint16_t n)
{
    return rb_pop(&rbGsmRaw, str, n);
}


/*******************************************************************************
** @brief		push data to ring buffer
** @param		*str is pointer of data output
** @param       n: the number on character
** @return		size of push
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbGsmPush(uint8_t * str, uint16_t n)
{
    return rb_push(&rbGsmRaw, str, n);
}

/*******************************************************************************
** @brief		get the current number of GSM string in ring buffer
** @param		
** @return		current number of GPS log
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbGsmGetCount(void)
{
    return rb_getCount(&rbGsmRaw);
}


/*******************************************************************************
** @brief       reset sms ring buffer
** @param       none
** @return      TRUE or FALSE
** @time
** @revision
**             
*******************************************************************************/
void rbSmsReset(void)
{
    rb_reset(&rbSmsRaw, rbSmsRawData, SMS_DATA_RAW_SIZE, SMS_DATA_RAW_SIZE);
}


/*******************************************************************************
** @brief		get data from ring buffer
** @param		*str is pointer of data output
** @return		TRUE or FALSE
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbSmsPop(uint8_t * str, uint16_t n)
{
    return rb_pop(&rbSmsRaw, str, n);
}



/*******************************************************************************
** @brief		put data to ring buffer
** @param		*str is poiter of data output
** @return		TRUE or FALSE
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbSmsPush(uint8_t * str, uint16_t n)
{
    return rb_push(&rbSmsRaw, str, n);
}


/*******************************************************************************
** @brief		get the current number of SMS UN-READ in ring buffer
** @param		
** @return		current number of SMS UN-READ
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbSmsGetCount(void)
{
    return rb_getCount(&rbSmsRaw);
}

void rbPcReset(void)
{
    rb_reset(&rbPcRaw, rbPcRawData, PC_DATA_RAW_SIZE, PC_RB_ELEMENT_SIZE);
}

/*******************************************************************************
** @brief		get data from ring buffer
** @param		*string is poiter of data output
** @return		TRUE or FALSE
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbPcPop(uint8_t * str, uint16_t n)
{
    return rb_pop(&rbPcRaw, str, n);
}

/*******************************************************************************
** @brief		push data to ring buffer
** @param		*str is poiter of data output
** @return		TRUE or FALSE
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbPcPush(uint8_t * str, uint16_t n)
{
    return rb_push(&rbPcRaw, str, n);
}

/*******************************************************************************
** @brief		get the current number of DATA PC in ring buffer
** @param		
** @return		current number of DATA PC
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbPcGetCount(void)
{
    return rb_getCount(&rbPcRaw);
}



uint16_t rbFwPop(uint8_t * str, uint16_t n)
{
    return rb_pop(&rbFwRaw, str, n);
}
uint16_t rbFwPush(uint8_t * string, uint16_t length)
{
    return rb_push(&rbFwRaw, string, length);
}
uint16_t rbFwGetCount(void)
{
    return rb_getCount(&rbFwRaw);
}
/*******************************************************************************
** @brief       reset event ring buffer
** @param       none
** @return      TRUE or FALSE
** @time
** @revision
**             
*******************************************************************************/
void rbEventReset(void)
{
    rb_reset(&rbEvent, rbEventData, EVENT_RB_SIZE, EVENT_RB_SIZE);
}

/*******************************************************************************
** @brief		get data from ring buffer
** @param		*string is pointer of data output
** @return		TRUE or FALSE
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbEventPop(uint8_t * str, uint16_t n)
{
    return rb_pop(&rbEvent, str, n);
}

/*******************************************************************************
** @brief		push data to ring buffer
** @param		*str is pointer of data output
** @param       n: the number on character
** @return		TRUE or FALSE
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbEventPush(uint8_t * str, uint16_t n)
{
    return rb_push(&rbEvent, str, n);
}

/*******************************************************************************
** @brief		get the current number of event in ring buffer
** @param		
** @return		current number of event
** @time		
** @revision	
**             
*******************************************************************************/
uint16_t rbEventGetCount(void)
{
    return rb_getCount(&rbEvent);
}

/* PC ACK ring buffer */
/*******************************************************************************
** @brief       reset PC ACK ring buffer
** @param       none
** @return      TRUE or FALSE
** @time
** @revision
**
*******************************************************************************/
void rbPcAckReset(void)
{
    rb_reset(&rbPCAck, rbPCAckData, PC_ACK_SIZE, PC_ACK_SIZE);
}
/*******************************************************************************
** @brief       get data from ring buffer
** @param       *string is pointer of data output
** @return      TRUE or FALSE
** @time
** @revision
**
*******************************************************************************/
uint16_t rbPcAckPop(uint8_t * str, uint16_t n)
{
    return rb_pop(&rbPCAck, str, n);
}

/*******************************************************************************
** @brief       push data to ring buffer
** @param       *str is pointer of data output
** @param       n: the number on character
** @return      TRUE or FALSE
** @time
** @revision
**
*******************************************************************************/
uint16_t rbPcAckPush(uint8_t * str, uint16_t n)
{
    return rb_push(&rbPCAck, str, n);
}

/*******************************************************************************
** @brief       get the current number of event in ring buffer
** @param
** @return      current number of event
** @time
** @revision
**
*******************************************************************************/
uint16_t rbPcAckGetCount(void)
{
    return rb_getCount(&rbPCAck);
}


/******************************** End of file *********************************/
