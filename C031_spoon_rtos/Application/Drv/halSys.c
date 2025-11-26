/*
 * halSys.c
 *
 *  Created on: Nov 25, 2020
 *      Author: nguyenquoctien
 */
#include "halSys.h"
#include "usart.h"
#include "mDelay.h"
#include <ringbuffer.h>
#include "tim.h"
//#include "iwdg.h"
#include "spi.h"
#include "math.h"
#include "crc.h"
#include "includes.h"

//((GPIO_TypeDef *) GPIOB_BASE);
typedef struct __uartRecei
{
    uint8_t * data;
    uint16_t size;
    uint16_t index;
    uint32_t tick;

}sUartReceiver;

volatile sSysDiagnostic g_sysDiagnostic = { 0 };

uint8_t                 uartRxBufferPC[PC_RB_ELEMENT_SIZE]      = {0};
sUartReceiver           uartPcRcv = {uartRxBufferPC, PC_RB_ELEMENT_SIZE, 0, 0};
volatile uint8_t        rxBuff_uart1 = 0;

volatile uint32_t maxspeedNo = 80;//547;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == huart1.Instance) /*PC*/
    {
        uartPcRcv.tick = getTick_ms();
        uartPcRcv.data[uartPcRcv.index] = rxBuff_uart1;
        if(((uartPcRcv.data[uartPcRcv.index-1]=='\r') && (uartPcRcv.data[uartPcRcv.index]=='\n')) ||
            (uartPcRcv.index >= uartPcRcv.size - 1)){
            rbPcPush(uartPcRcv.data, uartPcRcv.index + 1);
            uartPcRcv.index = 0;
        }
        else{
            uartPcRcv.index++;
        }
    }
}

/************************************************************************
 * Method   : HAL_GPIO_EXTI_Falling_Callback
 * FullName : HAL_GPIO_EXTI_Falling_Callback
 * Access   : public
 * Note     : GPIO callback
 * @param   : GPIO_Pin
 * @return  : void
 ************************************************************************/
volatile uint32_t timeCurrent4 = 0;
volatile uint32_t timeCurrent1 = 0;
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {
	if(g_sysDiagnostic.initialDone != 1)
		return;

	switch (GPIO_Pin) {
/*	case SPOON_IN1_Pin:
		LREP("SPOON_IN1\r\n");
		g_sysDiagnostic.spoonInt1 = 1;
		break;
	case SPOON_IN2_Pin:
		LREP("SPOON_IN2\r\n");
		g_sysDiagnostic.spoonInt2 = 1;
		break;
	case SPOON_IN3_Pin:
		LREP("SPOON_IN3\r\n");
		g_sysDiagnostic.spoonInt3 = 1;
		break;
	case SPOON_IN4_Pin:
		LREP("SPOON_IN4\r\n");
		g_sysDiagnostic.spoonInt4 = 1;
		break;*/
	case USER_IN1_Pin:
		LREP("USER_IN1\r\n");
		g_sysDiagnostic.userInt1 = 1;
		if(getTick_ms() - timeCurrent1 > 500){
			system_package pkg;
			pkg.e = SYS_PROC_ADD_SPOON;
			sys_proc_regNewEvent(&pkg);
		}
		break;
/*	case USER_IN2_Pin:
		LREP("USER_IN2\r\n");
		g_sysDiagnostic.userInt2 = 1;
		break;
	case USER_IN3_Pin:
		LREP("USER_IN3\r\n");
		g_sysDiagnostic.userInt3 = 1;
		break;*/
	case USER_IN4_Pin:
		LREP("USER_IN4\r\n");
		g_sysDiagnostic.userInt4 = 1;
//		if(getTick_ms() - timeCurrent4 > 500)
		{
			if(g_sysDiagnostic.processStart == 0 && g_sysDiagnostic.enableTest == 0){
				g_sysDiagnostic.processStart = 1;
				system_package pkg;
				pkg.e = SYS_PROC_PUSH_SPOON_AND_CHOPSTICK;
				sys_proc_regNewEvent(&pkg);
			}
		}
		break;
	default:
		break;
	}
}
/************************************************************************
 * Method   : HAL_GPIO_EXTI_Rising_Callback
 * FullName : HAL_GPIO_EXTI_Rising_Callback
 * Access   : public
 * Note     : GPIO callback
 * @param   : GPIO_Pin
 * @return  : void
 ************************************************************************/
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
	switch (GPIO_Pin) {
/*	case SPOON_IN1_Pin:
		g_sysDiagnostic.spoonInt1 = 0;
		break;
	case SPOON_IN2_Pin:
		g_sysDiagnostic.spoonInt2 = 0;
		break;
	case SPOON_IN3_Pin:
		g_sysDiagnostic.spoonInt3 = 0;
		break;
	case SPOON_IN4_Pin:
		g_sysDiagnostic.spoonInt4 = 0;
		break;*/
	case USER_IN1_Pin:
		g_sysDiagnostic.userInt1 = 0;
		timeCurrent1 = getTick_ms();
		break;
/*	case USER_IN2_Pin:
		g_sysDiagnostic.userInt2 = 0;
		break;
	case USER_IN3_Pin:
		g_sysDiagnostic.userInt3 = 0;
		break;*/
	case USER_IN4_Pin:
		g_sysDiagnostic.userInt4 = 0;
		timeCurrent4 = getTick_ms();
		break;
	default:
		break;
	}
}
/************************************************************************
 * Method   : HAL_TIM_PeriodElapsedCallback
 * FullName : HAL_TIM_PeriodElapsedCallback
 * Access   : public
 * Note     : timer callback
 * @param   : htim
 * @return  : void
 ************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM3){
		g_sysDiagnostic.step++;
	}else if(htim->Instance == TIM14){
		HAL_IWDG_Refresh(&hiwdg);
	}
}
/************************************************************************
 * Method   : HAL_TIM_PWM_PulseFinishedCallback
 * FullName : HAL_TIM_PWM_PulseFinishedCallback
 * Access   : public
 * Note     : timer callback
 * @param   : htim
 * @return  : void
 ************************************************************************/
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM3){
		g_sysDiagnostic.stepSpeed++;
		if (g_sysDiagnostic.stepSpeed < g_sysDiagnostic.maxSpeedNo) {
			__HAL_TIM_SET_AUTORELOAD(&htim3, freq_table[g_sysDiagnostic.stepSpeed]);
		}else{
			g_sysDiagnostic.stepSpeed = g_sysDiagnostic.maxSpeedNo;
		}
	}
}
/**/


