#include "SPL06.h"



#define SPL06_CS_SetBits HAL_GPIO_WritePin(SPI2_CS2_GPIO_Port, SPI2_CS2_Pin, GPIO_PIN_SET)      //提高cs位
#define SPL06_CS_ResetBits HAL_GPIO_WritePin(SPI2_CS2_GPIO_Port, SPI2_CS2_Pin, GPIO_PIN_RESET)  //降低cs位



static struct spl0601_t spl0601;
static struct spl0601_t *p_spl0601; 

float Baro_Offset, Alt_3;
float Temperature, AltHigh;
float BaroPressure;

/**********************************************************************************************************
*函 数 名: SPL06CSPin_Init
*功能说明: SPL06 CS脚使能
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
//void SPL06CSPin_Init ( void )
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    GPIO_InitStructure.GPIO_Pin = SPL06_CS_PIN;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init ( SPL06_CS_GPIO, &GPIO_InitStructure );
//    GPIO_SetBits ( SPL06_CS_GPIO, SPL06_CS_PIN );
//}
/**********************************************************************************************************
*函 数 名: SPl06Enable
*功能说明: SPL06 使能
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
static void SPl06Enable ( uint8_t ena )
{
    if ( ena )
        SPL06_CS_ResetBits;
    else
        SPL06_CS_SetBits;
}
/**********************************************************************************************************
*函 数 名: SPl0601Write
*功能说明: SPL06 读和写
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
static void SPl0601Write ( unsigned char regadr, unsigned char val )
{
    SPl06Enable ( 1 );
    SPI_SingleWirteAndRead(2, regadr );
    SPI_SingleWirteAndRead(2, val );
    SPl06Enable ( 0 );
}
static uint8_t SPl0601Read ( unsigned char regadr )
{
    uint8_t reg_data;
    SPl06Enable ( 1 );
    SPI_SingleWirteAndRead(2, regadr | 0x80 );
    reg_data = SPI_SingleWirteAndRead(2, 0xff );
    SPl06Enable ( 0 );
    return reg_data;
}

/**********************************************************************************************************
*函 数 名: SPl0601RateSet
*功能说明:设置温度传感器的每秒采样次数以及过采样率
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void SPl0601RateSet ( uint8_t iSensor, uint8_t u8SmplRate, uint8_t u8OverSmpl )
{
    uint8_t reg = 0;
    int32_t i32kPkT = 0;
    switch ( u8SmplRate )
    {
    case 2:
        reg |= ( 1 << 4 );
        break;
    case 4:
        reg |= ( 2 << 4 );
        break;
    case 8:
        reg |= ( 3 << 4 );
        break;
    case 16:
        reg |= ( 4 << 4 );
        break;
    case 32:
        reg |= ( 5 << 4 );
        break;
    case 64:
        reg |= ( 6 << 4 );
        break;
    case 128:
        reg |= ( 7 << 4 );
        break;
    case 1:
    default:
        break;
    }
    switch ( u8OverSmpl )
    {
    case 2:
        reg |= 1;
        i32kPkT = 1572864;
        break;
    case 4:
        reg |= 2;
        i32kPkT = 3670016;
        break;
    case 8:
        reg |= 3;
        i32kPkT = 7864320;
        break;
    case 16:
        i32kPkT = 253952;
        reg |= 4;
        break;
    case 32:
        i32kPkT = 516096;
        reg |= 5;
        break;
    case 64:
        i32kPkT = 1040384;
        reg |= 6;
        break;
    case 128:
        i32kPkT = 2088960;
        reg |= 7;
        break;
    case 1:
    default:
        i32kPkT = 524288;
        break;
    }

    if ( iSensor == 0 )
    {
        p_spl0601->i32kP = i32kPkT;
        SPl0601Write ( 0x06, reg );
        if ( u8OverSmpl > 8 )
        {
            reg = SPl0601Read ( 0x09 );
            SPl0601Write ( 0x09, reg | 0x04 );
        }
    }
    if ( iSensor == 1 )
    {
        p_spl0601->i32kT = i32kPkT;
        SPl0601Write ( 0x07, reg | 0x80 ); //Using mems temperature
        if ( u8OverSmpl > 8 )
        {
            reg = SPl0601Read ( 0x09 );
            SPl0601Write ( 0x09, reg | 0x08 );
        }
    }

}

/**********************************************************************************************************
*函 数 名: SPl0601GetCalibrationParam
*功能说明: 获取校准参数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void SPl0601GetCalibrationParam ( void )
{
    uint32 h;
    uint32 m;
    uint32 l;
    h =  SPl0601Read ( 0x10 );
    l  =  SPl0601Read ( 0x11 );
    p_spl0601->calib_param.c0 = ( int16_t ) h << 4 | l >> 4;
    p_spl0601->calib_param.c0 = ( p_spl0601->calib_param.c0 & 0x0800 ) ? ( 0xF000 | p_spl0601->calib_param.c0 ) : p_spl0601->calib_param.c0;
    h =  SPl0601Read ( 0x11 );
    l  =  SPl0601Read ( 0x12 );
    p_spl0601->calib_param.c1 = ( int16_t ) ( h & 0x0F ) << 8 | l;
    p_spl0601->calib_param.c1 = ( p_spl0601->calib_param.c1 & 0x0800 ) ? ( 0xF000 | p_spl0601->calib_param.c1 ) : p_spl0601->calib_param.c1;
    h =  SPl0601Read ( 0x13 );
    m =  SPl0601Read ( 0x14 );
    l =  SPl0601Read ( 0x15 );
    p_spl0601->calib_param.c00 = ( int32_t ) h << 12 | ( int32_t ) m << 4 | ( int32_t ) l >> 4;
    p_spl0601->calib_param.c00 = ( p_spl0601->calib_param.c00 & 0x080000 ) ? ( 0xFFF00000 | p_spl0601->calib_param.c00 ) : p_spl0601->calib_param.c00;
    h =  SPl0601Read ( 0x15 );
    m =  SPl0601Read ( 0x16 );
    l =  SPl0601Read ( 0x17 );
    p_spl0601->calib_param.c10 = ( int32_t ) h << 16 | ( int32_t ) m << 8 | l;
    p_spl0601->calib_param.c10 = ( p_spl0601->calib_param.c10 & 0x080000 ) ? ( 0xFFF00000 | p_spl0601->calib_param.c10 ) : p_spl0601->calib_param.c10;
    h =  SPl0601Read ( 0x18 );
    l  =  SPl0601Read ( 0x19 );
    p_spl0601->calib_param.c01 = ( int16_t ) h << 8 | l;
    h =  SPl0601Read ( 0x1A );
    l  =  SPl0601Read ( 0x1B );
    p_spl0601->calib_param.c11 = ( int16_t ) h << 8 | l;
    h =  SPl0601Read ( 0x1C );
    l  =  SPl0601Read ( 0x1D );
    p_spl0601->calib_param.c20 = ( int16_t ) h << 8 | l;
    h =  SPl0601Read ( 0x1E );
    l  =  SPl0601Read ( 0x1F );
    p_spl0601->calib_param.c21 = ( int16_t ) h << 8 | l;
    h =  SPl0601Read ( 0x20 );
    l  =  SPl0601Read ( 0x21 );
    p_spl0601->calib_param.c30 = ( int16_t ) h << 8 | l;
}

/**********************************************************************************************************
*函 数 名: SPl0601StartTemperature
*功能说明: 发起一次温度或压力测量
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void SPl0601StartTemperature ( void )
{
    SPl0601Write ( 0x08, 0x02 );
}
/**********************************************************************************************************
*函 数 名: SPI0601StartPressure
*功能说明: 发起一次温度或压力测量
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void SPI0601StartPressure ( void )
{
    SPl0601Write ( 0x08, 0x01 );
}
/**********************************************************************************************************
*函 数 名: SPl0601StartContinuous
*功能说明: 发起一次温度或压力测量
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void SPl0601StartContinuous ( uint8_t mode )
{
    SPl0601Write ( 0x08, mode + 4 );
}

/**********************************************************************************************************
*函 数 名: SPl0601GetRawTemp
*功能说明: 获取原始温度值
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void SPl0601GetRawTemp ( void )
{
    uint8_t Spl06Temp[3] = {0};
    Spl06Temp[0] = SPl0601Read ( 0x03 );
    Spl06Temp[1] = SPl0601Read ( 0x04 );
    Spl06Temp[2] = SPl0601Read ( 0x05 );
    p_spl0601->i32rawTemperature = ( int32_t ) Spl06Temp[0] << 16 | ( int32_t ) Spl06Temp[1] << 8 | ( int32_t ) Spl06Temp[2];
    p_spl0601->i32rawTemperature = ( p_spl0601->i32rawTemperature & 0x800000 ) ? ( 0xFF000000 | p_spl0601->i32rawTemperature ) : p_spl0601->i32rawTemperature;
}

/**********************************************************************************************************
*函 数 名: SPl0601GetRawPressure
*功能说明: 获取原始压力值
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void SPl0601GetRawPressure ( void )
{
    uint8_t Spl06Pressure[3];
    Spl06Pressure[0] = SPl0601Read ( 0x00 );
    Spl06Pressure[1] = SPl0601Read ( 0x01 );
    Spl06Pressure[2] = SPl0601Read ( 0x02 );
    p_spl0601->i32rawPressure = ( int32_t ) Spl06Pressure[0] << 16 | ( int32_t ) Spl06Pressure[1] << 8 | ( int32_t )Spl06Pressure[2];
    p_spl0601->i32rawPressure = ( p_spl0601->i32rawPressure & 0x800000 ) ? ( 0xFF000000 | p_spl0601->i32rawPressure ) : p_spl0601->i32rawPressure;
}
/**********************************************************************************************************
*函 数 名: Drv_Spl0601_Init
*功能说明: Spl0601初始化
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
uint8_t Drv_Spl0601_Init ( void )
{
	/*片选初始化*/
	//SPL06CSPin_Init ();
    p_spl0601 = &spl0601;
    p_spl0601->i32rawPressure = 0;
    p_spl0601->i32rawTemperature = 0;
    p_spl0601->chip_id = SPl0601Read ( 0x0D );
    SPl0601GetCalibrationParam();
    SPl0601RateSet ( PRESSURE_SENSOR, 128, 16 );
    SPl0601RateSet ( TEMPERATURE_SENSOR, 8, 8 );
    SPl0601StartContinuous ( CONTINUOUS_P_AND_T );
	if(p_spl0601->chip_id == 0x10)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
/**********************************************************************************************************
*函 数 名: Spl0601GetTemperature
*功能说明: 获取温度值
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
float Spl0601GetTemperature ( void )
{
    float fTCompensate;
    float fTsc;
    fTsc = p_spl0601->i32rawTemperature / ( float ) p_spl0601->i32kT;
    fTCompensate =  p_spl0601->calib_param.c0 * 0.5 + p_spl0601->calib_param.c1 * fTsc;
    return fTCompensate;
}
/**********************************************************************************************************
*函 数 名: Spl0601GetPressure
*功能说明: 获取压力值
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
float Spl0601GetPressure ( void )
{
    float fTsc, fPsc;
    float qua2, qua3;
    float fPCompensate;
    fTsc = p_spl0601->i32rawTemperature / ( float ) p_spl0601->i32kT;
    fPsc = p_spl0601->i32rawPressure / ( float ) p_spl0601->i32kP;
    qua2 = p_spl0601->calib_param.c10 + fPsc * ( p_spl0601->calib_param.c20 + fPsc * p_spl0601->calib_param.c30 );
    qua3 = fTsc * fPsc * ( p_spl0601->calib_param.c11 + fPsc * p_spl0601->calib_param.c21 );   
    fPCompensate = p_spl0601->calib_param.c00 + fPsc * qua2 + fTsc * p_spl0601->calib_param.c01 + qua3;
    return fPCompensate;
}



/**********************************************************************************************************
*函 数 名: Drv_Spl0601_Read
*功能说明: 读取压力温度值
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
float Drv_SPl0601_Read ( void )
{
	  
    SPl0601GetRawTemp();
    Temperature = Spl0601GetTemperature();
    SPl0601GetRawPressure();
    BaroPressure = Spl0601GetPressure();
	
   return  BaroPressure;
}



/**********************************************************************************************************
*函 数 名: Spl0601Get
*功能说明: 可用压力值及温度值读取
*形    参: 高度值
*返 回 值: 无
**********************************************************************************************************/
void Spl0601Get(float *Hight)
{	
	*Hight = Drv_SPl0601_Read();
}
/**********************************************************************************************************
*函 数 名: GetTemputer
*功能说明: 获取温度值
*形    参: 温度值
*返 回 值: 无
**********************************************************************************************************/
int32_t GetTemputer(void)
{	
	return (int32_t)Temperature;
}






