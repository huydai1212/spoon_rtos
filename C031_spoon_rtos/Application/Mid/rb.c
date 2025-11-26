/*******************************************************************************
 ** @file:       rb.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmn.h"


/*******************************************************************************
**                       INTERNAL MACRO DEFINITIONS
*******************************************************************************/
#define     NUM_BYTE_LEN    2
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

/*
** @brief       init all parameter of ring buffer
** @param
** @return      None
** @time        1:47 PM Wednesday, October 2, 2019
** @revision
**
*/
void rb_reset(sRb *pRb, uint8_t *pRbData, uint16_t dataSize, uint16_t eMaxSize)
{
    ASSERT(pRb);
    ASSERT(pRbData);

    pRb->data           = pRbData;
    pRb->dataMaxSize    = dataSize;
    pRb->dataRemainSize = dataSize;
    pRb->head           = 0;
    pRb->tail           = 0;
    pRb->count          = 0;
    //pRb->eMaxSize       = eMaxSize;
}

/*
** @brief       push data to ring buffer
** @param
** @return      size of push, if error return 0
** @time        1:48 PM Wednesday, October 2, 2019
** @revision
**
*/
uint16_t rb_push(sRb *pRb, uint8_t *pStr, uint16_t length)
{
    ASSERT(pRb);
    ASSERT(pStr);
    uint16_t remainData = 0;
    uint16_t lengthP1 = 0;
    uint16_t lengthP2 = 0;

    if(length > pRb->dataMaxSize - NUM_BYTE_LEN)
    {
        length = pRb->dataMaxSize - NUM_BYTE_LEN;
        LREP("length the large\r\n");
    }

    if(pRb->dataRemainSize < length + NUM_BYTE_LEN)
    {
        LREP("rbStr is full!\r\n");
        return 0;
    }
    else
    {
        /* Check data tu head den cuoi mang */
        remainData = pRb->dataMaxSize - pRb->head;
        if(remainData == 1)
        {
            *(pRb->data + pRb->head) = (uint8_t)(length >> 8);
            *pRb->data = (uint8_t)(length);
            /* copy data */
            memcpy((char*)pRb->data + 1, (char*)pStr, length);
        }
        else if(remainData == 2)
        {
            *(pRb->data + pRb->head) = (uint8_t)(length >> 8);
            *(pRb->data + pRb->head + 1) = (uint8_t)(length);
            /* copy data */
            memcpy((char*)pRb->data, (char*)pStr, length);
        }
        else if(length + NUM_BYTE_LEN <= remainData)
        {
             /* copy full data */
            *(pRb->data + pRb->head) = (uint8_t)(length >> 8);
            *(pRb->data + pRb->head + 1) = (uint8_t)(length);

            memcpy((char*)pRb->data + pRb->head + NUM_BYTE_LEN, (char*)pStr, length);
        }
        else
        {
            lengthP1 =  pRb->dataMaxSize - (pRb->head +2);
            lengthP2 = length - lengthP1;

            *(pRb->data + pRb->head) = (uint8_t)(length >> 8);
            *(pRb->data + pRb->head + 1) = (uint8_t)(length);

            memcpy((char*)pRb->data + pRb->head + NUM_BYTE_LEN, (char*)pStr, lengthP1);
            memcpy((char*)pRb->data, (char*)pStr + lengthP1, lengthP2);
        }

        /* data remain size */
        pRb->dataRemainSize -=(length + NUM_BYTE_LEN);
        /* head */
        pRb->head +=(length + NUM_BYTE_LEN);
        if(pRb->head >= pRb->dataMaxSize)
        {
            pRb->head -= pRb->dataMaxSize;
        }
        /* increase count */
        ++pRb->count;
    }
    return length;
}

/*
** @brief       pop data from ring buffer
** @param
** @return      size of pop, if error return 0.
** @time        1:49 PM Wednesday, October 2, 2019
** @revision
**
*/
uint16_t rb_pop(sRb *pRb, uint8_t *pStr, uint16_t length)
{
    ASSERT(pRb);
    ASSERT(pStr);

    uint16_t lengthHaveData = 0;
    uint16_t lengthP1 = 0;
    uint16_t lengthP2 = 0;
    uint16_t lenCalc = 0;
    uint16_t lenCpy = 0;


    if(0 == pRb->count)
    {
        return 0;
    }

    /* check size data tu tail den het mang data */
    lengthHaveData = pRb->dataMaxSize - pRb->tail;

    if(lengthHaveData == 1)
    {
        lenCalc = *(pRb->data + pRb->tail);
        lenCalc <<= 8;
        lenCalc += *(pRb->data);
        //LREP("lenCalc: %d\r\n", lenCalc);
        lenCpy = lenCalc;
        if(lenCalc > length - 1)
        {
            lenCpy = length - 1;
        }
        memcpy((char*)pStr, pRb->data + 1, lenCpy);
    }
    else if(lengthHaveData == 2)
    {
        lenCalc = *(pRb->data + pRb->tail);
        lenCalc <<= 8;
        lenCalc += *(pRb->data + pRb->tail + 1);
        //LREP("lenCalc: %d\r\n", lenCalc);
        lenCpy = lenCalc;
        if(lenCalc > length - 1)
        {
            lenCpy = length - 1;
        }
        memcpy((char*)pStr, pRb->data, lenCpy);
    }
    else
    {
        lenCalc = *(pRb->data + pRb->tail);
        lenCalc <<= 8;
        lenCalc += *(pRb->data + pRb->tail + 1);
        //LREP("lenCalc: %d\r\n", lenCalc);
        lenCpy = lenCalc;
        if(lenCalc > length - 1)
        {
            lenCpy = length - 1;
        }

        if(lenCpy < pRb->dataMaxSize - (pRb->tail + NUM_BYTE_LEN))
        {
            memcpy((char*)pStr, (char*)(pRb->data + pRb->tail + NUM_BYTE_LEN), lenCpy);
        }
        else
        {
            lengthP1 = pRb->dataMaxSize - (pRb->tail + NUM_BYTE_LEN);
            lengthP2 = lenCpy - lengthP1;
            memcpy((char*)pStr, (char*)(pRb->data + pRb->tail + NUM_BYTE_LEN), lengthP1);
            memcpy((char*)pStr + lengthP1, (char*)(pRb->data), lengthP2);
        }
    }

    /* set the last tring is 0x00*/
    *(pStr + lenCpy) = 0x00;

    /* data remain size */
    pRb->dataRemainSize += (lenCalc + NUM_BYTE_LEN);
    /* head */
    pRb->tail += (lenCalc + NUM_BYTE_LEN);
    if(pRb->tail >= pRb->dataMaxSize)
    {
        pRb->tail -= pRb->dataMaxSize;
    }
    /* increase count */
    --pRb->count;

    return lenCpy;
}


/*
** @brief       get count
** @param
** @return      None
** @time        9:31 AM Thursday, October 3, 2019
** @revision
**
*/
uint16_t rb_getCount(sRb *pRb)
{
    return pRb->count;
}




/******************************** End of file *********************************/
