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
#include "stm32f0xx_hal.h"

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
#define RS485_DE_Pin GPIO_PIN_12
#define RS485_DE_GPIO_Port GPIOA

#define GSEN_DE2_LOW	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,GPIO_PIN_RESET)
#define GSEN_DE2_HIGH	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1,GPIO_PIN_SET)

#define	RS485_DE_HIGH		HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET)
#define	RS485_DE_LOW		HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET)

#define DIO_IN1			HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)
#define DIO_IN2			HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2)
#define DIO_IN3			HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12)


#define DO_OUT1_LOW	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET)
#define DO_OUT1_HIGH	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET)

#define DO_OUT2_LOW	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET)
#define DO_OUT2_HIGH	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET)

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
