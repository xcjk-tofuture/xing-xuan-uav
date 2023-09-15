#ifndef __FILTER_H
#define __FILTER_H
#include "stm32f4xx.h"

typedef struct
{
 float InputButter[3];
 float OutputButter[3];
}ButterBufferData_t;

typedef struct
{
  float a[3];
  float b[3];
}ButterParameter_t;

typedef struct
{
	float in_est;    //Estimator
	float in_obs;    //Observation
	float fix_ki;
	float ei_limit;
	float e;
	float ei;
	float out;
}InteFixFilter_t;

typedef struct
{
	float in_est_d;   //Estimator
	float in_obs;    //Observation
	float fix_kp;
	float e_limit;
	float e;
	float out;
	
}FixInteFilter_t;

extern ButterParameter_t Butter_5HZ_Parameter_Acce;
extern ButterParameter_t Butter_80HZ_Parameter_Acce;

float LPButterworth(float curr_input,ButterBufferData_t *Buffer,ButterParameter_t *Parameter);
void FixInteFilter(float dT,FixInteFilter_t *data);
void InteFixFilter(float dT,InteFixFilter_t *data);
#endif


