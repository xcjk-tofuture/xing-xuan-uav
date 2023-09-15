#ifndef _KALMAN3_H_
#define _KALMAN3_H_

#include "mathTool.h"

typedef struct {
    /*状态矩阵*/
    Vector3f_t State;
    /*滑动窗口大小*/
    int16_t SlidWindowSize;
    /*状态滑动窗口*/
    Vector3f_t* StatusSlidWindow;
    /*观测信号相位补偿值*/
    Vector3i_t FuseDelay;
    /*残差矩阵*/
    Vector3f_t Residual;
    /*状态转移矩阵*/
    float f[9];
    /*状态转移矩阵的转置*/
    float f_t[9];
    /*观测映射矩阵*/
    float h[9];
    /*观测映射矩阵的转置*/
    float h_t[9];
    /*控制输入矩阵*/
    float b[9];
    /*卡尔曼增益矩阵*/
    float gain[9];
    /*误差协方差矩阵*/
    float covariance[9];
    /*过程噪声协方差矩阵*/
    float q[9];
    /*测量噪声协方差矩阵*/
    float r[9];
} Kalman_t;

extern Kalman_t KalmanRollPitch;
extern Kalman_t KalmanYaw;

void KalmanUpdate(Kalman_t* kalman, Vector3f_t input, Vector3f_t observe, bool flag);

void KalmanStateTransMatSet(Kalman_t* kalman, float* f);
void KalmanObserveMapMatSet(Kalman_t* kalman, float* h);
void KalmanCovarianceMatSet(Kalman_t* kalman, float* p);
void KalmanQMatSet(Kalman_t* kalman, float* q);
void KalmanRMatSet(Kalman_t* kalman, float* r);
void KalmanBMatSet(Kalman_t* kalman, float* b);

#define Sam_Frq		1000			// 采样频率 
#define Period		(float)1/Sam_Frq	// 采样周期
#define PHParam		1		// 压高系数


typedef struct Kalman_Filter
{
	float R [2][2];		// 测量误差方差
	float Q [3][3];		// 过程误差方差
	float F [3][3];		// 转移矩阵
	float Ft[3][3];		// 转移矩阵的转置
	float H [2][3];		// 输出矩阵
	float Ht[3][2];		// 输出矩阵的转置
	float P [3][3];		// 协方差矩阵
	float K [3][2];		// 卡尔曼增益矩阵
	float X [3][1];		// 状态矩阵
	float V [2][1];		// 输出噪声矩阵
}KalFil_t;

extern KalFil_t KalF;
/**
  * @func	卡尔曼滤波器初始化
  * @param	kf			卡尔曼滤波器结构体
  * 		init_hight	初始高度
  * 		init_acc	初始加速度
  */
void Kalman_Fil_Init(KalFil_t* kf, float pre_r, float acc_r, float init_pre);

/**
  * @func	卡尔曼滤波器计算
  * @param	kf			卡尔曼滤波器结构体
  * 		ia			输入加速度
  * 		ih			输入高度
  */
void Kalman_Fil_Calc(KalFil_t* kf, float ia, float ipre);

/**
  * @func	加速度计补偿值计算
  * @param	a			减去重力加速度后的加速度
  * 
  * @ret	补偿值
  * @add	循环使用该函数20次以上才能得到比较精确的补偿值
  */
float Acc_Comp(float a);

/**
  * @func	修改气压参考平面
  * @param	kf			卡尔曼滤波器结构体
  * 		pre			参考平面气压
  */
void Kalman_Pre_Ref(KalFil_t* kf, float pre);

/**
  * @func	修改气压高度换算系数(压高系数)
  * @param	kf			卡尔曼滤波器结构体
  * 		k			压高系数
  * @add	在海拔较低的地方k近似等于0.09
  */
void Kalman_Pre_Hight_Param(KalFil_t* kf, float k);

#endif
