#include "buzzer_proc.h"






osThreadId BuzzerTaskHandle;




//// ����PWM�����ں�ռ�ձ�
//#define PWM_PERIOD_MS 20


//// ���������ʱ���ľ��
//TimerHandle_t pwm_timer_handle;

// ����PWM��״̬����
//volatile uint8_t pwm_state = 0;

//// ����PWM�Ļص�����
//void pwm_timer_callback(TimerHandle_t xTimer)
//{
//    // ����PWM��״̬�������������ƽ
//    if (pwm_state) {
//        // ����ߵ�ƽ
//			HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_SET);
//        // ...
//    } else {
//        // ����͵�ƽ
//			HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
//        // ...
//    }

//    // �л�PWM��״̬����
//    pwm_state = !pwm_state;
//}

//// ��ʼ��PWM�ĺ���
//void pwm_init(uint16_t frequency, uint8_t duty_cycle)
//{
//    // ����PWM�����ں�ռ�ձȶ�Ӧ��ʱ��
//    uint32_t period_ms = 1000 / frequency;
//    uint32_t on_time_ms = period_ms * duty_cycle / 100;
//    uint32_t off_time_ms = period_ms - on_time_ms;

//    // ���������ʱ��
//    pwm_timer_handle = xTimerCreate("PWM Timer", pdMS_TO_TICKS(period_ms), pdTRUE, NULL, pwm_timer_callback);

//    // ���������ʱ��
//    xTimerStart(pwm_timer_handle, 0);

//    // ����PWM��״̬�����������ƽ
//    pwm_state = 0;
//    // ...
//}

//// �޸�PWM��Ƶ�ʺ�ռ�ձȵĺ���
//void pwm_set(uint16_t frequency, uint8_t duty_cycle)
//{
//    // ֹͣ�����ʱ��
//    xTimerStop(pwm_timer_handle, 0);

//    // ����PWM�����ں�ռ�ձȶ�Ӧ��ʱ��
//    uint32_t period_ms = 1000 / frequency;
//    uint32_t on_time_ms = period_ms * duty_cycle / 100;
//    uint32_t off_time_ms = period_ms - on_time_ms;

//    // �޸������ʱ��������
//    xTimerChangePeriod(pwm_timer_handle, pdMS_TO_TICKS(period_ms), 0);

//    // ����PWM��״̬�����������ƽ
//    pwm_state = 0;
//    // ...

//    // ���������ʱ��
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