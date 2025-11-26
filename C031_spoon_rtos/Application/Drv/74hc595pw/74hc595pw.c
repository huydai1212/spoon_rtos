/*
 * 74hc595pw.c
 *
 *  Created on: Aug 7, 2023
 *      Author: tienp
 */


#include "74hc595pw.h"

extern SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef * pSpi = &hspi1;

status_t spi_transmit(uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	SPI_NCS(GPIO_PIN_SET);
	HAL_StatusTypeDef stt = HAL_SPI_Transmit(pSpi, pData , Size , Timeout);
	if(HAL_OK != stt)
	{
		SPI_NCS(GPIO_PIN_RESET);
		LREP("write 74hc error\r\n");
		return RET_ERROR;
	}

	SPI_NCS(GPIO_PIN_RESET);
	HAL_Delay(10);
	SPI_NCS(GPIO_PIN_SET);
	return RET_OK;
}

