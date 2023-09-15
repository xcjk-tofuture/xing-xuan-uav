#include "motor_proc.h"




osThreadId MotorTaskHandle;



//// 定义PWM的周期和占空比
//#define PWM_PERIOD_MS 20


//// 定义软件定时器的句柄
//TimerHandle_t pwm_timer_handle;

//// 定义PWM的状态变量
//volatile uint8_t pwm_state = 0;

//// 定义PWM的回调函数
//void pwm_timer_callback(TimerHandle_t xTimer)
//{
//    // 根据PWM的状态变量设置输出电平
//    if (pwm_state) {
//        // 输出高电平
//			HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
//			HAL_GPIO_WritePin(PWM_SIM_OUT1_GPIO_Port, PWM_SIM_OUT1_Pin, GPIO_PIN_SET);
//			HAL_GPIO_WritePin(PWM_SIM_OUT2_GPIO_Port, PWM_SIM_OUT2_Pin, GPIO_PIN_SET);
//        // ...
//    } else {
//        // 输出低电平
//			HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(PWM_SIM_OUT1_GPIO_Port, PWM_SIM_OUT1_Pin, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(PWM_SIM_OUT2_GPIO_Port, PWM_SIM_OUT2_Pin, GPIO_PIN_RESET);
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


void Motor_Task_Proc(void const * argument)
{
	
  for(;;)
  {
		
    osDelay(200);
  }	
	
}