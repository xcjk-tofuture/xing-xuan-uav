#ifndef __SENSOR_DATA_PROC_H
#define __SENSOR_DATA_PROC_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "timers.h"
#include "parameter.h"
#include "SPL06.h"
#include "AK8975.h"
#include "BMI088.h"
#include "w25q32_proc.h"
//==����



//==����
typedef struct
{
	//
	u8 data_sta;  //0������  1����
	float IEM[3][3];
	//
	vec3_f f_gyrRaw;
	vec3_f f_accRaw;
	//
	vec3_f f_gyr_dps;
	vec3_f f_acc_cmpss;	
	//
	vec3_f f_gyr_dps_nb;
	vec3_f f_gyr_radps_nb;
	vec3_f f_acc_cmpss_nb;		
	//
	float f_temperature;
	vec3_f gyrSensitivity;
	vec3_f accSensitivity;
	//
	
}_imuData_st;

typedef struct
{
	u8 test_u8[4];
	float test[3];
}__attribute__ ((__packed__)) _test_st;
extern _test_st test_st;


//==��������
extern _imuData_st st_imuData;	

//==��������

//static


//public

/*IMU�����������ȳ�ʼ��*/
void ImuSensitivityInit(u8 ins_calibrated,vec3_f accRefValue);
/*IMU���������ݻ�ȡ*/
void ImuDataGet(vec3_s16 gyrRaw,vec3_s16 accRaw);
/*IMU�¶Ȼ�ȡ*/
void ImuTemperatureGet(float f_temperature);
/*IMU���ݼ��㴦��*/
void ImuDataCalcu(u8 ins_calibrated,vec3_f gyrOffset,vec3_f accOffset,float IEM[3][3]);

void SensorData_Task_Proc(void const * argument);

#endif