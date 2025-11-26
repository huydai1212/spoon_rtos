/*
 * flash.c
 *
 *  Created on: Mar 13, 2023
 *      Author: cbb
 */

#include "flash.h"

//static HAL_StatusTypeDef FLASH_PageErase(uint32_t PageAdress);
//
//HAL_StatusTypeDef FLASH_PageErase(uint32_t PageAdress){
//	HAL_StatusTypeDef status = HAL_ERROR;
//	FLASH_EraseInitTypeDef eraseInitStruct;
//
//	eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
//	eraseInitStruct.PageAddress = PageAdress;
//	eraseInitStruct.NbPages = 1;
//
//	uint32_t pageError = 0;
//	status = HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);
//
//	return status;
//}

HAL_StatusTypeDef FLASH_WriteData(uint32_t Address, uint32_t Data){
	HAL_StatusTypeDef status = HAL_ERROR;
//	if(HAL_OK == FLASH_PageErase(Address))
	{
		FLASH_EraseInitTypeDef eraseInitStruct;
		uint32_t pageError = 0;

		eraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
		eraseInitStruct.Page = Address;
		eraseInitStruct.NbPages = 1;

		HAL_FLASH_Unlock();
		status = HAL_FLASHEx_Erase(&eraseInitStruct, &pageError);
		if(status == HAL_OK){
			status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, Data);
		}
		HAL_FLASH_Lock();
	}
	return status;
}

uint32_t FLASH_ReadData(uint32_t Address){
	return (*(__IO uint32_t*)Address);
}

uint32_t u8_to_u32(const uint8_t* bytes) {
  // Every uint32_t consists of 4 bytes, therefore we can shift each uint8_t
  // to an appropriate location.
  // u32   ff  ff  ff  ff
  // u8[]  0   1   2   3
  uint32_t u32 = (bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3];
  return u32;
}

// We pass an output array in the arguments because we can not return arrays
void  u32_to_u8(const uint32_t u32, uint8_t* u8) {
  // To extract each byte, we can mask them using bitwise AND (&)
  // then shift them right to the first byte.
  u8[0] = (u32 & 0xff000000) >> 24;
  u8[1] = (u32 & 0x00ff0000) >> 16;
  u8[2] = (u32 & 0x0000ff00) >> 8;
  u8[3] = u32 & 0x000000ff;
}

int16_t ReadAngleOffset(uint32_t Address){
	uint32_t data = FLASH_ReadData(Address);
	LREP("dataFlash 0x%X\r\n", data);

	uint8_t buff[4] = {0};
	memset(buff, 0x00, 4);
	u32_to_u8(data, buff);
	uint8_t crc = crcBasic(buff, 2);
	LREP("crc 0x%X, 0x%X, 0x%X, 0x%X,0x%X\r\n", crc, buff[0],buff[1],buff[2],buff[3]);
	if(crc == buff[2]){
		uint16_t u16 = ((buff[0] << 24) + (buff[1] << 16)) >> 16;
		return u16;
	}
	return -1;
}

