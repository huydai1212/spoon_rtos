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

#ifndef _RING_BUFFER_H_
#define _RING_BUFFER_H_


#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 **                               INCLUDES
 ******************************************************************************/
#include "cmn.h"
#include "db.h"

/*******************************************************************************
 **                               DEFINES
 ******************************************************************************/
#define     GSM_RB_ELEMENT_SIZE         128//AT_RES_SIZE

#define     GPS_RB_ELEMENT_SIZE         (LOG_PKG_SIZE)
#define     GPS_RECEIVER_SIZE           (128)

#define     GPS_DATA_RAW_RB_ELEMENT_SIZE (GPS_RECEIVER_SIZE)
    
#define     PC_RB_ELEMENT_SIZE          (128)

#define     READ_SMS_ELEMENT_SIZE       (32)

#define     DUT_ACK_RB_ELEMENT_SIZE     8

typedef struct _sRbStr
{
	uint8_t *data;
	uint16_t head;
	uint16_t tail;
    uint16_t size;
    uint16_t maxSize;
    uint16_t elementSize;
}sRbStr_t;

typedef struct _sRb
{
	uint16_t *data;
	uint16_t head;
	uint16_t tail;
    uint16_t size;
    uint16_t maxSize;
}sRb_t;
/*******************************************************************************
**                     EXTERNAL VARIABLE DECLARATIONS
*******************************************************************************/

/*******************************************************************************
**                     EXTERNAL FUNCTION DECLARATIONS
*******************************************************************************/
void        rb_Init(void);


/* GPS codinate ring buffer */
void        rbCoordinateReset(void);
uint16_t    rbCoordinatePop(uint8_t * str, uint16_t n);
uint16_t    rbCoordinatePush(uint8_t * str, uint16_t n);
uint16_t    rbCoordinateGetCount(void);


/* GPS ring buffer */
void        rbGpsReset(void);
uint16_t    rbGpsPop(uint8_t * str, uint16_t n);
uint16_t    rbGpsPush(uint8_t * str, uint16_t n);
uint16_t    rbGpsGetCount(void);


/* GSM ring buffer */
void        rbGsmReset(void);
uint16_t    rbGsmPop(uint8_t * str, uint16_t n);
uint16_t    rbGsmPush(uint8_t * str, uint16_t n);
uint16_t    rbGsmGetCount(void);


/* firmware ring buffer */
void        rbFwReset(void);
uint16_t    rbFwPop(uint8_t * str, uint16_t n);
uint16_t    rbFwPush(uint8_t * str, uint16_t n);
uint16_t    rbFwGetCount(void);


/* sms ring buffer */
void        rbSmsReset(void);
uint16_t    rbSmsPop(uint8_t * str, uint16_t n);
uint16_t    rbSmsPush(uint8_t * str, uint16_t n);
uint16_t    rbSmsGetCount(void);


/* PC ring buffer */
void        rbPcReset(void);
uint16_t    rbPcPop(uint8_t * str, uint16_t n);
uint16_t    rbPcPush(uint8_t * str, uint16_t n);
uint16_t    rbPcGetCount(void);

/* Event ring buffer */
void        rbEventReset(void);
uint16_t    rbEventPop(uint8_t * str, uint16_t n);
uint16_t    rbEventPush(uint8_t * str, uint16_t n);
uint16_t    rbEventGetCount(void);

/* PC ACK ring buffer */
void        rbPcAckReset(void);
uint16_t    rbPcAckPop(uint8_t * str, uint16_t n);
uint16_t    rbPcAckPush(uint8_t * str, uint16_t n);
uint16_t    rbPcAckGetCount(void);



#ifdef __cplusplus
}
#endif

#endif

/******************************** End of file *********************************/
