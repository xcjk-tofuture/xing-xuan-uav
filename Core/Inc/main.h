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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;
typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;
typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */


typedef float vec3_f[3];
typedef float vec2_f[2];
typedef s32 vec3_s32[3];
typedef s32 vec2_s32[2];
typedef s16 vec3_s16[3];
typedef s16 vec2_s16[2];

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
#define FLASH_CS_Pin GPIO_PIN_0
#define FLASH_CS_GPIO_Port GPIOC
#define OLED_CS_Pin GPIO_PIN_4
#define OLED_CS_GPIO_Port GPIOC
#define OLED_DC_Pin GPIO_PIN_5
#define OLED_DC_GPIO_Port GPIOC
#define OLED_RES_Pin GPIO_PIN_1
#define OLED_RES_GPIO_Port GPIOB
#define RGB_R_Pin GPIO_PIN_7
#define RGB_R_GPIO_Port GPIOE
#define RGB_G_Pin GPIO_PIN_8
#define RGB_G_GPIO_Port GPIOE
#define RGB_B_Pin GPIO_PIN_10
#define RGB_B_GPIO_Port GPIOE
#define SPI2_CS0_Pin GPIO_PIN_15
#define SPI2_CS0_GPIO_Port GPIOE
#define SPI2_CS1_Pin GPIO_PIN_8
#define SPI2_CS1_GPIO_Port GPIOD
#define SPI2_CS2_Pin GPIO_PIN_9
#define SPI2_CS2_GPIO_Port GPIOD
#define SPI2_CS3_Pin GPIO_PIN_10
#define SPI2_CS3_GPIO_Port GPIOD
#define INT1_ACCEL_Pin GPIO_PIN_11
#define INT1_ACCEL_GPIO_Port GPIOD
#define INT2_ACCEL_Pin GPIO_PIN_14
#define INT2_ACCEL_GPIO_Port GPIOD
#define SPL3_SlaveMcu_CS_Pin GPIO_PIN_8
#define SPL3_SlaveMcu_CS_GPIO_Port GPIOA
#define KEY2_Pin GPIO_PIN_1
#define KEY2_GPIO_Port GPIOD
#define LED_4_Pin GPIO_PIN_4
#define LED_4_GPIO_Port GPIOD
#define LED_3_Pin GPIO_PIN_7
#define LED_3_GPIO_Port GPIOD
#define BUZZER_Pin GPIO_PIN_4
#define BUZZER_GPIO_Port GPIOB
#define LED_2_Pin GPIO_PIN_0
#define LED_2_GPIO_Port GPIOE
#define LED_1_Pin GPIO_PIN_1
#define LED_1_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
