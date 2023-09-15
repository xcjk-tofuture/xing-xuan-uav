#ifndef __MATHTOOL_H__
#define __MATHTOOL_H__

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdlib.h>
#include "stdbool.h"
#include <math.h>
#include "vector3.h"

#define REAL   float
#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) > 0 ? (x) : -(x))
#define My_PI                3.141592653f	//圆周率
#define DEG_TO_RAD           0.01745329f	//角度转弧度
#define RAD_TO_DEG           57.29577951f	//弧度转角度
#define EARTH_RADIUS         6371.004f      //km
#define GRAVITY_ACCEL        9.8f           //重力加速度 单位：m/s^2
#define HALF_SQRT_2          0.70710678118654757f	//根号2的值
#define LIMIT( x,min,max ) ( ((x) <= (min)) ? (min) : ( ((x) > (max))? (max) : (x) ) )
#define LPF_1_(hz,t,in,out) ((out) += ( 1 / ( 1 + 1 / ( (hz) *6.28f *(t) ) ) ) *( (in) - (out) ))	

#define REAL   float
#define TAN_MAP_RES     0.003921569f/* (smallest non-zero value in table) */
#define TAN_MAP_SIZE    256
#define MY_PPPIII_HALF   1.570796f
#define my_sign(x) ( ((x) > 1e-6f) ? 1:( ((x)<-1e-6f) ? -1 : 0) )
#define my_3_norm(x,y,z) (my_sqrt(my_pow((x)) + my_pow((y)) + my_pow((z))))
#define my_2_norm(x,y) (my_sqrt(my_pow((x)) + my_pow((y))))
#define my_pow(a) ((a)*(a))
#define safe_div(numerator,denominator,safe_value) ( (denominator == 0)? (safe_value) : ((numerator)/(denominator)) )
#define DELTA_LIMIT(x,dl,y) (y) += (LIMIT(((x) - (y)),-dl,dl))
#define _MIN(a, b) ((a) < (b) ? (a) : (b))
#define _MAX(a, b) ((a) > (b) ? (a) : (b))
#define my_pow_2_curve(in,a,max) (((1.0f - (a)) + (a) *LIMIT(ABS((in) / (max)),0,1)) * in)
#define To_180_degrees range_to_180deg
#define range_to_180deg(a) ( (a)>180 ? (a - 360) : ((a)<-180 ? (a + 360) : (a)) )

float SafeArcsin(float v);
float ConstrainFloat(float amt, float low, float high);
int16_t ConstrainInt16(int16_t amt, int16_t low, int16_t high);
uint16_t ConstrainUint16(uint16_t amt, uint16_t low, uint16_t high);
int32_t ConstrainInt32(int32_t amt, int32_t low, int32_t high);
int32_t ApplyDeadbandInt(int32_t value, int32_t deadband);
float ApplyDeadbandFloat(float value, float deadband);
float Radians(float deg);
float Degrees(float rad);
float Sq(float v);
float Pythagorous2(float a, float b);
float Pythagorous3(float a, float b, float c);
float Pythagorous4(float a, float b, float c, float d);
float WrapDegree360(float angle);
float WrapDegree180(float angle);
int32_t GetRandom(void);

float my_abs(float f);
REAL fast_atan2(REAL y, REAL x);
float my_sqrt(float number);
float my_sqrt_reciprocal(float number);
double mx_sin(double rad);
double my_sin(double rad);
float my_cos(double rad);
float my_deadzone(float x,float ,float zoom);
float my_deadzone_2(float x,float ,float zoom);
float my_deadzone_p(float x,float zone);
float my_deadzone_n(float x,float zone);
double To_180_degrees_db(double x);
float fifo(uint8_t arr_num,uint8_t *cnt,float *arr,float in);
float linear_interpolation_5(float range[5],float interpolation[5],float in);//range 必须从小到大
void length_limit(float *in1,float *in2,float limit,float out[]);
void rot_vec_2(float in[2],float sinx,float out[2]);
float vec_2_cross_product(float in1[2],float in2[2]);//正负为in1->in2 夹角逆时针
float vec_2_dot_product(float in1[2],float in2[2]);//正负为in1->in2 夹角（空间实际夹角）
void vec_3_cross_product_err_sinx(float in1[3],float in2[3],float out[3]);//输出xyz误差夹角x 的sin(x)，右手螺旋
float vec_3_dot_product(float in1[3],float in2[3]); //正负为in1->in2 夹角（空间实际夹角）

#endif


