#ifndef __BMI088_H
#define __BMI088_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "timers.h"
#include "parameter.h"
#include "spi.h"
#include "bmi088_proc.h"


typedef struct 
{
	s16 s16_acc[3];
	s16 s16_gyr[3];
	
}__attribute__ ((__packed__)) _bmi_s16_st;// __packed 

typedef union 
{
	u8 u8_buffer[12];
	_bmi_s16_st st_data;
}_bmi_data_un;

typedef struct
{
	_bmi_data_un un_ins;
	s16 temperature_100;

}_bmi_data_st;
//
extern _bmi_data_st st_bmi_data;
//
void DrvGpioSenser088CsPinInit(void);
u8 DrvBmi088Init(void);
void DrvBmi088ReadServices(void);
void DrvBmi088TemperatureRead(void);
void DrvBmi088AccelerationRead(void);
void DrvGpioCsPinCtrlBmi088Gyr(u8 ena);
void DrvGpioCsPinCtrlBmi088Acc(u8 ena);
u8 Drv_SPI2_RW(u8 dat);
void Drv_SPI2_Receive(uint8_t *pData, uint16_t Size);
#endif






