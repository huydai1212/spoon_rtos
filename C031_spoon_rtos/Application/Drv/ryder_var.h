/*
 * ryder_var.h
 *
 *  Created on: Mar 23, 2023
 *      Author: cbb
 */
//uart 2: nhan rs485 tu B01 hoac B02
//uart 6: debug
#ifndef INC_RYDER_VAR_H_
#define INC_RYDER_VAR_H_

#include "cmn.h"

#define SPEED_NO ((uint16_t) 1301)
#define PAUSE_NO ((uint16_t) 342)
#define T_ANGLE_NO 151

#define STEPPER 40
#define MIN_SPEED 120
#define MAX_SPEED 1000

#define MAX_T_ANGLE 150


extern uint16_t freq_table[SPEED_NO];
extern uint16_t pause_table[PAUSE_NO];

#endif /* INC_RYDER_VAR_H_ */
