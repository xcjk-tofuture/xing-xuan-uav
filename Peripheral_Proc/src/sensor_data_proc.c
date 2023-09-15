#include "sensor_data_proc.h"
#include "oled_proc.h"

#define RAD_PER_DEG     0.017453293f
#define DEG_PER_RAD     57.29577951f


#define SENSORS_ENABLE_SPL06 1;



//extern mpu BMI088;

u8 SensorError = 0;

osThreadId SensorDataTaskHandle;
u16 FlashTest;
acc_raw_data_t test_acc;
gyro_raw_data_t test_gyro;
float Temp;


Vector3f_t MagTest;


float Pressure;


u8 Bmi088Init_Flag = 1;
u8 AK8975Flag = 1;
u8 SPL06Flag = 1;

void SensorData_Task_Proc(void const * argument)
{

 for(;;)
	{
		ReadAccTemperature(&Temp);
		ReadAccData(&test_acc);
		ReadGyroData(&test_gyro);
		DrvAk8975Read();
	  Mag_Get(&MagTest);
		Spl0601Get(&Pressure);
//		FlashTest = W25QXX_ReadID();
		
    osDelay(10);
	}

}
		

void Sensors_Init()  //传感器初始化
{
	Bmi088Init_Flag = BMI088_INIT();
	AK8975Flag      = DrvAK8975Check();
	#ifdef SENSORS_ENABLE_SPL06
		SPL06Flag				=	Drv_Spl0601_Init();
	#endif

 if(Bmi088Init_Flag || AK8975Flag)
 {
	#ifdef SENSORS_ENABLE_SPL06
		if(SPL06Flag)
				SensorError = 1;
	#endif
				SensorError = 1;
 }
}




#define G_1G_CMPSS 981
//
_imuData_st st_imuData;
_test_st test_st;
/*IMU传感器灵敏度初始化*/
void ImuSensitivityInit(u8 ins_calibrated,vec3_f accRefValue)
{
	//陀螺仪灵敏度换算
	for(u8 i=0;i<3;i++)
	{
		st_imuData.gyrSensitivity[i] = 2000.0f/32767.0f;
	}
	//加速度计灵敏度换算
	if(ins_calibrated==1)
	{
		st_imuData.accSensitivity[0] = G_1G_CMPSS/accRefValue[0];
		st_imuData.accSensitivity[1] = G_1G_CMPSS/accRefValue[1];
		st_imuData.accSensitivity[2] = G_1G_CMPSS/accRefValue[2];
	}
	else
	{
		st_imuData.accSensitivity[0] = 
		st_imuData.accSensitivity[1] = 
		st_imuData.accSensitivity[2] = 0;
	}
}
/*IMU传感器数据获取*/
void ImuDataGet(vec3_s16 gyrRaw,vec3_s16 accRaw)
{
	for(u8 i=0;i<3;i++)
	{
		//
		st_imuData.f_gyrRaw[i] = gyrRaw[i];
		st_imuData.f_accRaw[i] = accRaw[i];
		//		
	}
}
/*IMU温度获取*/
void ImuTemperatureGet(float f_temperature)
{
	st_imuData.f_temperature = f_temperature;
}
/*IMU数据计算处理*/
//void ImuDataCalcu(u8 ins_calibrated,vec3_f gyrOffset,vec3_f accOffset,float IEM[3][3])
//{
//	if(ins_calibrated==1)
//	{
//		//
//		st_imuData.data_sta = 1;
//		//转换单位
//		for(u8 i=0;i<3;i++)
//		{
//			//
//			st_imuData.f_gyr_dps[i]   = (st_imuData.f_gyrRaw[i] - gyrOffset[i])*st_imuData.gyrSensitivity[i];
//			st_imuData.f_acc_cmpss[i] = (st_imuData.f_accRaw[i] - accOffset[i])*st_imuData.accSensitivity[i];
//			//		
//		}
//		//转ANO坐标系
//		Vec3f_Mul_MatrixT(st_imuData.f_gyr_dps,IEM,st_imuData.f_gyr_dps_nb);
//		Vec3f_Mul_MatrixT(st_imuData.f_acc_cmpss,IEM,st_imuData.f_acc_cmpss_nb);
//		//角度转弧度
//		for(u8 i=0;i<3;i++)
//		{
//			st_imuData.f_gyr_radps_nb[i] = RAD_PER_DEG *st_imuData.f_gyr_dps_nb[i];
//		}
//	}
//	else
//	{
//		//
//		st_imuData.data_sta = 0;
//		//
//		for(u8 i=0;i<3;i++)
//		{
//			//未校准，输出原始值
//			st_imuData.f_gyr_dps_nb[i]   = st_imuData.f_gyrRaw[i];
//			st_imuData.f_acc_cmpss_nb[i] = st_imuData.f_accRaw[i];
//		}	
//	}
//}