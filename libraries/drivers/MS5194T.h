#ifndef MS5194T_H_
#define MS5194T_H_
//#include "stm32f4xx_hal.h"
#include "os_implement.h"
#include "stm32f4xx_hal.h"

#define R_STATUS        0x40
#define R_MODE          0x48
#define W_MODE          0x08

#define R_CONFIGURATION 0x50
#define W_CONFIGURATION 0x10

#define R_ID            0x60 //ID寄存器 （读设备ID）  

#define R_DATA          0x58




typedef void (*NssFunc)(u8);
typedef struct MS5194TCONFIG
{
	SPI_HandleTypeDef * adusedspi;
	u8 channel_cnt;	
	NssFunc nssfunc;
	
}MS5194TCONFIG;

u16 MS5194T_GetDataAverage(MS5194TCONFIG* self, u8 channel, u16 average_cnt);
u16 MS5194T_GetSingleData(MS5194TCONFIG* self, u8 channel);

void MS5194T_Init(MS5194TCONFIG* self, SPI_HandleTypeDef * spi,u8 channel_cnt,NssFunc nssf);
u16 MS5194TAllRead(MS5194TCONFIG* self,u16* adData);
void MS5194TAllRead_average(MS5194TCONFIG* self,u16 *adData, u8 average_cnt);



f32 MS5194T_GetData(MS5194TCONFIG* self, u8 channel_id);
void MS5194T_Init(MS5194TCONFIG* self, SPI_HandleTypeDef * spi,u8 channel_cnt,NssFunc nssf);


#endif




