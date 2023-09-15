#ifndef __BMI088_PROC_H
#define __BMI088_PROC_H



#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "timers.h"
#include "BMI088.h"
/*!***************************************************
 * @file: MYGYROData.c
 * @brief: �������ö������Ǽ��ٶȼƽ������ݵ�λ����
 * @note:	
 ****************************************************/
#define BMI088 BMI088
#define xx 0
#define yy 1
#define zz 2

#define G_Z 0

/*�����Ǽ��ٶȼ�ȥ��Ư��ʽѡ��*/

#define MPU_ACC_CALOFFSET_AUTO
#define  MPU_GROY_CALOFFSET_AUTO
/*---------------------------------------------*/

#define XAISN   (3)  //3������
#define ITEMS  6
#define MPU9250_FILTER_NUM   4
#define MPU_CORRECTION_FLASH     0x0800F000        //�洢У�����ݵ�FLASH��ַ��SIZE=6*3*4�ֽ�
#define GG 9.8															// ��������?
/* ���������趨Ϊ��6g */
#define MPU_ACCE_M_S_2 (24.0 * GG / 32768.0) //��λ���㣬��LSB��Ϊm/(s^2)��GGǰ���"X"����������޸�
#define MPU_GYRO_TO_DPS (2000 / 32768.0)    //��λ���㣬��LSB��Ϊ gegree/s
#define MPU_MAGN_TO_GS (4800 / 16384.0 / 100.0)            //��λ���㣬��LSB��ΪGs
#define MPU_MAGN_TO_UT (4800 / 16384.0)            //��λ���㣬��GS��ΪUT
#define MPU_TEMP_K (0.002995177763f)            //degC/LSB

typedef struct _accdata
{
    short origin[XAISN];  //ԭʼֵ
    float offset[XAISN];      //��ƫֵ
    float offset_max[XAISN];  //��ƫֵ���ֵ
    float offset_min[XAISN];  //��ƫֵ��Сֵ
    float calibration[XAISN]; //У׼ֵ
    float filter[XAISN];      //����ƽ���˲�ֵ
    float m_s_2[XAISN];      //��ÿ���η���
} accdata;
typedef struct _gyrodata
{
    short origin[XAISN];  //ԭʼֵ
    float offset_max[XAISN];  //��ƫֵ���ֵ
    float offset_min[XAISN];  //��ƫֵ��Сֵ
    float offset[XAISN];      //��ƫֵ
    float calibration[XAISN]; //У׼ֵ
    float filter[XAISN];      //����ƽ���˲�ֵ
    float dps[XAISN];         //��ÿ��
    float radps[XAISN];       //����ÿ��
		/* 2022-01-11 ���뻬���˲� */
		float last_filter[XAISN];
} gyrodata;

struct _mpu
{
    accdata acc;
    gyrodata gyro;

    float Temperature;
    uint8_t acc_id, gyro_id;
    union
    {
        int16_t data[3];
        uint8_t buff[6];
    } ACC, GYRO;
    uint8_t temp_originalbuff[2];
    uint8_t gyro_times;
    uint8_t acc_times;
    enum
    {
        ReadingACC,
        ReadingGYRO,
        IDLE,
    } state;
		
    float pitch;
    float roll;
    float yaw;
    float lastyaw;
		/* ��¼�������ۼ���ת���ٶ� */
		float yawsum;
		/* �����Ƕ�̬���ݽ��� */
		uint8_t DynamicOffsetEnable;	// �Ƿ���ж�̬У׼��־λ
		int32_t DynamicTmp[3];				// ��̬�����ۼ�
		uint32_t DynamicTimes;				// У׼ʱ�����
		uint16_t OffsetCnt;						// У׼����
		uint16_t OffsetErrorRange;		// У׼��Χ
		float gyro_z;
		int16_t yaw_turns;
		
		uint8_t Temp_OK;
};

typedef struct _mpu mpu;
extern mpu BMI088;

/**
  * @brief 	��ȡ�����ǵ�����        
  * @retval  ����ȡ���������ݸ�ֵ��ԭʼ��
  *	@note	 data���ݳ�����3  ԭʼ���ݳ�����3  �ɼ����������ݾ���ԭʼ����
  * @attention ���⿪ʼʵ�ʵ�Ƶ�ʱ�� 2000hz
  */
void BMI088_Read_Gyro_Data(void);

/**
  * @brief 	��ȡ���ٶȼƵ�����        
  * @retval  ����ȡ���������ݸ�ֵ��ԭʼ��
  *	@note	 data���ݳ�����3  ԭʼ���ݳ�����3  �ɼ����������ݾ���ԭʼ����
  * @attention ���⿪ʼʵ�ʵ�Ƶ�ʱ�� 1600hz
  */
void BMI088_Read_Acc_Data(void);

/**
  * @brief 	��ȡ�¶�     
  * @retval  ����ȡ���������ݸ�ֵ��ԭʼ��
  *	@note	 data���ݳ�����3  ԭʼ���ݳ�����3  �ɼ����������ݾ���ԭʼ����
  * @attention ���⿪ʼʵ�ʵ�Ƶ�ʱ�� 1600hz
  */
void BMI088_Read_Tmp_Data(void);

/*!***************************************************
 * @file: MYGYROData.c
 * @brief:��ȡ���ٶȣ���ת��Ϊ m/(s^2)
 * @note:	
 ****************************************************/
void IMU_Read(void);








#endif