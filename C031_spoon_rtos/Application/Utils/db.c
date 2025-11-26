/*
 * db.c
 *
 *  Created on: Dec 14, 2020
 *      Author: nguyenquoctien
 */

#include "cmn.h"
#include "db.h"
#include "crc.h"
#include <string.h>

sSys_Object_test            g_sysObjectTest         __attribute__ ((aligned (32))) = {0};
sSys_Object_test_to_save    g_sysObjectTestToSave   __attribute__ ((aligned (32))) = {0};


static status_t db_verifyData(char* data, uint32_t sizeOfData, uint8_t crcOfData);

static status_t db_verifyData(char* data, uint32_t sizeOfData, uint8_t crcOfData)
{
    uint8_t crcCalculate = 0;
    uint32_t i;
    //LREP("data length: %d\r\n", sizeOfData);
    crcCalculate = crcBasic((uint8_t*)(data), sizeOfData);
    if(crcOfData != crcCalculate)
    {
        LREP("ERROR: not verify data:\r\n");
        for(i = 0; i < sizeOfData; ++i)
        {
            LREP("%02X, ", data[i]);
        }
        LREP("\r\nCRC:[%02X] [%02X]\r\n", crcOfData, crcCalculate);
        return RET_ERROR;
    }
    return RET_OK;
}

status_t db_readFromInternalFlash(uint8_t *pData, uint32_t addr, uint32_t noByte)
{
    uint32_t i = 0;
    for( i = 0; i < noByte; i++)
    {
        *(pData + i) = *(uint8_t*)(addr + i);
    }
    return RET_OK;
}

status_t db_writeToInternalFlash(uint8_t *pData, uint32_t addr, uint32_t noByte)
{
    uint32_t *pWriteData = (uint32_t*)pData;
    uint32_t u32Data = 0;
    uint32_t i;
    uint32_t length = ((noByte + 3)/4)*4;
    for(i = 0; i < length; i+=4)
    {
        u32Data = *(pWriteData + i/4);
        if(HAL_OK != HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (addr + i), u32Data))
        {
            return RET_ERROR;
        }
    }
    return RET_OK;
}
/************************************************************************
 * Method   : db_InitDataTest
 * FullName : db_InitDataTest
 * Access   : public
 * Note     :
 * @ID      :
 * @param   :
 * @return  : status_t
 ************************************************************************/
status_t db_InitDataTest(void)
{
    status_t ret;
    memset((char*)&g_sysObjectTest, 0x00, sizeof(g_sysObjectTest));
    memset((char*)&g_sysObjectTestToSave, 0x00, sizeof(g_sysObjectTestToSave));

    /* Read data in flash */
    ret = db_readFromInternalFlash((uint8_t*)&g_sysObjectTestToSave, ADDR_SYSTEM_DATA_TEST, sizeof(g_sysObjectTestToSave));
    if(RET_OK != ret)
    {
        LREP("Read data form nand flash fail!\r\n");
        return RET_ERROR;
    }

    /* verify test result */
    ret = db_verifyData((char*)&g_sysObjectTestToSave.u8FlagTestResult,\
                        sizeof(g_sysObjectTestToSave.u8FlagTestResult),\
                        g_sysObjectTestToSave.u8FlagTestResultCRC);
    if(RET_OK != ret)
    {
        return RET_ERROR;
    }
    /* copy data */
    g_sysObjectTest.u8FlagTestResult = g_sysObjectTestToSave.u8FlagTestResult;
    return RET_OK;
}




/************************************************************************
 * Method   : db_InitDataTest
 * FullName : db_InitDataTest
 * Access   : public
 * Note     :
 * @ID      :
 * @param   :
 * @return  : status_t
 ************************************************************************/
static status_t db_sysParamSaveDataTest(void)
{
    FLASH_EraseInitTypeDef  pEraseInit;
    uint32_t SectorError = 0;
    /* please don't memset g_sysParaNeedToSaved to not lose CRC data*/

    //db_copyDataBeforeSave();

//
//    pEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
//    pEraseInit.Sector = FLASH_SECTOR_2;
//    pEraseInit.NbSectors =  1;



    if(HAL_OK == HAL_FLASH_Unlock())
    {
        if( HAL_OK == HAL_FLASHEx_Erase(&pEraseInit, &SectorError))
        {
            /* Write systeam data */
            if(RET_OK != db_writeToInternalFlash((uint8_t*)(&g_sysObjectTestToSave), ADDR_SYSTEM_DATA_TEST, sizeof(g_sysObjectTestToSave)))
            {
                HAL_FLASH_Lock();
                return RET_ERROR;
            }
            HAL_FLASH_Lock();
            return RET_OK;
        }
    }
    HAL_FLASH_Lock();
    LREP("Error Don't erase Flash\r\n");
    return RET_ERROR;
}

/************************************************************************
 * Method   : db_SaveDataTest
 * FullName : db_SaveDataTest
 * Access   : public
 * Note     :
 * @ID      :
 * @param   :
 * @return  : status_t
 ************************************************************************/
status_t db_SaveDataTest(void)
{
    /* save data */
    g_sysObjectTestToSave.u8FlagTestResult = g_sysObjectTest.u8FlagTestResult;

    /* calculate CRC */
    g_sysObjectTestToSave.u8FlagTestResultCRC =  crcBasic((uint8_t*)(&g_sysObjectTestToSave.u8FlagTestResult), sizeof(g_sysObjectTestToSave.u8FlagTestResult));

    /* save to flash */
    return db_sysParamSaveDataTest();
}
