#ifndef __SBUS_PROC_H
#define __SBUS_PROC_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "timers.h"
#include "parameter.h"


void Sbus_Uart6_Task_Proc(void const * argument);
void Sbus_Uart6_IDLE_Proc(uint16_t Size);
void Sbus_Channels_Proc(void);

u16 Sbus_To_Pwm(u16 sbus_value);
float Sbus_To_Range(u16 sbus_value, float p_min, float p_max);




#endif