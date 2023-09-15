#include "buzzer_proc.h"






osThreadId BuzzerTaskHandle;




//// 定义PWM的周期和占空比
//#define PWM_PERIOD_MS 20


//// 定义软件定时器的句柄
//TimerHandle_t pwm_timer_handle;

// 定义PWM的状态变量
//volatile uint8_t pwm_state = 0;

//// 定义PWM的回调函数
//void pwm_timer_callback(TimerHandle_t xTimer)
//{
//    // 根据PWM的状态变量设置输出电平
//    if (pwm_state) {
//        // 输出高电平
//			HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
//        // ...
//    } else {
//        // 输出低电平
//			HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
//        // ...
//    }

//    // 切换PWM的状态变量
//    pwm_state = !pwm_state;
//}

//// 初始化PWM的函数
//void pwm_init(uint16_t frequency, uint8_t duty_cycle)
//{
//    // 计算PWM的周期和占空比对应的时间
//    uint32_t period_ms = 1000 / frequency;
//    uint32_t on_time_ms = period_ms * duty_cycle / 100;
//    uint32_t off_time_ms = period_ms - on_time_ms;

//    // 创建软件定时器
//    pwm_timer_handle = xTimerCreate("PWM Timer", pdMS_TO_TICKS(period_ms), pdTRUE, NULL, pwm_timer_callback);

//    // 启动软件定时器
//    xTimerStart(pwm_timer_handle, 0);

//    // 设置PWM的状态变量和输出电平
//    pwm_state = 0;
//    // ...
//}

//// 修改PWM的频率和占空比的函数
//void pwm_set(uint16_t frequency, uint8_t duty_cycle)
//{
//    // 停止软件定时器
//    xTimerStop(pwm_timer_handle, 0);

//    // 计算PWM的周期和占空比对应的时间
//    uint32_t period_ms = 1000 / frequency;
//    uint32_t on_time_ms = period_ms * duty_cycle / 100;
//    uint32_t off_time_ms = period_ms - on_time_ms;

//    // 修改软件定时器的周期
//    xTimerChangePeriod(pwm_timer_handle, pdMS_TO_TICKS(period_ms), 0);

//    // 设置PWM的状态变量和输出电平
//    pwm_state = 0;
//    // ...

//    // 启动软件定时器
//    xTimerStart(pwm_timer_handle, 0);
//}








/* USER CODE BEGIN Header_Buzzier_Task_Proc */
/**
* @brief Function implementing the BuzzierTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Buzzier_Task_Proc */
void Buzzer_Task_Proc(void const * argument)
{
  /* USER CODE BEGIN Buzzier_Task_Proc */
  /* Infinite loop */
	//pwm_init(1000, 50);
	
  for(;;)
  {
		
		
    osDelay(1000);
  }
  /* USER CODE END Buzzier_Task_Proc */
}