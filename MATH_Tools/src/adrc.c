/*----------------------------------------------------------------
�ļ����ܣ��Կ��ſ���
------------------------------------------------------------------*/
#include "adrc.h"
#include "mathTool.h"


/**********************************************************************************************************
*�� �� ��: ADRC_Fhan
*����˵��: ���ٿ��������ۺϺ���
*��    ��: v1, v2, r0, h0
*�� �� ֵ: hfst
**********************************************************************************************************/
float ADRC_Fhan(float v1, float v2, float r0, float h0)
{
	float d = h0 * h0 * r0;
	float a0 = h0 * v2;
	float y = v1 + a0; //y = x1 +hx2
	
	float a1 = sqrtf(d*(d + 8.0f*fabsf(y)));//a
	/*��ͬ�������*/
	float a2 = a0 + Sign_ADRC(y)*(a1-d)*0.5f;
	float sy = (Sign_ADRC(y+d) - Sign_ADRC(y-d))*0.5f;
	float a = (a0 + y - a2)*sy + a2;
	float sa = (Sign_ADRC(a+d) - Sign_ADRC(a-d))*0.5f;	
	return -r0*(a/d - Sign_ADRC(a))*sa - r0*Sign_ADRC(a);//hfst
}

/**********************************************************************************************************
*�� �� ��: Sign_ADRC
*����˵��: �Ƚϴ�С�жϺ���
*��    ��: Val
*�� �� ֵ: -1 1�����־λ
**********************************************************************************************************/
int16_t Sign_ADRC(float Val)
{
    if(Val >= 0.0f)
		return 1.0f;
	else
		return -1.0f;
}

/**********************************************************************************************************
*�� �� ��: ADRC_Fal
*����˵��: ��������� 
*��    ��: 
*�� �� ֵ: ������
*��    ע: alpha<1 fal�������� С�������� �����С���� ������
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
*�� �� ��: ADRC_TD��������ȡ΢�ַ�����
*����˵��: ADRC�������ĵ�һ���� ����΢����
*��    ��: ADRC_TD�ṹ�� ����ֵ
*�� �� ֵ: 1����������ֵv1
		   2������΢�ָ���ֵv2 
*��    ע: r0 h0�ɵ� r���������ٶȣ��������ٶ����ӡ� h0����������˲����ó������˲����ӡ�
**********************************************************************************************************/
void ADRC_TD(ADRC_TD_Def*TD,float ADRC_Expect)
{
	TD->h0 = TD->h * TD->N;
	TD->v1 += TD->h * TD->v2;
	TD->v2 += TD->h * ADRC_Fhan(TD->v1 - ADRC_Expect , TD->v2, TD->r0, TD->h0);
}

/**********************************************************************************************************
*�� �� ��: ADRC_ESO
*����˵��: ADRC�������ĵڶ����� ����״̬�۲���(������)
*��    ��: ADRC_ESO�ṹ�� ����ֵ(����ֵ)
*�� �� ֵ: 1��״̬�ź�z1
		   2��״̬�ٶ��ź�z2
		   3��״̬���ٶ��ź�z3	
*��    ע: ϵͳ���ֵΪ��������״̬������ESO����״̬�۲���������
**********************************************************************************************************/
void ADRC_ESO(ADRC_ESO_Def*ESO,float ADRC_FeedBack)
{
	float e = ESO->z1 - ADRC_FeedBack;

	ESO->zeta = ESO->h * ESO->N1;
	/*P16 �����Ժ�������ȡ����״̬�뵱ǰ״̬��� a2<a1 a1=0.5 a2=0.25*/
	float fe  = ADRC_Fal(e, 0.5f, ESO->zeta);
	float fe1 = ADRC_Fal(e, 0.25f, ESO->zeta);
	
	ESO->z1 += ESO->h *(ESO->z2 - ESO->beta1 * e);
	ESO->z2 += ESO->h *(ESO->z3 - ESO->beta2 * fe + ESO->b * ESO->u );
    ESO->z3 += ESO->h * (-ESO->beta3 * fe1);
}

/**********************************************************************************************************
*�� �� ��: ADRC_NLSEF
*����˵��: ADRC�������ĵ������� ����������������
*��    ��: 
*�� �� ֵ: 
*��    ע: ��һ�������ʽ u0=beta_1*e1+beta_2*e2+(beta_0*e0);
		   �ڶ��������ʽ u0=beta_1*fal(e1,alpha1,zeta)+beta_2*fal(e2,alpha2,zeta)
           �����������ʽ u0=-fhan(e1,e2,r,h1);
		   �����������ʽ u0=-fhan(e1,c*e2*e2,r,h1);
**********************************************************************************************************/
void ADRC_NLSEF(ADRC_NLSEF_Def* NLSEF,ADRC_ESO_Def*ESO,ADRC_TD_Def*TD)
{
    NLSEF->e1 = TD->v1 -  ESO->z1;
	NLSEF->e2 = TD->v2 -  ESO->z2; 

	NLSEF->e1 = ConstrainFloat(NLSEF->e1,-200 ,200);
	NLSEF->e2 = ConstrainFloat(NLSEF->e2,-3000,3000);
	
	/*��һ�������ʽ*/
	NLSEF->u0 = NLSEF->beta1* NLSEF->e1+NLSEF->beta2*NLSEF->e2;
//	/*�ڶ��������ʽ*/
//	NLSEF->u0 = NLSEF->beta1* ADRC_Fal(NLSEF->e1,NLSEF->alpha1,NLSEF->zeta)
//             +NLSEF->beta2 *ADRC_Fal(NLSEF->e2,NLSEF->alpha2,NLSEF->zeta);	
	
	/*�����������ʽ*/
//	NLSEF->u0 = -ADRC_Fhan(NLSEF->e1, NLSEF->e2, NLSEF->r1, NLSEF->h1);	
	/*�����������ʽ*/
//	NLSEF->u0 = -ADRC_Fhan(NLSEF->e1, NLSEF->c * NLSEF->e2, NLSEF->r1, NLSEF->h1);
	
	NLSEF->u  = NLSEF->u0- ESO->z3/NLSEF->b0;;
}

/**********************************************************************************************************
*�� �� ��: ADRC_Control
*����˵��: ADRC���� 
*��    ��: ADRC�ṹ�� ������ ������
*�� �� ֵ: ԭʼ������u0�� ��������Ŷ���������u
**********************************************************************************************************/
float ADRC_Control(ADRC_Def*ADRC,float Expect,float FeedBack ,float T)
{
	/*���Կ��ſ��������㷨 x2=f(x,x1,w,t) + bu
	 *					   y = x	
	 * ��������ϵͳ�� xΪ����ֵ(ң����ֵ) yΪ����ֵ(�����ǲ����Ľ��ٶ�)
	 *
	 *���Կ��š����������ڡ���������-z3(t)/b
	 * ϵͳ�ġ�δ��ģ��̬���͡�δ֪���š�����һ��������ƺͲ���
	 *
	 * Դ�ԡ���PID�������Կ��ſ��Ƽ����������� P17
	 */	
	
	
	/*�Կ��ſ�������1��:���Ź��ȹ���
	  ����Ϊ�����������õ������źŵĸ����ź�*/
	
	/*��¼����*/
	ADRC->TD.h = T;
	ADRC_TD(&ADRC->TD, Expect);
	
	/*�Կ��ſ�������2��������״̬�����Ŷ�
      ����״̬�۲������õ������źŵ�����״̬*/
	ADRC->ESO.h = T;
	ADRC_ESO(&ADRC->ESO,FeedBack);
	
	/*�Կ��ſ�������3��*:���������γ�
	  ����TD�ó���v1��v2��ESO�ĵó���z1 z2�����������ֵ���п��ƺ��Ŷ�����*/
	
    ADRC_NLSEF(&ADRC->NLSEF,&ADRC->ESO,&ADRC->TD);
	
//	ADRC->NLSEF.u0 = ConstrainFloat(ADRC->NLSEF.u,-350,350);
	
	return	ADRC->NLSEF.u;//�����Ŷ�����
//	return	ADRC->NLSEF.u0;//�������Ŷ�����
}


/**********************************************************************************************************
*�� �� ��: ADRC_Init
*����˵��: ADRC��ʼ�� 
*��    ��: ADRC�ṹ��
*�� �� ֵ: 
**********************************************************************************************************/
void ADRC_Init(ADRC_Def*ADRC ,float *ADRC_Data)
{
	/*TD����*/
	ADRC->TD.N  = ADRC_Data[0];
	ADRC->TD.h  = ADRC_Data[1];
	ADRC->TD.r0 = ADRC_Data[2];
	/*ESO����*/
	ADRC->ESO.beta1 = ADRC_Data[3];
	ADRC->ESO.beta2 = ADRC_Data[4];
	ADRC->ESO.beta3 = ADRC_Data[5];
	ADRC->ESO.b = ADRC_Data[6];
	ADRC->ESO.N1 = ADRC_Data[7];

	/*NLSEF����*/
	ADRC->NLSEF.beta0 = ADRC_Data[8];
	ADRC->NLSEF.beta1 = ADRC_Data[9];
	ADRC->NLSEF.beta2 = ADRC_Data[10];
	ADRC->NLSEF.alpha1 =ADRC_Data[11];
	ADRC->NLSEF.alpha2 =ADRC_Data[12];
	ADRC->NLSEF.zeta  = ADRC_Data[13];
	ADRC->NLSEF.b0 = 	ADRC_Data[14];
		
}




