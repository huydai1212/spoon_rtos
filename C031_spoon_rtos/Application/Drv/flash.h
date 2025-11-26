/*
 * flash.h
 *
 *  Created on: Mar 13, 2023
 *      Author: cbb
 */

#ifndef DRV_FLASH_H_
#define DRV_FLASH_H_
#include "halSys.h"
#include "main.h"
#include "crc.h"


#define EEPROM_ADDRESS ((uint32_t) 0x0803F800)

HAL_StatusTypeDef FLASH_WriteData(uint32_t Address, uint32_t Data);
uint32_t FLASH_ReadData(uint32_t Address);
int16_t ReadAngleOffset(uint32_t Address);

#endif /* DRV_FLASH_H_ */
