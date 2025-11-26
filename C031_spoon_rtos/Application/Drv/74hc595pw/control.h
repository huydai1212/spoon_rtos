/*
 * control.h
 *
 *  Created on: Aug 9, 2023
 *      Author: tiennguyen
 */

#ifndef DRV_74HC595PW_CONTROL_H_
#define DRV_74HC595PW_CONTROL_H_

#include "includes.h"

#define STEP 990
#define SPEED_VACUUM_PUMP 400
#define SPEED_UP_SPOON 600
#define SPEED_DOWN_SPOON 1200

status_t controlInit();

status_t enableSM1(uint8_t dir);
status_t disableSM1();

status_t enableSM2(uint8_t dir);
status_t disableSM2();

status_t enableBeam();
status_t disableBeam();

status_t enablePushChopstick();
status_t disablePushChopstick();

status_t beamMoveToOffset(uint32_t timeOut);
status_t spoonMoveToOffset(uint32_t timeOut);
status_t chopstickMoveToOffset(uint32_t timeOut);

status_t proc_push_spoon();
status_t proc_add_spoon();
status_t proc_push_chopsticks();
status_t proc_runTest();

#endif /* DRV_74HC595PW_CONTROL_H_ */
