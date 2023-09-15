/*----------------------------------------------------------------
文件功能：姿态 高度等PID的封装函数 以便于控制时的直接调用
------------------------------------------------------------------*/
#include "pid.h"
#include "mathTool.h"
//#include "flight_control.h"
//#include "signal_management.h"
//#include "flight_status.h"
/**********************************************************************************************************
*函 数 名: PID_Control
*功能说明: PID控制
*形    参: PID主结构体 PID数据结构体 周期T  N  期望量 反馈量
*返 回 值: PID输出OUT
**********************************************************************************************************/
float PID_Control(PID *PID_Controler,PID_DATA * Data,float T,float Feedforward,float Expect,float FeedBack,float dCutFreq)
{
	PID_Controler->Hz = 1/T;
		if(dCutFreq == 0)
       PID_Controler->dFilter = 0;   
    else
         PID_Controler->dFilter = 1 / (2 *  My_PI * dCutFreq);
	/*********************误差计算*********************/
	PID_Controler->Error = Expect - FeedBack;//期望减去反馈得到误差
	PID_Controler->Error = LIMIT(PID_Controler->Error,-Data->ErrorMax,Data->ErrorMax);//误差限幅
	/*********************积分与限幅*********************/
		    /*********************积分与限幅*********************/
	PID_Controler->Integrate += Data-> Ki * PID_Controler->Error * T;//计算积分值
	PID_Controler->Integrate = LIMIT(PID_Controler->Integrate,-Data->IntegrateMax,Data->IntegrateMax);//积分限幅

	/*********************微分计算*********************/
	PID_Controler->FeedBackDifferential = (PID_Controler->Error - PID_Controler->LastError) * PID_Controler->Hz;//误差微分
	PID_Controler->Differential = Data-> Kd * PID_Controler->FeedBackDifferential;//计算微分量
	PID_Controler->Differential = PID_Controler->LastDifferential + (T/(1 / (2 * My_PI * dCutFreq) + T)) * (PID_Controler->Differential - PID_Controler->LastDifferential);
	PID_Controler->Differential = LIMIT(PID_Controler->Differential,-Data->DifferentialMax,Data->DifferentialMax);//微分限幅
	PID_Controler->LastDifferential = PID_Controler->Differential;
	/*********************总输出计算*********************/
	PID_Controler->Out = Feedforward * Data->Kf + Data->Kp *  PID_Controler->Error + PID_Controler-> Differential +PID_Controler->Integrate;
	
	PID_Controler->LastError = PID_Controler->Error;//保存上次误差
	PID_Controler->LastFeedBack= FeedBack;//保存上次反馈
	return PID_Controler->Out;//返回控制量
}


/**********************************************************************************************************
*函 数 名: PID_Reset_I
*功能说明: 微分项清零
*形    参: PID主结构体
*返 回 值: 无
**********************************************************************************************************/
void PID_Reset_I(PID *PID_Controler)
{
    PID_Controler->Integrate = 0;
}

/**********************************************************************************************************
*函 数 名: GetControlError
*功能说明: 获取误差量
*形    参: PID主结构体
*返 回 值: 无
**********************************************************************************************************/
float GetControlError(PID *PID_Controler)
{
    return PID_Controler->Error;
}

/**********************************************************************************************************
*函 数 名: GetControlD
*功能说明: 获取微分量
*形    参: PID主结构体
*返 回 值: 无
**********************************************************************************************************/
float GetControlD(PID *PID_Controler)
{
    return PID_Controler->FeedBackDifferential;
}


/**********************************************************************************************************
*函 数 名: PID_Init
*功能说明: PID初始化
*形    参: PID主结构体
*返 回 值: 无
**********************************************************************************************************/
 void PID_DataInit(PID_DATA *Data ,float* PID_Data)
{
    /*PID参数*/
	Data->Kp = PID_Data[0];
  Data->Ki = PID_Data[1];
	Data->Kd = PID_Data[2];
	Data->Kf = PID_Data[3];
  Data->ErrorMax = PID_Data[4];
	Data->IntegrateMax = PID_Data[5];
	Data->DifferentialMax = PID_Data[6];
}


