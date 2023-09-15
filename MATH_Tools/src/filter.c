/*------------------------------------------------------------
文件功能：数据滤波处理
--------------------------------------------------------------*/
#include "filter.h"
#include "mathTool.h"

ButterParameter_t Butter_5HZ_Parameter_Acce;
ButterParameter_t Butter_80HZ_Parameter_Acce;

ButterParameter_t Butter_80HZ_Parameter_Acce={
  //200hz---80hz
1,     1.14298050254,   0.4128015980962,
0.638945525159,    1.277891050318,    0.638945525159
};

//ButterParameter_t Butter_60HZ_Parameter_Acce={
//  //200hz---60hz
//1,   0.3695273773512,   0.1958157126558,
//0.3913357725018,   0.7826715450035,   0.3913357725018
//};

//ButterParameter_t Butter_51HZ_Parameter_Acce={
//  //200hz---51hz
//1,  0.03680751639284,   0.1718123812701,
//0.3021549744157,   0.6043099488315,   0.3021549744157,
//};

//ButterParameter_t Butter_30HZ_Parameter_Acce={
//  //200hz---30hz
//1,  -0.7477891782585,    0.272214937925,
//0.1311064399166,   0.2622128798333,   0.1311064399166
//};
//ButterParameter_t Butter_20HZ_Parameter_Acce={
//  //200hz---20hz
//  1,    -1.14298050254,   0.4128015980962,
//  0.06745527388907,   0.1349105477781,  0.06745527388907
//};
//ButterParameter_t Butter_15HZ_Parameter_Acce={
//  //200hz---15hz
//  1,   -1.348967745253,   0.5139818942197,
//  0.04125353724172,  0.08250707448344,  0.04125353724172
//};

//ButterParameter_t Butter_10HZ_Parameter_Acce={
//  //200hz---10hz
//  1,   -1.561018075801,   0.6413515380576,
//  0.02008336556421,  0.04016673112842,  0.02008336556421};
ButterParameter_t Butter_5HZ_Parameter_Acce={
  //200hz---5hz
  1,   -1.778631777825,   0.8008026466657,
  0.005542717210281,  0.01108543442056, 0.005542717210281
};

//ButterParameter_t Butter_2HZ_Parameter_Acce={
//  //200hz---2hz
//  1,   -1.911197067426,   0.9149758348014,
//  0.0009446918438402,  0.00188938368768,0.0009446918438402
//};



/*************************************************
函数名:	float LPButterworth(float curr_input,Butter_BufferData *Buffer,Butter_Parameter *Parameter)
说明:	加速度计低通滤波器
入口:	float curr_input 当前输入加速度计,滤波器参数，滤波器缓存
出口:	无
备注:	2阶Butterworth低通滤波器
*************************************************/
float LPButterworth(float curr_input,ButterBufferData_t *Buffer,ButterParameter_t *Parameter)
{
	/* 获取最新x(n) */
	static int LPF_Cnt=0;
	Buffer->InputButter[2]=curr_input;
	if(LPF_Cnt>=100)
	{
		/* Butterworth滤波 */
		Buffer->OutputButter[2]=
		 Parameter->b[0] * Buffer->InputButter[2]
		+Parameter->b[1] * Buffer->InputButter[1]
		+Parameter->b[2] * Buffer->InputButter[0]
		-Parameter->a[1] * Buffer->OutputButter[1]
		-Parameter->a[2] * Buffer->OutputButter[0];
	}
	else
	{
		  Buffer->OutputButter[2]=Buffer->InputButter[2];
		  LPF_Cnt++;
	}
	/* x(n) 序列保存 */
	Buffer->InputButter[0]=Buffer->InputButter[1];
	Buffer->InputButter[1]=Buffer->InputButter[2];
	/* y(n) 序列保存 */
	Buffer->OutputButter[0]=Buffer->OutputButter[1];
	Buffer->OutputButter[1]=Buffer->OutputButter[2];
	return Buffer->OutputButter[2];
}

void InteFixFilter(float dT,InteFixFilter_t *data)
{
	float ei_lim_val;
	
	if(data->ei_limit>0)
	{		
		ei_lim_val = LIMIT(data->ei,-data->ei_limit,data->ei_limit);
	}
	else
	{
		ei_lim_val = data->ei;
	}	
	
	data->out = (data->in_est + ei_lim_val);
	
	data->e = data->fix_ki *(data->in_obs - data->out);

	data->ei += data->e *dT;


}

void FixInteFilter(float dT,FixInteFilter_t *data)
{
	
	data->out += (data->in_est_d + data->e ) *dT;
	
	data->e = data->fix_kp *(data->in_obs - data->out);

	if(data->e_limit>0)
	{		
		data->e = LIMIT(data->e,-data->e_limit,data->e_limit);
	}	
	
}


