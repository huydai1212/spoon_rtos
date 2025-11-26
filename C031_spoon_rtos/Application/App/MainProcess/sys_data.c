/*******************************************************************************
** @file:      	sys_data.c
** @author:    	HUY DAI
** @version:   	V1.0.0
** @time:      	4:31 PM Monday, September 9, 2019
** @brief:		 
** @revision:
**             	- version 1.0.1: <date> <name>
**             	<discribe the change>
*******************************************************************************/

/*******************************************************************************
**                               INCLUDES
*******************************************************************************/
#include <ringbuffer.h>
#include "cmn.h"
#include "system_event.h"
#include "system_defines.h"
#include "crc.h"
#include "system_proc.h"
#include "str.h"
#include "gpio.h"
#include "string.h"
#include "crc.h"
#include "flash.h"
#include "usart.h"
#include "includes.h"
/*******************************************************************************
**                       INTERNAL MACRO DEFINITIONS
*******************************************************************************/

/*******************************************************************************
**                      COMMON VARIABLE DEFINITIONS
*******************************************************************************/
uint32_t g_u32PcbId;
/*******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/

/*******************************************************************************
**                      INTERNAL FUNCTION PROTOTYPES
*******************************************************************************/
static status_t sys_analysisDataRcvFromPC(void);
//static status_t parserDataReceiver(uint8_t* inputStr, uint8_t* headerStr, uint16_t* cmd, uint16_t* pcbID, uint8_t* dataRet);
//static uint16_t getData(char *sData);
/*******************************************************************************
**                           FUNCTION DEFINITIONS
*******************************************************************************/
uint16_t getData(char *sData) {
	uint16_t data = 0;
	uint8_t tokenCount = 0;
	char *token;

	token = strtok((char*) sData, ",");
	while (token != NULL) {
		++tokenCount;
		if (tokenCount == 2) {
			data = atoi(token);
			return data;
		}
		token = strtok(NULL, "#");
	}

	return data;
}

void sys_data_analysis_withOutAtRespond(void)
{
    /* process PC data */
    sys_analysisDataRcvFromPC();
}
void sys_data_analysis(void)
{
	HAL_UART_Receive_DMA(&huart1, (unsigned char *)&rxBuff_uart1, 1);
    sys_data_analysis_withOutAtRespond();
}
/*******************************************************************************
** @brief		
** @param		
** @return		None
** @time	    4:31 PM Monday, September 9, 2019	
** @revision	
**             
********************************************************************************/

/* tran data to device */
status_t sys_dataForwardCmd(char* dataStr, uint16_t pcbId)
{
    return RET_OK;
}

/*
 * Method   : sys_analysisDataRcvFromPC
 * FullName : sys_analysisDataRcvFromPC
 * Access   : private
 * Note     :
 * @param   :
 * @param   :
 * @return  : status_t
 */
static status_t sys_analysisDataRcvFromPC(void)
{
    uint8_t dataStr[PC_RB_ELEMENT_SIZE + 1] = {0};
    uint16_t length = 0;
#if 0
    uint8_t crcStr[3] = {0};
    uint8_t crc = 0;
    uint8_t crcCalc = 0;

    uint8_t headerStr[5] = {0};
    uint8_t dataRet[180] = {0};
    uint16_t cmdRet = 0;
    uint16_t pcbIdRet = 0;
    system_package pkg;
#endif

    while(0 != rbPcGetCount())
    {
        memset(dataStr, 0x00, PC_RB_ELEMENT_SIZE + 1);
        length = rbPcPop(dataStr, PC_RB_ELEMENT_SIZE);
        LREP((char*)dataStr);
        /* add events */

/*        if (0 == strcmp((char*) dataStr, "pushSpoon\r\n")) {
        	pkg.e = SYS_PROC_PUSH_SPOON;
        	sys_proc_regNewEvent(&pkg);
        	return RET_OK;
        }else if (0 == strcmp((char*) dataStr, "enBeam\r\n")) {
        	enableBeam();
        	return RET_OK;
        }else if (0 == strcmp((char*) dataStr, "disBeam\r\n")) {
        	disableBeam();
        	return RET_OK;
        }else if (0 == strcmp((char*) dataStr, "beamOffset\r\n")) {
        	beamMoveToOffset(10000);
        	return RET_OK;
        }else if (0 == strcmp((char*) dataStr, "enChops\r\n")) {
        	enablePushChopstick();
        	return RET_OK;
        }else if (0 == strcmp((char*) dataStr, "disChops\r\n")) {
        	disablePushChopstick();
        	return RET_OK;
        }else if (0 == strcmp((char*) dataStr, "pushChop\r\n")) {
			pkg.e = SYS_PROC_PUSH_CHOPSTICK;
			sys_proc_regNewEvent(&pkg);
        	return RET_OK;
        }else */
        if (0 == strncmp((char*) dataStr, "test,", 5)) {
        	g_sysDiagnostic.enableTest = getData((char*)dataStr);
        	LREP("enable test: %d\r\n",g_sysDiagnostic.enableTest);
        	return RET_OK;
        }

        /* the end add events*/
#if 0
        /* Split crc string */
        memset((char*)crcStr, 0x00, 3);
        if(length >=4)
        {
            strncpy((char*)crcStr, (const char*)&dataStr[length - 4], 2);
            crc = 0xff & strtoul((char*)crcStr, 0, 16);

            /* remove crc from dataStr */
            dataStr[length - 4] = 0;

            /* verify data is received */
            crcCalc = crcBasic(dataStr, strlen((char*)dataStr));
            //LREP("[crc], [crc Calculate]: [%2x][%2x]\r\n", crc, crcCalc);
            if(crcCalc == crc)
            {
                if(0 == strncmp((char*)dataStr, (char*) HEADER_PC, strlen((char*)HEADER_PC)))
                {
                    LREP("\r\nACK\r\n");
                    /* parser data receiver */
                    parserDataReceiver(dataStr, headerStr, &cmdRet, &pcbIdRet, dataRet);
                    //LREP("headerStr: %s\r\n", headerStr);
//                    LREP("cmdRet: %d\r\n", cmdRet);
//                    LREP("pcbIdRet: %d\r\n", pcbIdRet);
//                    LREP("dataRet: %s\r\n", dataRet);
                    pkg.u32Data = pcbIdRet;
                    pkg.e = cmdRet;

                    switch(pkg.e)
                    {
                        case CMD_TEST_LED:
                            /* Blink led power 1Hz in 30 minutes*/
    //                        gpio_ledPower_blink(1, 9, 1800);
    //                        continue;
                        break;
                        case CMD_RESET_DEVICE:
                            HAL_NVIC_SystemReset();
                            break;
                        case CMD_WRITE_DATA_TO_FLASH:
                            break;

                        default:
                            break;
                    }


                    if(pkg.e != CMD_RESET_DEVICE)
                    {
                        sys_proc_regNewEvent(&pkg);
                    }
                }
            }
//            else
//            {
//                LREP(" ERROR: [crc], [crc Calculate]: [%2x][%2x]\r\n", crc, crcCalc);
//            }
        }
#endif
    }
    return RET_OK;
}
#if 0
/*
 * Method   : parserDataReceiver
 * FullName : parserDataReceiver
 * Access   : static
 * Note     :
 * @param   :
 * @param   :
 * @return  : status_t
 */
static status_t parserDataReceiver(uint8_t* inputStr, uint8_t* headerStr, uint16_t* cmd, uint16_t* pcbID, uint8_t* dataRet)
{
    ASSERT(inputStr);
    ASSERT(headerStr);
    ASSERT(cmd);
    ASSERT(pcbID);
    ASSERT(dataRet);

    char dataTmp[120] = {0};

    /* header */
    strnsplit((char*)inputStr, 0, 0, '#', 1,(char*) headerStr, 5);

    /* cmd */
    strnsplit((char*)inputStr, '#', 1, '#', 2, dataTmp, 5);
    if(0 == strlen(dataTmp))
    {
        return RET_ERROR;
    }
    *cmd = (uint16_t)atol(dataTmp);

    /* pcbID */
    memset(dataTmp, 0x00, 120);
    strnsplit((char*)inputStr, '#', 2, '#', 3, dataTmp, 5);
    if(0 == strlen(dataTmp))
    {
        return RET_ERROR;
    }
    *pcbID = (uint16_t)atol(dataTmp);

    /* pcbID */
    strnsplit((char*)inputStr, '#', 3, '#', 4, (char*)dataRet, 180);
    return RET_OK;
}

/* trans data to PC*/
/*
 * Method   : sys_dataSendToPc
 * FullName : sys_dataSendToPc
 * Access   : public
 * Note     : tran data to PC
 * @param   : str
 * @param   : u16Len
 * @return  : status_t
 */
#define ACK_TIMEOUT         500
#define ACK_NUM_RETRY       3
status_t sys_dataSend(char* headerStr, uint16_t cmd, uint16_t pcbId, char* dataSend)
{
#if 1
    ASSERT(headerStr);
    ASSERT(dataSend);
    uint8_t pkgDataSend[300] = {0};

    SEND_DATA_TO_PC("\r\n");
    snprintf((char*)pkgDataSend, 300, "%s#%d#%d#%s#", (char*)headerStr, cmd, pcbId, (char*)dataSend);
    uint8_t crc = crcBasic(pkgDataSend, strlen((char*)pkgDataSend));
    uint8_t crcStr[3] = {0};
    snprintf((char*)crcStr, 3, "%02X", crc);
    strncat((char*)pkgDataSend, (char*)crcStr, 3);
    SEND_DATA_TO_PC("%s\r\n", pkgDataSend);
    return RET_OK;
#else
    ASSERT(headerStr);
    ASSERT(dataSend);
    uint8_t pkgDataSend[300] = {0};
    uint32_t tickStart = 0;
    uint8_t i = 0;
    uint8_t dataAck[DUT_ACK_RB_ELEMENT_SIZE + 1] = {0};

    //SEND_DATA_TO_PC("\r\n");
    snprintf((char*)pkgDataSend, 300, "%s#%d#%d#%s#", (char*)headerStr, cmd, pcbId, (char*)dataSend);
    uint8_t crc = crcBasic(pkgDataSend, strlen((char*)pkgDataSend));
    uint8_t crcStr[3] = {0};
    snprintf((char*)crcStr, 3, "%02X", crc);
    strncat((char*)pkgDataSend, (char*)crcStr, 3);


    do
    {
        LREP("retry: %d\r\n", i);
        ++i;
        rbPcAckReset();
        SEND_DATA_TO_PC("\r\n%s\r\n", pkgDataSend);
        tickStart = getTick_ms();
        /* wait ACK */
        do
        {
            /*get and check data from ringbuffer */
            if(rbPcAckGetCount())
            {
                memset((char*)dataAck, 0x00, DUT_ACK_RB_ELEMENT_SIZE + 1);
                rbPcAckPop(dataAck, DUT_ACK_RB_ELEMENT_SIZE);
                LREP("respond: %s\r\n", dataAck);
                if(0 == strncmp((char*)dataAck, "ACK", 3))
                {
                    return RET_OK;
                }
            }
            else
            {
                delay_ms(10);
            }

        }
        while(getTick_ms() - tickStart < ACK_TIMEOUT);
    }
    while(i < ACK_NUM_RETRY);
    LREP("slaves send cmd to JIG timeout\r\n");
    return RET_OK;
#endif
}


#endif


/******************************** End of file **********************************/
