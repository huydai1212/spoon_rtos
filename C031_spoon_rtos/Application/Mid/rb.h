/*******************************************************************************
 ** @file:       rb.h
 ** @author:     HUY DAI
 ** @version:    V1.0.0
 ** @time:       9:26 AM Thursday, October 3, 2019
 ** @brief:		 
 ** @revision:
 **             	- version 1.0.1: <date> <name>
 **             	<discribe the change>
 ******************************************************************************/

#ifndef _RING_BUF_MAIL_BOX_H_
#define _RING_BUF_MAIL_BOX_H_


#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 **                               INCLUDES
 ******************************************************************************/
#include "cmn.h"
#include "stdbool.h"
//#include "db.h"

/*******************************************************************************
 **                               DEFINES
 ******************************************************************************/
typedef struct _sRb_
{
    uint8_t *data;
    uint16_t dataMaxSize;
    uint16_t dataRemainSize;
    uint16_t head;
    uint16_t tail;
    uint16_t count;
}sRb;




/*******************************************************************************
**                     EXTERNAL VARIABLE DECLARATIONS
*******************************************************************************/

/*******************************************************************************
**                     EXTERNAL FUNCTION DECLARATIONS
*******************************************************************************/
void     rb_reset(sRb *pRb, uint8_t *pRbData, uint16_t dataSize, uint16_t eMaxSize);
uint16_t rb_push(sRb *pRb, uint8_t *pStr, uint16_t length);
uint16_t rb_pop(sRb *pRb, uint8_t *pStr, uint16_t length);
uint16_t rb_getCount(sRb *pRb);


#ifdef __cplusplus
}
#endif

#endif

/******************************** End of file *********************************/
