/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct pin_s {
	uint16_t pin;
	GPIO_TypeDef* port;
} pin_s;
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
#define Enable_Pin GPIO_PIN_0
#define Enable_GPIO_Port GPIOF
#define RS_Pin GPIO_PIN_1
#define RS_GPIO_Port GPIOF
#define Current_Sense_Pin GPIO_PIN_0
#define Current_Sense_GPIO_Port GPIOA
#define Display_ADC_Pin GPIO_PIN_2
#define Display_ADC_GPIO_Port GPIOA
#define Trigger_Pin GPIO_PIN_4
#define Trigger_GPIO_Port GPIOA
#define D4_Pin GPIO_PIN_5
#define D4_GPIO_Port GPIOA
#define D5_Pin GPIO_PIN_1
#define D5_GPIO_Port GPIOB
#define D6_Pin GPIO_PIN_9
#define D6_GPIO_Port GPIOA
#define D7_Pin GPIO_PIN_10
#define D7_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
