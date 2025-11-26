/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32c0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA
#define SPOON_IN1_Pin GPIO_PIN_0
#define SPOON_IN1_GPIO_Port GPIOB
#define SPOON_IN1_EXTI_IRQn EXTI0_1_IRQn
#define SPOON_IN2_Pin GPIO_PIN_1
#define SPOON_IN2_GPIO_Port GPIOB
#define SPOON_IN2_EXTI_IRQn EXTI0_1_IRQn
#define SPOON_IN3_Pin GPIO_PIN_2
#define SPOON_IN3_GPIO_Port GPIOB
#define SPOON_IN3_EXTI_IRQn EXTI2_3_IRQn
#define TMP_ALERT_Pin GPIO_PIN_9
#define TMP_ALERT_GPIO_Port GPIOA
#define TMP_ALERT_EXTI_IRQn EXTI4_15_IRQn
#define USER_IN3_Pin GPIO_PIN_10
#define USER_IN3_GPIO_Port GPIOA
#define USER_IN3_EXTI_IRQn EXTI4_15_IRQn
#define nFAULT_SM1_Pin GPIO_PIN_11
#define nFAULT_SM1_GPIO_Port GPIOA
#define nFAULT_SM1_EXTI_IRQn EXTI4_15_IRQn
#define nFAULT_SM2_Pin GPIO_PIN_12
#define nFAULT_SM2_GPIO_Port GPIOA
#define nFAULT_SM2_EXTI_IRQn EXTI4_15_IRQn
#define USER_IN4_Pin GPIO_PIN_15
#define USER_IN4_GPIO_Port GPIOA
#define USER_IN4_EXTI_IRQn EXTI4_15_IRQn
#define SPOON_IN4_Pin GPIO_PIN_3
#define SPOON_IN4_GPIO_Port GPIOB
#define SPOON_IN4_EXTI_IRQn EXTI2_3_IRQn
#define CHOPTICKS_IN1_Pin GPIO_PIN_4
#define CHOPTICKS_IN1_GPIO_Port GPIOB
#define CHOPTICKS_IN1_EXTI_IRQn EXTI4_15_IRQn
#define CHOPTICKS_IN2_Pin GPIO_PIN_5
#define CHOPTICKS_IN2_GPIO_Port GPIOB
#define CHOPTICKS_IN2_EXTI_IRQn EXTI4_15_IRQn
#define USER_IN1_Pin GPIO_PIN_6
#define USER_IN1_GPIO_Port GPIOB
#define USER_IN1_EXTI_IRQn EXTI4_15_IRQn
#define USER_IN2_Pin GPIO_PIN_7
#define USER_IN2_GPIO_Port GPIOB
#define USER_IN2_EXTI_IRQn EXTI4_15_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
