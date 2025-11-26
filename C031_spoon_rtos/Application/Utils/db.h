/*******************************************************************************
 * @file   : db.h
 * @date   : 2018/08/02 15:26
 * @author : LieuND
 * @brief  : 
 * TODO    : 
 * @note   :
 *******************************************************************************/
#ifndef db_h__
#define db_h__

#include "cmn.h"
#include "sem.h"

/*define*/
#define VELOCITY_THRESHOLD                     10              /*10 km/h*/

#define LOG_PKG_SIZE                           64
#define LOG_PKG_SENT                           0x01
#define FLASH_CURPAGE_SIZE                     256
#define FLASH_SECTOR_SIZE                      (16*FLASH_CURPAGE_SIZE)


#define EVENT_PKG_SIZE                           64

/*----------------------------------------------------------------------*/
/* SYS_PARAM |   DATA LOG                                | FIRMWARE     */
/*----------------------------------------------------------------------*/
/* 4KB       |   3MB - 4KB                               | 1MB          */
/*----------------------------------------------------------------------*/
/* 1 SECTOR  |   767 SECTOR                              | 256 SECTOR   */
/*----------------------------------------------------------------------*/


/* Down size to 16Mbit <=> 2Mbye */
/*----------------------------------------------------------------------*/
/* SYS_PARAM |   DATA LOG                                | FIRMWARE     */
/*----------------------------------------------------------------------*/
/* 4KB       |   1.5MB - 4KB                             | 0.5MB        */
/*----------------------------------------------------------------------*/
/* 1 SECTOR  |   383 SECTOR                              | 128 SECTOR   */
/*----------------------------------------------------------------------*/


#define ADDR_PATITION_SYSTEM_PARAM             0
#define ADDR_PATITION_DATA_LOG                 FLASH_SECTOR_SIZE
#define ADDR_PATITION_FIRMWARE                 (0x40000)/*0.5MB*/
#define PATITION_LOG_SIZE                      (ADDR_PATITION_FIRMWARE - ADDR_PATITION_DATA_LOG)
#define NUM_OF_LOG_SECTORS                     (PATITION_LOG_SIZE/ FLASH_SECTOR_SIZE)
/* for event log */
#define ADDR_PATITION_EVENT                    ADDR_PATITION_FIRMWARE
#define ADDR_PATITION_EVENT_END                (0x70000)/*0.5MB*/
#define PATITION_EVENT_SIZE                    (ADDR_PATITION_EVENT_END - ADDR_PATITION_EVENT)
#define NUM_OF_EVENT_SECTORS                   (PATITION_EVENT_SIZE/ FLASH_SECTOR_SIZE)

#define ADDR_SYSTEM_DATA                       (SYSTEM_PARAMETER_BASE_ADDR + 0)
#define ADDR_SYSTEM_DATA_TEST                  SYSTEM_PARAMETER_TEST_BASE_ADDR

#define ADDR_EPO_FILE                          ADDR_PATITION_EVENT_END
/*
+------------------------------------------------------------------------------------------------+
|									SYSTEM_PARAMETER = 12KB						                 |
+-------------------------+------------------+-----------------+----------------+----------------+
|      SYSTEM_DATA        |      ROOT_CA     |    CLIENT_CA    |   CLIENT_KEY   | 	OTHER...     |
+-------------------------+------------------+-----------------+----------------+----------------+
|          4KB            |       2KB        |      2KB        |      2KB       |		2KB		 |
+-------------------------+------------------+-----------------+----------------+----------------+
*/
#define SYSTEM_DATA_SIZE                    (0x1000)    //4KB
#define CERTIFICATE_SIZE                    (0x800)     //2KB



#define SYSTEM_DATA_ROOT_CA_ADDR            (ADDR_SYSTEM_DATA + SYSTEM_DATA_SIZE)
#define SYSTEM_DATA_CLIENT_CA_ADDR          (ADDR_SYSTEM_DATA + SYSTEM_DATA_SIZE + CERTIFICATE_SIZE)
#define SYSTEM_DATA_CLIENT_KEY_ADDR         (ADDR_SYSTEM_DATA + SYSTEM_DATA_SIZE + CERTIFICATE_SIZE  + CERTIFICATE_SIZE)

/*declare ac_cert file */
//extern char certificateFile[];
//extern const char* ca_pem;
//extern const char* client_key;
//extern const char* client_pem;
/*struct*/
extern sSys_Object_test        g_sysObjectTest;
extern sSys_Object_test_to_save g_sysObjectTestToSave;
/*semaphore*/
extern sem smoduleSimRecvData_sem;
/*struct*/


extern volatile sSysDiagnostic g_sysDiagnostic;

extern uint8_t        g_gpsSentencesIncorrect[];



/*API*/
status_t db_Init(void);
status_t db_GetDeviceImei(char *deviceImei, uint8_t u8Len);
status_t db_GetDeviceInfo(char *deviceInfo, uint8_t u8Len);
status_t db_ReadVoltageLevel(void);
status_t db_ReadTemprature(void);
status_t db_getRemainData4GRemain(void);

status_t db_GetRemainLogSize(uint32_t *u32NumRec);
status_t db_sysParamEraseChip(void);
status_t db_ReadLogData(uint8_t *pRetData, uint32_t *u32Len);
status_t db_WriteLogData(uint8_t *pData, uint32_t u32Len);
status_t db_PopLogDoneConfirm(uint32_t u32Val);

status_t db_GetRemainEventSize(uint32_t *u32NumRec);
status_t db_ReadEvent(uint8_t *pRetData, uint32_t *u32Len);
status_t db_WriteEvent(uint8_t *pData, uint32_t u32Len);
status_t db_PopEventDoneConfirm(uint32_t u32Val);
status_t db_ScanAllEvent(void);

status_t db_autoCalculateCRC(void);
status_t db_sysParamSave(void);
status_t db_sysParamLoad(void);
status_t db_ScanAllLog(void);
status_t db_FirmwareErase(void);
status_t db_readFromInternalFlash(uint8_t *pData, uint32_t addr, uint32_t noByte);
status_t db_writeToInternalFlash(uint8_t *pData, uint32_t addr, uint32_t noByte);

/* read and write certificate file */
status_t db_readRootCa(uint8_t *pData, uint32_t noByte);
status_t db_writeRootCa(uint8_t *pData, uint32_t noByte);
status_t db_readClientCa(uint8_t *pData, uint32_t noByte);
status_t db_writeClientCa(uint8_t *pData, uint32_t noByte);
status_t db_readClientKey(uint8_t *pData, uint32_t noByte);
status_t db_writeClientKey(uint8_t *pData, uint32_t noByte);

status_t db_getBroker(char* brokerIp, uint16_t brokerIpLen, uint16_t* brokerPort);
status_t db_getTopic(char* topic, uint16_t topicLen, uint8_t level);
status_t db_resetTopic(void);
status_t db_saveTopic(char* topic);
BOOL     db_haveTheLastCoordinate(void);
status_t db_InitDataTest(void);
status_t db_SaveDataTest(void);
#endif // db_h__
