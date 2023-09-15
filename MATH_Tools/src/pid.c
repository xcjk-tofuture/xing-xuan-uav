/*----------------------------------------------------------------
�ļ����ܣ���̬ �߶ȵ�PID�ķ�װ���� �Ա��ڿ���ʱ��ֱ�ӵ���
------------------------------------------------------------------*/
#include "pid.h"
#include "mathTool.h"
//#include "flight_control.h"
//#include "signal_management.h"
//#include "flight_status.h"
/**********************************************************************************************************
*�� �� ��: PID_Control
*����˵��: PID����
*��    ��: PID���ṹ�� PID���ݽṹ�� ����T  N  ������ ������
*�� �� ֵ: PID���OUT
**********************************************************************************************************/
float PID_Control(PID *PID_Controler,PID_DATA * Data,float T,float Feedforward,float Expect,float FeedBack,float dCutFreq)
{
	PID_Controler->Hz = 1/T;
		if(dCutFreq == 0)
       PID_Controler->dFilter = 0;   
    else
         PID_Controler->dFilter = 1 / (2 *  My_PI * dCutFreq);
	/*********************������*********************/
	PID_Controler->Error = Expect - FeedBack;//������ȥ�����õ����
	PID_Controler->Error = LIMIT(PID_Controler->Error,-Data->ErrorMax,Data->ErrorMax);//����޷�
	/*********************�������޷�*********************/
		    /*********************�������޷�*********************/
	PID_Controler->Integrate += Data-> Ki * PID_Controler->Error * T;//�������ֵ
	PID_Controler->Integrate = LIMIT(PID_Controler->Integrate,-Data->IntegrateMax,Data->IntegrateMax);//�����޷�

	/*********************΢�ּ���*********************/
	PID_Controler->FeedBackDifferential = (PID_Controler->Error - PID_Controler->LastError) * PID_Controler->Hz;//���΢��
	PID_Controler->Differential = Data-> Kd * PID_Controler->FeedBackDifferential;//����΢����
	PID_Controler->Differential = PID_Controler->LastDifferential + (T/(1 / (2 * My_PI * dCutFreq) + T)) * (PID_Controler->Differential - PID_Controler->LastDifferential);
	PID_Controler->Differential = LIMIT(PID_Controler->Differential,-Data->DifferentialMax,Data->DifferentialMax);//΢���޷�
	PID_Controler->LastDifferential = PID_Controler->Differential;
	/*********************���������*********************/
	PID_Controler->Out = Feedforward * Data->Kf + Data->Kp *  PID_Controler->Error + PID_Controler-> Differential +PID_Controler->Integrate;
	
	PID_Controler->LastError = PID_Controler->Error;//�����ϴ����
	PID_Controler->LastFeedBack= FeedBack;//�����ϴη���
	return PID_Controler->Out;//���ؿ�����
}


/**********************************************************************************************************
*�� �� ��: PID_Reset_I
*����˵��: ΢��������
*��    ��: PID���ṹ��
*�� �� ֵ: ��
**********************************************************************************************************/
void PID_Reset_I(PID *PID_Controler)
{
    PID_Controler->Integrate = 0;
}

/**********************************************************************************************************
*�� �� ��: GetControlError
*����˵��: ��ȡ�����
*��    ��: PID���ṹ��
*�� �� ֵ: ��
**********************************************************************************************************/
float GetControlError(PID *PID_Controler)
{
    return PID_Controler->Error;
}

/**********************************************************************************************************
*�� �� ��: GetControlD
*����˵��: ��ȡ΢����
*��    ��: PID���ṹ��
*�� �� ֵ: ��
**********************************************************************************************************/
float GetControlD(PID *PID_Controler)
{
    return PID_Controler->FeedBackDifferential;
}


/**********************************************************************************************************
*�� �� ��: PID_Init
*����˵��: PID��ʼ��
*��    ��: PID���ṹ��
*�� �� ֵ: ��
**********************************************************************************************************/
 void PID_DataInit(PID_DATA *Data ,float* PID_Data)
{
    /*PID����*/
	Data->Kp = PID_Data[0];
  Data->Ki = PID_Data[1];
	Data->Kd = PID_Data[2];
	Data->Kf = PID_Data[3];
  Data->ErrorMax = PID_Data[4];
	Data->IntegrateMax = PID_Data[5];
	Data->DifferentialMax = PID_Data[6];
}


