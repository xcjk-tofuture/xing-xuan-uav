#ifndef __BUZZIER_PROC_H
#define __BUZZIER_PROC_H


#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "timers.h"






void Buzzer_Task_Proc(void const * argument);

//void pwm_timer_callback(TimerHandle_t xTimer);
//void pwm_init(uint16_t frequency, uint8_t duty_cycle);
//void pwm_set(uint16_t frequency, uint8_t duty_cycle);





#endif