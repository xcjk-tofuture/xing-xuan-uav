#ifndef ADRC_H
#define ADRC_H
#include "stm32f4xx.h"

/*安排过度过程*/
typedef struct
{
	float v1;
	float v2;
	float r0;
	float h0;
	float h;
	uint16_t N;
}ADRC_TD_Def;

/*扩张状态观测器*/
typedef struct
{
	float h;
	uint16_t   N1;
	float beta1;
	float beta2;
	float beta3;
	float zeta;//线性段的区间长度
	float u;
	float b;
	/* ESO */
	float z1;
	float z2;
	float z3;
	
}ADRC_ESO_Def;

/*系统状态误差反馈率*/
typedef struct
{
	float beta0;//线性
	float beta1;//非线性组合参数
	float beta2;//u0=beta_1*e1+beta_2*e2+(beta_0*e0);
	float alpha1;
	float alpha2;
	float zeta;
	
	float c;
	float r1;
	float h1;
	
	float e1;
	float e2;
	float b0;//扰动补偿
    float u0;//非线性组合系统输出
	float u;//带扰动补偿后的输出

}ADRC_NLSEF_Def;

typedef struct
{
	ADRC_TD_Def TD;
	ADRC_ESO_Def ESO;
	ADRC_NLSEF_Def NLSEF;

}ADRC_Def;

float ADRC_Fhan(float v1, float v2, float r0, float h0);
int16_t Sign_ADRC(float Input);
void ADRC_TD(ADRC_TD_Def*TD,float ADRC_Expect);
void ADRC_ESO(ADRC_ESO_Def*ESO,float y);
void ADRC_NLSEF(ADRC_NLSEF_Def* NELSEF,ADRC_ESO_Def*ESO,ADRC_TD_Def*TD);
float ADRC_Fal(float e, float alpha, float delta);
void ADRC_Init(ADRC_Def*ADRC ,float *ADRC_Data);
float ADRC_Control(ADRC_Def*ADRC,float Expect,float FeedBack ,float T);

#endif
