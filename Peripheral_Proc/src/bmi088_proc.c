#include "bmi088_proc.h"

/*!***************************************************
 * @brief: �������ö������Ǽ��ٶȼƽ��е�λ����
 * @note:	
 ****************************************************/


//��������ƫ����
mpu BMI088;

/**
  * @brief 	��ȡ�����ǵ�����        
  * @retval  ����ȡ���������ݸ�ֵ��ԭʼ��
	*	@note	 data���ݳ�����3  ԭʼ���ݳ�����3  �ɼ����������ݾ���ԭʼ����
	* @attention ���⿪ʼʵ�ʵ�Ƶ�ʱ�� 2000hz
  */
void BMI088_Read_Gyro_Data(void)
{
    BMI088.gyro.origin [xx] = BMI088.GYRO.data[xx];
    BMI088.gyro.origin [yy] = BMI088.GYRO.data[yy];
    BMI088.gyro.origin [zz] = BMI088.GYRO.data[zz];
}

/**
  * @brief 	��ȡ���ٶȼƵ�����        
  * @retval  ����ȡ���������ݸ�ֵ��ԭʼ��
	*	@note	 data���ݳ�����3  ԭʼ���ݳ�����3  �ɼ����������ݾ���ԭʼ����
	* @attention ���⿪ʼʵ�ʵ�Ƶ�ʱ�� 1600hz
  */
void BMI088_Read_Acc_Data(void)
{
    BMI088.acc.origin [xx] = BMI088.ACC.data[xx];
    BMI088.acc.origin [yy] = BMI088.ACC.data[yy];
    BMI088.acc.origin [zz] = BMI088.ACC.data[zz];
}

/**
  * @brief 	��ȡ�¶�     
  * @retval  ����ȡ���������ݸ�ֵ��ԭʼ��
	*	@note	 data���ݳ�����3  ԭʼ���ݳ�����3  �ɼ����������ݾ���ԭʼ����
	* @attention ���⿪ʼʵ�ʵ�Ƶ�ʱ�� 1600hz
  */
//void BMI088_Read_Tmp_Data(void)
//{
//	BMI088_Read_TMP(&BMI088.Temperature);
//}

/*!***************************************************
 * @file: MYGYROData.c
 * @brief:��ȡ���ٶȣ���ת��Ϊ m/(s^2)
 * @note:	
 ****************************************************/
void IMU_Read(void)
{
	mpu *mympu=&BMI088;
	BMI088_Read_Gyro_Data();	/* ��ȡ���������� */
	BMI088_Read_Acc_Data();		/* ��ȡ���ٶ����� */
	
	mympu->gyro.dps[xx]  = mympu->gyro.origin[xx] * MPU_GYRO_TO_DPS;
	mympu->gyro.dps[yy]  = mympu->gyro.origin[yy] * MPU_GYRO_TO_DPS;
	mympu->gyro.dps[zz]  = mympu->gyro.origin[zz] * MPU_GYRO_TO_DPS;
	
	mympu->acc.m_s_2 [xx] = mympu->acc.origin[xx] * MPU_ACCE_M_S_2;
	mympu->acc.m_s_2 [yy] = mympu->acc.origin[yy] * MPU_ACCE_M_S_2;
	mympu->acc.m_s_2 [zz] = mympu->acc.origin[zz] * MPU_ACCE_M_S_2;
}
