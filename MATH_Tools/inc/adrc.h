#ifndef ADRC_H
#define ADRC_H
#include "stm32f4xx.h"

/*���Ź��ȹ���*/
typedef struct
{
	float v1;
	float v2;
	float r0;
	float h0;
	float h;
	uint16_t N;
}ADRC_TD_Def;

/*����״̬�۲���*/
typedef struct
{
	float h;
	uint16_t   N1;
	float beta1;
	float beta2;
	float beta3;
	float zeta;//���Զε����䳤��
	float u;
	float b;
	/* ESO */
	float z1;
	float z2;
	float z3;
	
}ADRC_ESO_Def;

/*ϵͳ״̬������*/
typedef struct
{
	float beta0;//����
	float beta1;//��������ϲ���
	float beta2;//u0=beta_1*e1+beta_2*e2+(beta_0*e0);
	float alpha1;
	float alpha2;
	float zeta;
	
	float c;
	float r1;
	float h1;
	
	float e1;
	float e2;
	float b0;//�Ŷ�����
    float u0;//���������ϵͳ���
	float u;//���Ŷ�����������

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
