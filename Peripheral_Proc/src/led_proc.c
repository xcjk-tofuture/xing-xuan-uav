#include "led_proc.h"






osThreadId RGBTaskHandle;

u8 ucLed;








void RGB_Task_Proc(void const * argument)           //RGB进程主程序
{
  /* USER CODE BEGIN RGB_Task_Proc */
  /* Infinite loop */
	ucLed = 0x01;
  for(;;)
  {
//    ucLed *= 2;
//		if(ucLed > 0x04) ucLed = 0x01;
//		RGB_Show_Proc(ucLed);
		
		RGB_Show_Proc(0x4);
		osDelay(200);
  }
  /* USER CODE END RGB_Task_Proc */
}



void RGB_Show_Proc(u8 ucled)                      //0b000 后三位赋1 灯亮
{
	
	HAL_GPIO_WritePin(GPIOE, RGB_R_Pin, (ucled & 0x04) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, RGB_G_Pin, (ucled & 0x02) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOE, RGB_B_Pin, (ucled & 0x01) == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET);

};