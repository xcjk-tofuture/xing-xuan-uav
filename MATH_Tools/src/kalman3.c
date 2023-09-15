/*----------------------------------------------------------------————————————————————————--
文件功能：3阶卡尔曼滤波算法 移植于BlueSkyFlightControl
----------------------------------------------------------------————————————————————————--*/
#include "kalman3.h"
#include "matrix3.h"

//单位矩阵
static float I[9] = {1, 0, 0, 0, 1, 0, 0, 0, 1};

static void KalmanSlidWindowUpdate(Kalman_t* kalman);

/*-------------------------------矩阵运算-------------------------------*/
/**
  * @func	矩阵转置
  * @brief	A=A'
  * @param	m		行
  * 		n		列
  */
void matrix_trans(float* A, float* B, unsigned char m, unsigned char n)
{
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			if(j>=i){
				B[i*n+j] = A[j*n+i];
				B[j*n+i] = A[i*n+j];
			}
		}
	}
}

/**
  * @func	矩阵加法
  * @brief	A+B=C
  * @param	m		行
  * 		n		列
  */
void matrix_plus(float* A, float* B, float* C, unsigned char m, unsigned char n)
{
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			C[i*n+j] = A[i*n+j] + B[i*n+j];
		}
	}
}

/**
  * @func	矩阵减法
  * @brief	A-B=C
  * @param	m		行
  * 		n		列
  */
void matrix_minus(float* A, float* B, float* C, unsigned char m, unsigned char n)
{
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			C[i*n+j] = A[i*n+j] - B[i*n+j];
		}
	}
}

/**
  * @func	矩阵乘法
  * @brief	A*B=C
  * @param	m		矩阵A的行
  * 		p		矩阵A的列(也是矩阵B的行)
  * 		n		矩阵B的列
  */
void matrix_multiply(float* A, float* B, float* C, unsigned char m, unsigned char p, unsigned char n)
{
	for(int i=0; i<m; i++){
		for(int j=0; j<n; j++){
			C[i*n+j] = 0;
			for(int k=0; k<p; k++){
				C[i*n+j] += A[i*p+k] * B[k*n+j];
			}
		}
	}
}

/**
  * @func	二阶矩阵求逆
  * @brief	B = A^-1
  * @ret	0	可逆
  * 		-1	不可逆
  */
int mareix_inv_rank2(float* A, float* B)
{
	float div = A[0]*A[3] - A[1]*A[2];
	if(div == 0)
		return -1;
	B[0] = A[3]/div;
	B[1] = -A[1]/div;
	B[2] = -A[2]/div;
	B[3] = A[0]/div;
	return 0;
}

/**
  * @func	三阶矩阵求逆
  * @brief	B = A^-1
  * @ret	0	可逆
  * 		-1	不可逆
  */
int mareix_inv_rank3(float* A, float* B)
{
	float div = A[0]*A[4]*A[8] + A[1]*A[5]*A[6] + A[2]*A[3]*A[7] - A[0]*A[5]*A[7] - A[1]*A[3]*A[8] - A[2]*A[4]*A[6];
	if(div == 0)
		return -1;
	B[0] =  (A[4]*A[8]-A[5]*A[7])/div;
	B[1] = -(A[1]*A[8]-A[2]*A[7])/div;
	B[2] =  (A[1]*A[5]-A[2]*A[4])/div;
	B[3] = -(A[3]*A[8]-A[5]*A[6])/div;
	B[4] =  (A[0]*A[8]-A[2]*A[6])/div;
	B[5] = -(A[0]*A[5]-A[2]*A[3])/div;
	B[6] =  (A[3]*A[7]-A[4]*A[6])/div;
	B[7] = -(A[0]*A[7]-A[1]*A[6])/div;
	B[8] =  (A[0]*A[4]-A[1]*A[3])/div;
	return 0;
}

/*-------------------------------加速度补偿-------------------------------*/
/**
  * @func	加速度计补偿值计算
  * @param	a			减去重力加速度后的加速度
  * 
  * @ret	补偿值
  * @use	循环使用该函数20次以上才能得到比较精确的补偿值
  */
float Acc_Comp(float a)
{
	static float b = 0;
	b = b*0.95f + a*0.05f;
	return -b;
}

/*-------------------------------卡尔曼运算-------------------------------*/
/**
  * @func	卡尔曼滤波器初始化
  * @ret	kf			卡尔曼滤波器结构体
  * 		pre_r		气压计误差方差，值越大滤波作用越强，响应越慢
  * 		acc_r		加速度计误差方差，值越大滤波作用越强，响应越慢
  * 		init_pre	初始位置气压值
  */
void Kalman_Fil_Init(KalFil_t* kf, float pre_r, float acc_r, float init_pre)
{
	float r [2][2] = {{pre_r,0},{0,acc_r}};									// 测量误差方差
	float q [3][3] = {{0.0001f,0,0},{0,0.00001f,0},{0,0,0.0001f}};				// 过程误差方差
	float f [3][3] = {{1,Period,0.5f*Period*Period},{0,1,Period},{0,0,1}};	// 转移矩阵
	float h [2][3] = {{1/PHParam,0,0},{0,0,1}};							// 输出矩阵
	float x [3][1] = {{0},{0},{0}};											// 状态矩阵
	float p [3][3] = {{10,0,0},{0,0,0},{0,0,1}};							// 协方差矩阵
	float v [2][1] = {{init_pre},{0}};
	float ft[3][3] = {{1,0,0},{Period,1,0},{0.5f*Period*Period,Period,1}};	// 转移矩阵的转置
	float ht[3][2] = {{1/PHParam,0},{0,0},{0,1}};	
	memset(kf, 0, sizeof(KalFil_t));
	
	memcpy(kf->R , r ,sizeof(r ));
	memcpy(kf->Q , q ,sizeof(q ));
	memcpy(kf->F , f ,sizeof(f ));
	memcpy(kf->H , h ,sizeof(h ));
	memcpy(kf->X , x ,sizeof(x ));
	memcpy(kf->P , p ,sizeof(p ));
	memcpy(kf->V , v ,sizeof(v ));
	memcpy(kf->Ft, ft,sizeof(ft));
	memcpy(kf->Ht, ht,sizeof(ht));
}

/**
  * @func	卡尔曼滤波器计算
  * @ret	kf			卡尔曼滤波器结构体
  * 		ia			输入加速度(需要减去重力加速度)
  * 		ih			输入高度
  * @add	最终输出的高度、速度、加速度存在结构体状态变量中
  * 		高度：X[0][0]  速度：X[1][0]  加速度：X[2][0]
  */
void Kalman_Fil_Calc(KalFil_t* kf, float ia, float ipre)
{
	int ret = 0;
	float I[3][3] = {{1,0,0},{0,1,0},{0,0,1}};
	// 观测值：高度、加速度
	float Z[2][1] = {{ipre},{ia}};
	// 中间变量
	float X1[3][1];
	float P1[3][3];
	float P2[3][3];
	float K1[3][2];
	float K2[2][3];
	float K3[2][2];
	float K4[2][2];
	float X2[2][1];
	float X3[2][1];
	float X4[3][1];
	float P3[3][3];
	
	
	// 下一状态估计
	matrix_multiply((float*)kf->F, (float*)kf->X, (float*)X1, 3, 3, 1);		// X1 = F*kf->X
	// 计算协方差矩阵
	matrix_multiply((float*)kf->F, (float*)kf->P, (float*)P1, 3, 3, 3);		// P1 = F*kf->P
	matrix_multiply((float*)P1, (float*)kf->Ft, (float*)P2, 3, 3, 3);		// P2 = P1*kf->Ft
	matrix_plus((float*)kf->Q, (float*)P2, (float*)P1, 3, 3);				// P1 = P2 + Q
	// 计算卡尔曼增益矩阵
	matrix_multiply((float*)P1, (float*)kf->Ht, (float*)K1, 3, 3, 2);		// K1 = P1*kf->Ht
	matrix_multiply((float*)kf->H, (float*)P1, (float*)K2, 2, 3, 3);		// K2 = kf->H*P1
	matrix_multiply((float*)K2, (float*)kf->Ht, (float*)K3, 2, 3, 2);		// K3 = K2*kf->Ht
	matrix_plus((float*)kf->R, (float*)K3, (float*)K4, 2, 2);				// K4 = K3+R
	ret = mareix_inv_rank2((float*)K4, (float*)K3);							// K3 = K4^-1
	if(ret == -1) return;
	matrix_multiply((float*)K1, (float*)K3, (float*)kf->K, 3, 2, 2);		// kf->K = K1*K3
	// 计算状态最优估计
	matrix_multiply((float*)kf->H, (float*)X1, (float*)X2, 2, 3, 1);		// X2 = kf->H*X1
	matrix_minus((float*)Z, (float*)X2, (float*)X3, 2, 1);					// X3 = Z - X2
	matrix_minus((float*)X3, (float*)kf->V, (float*)X2, 2, 1);				// X2 = X3 - V
	matrix_multiply((float*)kf->K, (float*)X2, (float*)X4, 3, 2, 1);		// X4 = kf->K*X2
	matrix_plus((float*)X1, (float*)X4, (float*)kf->X, 3, 1);				// kf->X = X1 + X4
	// 更新协方差矩阵
	matrix_multiply((float*)kf->K, (float*)kf->H, (float*)P3, 3, 2, 3);		// P3 = kf->K*kf->H
	matrix_minus((float*)I, (float*)P3, (float*)P2, 3, 3);					// P2 = I - P3
	matrix_multiply((float*)P2, (float*)P1, (float*)kf->P, 3, 3, 3);		// kf->P = P2*P1
	
}

/**
  * @func	修改气压参考平面
  * @param	kf			卡尔曼滤波器结构体
  * 		pre			参考平面气压
  */
void Kalman_Pre_Ref(KalFil_t* kf, float pre)
{
	kf->V[0][0] = pre;
}

/**
  * @func	修改气压高度换算系数(压高系数)
  * @param	kf			卡尔曼滤波器结构体
  * 		k			压高系数
  * @add	在海拔较低的地方k近似等于0.09
  */
void Kalman_Pre_Hight_Param(KalFil_t* kf, float k)
{
	kf->H [0][0] = -1/k;
	kf->Ht[0][0] = -1/k;
}

/**********************************************************************************************************
*函 数 名: KalmanUpdate
*功能说明: 三阶卡尔曼算法更新
*形    参: 卡尔曼结构体指针 输入矩阵 观测矩阵 更新标志位(为真时才融合，否则只进行状态预估)
*返 回 值: 无
**********************************************************************************************************/
void KalmanUpdate(Kalman_t* kalman, Vector3f_t input, Vector3f_t observe, bool flag)
{
    /*用于存放计算结果的临时矩阵*/
    float S[9], m1[9], m2[9], m3[9], m4[9], m5[9];
    /*1:状态预估计 Xk = Fk*Xk-1 + Bk*Uk*/
    kalman->State = Vector3f_Add(Matrix3MulVector3(kalman->f, kalman->State), Matrix3MulVector3(kalman->b, input));
    /*状态窗口更新*/
    KalmanSlidWindowUpdate(kalman);
    //当观测值未更新时不进行融合，退出本函数
    if(flag == false)
    {
        return;
    }
    /*2：误差协方差矩阵预更新 Pk = Fk*Pk-1*FkT + Qk*/
    Matrix3_Mul(kalman->f, kalman->covariance, m1);
    Matrix3_Mul(m1, kalman->f_t, m2);
    Matrix3_Add(m2, kalman->q, kalman->covariance);
    /*取出窗口中的状态量*/
    Vector3f_t statusDelay;
    statusDelay.x = kalman->StatusSlidWindow[kalman->SlidWindowSize - kalman->FuseDelay.x].x;
    statusDelay.y = kalman->StatusSlidWindow[kalman->SlidWindowSize - kalman->FuseDelay.y].y;
    statusDelay.z = kalman->StatusSlidWindow[kalman->SlidWindowSize - kalman->FuseDelay.z].z;
    /*3：计算残差矩阵 Yk = Zk - Hk*Xk*/
    kalman->Residual = Vector3f_Sub(observe, Matrix3MulVector3(kalman->h, statusDelay));
    /*4：Sk = Hk*Pk*HkT + Rk*/
    Matrix3_Mul(kalman->h, kalman->covariance, m1);
    Matrix3_Mul(m1, kalman->h_t, m2);
    Matrix3_Add(m2, kalman->r, S);
    /*5：计算卡尔曼增益 Kk = Pk*HkT*Sk-1*/
    Matrix3_Det(S, m1);
    Matrix3_Mul(kalman->covariance, kalman->h_t, m2);
    Matrix3_Mul(m2, m1,kalman->gain);
    /*6：修正当前状态 Xk = Xk + Kk*Yk*/
    kalman->State = Vector3f_Add(kalman->State, Matrix3MulVector3(kalman->gain, kalman->Residual));
    /*7：更新协方差矩阵 Pk = (I-Kk*Hk)*Pk*(I-Kk*Hk)T + Kk*Rk*KkT*/
    Matrix3_Mul(kalman->gain, kalman->h, m1);
    Matrix3_Sub(I, m1, m2);
    Matrix3_Tran(m2, m3);
    Matrix3_Mul(m2, kalman->covariance, m4);
    Matrix3_Mul(m4, m3, m5);
    Matrix3_Mul(kalman->gain, kalman->r, m1);
    Matrix3_Tran(kalman->gain, m2);
    Matrix3_Mul(m1, m2, m3);
    Matrix3_Add(m5, m3, kalman->covariance);
}

/**********************************************************************************************************
*函 数 名: KalmanStateTransMatSet
*功能说明: 状态转移矩阵设置
*形    参: 卡尔曼结构体指针 状态转移矩阵
*返 回 值: 无
**********************************************************************************************************/
void KalmanStateTransMatSet(Kalman_t* kalman, float* f)
{
    uint8_t i;

    /*状态转移矩阵赋值*/
    for(i=0; i<9; i++)
    {
        kalman->f[i] = f[i];
    }
    /*计算状态转移矩阵的转置*/
    Matrix3_Tran(kalman->f, kalman->f_t);
}

/**********************************************************************************************************
*函 数 名: KalmanObserveMapMatSet
*功能说明: 观测映射矩阵设置
*形    参: 卡尔曼结构体指针 观测映射矩阵
*返 回 值: 无
**********************************************************************************************************/
void KalmanObserveMapMatSet(Kalman_t* kalman, float* h)
{
    uint8_t i;

    /*观测映射矩阵赋值*/
    for(i=0; i<9; i++)
    {
        kalman->h[i] = h[i];
    }
    /*计算观测映射矩阵的转置*/
    Matrix3_Tran(kalman->h, kalman->h_t);
}

/**********************************************************************************************************
*函 数 名: KalmanCovarianceMatSet
*功能说明: 协方差矩阵设置
*形    参: 卡尔曼结构体指针 协方差矩阵
*返 回 值: 无
**********************************************************************************************************/
void KalmanCovarianceMatSet(Kalman_t* kalman, float* p)
{
    uint8_t i;

    /*协方差矩阵赋值*/
    for(i=0; i<9; i++)
    {
        kalman->covariance[i] = p[i];
    }
}

/**********************************************************************************************************
*函 数 名: KalmanQMatSet
*功能说明: 过程噪声协方差矩阵设置
*形    参: 卡尔曼结构体指针 协方差矩阵
*返 回 值: 无
**********************************************************************************************************/
void KalmanQMatSet(Kalman_t* kalman, float* q)
{
    uint8_t i;

    /*协方差矩阵赋值*/
    for(i=0; i<9; i++)
    {
        kalman->q[i] = q[i];
    }
}

/**********************************************************************************************************
*函 数 名: KalmanRMatSet
*功能说明: 测量噪声协方差矩阵设置
*形    参: 卡尔曼结构体指针 协方差矩阵
*返 回 值: 无
**********************************************************************************************************/
void KalmanRMatSet(Kalman_t* kalman, float* r)
{
    uint8_t i;

    /*协方差矩阵赋值*/
    for(i=0; i<9; i++)
    {
        kalman->r[i] = r[i];
    }
}

/**********************************************************************************************************
*函 数 名: KalmanBMatSet
*功能说明: 输入控制矩阵设置
*形    参: 卡尔曼结构体指针 输入控制矩阵
*返 回 值: 无
**********************************************************************************************************/
void KalmanBMatSet(Kalman_t* kalman, float* b)
{
    uint8_t i;

    for(i=0; i<9; i++)
    {
        kalman->b[i] = b[i];
    }
}

/**********************************************************************************************************
*函 数 名: KalmanSlidWindowUpdate
*功能说明: 卡尔曼状态量滑动窗口更新
*形    参: 卡尔曼结构体指针
*返 回 值: 无
**********************************************************************************************************/
static void KalmanSlidWindowUpdate(Kalman_t* kalman)
{
    for(uint16_t i=0; i<kalman->SlidWindowSize-1; i++)
    {
        kalman->StatusSlidWindow[i] = kalman->StatusSlidWindow[i+1];
    }
    kalman->StatusSlidWindow[kalman->SlidWindowSize - 1] = kalman->State;
}




