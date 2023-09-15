

/****************************************************

约定所有变量按小驼峰命名法，函数名为下划线命名法 所有首字母小写 用以区分hal库函数

同时请不要在bsp中添加功能性函数  所有进程  和 功能性实现  保存在  Peripheral_Proc组下

******************************************************/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"

#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"
#include "dma.h"
#include "sbus_proc.h"
#include "led_proc.h"
#include "parameter.h"
#include "SPL06.h"
#include "AK8975.h"
#include "oled_proc.h"
#include "oledfont.h"
#include "w25q32_proc.h"
#include "bmi088reg.h"
/* Private includes ----------------------------------------------------------*/










u8 SbusRxBuf[40];
u8 BleLogRxBuf[40];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
	MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_ADC3_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM8_Init();
  MX_TIM9_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_USB_OTG_FS_HCD_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
	
	
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	
	//spi cs脚初始化
	HAL_GPIO_WritePin(GPIOD,SPI2_CS1_Pin|SPI2_CS2_Pin|SPI2_CS3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE,SPI2_CS0_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, FLASH_CS_Pin|OLED_CS_Pin, GPIO_PIN_SET);
	 OLED_Init();                           //OLED初始
   OLED_Clear();                         //清屏
	W25QXX_Init();                         //FALSH初始化
	//Drv_Spl0601_Init();
	//BMI088_CONF_INIT();
	//BMI088_FLOAT_ACC_GYRO_Init();
	TIM2->ARR = 499;
	TIM2->CCR2 = 299;
	//AK8975CSPin_Init();
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();    
	
	
	HAL_UARTEx_ReceiveToIdle_DMA(&huart6, SbusRxBuf, 40);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, BleLogRxBuf, 40);
	
	
	

/* USER CODE BEGIN 2 */





	//HAL_UART_Receive_IT(&huart6, RxBuf, 25);

  /* Start scheduler */
  osKernelStart();

  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

u8 TxBufTemp[40];
/* USER CODE BEGIN 4 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  
	if(huart->Instance == USART6)
	{
		//printf("1111111111");
		//Sbus_Uart6_IDLE_Proc(Size);
		//printf("length : %d\r\n",Size);
		//sprintf((char *)TxBufTemp, "hello world");
		//HAL_UART_Transmit(&huart3, SbusRxBuf,Size, 50);
		Sbus_Uart6_IDLE_Proc(Size);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart6, SbusRxBuf, 40);
		
	}
	if(huart->Instance == USART3)
	{
		HAL_UART_Transmit(&huart3, BleLogRxBuf, Size, 50);
		HAL_UARTEx_ReceiveToIdle_DMA(&huart3, BleLogRxBuf, 40);		
	}



}
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{



//}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM7) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

