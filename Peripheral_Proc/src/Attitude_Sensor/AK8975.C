#include "AK8975.h"


#define GPIO_ResetBits() HAL_GPIO_WritePin(SPI2_CS3_GPIO_Port, SPI2_CS3_Pin, GPIO_PIN_RESET)
#define GPIO_SetBits()   HAL_GPIO_WritePin(SPI2_CS3_GPIO_Port, SPI2_CS3_Pin, GPIO_PIN_SET)

#define MAG_ROTATION ROTATION_NONE

s16 ADCMAG[3];
static uint8_t ak8975_buf[6];


u8 Drv_SPI2_RW(u8 dat)
{
 return SPI_SingleWirteAndRead(2,dat);
}	


static void ak8975_enable(u8 ena)
{
	if(ena)
		GPIO_ResetBits();
	else
		GPIO_SetBits();
}

u8 DrvAK8975Check(void)
{
	u8 _tmp;
	ak8975_enable(1);
	Drv_SPI2_RW(AK8975_WIA_REG|0x80);
	_tmp = Drv_SPI2_RW(0xFF);
	ak8975_enable(0);
	if(_tmp == 0x48)
		return 0;
	else
		return 1;
}

static void ak8975_Trig(void)
{
	ak8975_enable(1);
	Drv_SPI2_RW(AK8975_CNTL_REG);
	Drv_SPI2_RW(0x01);
	ak8975_enable(0);
}

void DrvAk8975Read(void)
{	
	
	ak8975_enable(1);
	Drv_SPI2_RW(AK8975_HXL_REG|0x80);
	for(u8 i=0; i<6; i++)
		ak8975_buf[i] = Drv_SPI2_RW(0xff);
	ak8975_enable(0);
	
	ak8975_Trig();
	
}

//拓空者PRO-088
void Mag_Get(Vector3f_t* Mag)
{
	s16 tmp[3];
	
	tmp[0] = ((((int16_t)ak8975_buf[1]) << 8) | ak8975_buf[0]) ;
	tmp[1] = ((((int16_t)ak8975_buf[3]) << 8) | ak8975_buf[2]) ;
	tmp[2] = ((((int16_t)ak8975_buf[5]) << 8) | ak8975_buf[4]) ;
	
	/*转换坐标*/
	Mag->x = -tmp[0];
	Mag->y = +tmp[1];
	Mag->z = -tmp[2];
}








///**********************************************************************************************************
//*函 数 名: Ak8975Enable
//*功能说明: 磁力计使能
//*形    参: 是否使能
//*返 回 值: 无
//**********************************************************************************************************/
//static void Ak8975Enable(uint8_t Ena)
//{
//	if(Ena)
//		GPIO_ResetBits();
//	else
//		GPIO_SetBits();
//}
///**********************************************************************************************************
//*函 数 名: AK8975Trig
//*功能说明: 磁力计触发
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
//static void AK8975Trig(void)
//{
//	Ak8975Enable(1);
//	SPI_SingleWirteAndRead(2,AK8975_CNTL_REG);
//	SPI_SingleWirteAndRead(2,0x01);
//	Ak8975Enable(0);
//}
///**********************************************************************************************************
//*函 数 名: AK8975_Read
//*功能说明: 读磁力计数据
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/

//void AK8975_Read(void)
//{	
//	Ak8975Enable(1);
//	SPI_SingleWirteAndRead(2,AK8975_HXL_REG|0x80);
//	for(uint8_t i=0; i<6; i++)
//		ak8975_buf[i] = SPI_SingleWirteAndRead(2,0XFF);
//	Ak8975Enable(0);
//	AK8975Trig();
//	ADCMAG[0] = ((((int16_t)ak8975_buf[1]) << 8) | ak8975_buf[0]) ;
//	ADCMAG[1] = ((((int16_t)ak8975_buf[3]) << 8) | ak8975_buf[2]) ;
//	ADCMAG[2] = ((((int16_t)ak8975_buf[5]) << 8) | ak8975_buf[4]) ;
//}
///**********************************************************************************************************
//*函 数 名: MagGet
//*功能说明: 获取磁力计数据
//*形    参: 无
//*返 回 值: 无
//**********************************************************************************************************/
////void MagGet(Vector3f_t* Mag)
////{
////	AK8975_Read();
////	Mag->x =   ADCMAG[0];
////	Mag->y =   -ADCMAG[1];
////	Mag->z =   ADCMAG[2];
////	RotateVector3f(MAG_ROTATION, Mag);
////}