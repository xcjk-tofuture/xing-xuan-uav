#ifndef __SPL06_H
#define __SPL06_H

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "timers.h"
#include "parameter.h"
#include "spi.h"


#define uint32 unsigned int
	
#define CONTINUOUS_PRESSURE     1
#define CONTINUOUS_TEMPERATURE  2
#define CONTINUOUS_P_AND_T      3
#define PRESSURE_SENSOR     0
#define TEMPERATURE_SENSOR  1

struct spl0601_calib_param_t {	
    int16_t c0;
    int16_t c1;
    int32_t c00;
    int32_t c10;
    int16_t c01;
    int16_t c11;
    int16_t c20;
    int16_t c21;
    int16_t c30;       
};

struct spl0601_t 
{	
    struct spl0601_calib_param_t calib_param;/**<calibration data*/	
    uint8_t 			chip_id; /**<chip id*/	
    int32_t 	i32rawPressure;
    int32_t 	i32rawTemperature;
    int32_t 	i32kP;    
    int32_t 	i32kT;
}; 

//void SPL06CSPin_Init(void);
uint8_t Drv_Spl0601_Init(void);
void Spl0601Get(float *Hight);
int32_t GetTemputer(void);
float Get_Altitude_SPL06_001(float baroPress);





#endif 