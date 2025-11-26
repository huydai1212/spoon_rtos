/*
 * 74hc595pw.h
 *
 *  Created on: Aug 7, 2023
 *      Author: tienp
 */

#ifndef DRV_74HC595PW_74HC595PW_H_
#define DRV_74HC595PW_74HC595PW_H_

#include "includes.h"


#define SPI_NCS(x)  HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, x)

status_t spi_transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout);

#endif /* DRV_74HC595PW_74HC595PW_H_ */
