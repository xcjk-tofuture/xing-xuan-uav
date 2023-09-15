#include "uart3_ble_proc.h"



extern UART_HandleTypeDef huart3;

s8 UartTxBuf[40];


osThreadId bleUart3TaskHandle;






void Ble_Uart3_Task_Proc(void const * argument)        //日志处理主进程
{
  /* USER CODE BEGIN Uart_Task_Proc */
  /* Infinite loop */
  for(;;)
  {
    //sprintf((char *)UartTxBuf, "hello world\r\n");
		//HAL_UART_Transmit(&huart3, UartTxBuf, strlen(UartTxBuf), 40);
		//printf("hello world\r\n");
		osDelay(200);
  }
  /* USER CODE END Uart_Task_Proc */
}