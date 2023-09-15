/*----------------------------------------------------------------
文件功能：自抗扰控制
------------------------------------------------------------------*/
#include "adrc.h"
#include "mathTool.h"


/**********************************************************************************************************
*函 数 名: ADRC_Fhan
*功能说明: 快速控制最优综合函数
*形    参: v1, v2, r0, h0
*返 回 值: hfst
**********************************************************************************************************/
float ADRC_Fhan(float v1, float v2, float r0, float h0)
{
	float d = h0 * h0 * r0;
	float a0 = h0 * v2;
	float y = v1 + a0; //y = x1 +hx2
	
	float a1 = sqrtf(d*(d + 8.0f*fabsf(y)));//a
	/*不同情况计算*/
	float a2 = a0 + Sign_ADRC(y)*(a1-d)*0.5f;
	float sy = (Sign_ADRC(y+d) - Sign_ADRC(y-d))*0.5f;
	float a = (a0 + y - a2)*sy + a2;
	float sa = (Sign_ADRC(a+d) - Sign_ADRC(a-d))*0.5f;	
	return -r0*(a/d - Sign_ADRC(a))*sa - r0*Sign_ADRC(a);//hfst
}

/**********************************************************************************************************
*函 数 名: Sign_ADRC
*功能说明: 比较大小判断函数
*形    参: Val
*返 回 值: -1 1计算标志位
**********************************************************************************************************/
int16_t Sign_ADRC(float Val)
{
    if(Val >= 0.0f)
		return 1.0f;
	else
		return -1.0f;
}

/**********************************************************************************************************
*函 数 名: ADRC_Fal
*功能说明: 非线性组合 
*形    参: 
*返 回 值: 运算结果
*备    注: alpha<1 fal函数具有 小误差大增益 大误差小增益 的特征
**********************************************************************************************************/
float ADRC_Fal(float e, float alpha, float delta)
{
	if(fabsf(e) <= delta)
	{
		return e / (powf(delta, 1.0f-alpha));
	}
	else
	{
		return powf(fabsf(e), alpha) * Sign_ADRC(e);
	}
}

/**********************************************************************************************************
*函 数 名: ADRC_TD（合理提取微分方法）
*功能说明: ADRC控制器的第一部分 跟踪微分器
*形    参: ADRC_TD结构体 期望值
*返 回 值: 1、期望跟踪值v1
		   2、期望微分跟踪值v2 
*备    注: r0 h0可调 r决定跟踪速度，称作“速度因子” h0起对噪声的滤波作用称作“滤波因子”
**********************************************************************************************************/
void ADRC_TD(ADRC_TD_Def*TD,float ADRC_Expect)
{
	TD->h0 = TD->h * TD->N;
	TD->v1 += TD->h * TD->v2;
	TD->v2 += TD->h * ADRC_Fhan(TD->v1 - ADRC_Expect , TD->v2, TD->r0, TD->h0);
}

/**********************************************************************************************************
*函 数 名: ADRC_ESO
*功能说明: ADRC控制器的第二部分 扩张状态观测器(非线性)
*形    参: ADRC_ESO结构体 反馈值(测量值)
*返 回 值: 1、状态信号z1
		   2、状态速度信号z2
		   3、状态加速度信号z3	
*备    注: 系统输出值为反馈量，状态反馈，ESO扩张状态观测器的输入
**********************************************************************************************************/
void ADRC_ESO(ADRC_ESO_Def*ESO,float ADRC_FeedBack)
{
	float e = ESO->z1 - ADRC_FeedBack;

	ESO->zeta = ESO->h * ESO->N1;
	/*P16 非线性函数，提取跟踪状态与当前状态误差 a2<a1 a1=0.5 a2=0.25*/
	float fe  = ADRC_Fal(e, 0.5f, ESO->zeta);
	float fe1 = ADRC_Fal(e, 0.25f, ESO->zeta);
	
	ESO->z1 += ESO->h *(ESO->z2 - ESO->beta1 * e);
	ESO->z2 += ESO->h *(ESO->z3 - ESO->beta2 * fe + ESO->b * ESO->u );
    ESO->z3 += ESO->h * (-ESO->beta3 * fe1);
}

/**********************************************************************************************************
*函 数 名: ADRC_NLSEF
*功能说明: ADRC控制器的第三部分 非线性误差反馈控制律
*形    参: 
*返 回 值: 
*备    注: 第一种组合形式 u0=beta_1*e1+beta_2*e2+(beta_0*e0);
		   第二种组合形式 u0=beta_1*fal(e1,alpha1,zeta)+beta_2*fal(e2,alpha2,zeta)
           第三种组合形式 u0=-fhan(e1,e2,r,h1);
		   第四种组合形式 u0=-fhan(e1,c*e2*e2,r,h1);
**********************************************************************************************************/
void ADRC_NLSEF(ADRC_NLSEF_Def* NLSEF,ADRC_ESO_Def*ESO,ADRC_TD_Def*TD)
{
    NLSEF->e1 = TD->v1 -  ESO->z1;
	NLSEF->e2 = TD->v2 -  ESO->z2; 

	NLSEF->e1 = ConstrainFloat(NLSEF->e1,-200 ,200);
	NLSEF->e2 = ConstrainFloat(NLSEF->e2,-3000,3000);
	
	/*第一种组合形式*/
	NLSEF->u0 = NLSEF->beta1* NLSEF->e1+NLSEF->beta2*NLSEF->e2;
//	/*第二种组合形式*/
//	NLSEF->u0 = NLSEF->beta1* ADRC_Fal(NLSEF->e1,NLSEF->alpha1,NLSEF->zeta)
//             +NLSEF->beta2 *ADRC_Fal(NLSEF->e2,NLSEF->alpha2,NLSEF->zeta);	
	
	/*第三种组合形式*/
//	NLSEF->u0 = -ADRC_Fhan(NLSEF->e1, NLSEF->e2, NLSEF->r1, NLSEF->h1);	
	/*第四种组合形式*/
//	NLSEF->u0 = -ADRC_Fhan(NLSEF->e1, NLSEF->c * NLSEF->e2, NLSEF->r1, NLSEF->h1);
	
	NLSEF->u  = NLSEF->u0- ESO->z3/NLSEF->b0;;
}

/**********************************************************************************************************
*函 数 名: ADRC_Control
*功能说明: ADRC控制 
*形    参: ADRC结构体 期望量 反馈量
*返 回 值: 原始控制量u0和 加入估计扰动控制量的u
**********************************************************************************************************/
float ADRC_Control(ADRC_Def*ADRC,float Expect,float FeedBack ,float T)
{
	/*“自抗扰控制器”算法 x2=f(x,x1,w,t) + bu
	 *					   y = x	
	 * 该四旋翼系统中 x为期望值(遥控器值) y为反馈值(陀螺仪测量的角速度)
	 *
	 *“自抗扰”的意义在于“补偿”项-z3(t)/b
	 * 系统的“未建模动态”和“未知外扰”作用一并给予估计和补偿
	 *
	 * 源自《从PID技术到自抗扰控制技术》韩京清 P17
	 */	
	
	
	/*自抗扰控制器第1步:安排过度过程
	  输入为期望给定，得到期望信号的跟踪信号*/
	
	/*记录步长*/
	ADRC->TD.h = T;
	ADRC_TD(&ADRC->TD, Expect);
	
	/*自抗扰控制器第2步：估计状态和总扰动
      扩张状态观测器，得到反馈信号的扩张状态*/
	ADRC->ESO.h = T;
	ADRC_ESO(&ADRC->ESO,FeedBack);
	
	/*自抗扰控制器第3步*:控制量的形成
	  根据TD得出的v1、v2和ESO的得出的z1 z2计算出的误差差值进行控制和扰动补偿*/
	
    ADRC_NLSEF(&ADRC->NLSEF,&ADRC->ESO,&ADRC->TD);
	
//	ADRC->NLSEF.u0 = ConstrainFloat(ADRC->NLSEF.u,-350,350);
	
	return	ADRC->NLSEF.u;//加入扰动补偿
//	return	ADRC->NLSEF.u0;//不加入扰动补偿
}


/**********************************************************************************************************
*函 数 名: ADRC_Init
*功能说明: ADRC初始化 
*形    参: ADRC结构体
*返 回 值: 
**********************************************************************************************************/
void ADRC_Init(ADRC_Def*ADRC ,float *ADRC_Data)
{
	/*TD参数*/
	ADRC->TD.N  = ADRC_Data[0];
	ADRC->TD.h  = ADRC_Data[1];
	ADRC->TD.r0 = ADRC_Data[2];
	/*ESO参数*/
	ADRC->ESO.beta1 = ADRC_Data[3];
	ADRC->ESO.beta2 = ADRC_Data[4];
	ADRC->ESO.beta3 = ADRC_Data[5];
	ADRC->ESO.b = ADRC_Data[6];
	ADRC->ESO.N1 = ADRC_Data[7];

	/*NLSEF参数*/
	ADRC->NLSEF.beta0 = ADRC_Data[8];
	ADRC->NLSEF.beta1 = ADRC_Data[9];
	ADRC->NLSEF.beta2 = ADRC_Data[10];
	ADRC->NLSEF.alpha1 =ADRC_Data[11];
	ADRC->NLSEF.alpha2 =ADRC_Data[12];
	ADRC->NLSEF.zeta  = ADRC_Data[13];
	ADRC->NLSEF.b0 = 	ADRC_Data[14];
		
}




