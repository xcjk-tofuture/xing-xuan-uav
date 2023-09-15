#ifndef PID_H
#define PID_H
#include "stm32f4xx.h"

typedef struct
{
	float LastFeedBack;			//�ϴη���
	float LastDifferential;		//΢��ֵ
	float Error,LastError;		//���
	float Hz;					//Ƶ��
	float Integrate;			//����ֵ
    float Differential;			//΢��ֵ
	float FeedBackDifferential;//����΢��
	float   dFilter;
	float Out;
}PID;

typedef struct
{
	float Kp;					//����ϵ��
    float Ki;					//����ϵ��
    float Kd;					//����΢��ϵ��
	 float Kf;					//ǰ��ϵ��
	float ErrorMax;				//����޷�
    float IntegrateMax;			//�����޷�ֵ
	float DifferentialMax;		//�����޷�ֵ
}PID_DATA;

float PID_Control(PID *PID_Controler,PID_DATA * Data,float T,float Feedforward,float Expect,float FeedBack,float dCutFreq);
extern void PID_DataInit(PID_DATA *Data ,float* PID_Data);
void PID_Reset_I(PID *PID_Controler);
float GetControlError(PID *PID_Controler);
float GetControlD(PID *PID_Controler);

#endif

