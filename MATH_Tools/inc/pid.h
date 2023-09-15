#ifndef PID_H
#define PID_H
#include "stm32f4xx.h"

typedef struct
{
	float LastFeedBack;			//上次反馈
	float LastDifferential;		//微分值
	float Error,LastError;		//误差
	float Hz;					//频率
	float Integrate;			//积分值
    float Differential;			//微分值
	float FeedBackDifferential;//反馈微分
	float   dFilter;
	float Out;
}PID;

typedef struct
{
	float Kp;					//比例系数
    float Ki;					//积分系数
    float Kd;					//反馈微分系数
	 float Kf;					//前馈系数
	float ErrorMax;				//误差限幅
    float IntegrateMax;			//积分限幅值
	float DifferentialMax;		//积分限幅值
}PID_DATA;

float PID_Control(PID *PID_Controler,PID_DATA * Data,float T,float Feedforward,float Expect,float FeedBack,float dCutFreq);
extern void PID_DataInit(PID_DATA *Data ,float* PID_Data);
void PID_Reset_I(PID *PID_Controler);
float GetControlError(PID *PID_Controler);
float GetControlD(PID *PID_Controler);

#endif

