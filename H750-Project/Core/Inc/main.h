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
#include "stm32h7xx_hal.h"

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
#define Servo_Pin_Pin GPIO_PIN_5
#define Servo_Pin_GPIO_Port GPIOE
#define Right_Motor_Pin GPIO_PIN_4
#define Right_Motor_GPIO_Port GPIOA
#define Left_Motor_Pin GPIO_PIN_5
#define Left_Motor_GPIO_Port GPIOA
#define LED_1_Pin GPIO_PIN_5
#define LED_1_GPIO_Port GPIOC
#define LED_2_Pin GPIO_PIN_0
#define LED_2_GPIO_Port GPIOB
#define LED_3_Pin GPIO_PIN_1
#define LED_3_GPIO_Port GPIOB
#define LED_4_Pin GPIO_PIN_2
#define LED_4_GPIO_Port GPIOB
#define Button_3_Pin GPIO_PIN_2
#define Button_3_GPIO_Port GPIOD
#define Button_4_Pin GPIO_PIN_3
#define Button_4_GPIO_Port GPIOD
#define Button_5_Pin GPIO_PIN_6
#define Button_5_GPIO_Port GPIOD
#define Button_2_Pin GPIO_PIN_4
#define Button_2_GPIO_Port GPIOB
#define Button_1_Pin GPIO_PIN_5
#define Button_1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
