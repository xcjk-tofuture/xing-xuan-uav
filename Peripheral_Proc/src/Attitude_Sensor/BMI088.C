/*!***************************************************
 * @file: BMI088.c
 * @brief: 用于配置BMI088陀螺仪 加速度计的各项参数并读取数据
 * @author:        
 * @date:2022/05/31
 * @note:	
 ****************************************************/
#include "BMI088.h"



extern mpu BMI088;

//HAL_GPIO_WritePin(GPIOD,SPI2_CS1_Pin|SPI2_CS2_Pin|SPI2_CS3_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOE,SPI2_CS0_Pin, GPIO_PIN_SET);
//#define SPI2_ACC_Enable       {  HAL_GPIO_WritePin(GPIOE,SPI2_CS0_Pin, GPIO_PIN_RESET);HAL_GPIO_WritePin(GPIOD,SPI2_CS1_Pin, GPIO_PIN_SET);}//PE15 PD8
//#define SPI2_ACC_Disable       {  HAL_GPIO_WritePin(GPIOE,SPI2_CS0_Pin, GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOD,SPI2_CS1_Pin, GPIO_PIN_SET);}
//#define SPI2_GYRO_Enable     {  HAL_GPIO_WritePin(GPIOE,SPI2_CS0_Pin, GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOD,SPI2_CS1_Pin, GPIO_PIN_RESET);}
//#define SPI2_GYRO_Disable     {  HAL_GPIO_WritePin(GPIOE,SPI2_CS0_Pin, GPIO_PIN_SET);HAL_GPIO_WritePin(GPIOD,SPI2_CS1_Pin, GPIO_PIN_SET);}

#define SPI2_ACC_Enable       HAL_GPIO_WritePin(GPIOE,SPI2_CS0_Pin, GPIO_PIN_RESET);//PE15 PD8
#define SPI2_ACC_Disable      HAL_GPIO_WritePin(GPIOD,SPI2_CS1_Pin, GPIO_PIN_SET);
#define SPI2_GYRO_Enable     	HAL_GPIO_WritePin(GPIOD,SPI2_CS1_Pin, GPIO_PIN_RESET);
#define SPI2_GYRO_Disable     HAL_GPIO_WritePin(GPIOD,SPI2_CS1_Pin, GPIO_PIN_SET);


static void delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--)
    {
        ;
    }
}

void Delay_ms ( uint32_t ms )
{
    while ( ms-- )
        delay_us ( 1000 );
}

u8 Drv_SPI2_RW(u8 dat)
{
   uint8_t RxData = 0X00;
    if(HAL_SPI_TransmitReceive(&hspi2, &dat, &RxData, 1, 10) != HAL_OK)
    {
        RxData = 0XFF;
    }
    return RxData;

}

void Drv_SPI2_Receive(uint8_t *pData, uint16_t Size)
{
    for(uint16_t i=0; i<Size; i++)
    {
        pData[i] = Drv_SPI2_RW(0);
    }
}

void DrvGpioCsPinCtrlBmi088Acc(u8 ena)
{
	if ( ena )
		{
		SPI2_ACC_Enable;
		}        
    else
		{
		 SPI2_ACC_Disable;
		}
        
}
void DrvGpioCsPinCtrlBmi088Gyr(u8 ena)
{
	if ( ena )
	{
		SPI2_GYRO_Enable;
	}
	else
	{
	  SPI2_GYRO_Disable;
	}
        
}

static void bmi088_acc_enable(u8 ena)
{
	DrvGpioCsPinCtrlBmi088Acc(ena);
}
static void bmi088_gyr_enable(u8 ena)
{
	DrvGpioCsPinCtrlBmi088Gyr(ena);
}

static u8 bmi088_acc_read(unsigned char regadr)
{
	//088的acc读取，在发送读取地址后，读取到的第一字节为0，第二字节才是正确数据，所以要多读取一字节
	u8 reg_data;
	bmi088_acc_enable(1);
	Drv_SPI2_RW(regadr | 0x80);
	Drv_SPI2_RW(0xff);
	reg_data = Drv_SPI2_RW(0x00);
	bmi088_acc_enable(0);
	return reg_data;
}
static void bmi088_acc_write(unsigned char regadr, unsigned char val)
{
	bmi088_acc_enable(1);
	Drv_SPI2_RW(regadr);
	Drv_SPI2_RW(val);
	bmi088_acc_enable(0);
}

static u8 bmi088_gyr_read(unsigned char regadr)
{
	u8 reg_data;
	bmi088_gyr_enable(1);
	Drv_SPI2_RW(regadr | 0x80);
	reg_data = Drv_SPI2_RW(0xff);
	bmi088_gyr_enable(0);
	return reg_data;
}
static void bmi088_gyr_write(unsigned char regadr, unsigned char val)
{
	bmi088_gyr_enable(1);
	Drv_SPI2_RW(regadr);
	Drv_SPI2_RW(val);
	bmi088_gyr_enable(0);
}

u8 DrvBmi088Init(void)
{
	u8 acc_id = 0;
	u8 gyr_id = 0;

//	DrvGpioSenser088CsPinInit();
  bmi088_acc_enable(0);
	bmi088_gyr_enable(0);
	//需要发送一个空字节，激活acc的spi模式
	bmi088_acc_enable(1);
	Drv_SPI2_RW(0xff);
	bmi088_acc_enable(0);
	Delay_ms(20);				  //100
	
	acc_id = bmi088_acc_read(0);
	gyr_id = bmi088_gyr_read(0);

	if ((acc_id != 0x1E) || (gyr_id != 0x0F))
		return 0;

	bmi088_acc_write(0x7e, 0xb6); //复位
	Delay_ms(20);				  //100
	bmi088_acc_enable(1);		  //激活SPI
	Drv_SPI2_RW(0xff);
	bmi088_acc_enable(0);
	Delay_ms(20);
	bmi088_acc_write(0x7d, 0x04); //打开加速度计
	Delay_ms(20);
	//(0x02,0x0b)加速度计ODR 800hz,无重采样;  (0x00,0x0c) ODR 1600hz，4重采样
	bmi088_acc_write(0x40, ((1 << 7) | (0x00 << 4) | (0x0b << 0)));
	Delay_ms(20);
	bmi088_acc_write(0x41, 0x03); //加速度计量程24g
	Delay_ms(20);
	bmi088_acc_write(0x7c, 0x00); //进入Active模式
	Delay_ms(20);

	bmi088_gyr_write(0x14, 0xb6); //复位
	Delay_ms(20);
	bmi088_gyr_write(0x0f, 0x00); //GYRO_RANGE(0x0f): 2000deg/s
	Delay_ms(20);
	//0x81: ODR=2000hz bandwidth=230hz, 0x00 ODR=2000hz bandwidth=532hz
	bmi088_gyr_write(0x10, 0x02); //0x01
	Delay_ms(20);
	bmi088_gyr_write(0x11, 0x00); //GYRO_LPM1(0x11): normal mode
	Delay_ms(20);
	bmi088_gyr_write(0x15, 0x80); //开启数据中断
	bmi088_gyr_write(0x16, 0x01); //int3引脚高电平有效
	bmi088_gyr_write(0x18, 0x01); //中断输出到int3
	return 1;
}

static u8 read_temperature_f = 1;
static u8 read_acceleration_f = 1;
_bmi_data_st st_bmi_data;
void DrvBmi088ReadServices(void)
{
	//==读加速度计
	if (read_acceleration_f)
	{
		read_acceleration_f = 0;
		//
		bmi088_acc_enable(1);
		Drv_SPI2_RW(0x12 | 0x80);
		Drv_SPI2_RW(0xff);
		Drv_SPI2_Receive(&st_bmi_data.un_ins.u8_buffer[0], 6);
		bmi088_acc_enable(0);
	}

	//==读温度
	if (read_temperature_f != 0)
	{
		read_temperature_f = 0;
//#ifdef USE_BMI_TEMPER
//		static u8 temptem[2];
//		static s16 temperature_tem[2];
//		//
//			bmi088_acc_enable(1);
//			DrvSSSpiRTOneByte(0x22 | 0x80);
//			DrvSSSpiRTOneByte(0xff);
//			DrvSSSpiRxMultByte(temptem, 2);
//			bmi088_acc_enable(0);
//			//
//			s16 s16_temp_u11;
//			s16_temp_u11 = ((temptem[0] << 3) + (temptem[1] >> 5));
//			if (s16_temp_u11 > 1023)
//			{
//				s16_temp_u11 = s16_temp_u11 - 2048;
//			}
//			temperature_tem[0] = (s16)(s16_temp_u11 * 12.5f + 2300); //0.125f +23
//			st_bmi_data.temperature_100 = temperature_tem[0] + 0.8f * (temperature_tem[0] - temperature_tem[1]);
//			//
//			temperature_tem[1] = temperature_tem[0];
	//#endif
	//#include "Drv_spl06.h"
	//		st_bmi_data.temperature_100 = (s16)(spl_data.temperature*100);

		}

		//==读陀螺仪
		bmi088_gyr_enable(1);
		Drv_SPI2_RW(0x02 | 0x80);
		Drv_SPI2_Receive(&st_bmi_data.un_ins.u8_buffer[6], 6);
		bmi088_gyr_enable(0);
}

void DrvBmi088TemperatureRead(void)
{
	read_temperature_f = 1;
}

void DrvBmi088AccelerationRead(void)
{
	read_acceleration_f = 1;
}
