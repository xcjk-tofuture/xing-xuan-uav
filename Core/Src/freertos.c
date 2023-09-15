/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define portGET_RUN_TIME_COUNTER_VALUE()	FreeRTOSRunTimeTicks
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern osThreadId RGBTaskHandle;
extern osThreadId bleUart3TaskHandle;
extern osThreadId BuzzerTaskHandle;
extern osThreadId MotorTaskHandle;
extern osThreadId SbusTaskHandle;
extern osThreadId W25q32TaskHandle;
extern osThreadId SensorDataTaskHandle;
extern osThreadId OLEDTaskHandle;
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

osThreadId KeyTaskHandle;
osThreadId PWMTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

extern void RGB_Task_Proc(void const * argument);
void Key_Task_Proc(void const * argument);
void PWM_Task_Proc(void const * argument);
extern void Buzzer_Task_Proc(void const * argument);
extern void Ble_Uart3_Task_Proc(void const * argument);
extern void Motor_Task_Proc(void const * argument);
extern void Sbus_Uart6_Task_Proc(void const * argument);
extern void W25q32_Task_Proc(void const * argument);
extern void SensorData_Task_Proc(void const * argument);
extern void OLED_Task_Proc(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
	
//飞机进程初始化	
	printf("rtos loading...........\r\n");
	printf("\\   |   /\r\n");
	printf("-  xcjk  -\r\n");
	printf("/    |   \\\r\n");

  /* definition and creation of RGBTask */      //RGB进程
  osThreadDef(RGBTask, RGB_Task_Proc, osPriorityIdle, 0, 128);
  RGBTaskHandle = osThreadCreate(osThread(RGBTask), NULL);
	if(RGBTaskHandle != NULL)   printf("rgb_task_creat_sucess!\r\n");    //指示打印是否正常
  /* definition and creation of KeyTask */			//按键进程
  osThreadDef(KeyTask, Key_Task_Proc, osPriorityIdle, 0, 128);
  KeyTaskHandle = osThreadCreate(osThread(KeyTask), NULL);
	if(KeyTaskHandle != NULL)   printf("key_task_creat_sucess!\r\n");
  /* definition and creation of PWMTask */      //无人机电机进程
  osThreadDef(PWMTask, PWM_Task_Proc, osPriorityIdle, 0, 128);
  PWMTaskHandle = osThreadCreate(osThread(PWMTask), NULL);
	if(PWMTaskHandle != NULL)   printf("pwm_task_creat_sucess!\r\n");
  /* definition and creation of BuzzierTask */ //蜂鸣器进程
  osThreadDef(BuzzierTask, Buzzer_Task_Proc, osPriorityIdle, 0, 128);
  BuzzerTaskHandle = osThreadCreate(osThread(BuzzierTask), NULL);
	if(BuzzerTaskHandle != NULL)   printf("buzzier_task_creat_sucess!\r\n");
	

  /* definition and creation of UartTask */    //蓝牙进程
  osThreadDef(LogTask, Ble_Uart3_Task_Proc, osPriorityIdle, 0, 128);
  bleUart3TaskHandle = osThreadCreate(osThread(LogTask), NULL);
	if(bleUart3TaskHandle != NULL)   printf("ble_task_creat_sucess!\r\n");
	
																							//sbus 进程
	osThreadDef(SbusTask, Sbus_Uart6_Task_Proc, osPriorityIdle, 0, 128);
  SbusTaskHandle = osThreadCreate(osThread(SbusTask), NULL);
	if(SbusTaskHandle != NULL)   printf("sbus_task_creat_sucess!\r\n");
	
	
																							//小车有刷电机进程
	osThreadDef(MotorTask, Motor_Task_Proc, osPriorityIdle, 0, 128);
  MotorTaskHandle = osThreadCreate(osThread(MotorTask), NULL);
	if(MotorTaskHandle != NULL)   printf("w25q32_task_creat_sucess!\r\n");
					
					
																							//flash数据处理进程
	osThreadDef(W25q32Task, W25q32_Task_Proc, osPriorityIdle, 0, 128);
  W25q32TaskHandle = osThreadCreate(osThread(W25q32Task), NULL);
	if(W25q32TaskHandle != NULL)   printf("motor_task_creat_sucess!\r\n");


																							//传感器数据处理进程
	osThreadDef(SensorDataTask, SensorData_Task_Proc, osPriorityIdle, 0, 256);
  SensorDataTaskHandle = osThreadCreate(osThread(SensorDataTask), NULL);
	if(SensorDataTaskHandle != NULL)   printf("motor_task_creat_sucess!\r\n");
	
	                                            //OLED处理进程
	osThreadDef(OLEDTask, OLED_Task_Proc, osPriorityIdle, 0, 256);
  OLEDTaskHandle = osThreadCreate(osThread(OLEDTask), NULL);
	if(OLEDTaskHandle != NULL)   printf("OLED_task_creat_sucess!\r\n");

}


void Key_Task_Proc(void const * argument)
{
  /* USER CODE BEGIN Key_Task_Proc */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Key_Task_Proc */
}

/* USER CODE BEGIN Header_PWM_Task_Proc */
/**
* @brief Function implementing the PWMTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_PWM_Task_Proc */
void PWM_Task_Proc(void const * argument)
{
  /* USER CODE BEGIN PWM_Task_Proc */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END PWM_Task_Proc */
}



/* USER CODE BEGIN Header_Uart_Task_Proc */
/**
* @brief Function implementing the UartTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Uart_Task_Proc */


/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
