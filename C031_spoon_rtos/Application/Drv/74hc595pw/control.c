/*
 * control.c
 *
 *  Created on: Aug 9, 2023
 *      Author: tiennguyen
 */
#include "includes.h"

uint8_t data[3] = {0x00, 0x00, 0x00};
/*
 *
 * */
status_t controlInit(){
	status_t ret = RET_ERROR;
	ret = spi_transmit(data, 3, 1000);
	return ret;
}
/*
 *
 * */

status_t enableSM1(uint8_t dir){
	status_t ret = RET_ERROR;
//	uint8_t temp = 0b01100000 | dir << 7;
	data[1] = (dir == 1) ? 0b11100000 : 0b01100000;
	ret = spi_transmit(data, 3, 1000);
	return ret;
}

/*
 *
 * */

status_t disableSM1(){
	status_t ret = RET_ERROR;
	uint8_t temp = 0b00001111;
	data[1] &= temp;
	ret = spi_transmit(data, 3, 1000);
	return ret;
}
/*
 *
 * */

status_t enableSM2(uint8_t dir){
	status_t ret = RET_ERROR;
//	uint8_t temp = 0b00000110 | dir << 3;
	data[1] = (dir == 1) ? 0b00001110 : 0b00000110;
	ret = spi_transmit(data, 3, 1000);
	return ret;
}

/*
 *
 * */

status_t disableSM2(){
	status_t ret = RET_ERROR;
	uint8_t temp = 0b11110000;
	data[1] &= temp;
	ret = spi_transmit(data, 3, 1000);
	return ret;
}

/*
 *
 * */

status_t enableBeam(){
	status_t ret = RET_ERROR;
	uint8_t temp = 0b00000001;
	data[0] |= temp;
	ret = spi_transmit(data, 3, 1000);
	return ret;
}

/*
 *
 * */

status_t disableBeam(){
	status_t ret = RET_ERROR;
	uint8_t temp = 0b11111100;
	data[0] &= temp;
	ret = spi_transmit(data, 3, 1000);
	return ret;
}

/*
 *
 * */

status_t enablePushChopstick(){
	status_t ret = RET_ERROR;
	uint8_t temp = 0b01000000;
	data[0] |= temp;
	ret = spi_transmit(data, 3, 1000);
	return ret;
}

/*
 *
 * */

status_t disablePushChopstick(){
	status_t ret = RET_ERROR;
	uint8_t temp = 0b00111111;
	data[0] &= temp;
	ret = spi_transmit(data, 3, 1000);
	return ret;
}

/*
 *
 * */
status_t beamMoveToOffset(uint32_t timeOut){
	if(HAL_GPIO_ReadPin(GPIOB, SPOON_IN1_Pin) == 0)
		return RET_OK;
	g_sysDiagnostic.maxSpeedNo = SPEED_VACUUM_PUMP;
//	  HAL_TIM_Base_Start_IT(&htim3);
	enableSM1(0);
	g_sysDiagnostic.stepSpeed = 0;
	__HAL_TIM_SET_AUTORELOAD(&htim3, freq_table[g_sysDiagnostic.stepSpeed]);
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
	uint32_t time = getTick_ms();
	do{

	}while(HAL_GPIO_ReadPin(GPIOB, SPOON_IN1_Pin) != 0 && abs(getTick_ms() - time) < timeOut);

	HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
	disableSM1();
//	HAL_Delay(100);
	return RET_OK;
}

/*
 *
 * */

status_t spoonMoveToOffset(uint32_t timeOut){
	if(HAL_GPIO_ReadPin(GPIOB, SPOON_IN3_Pin) == 0){
		LREP("out of spoons\r\n");
//		proc_add_spoon();
		return RET_ERROR;
	}
	if(HAL_GPIO_ReadPin(GPIOB, SPOON_IN2_Pin) == 0){
		return RET_OK;
	}

	g_sysDiagnostic.maxSpeedNo = SPEED_UP_SPOON;
//	  HAL_TIM_Base_Start_IT(&htim3);
	enableSM2(1);
	g_sysDiagnostic.stepSpeed = 0;
	__HAL_TIM_SET_AUTORELOAD(&htim3, freq_table[g_sysDiagnostic.stepSpeed]);
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
	uint32_t time = getTick_ms();
	do{
		if(HAL_GPIO_ReadPin(GPIOB, SPOON_IN3_Pin) == 0){
			LREP("out of spoons\r\n");
			break;
		}
	}while(HAL_GPIO_ReadPin(GPIOB, SPOON_IN2_Pin) != 0 && abs(getTick_ms() - time) < timeOut);

	HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
	disableSM2();
//	HAL_Delay(100);
	return RET_OK;
}
/*
 *
 * */
status_t chopstickMoveToOffset(uint32_t timeOut){
	if(HAL_GPIO_ReadPin(GPIOB, CHOPTICKS_IN2_Pin) == 0){
		return RET_OK;
	}
	enablePushChopstick();
	uint32_t time = getTick_ms();
	do{

	}while(HAL_GPIO_ReadPin(GPIOB, CHOPTICKS_IN2_Pin) != 0 && abs(getTick_ms() - time) < timeOut);
	disablePushChopstick();
	return RET_OK;
}
/*
 *
 * */
status_t proc_push_spoon(){
	uint8_t beamOn = 0;
	beamMoveToOffset(10000);
	if(RET_ERROR == spoonMoveToOffset(150000)){
		return RET_ERROR;
	}

	enableSM1(1);
	g_sysDiagnostic.stepSpeed = 0;
	g_sysDiagnostic.step = 0;
	g_sysDiagnostic.maxSpeedNo = SPEED_VACUUM_PUMP;
	HAL_TIM_Base_Start_IT(&htim3);
	__HAL_TIM_SET_AUTORELOAD(&htim3, freq_table[g_sysDiagnostic.stepSpeed]);
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);

	uint32_t time = getTick_ms();
	do{
		if(beamOn == 0 && g_sysDiagnostic.step > (STEP*0.6)){
			beamOn = 1;
			enableBeam();
		}
	}while(g_sysDiagnostic.step <= STEP && abs(getTick_ms() - time) < 5000);
	if(abs(getTick_ms() - time) >= 5000){
		LREP("timeout\r\n");
	}
	HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
	HAL_TIM_Base_Stop_IT(&htim3);

	time = getTick_ms();
	do{

	}while(abs(getTick_ms() - time) < 200);
	beamMoveToOffset(10000);
	disableBeam();
	spoonMoveToOffset(10000);

	return RET_OK;
}
/*
 *
 * */
status_t proc_add_spoon(){
	if(HAL_GPIO_ReadPin(GPIOB, SPOON_IN4_Pin) == 0)
		return RET_OK;

	g_sysDiagnostic.maxSpeedNo = SPEED_DOWN_SPOON;
	enableSM2(0);
	g_sysDiagnostic.stepSpeed = 0;
	__HAL_TIM_SET_AUTORELOAD(&htim3, freq_table[g_sysDiagnostic.stepSpeed]);
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
	uint32_t time = getTick_ms();
	do{
	}while(HAL_GPIO_ReadPin(GPIOB, SPOON_IN4_Pin) != 0 && abs(getTick_ms() - time) < 100000);
	if(abs(getTick_ms() - time) >= 100000){
		LREP("timeout\r\n");
	}
	HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
	disableSM2();
	return RET_OK;
}
/*
 *
 * */
status_t proc_push_chopsticks(){
	if(HAL_GPIO_ReadPin(GPIOB, CHOPTICKS_IN1_Pin) == 1){
		LREP("out of chopsticks\r\n");
		return RET_ERROR;
	}
	chopstickMoveToOffset(10000);
	enablePushChopstick();
	uint32_t time = getTick_ms();
	do{
	}while(HAL_GPIO_ReadPin(GPIOB, CHOPTICKS_IN2_Pin) == 0 && abs(getTick_ms() - time) < 10000);

	time = getTick_ms();
	do{
		}while(HAL_GPIO_ReadPin(GPIOB, CHOPTICKS_IN2_Pin) != 0 && abs(getTick_ms() - time) < 10000);
	HAL_Delay(100);
	disablePushChopstick();
	return RET_OK;
}
/*
 *
 * */

status_t spoonMoveUpDownToTest(uint32_t timeOut){
	static uint8_t tonggle = 0;
	tonggle = !tonggle;
	g_sysDiagnostic.maxSpeedNo = SPEED_UP_SPOON;

	enableSM2(tonggle);
	g_sysDiagnostic.stepSpeed = 0;
	__HAL_TIM_SET_AUTORELOAD(&htim3, freq_table[g_sysDiagnostic.stepSpeed]);
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
	uint32_t time = getTick_ms();
	do{
		if(HAL_GPIO_ReadPin(GPIOB, SPOON_IN3_Pin) == 0){
			LREP("out of spoons\r\n");
			break;
		}
	}while(abs(getTick_ms() - time) < timeOut);

	HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
	disableSM2();
	return RET_OK;
}
/*
 *
 * */
status_t proc_runTest(){
	/*test proc_push_chopsticks*/
	chopstickMoveToOffset(10000);
	enablePushChopstick();
	uint32_t time = getTick_ms();
	do{
	}while(HAL_GPIO_ReadPin(GPIOB, CHOPTICKS_IN2_Pin) == 0 && abs(getTick_ms() - time) < 10000);

	time = getTick_ms();
	do{
		}while(HAL_GPIO_ReadPin(GPIOB, CHOPTICKS_IN2_Pin) != 0 && abs(getTick_ms() - time) < 10000);
	HAL_Delay(100);
	disablePushChopstick();

	/*test proc_push_spoon*/
	uint8_t beamOn = 0;
	beamMoveToOffset(10000);
/*	if(RET_ERROR == spoonMoveToOffset(150000)){
		return RET_ERROR;
	}*/

	enableSM1(1);
	g_sysDiagnostic.stepSpeed = 0;
	g_sysDiagnostic.step = 0;
	g_sysDiagnostic.maxSpeedNo = SPEED_VACUUM_PUMP;
	HAL_TIM_Base_Start_IT(&htim3);
	__HAL_TIM_SET_AUTORELOAD(&htim3, freq_table[g_sysDiagnostic.stepSpeed]);
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);

	time = getTick_ms();
	do{
		if(beamOn == 0 && g_sysDiagnostic.step > (STEP*0.6)){
			beamOn = 1;
			enableBeam();
		}
	}while(g_sysDiagnostic.step <= STEP && abs(getTick_ms() - time) < 5000);

	HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
	HAL_TIM_Base_Stop_IT(&htim3);

	time = getTick_ms();
	do{

	}while(abs(getTick_ms() - time) < 200);
	beamMoveToOffset(10000);
	disableBeam();

	spoonMoveUpDownToTest(500);
	return RET_OK;
}

/*---END---*/
