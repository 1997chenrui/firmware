#ifndef TEMPSENSOR_DS18B20_H
#define TEMPSENSOR_DS18B20_H

#include <stdint.h>
#include <stdbool.h>
#include "ErrCode.h"
#include "os_implement.h"

/*************ROM命令****************/
#define  SkipROM    0xCC  //跳过ROM  
#define  SearchROM  0xF0  //搜索ROM  
#define  ReadROM    0x33  //读ROM(总线仅有一个数据时使用)  
#define  MatchROM   0x55  //匹配ROM  
#define  AlarmROM   0xEC  //警报搜索

/*************功能命令****************/
#define  StartConvert    0x44  //温度转换指令.
#define  ReadScratchpad  0xBE  //读暂存指令  
#define  WriteScratchpad 0x4E  //写暂存指令  
#define  CopyScratchpad  0x48  //拷贝暂存寄存器
#define  RecallEEPROM    0xB8    //召回 EEPROM 
#define  ReadPower       0xB4    //读供电方式:0寄生电源;1外接电源  
#define DS18B20_RAM_SIZE   9


typedef struct{
	uint8_t TempLow;
	uint8_t TempHigh;
	uint8_t AlarmHigh;
	uint8_t AlarmLow;
	uint8_t ConfigReg;
	uint8_t ReservedReg0;
	uint8_t ReservedReg1;
	uint8_t ReservedReg2;
	uint8_t crc;
}TempSensorRAM_TypeDef;  

#define DS_PRECISION 0x7f //12Bit精度。0x1f:9Bit    0x3f:10Bit   0x5f:11Bit 
#define DS_AlarmTH  0x64  
#define DS_AlarmTL  0x8a  
#define DS_CONVERT_TICK 1000  

typedef struct
{
	void (*set_dq_state)(uint8_t val);
	uint8_t (*get_dq_state)();
    float current_temp;
    bus_os_sem temp_read_mutex;
}TempSensor_Struct;


float GetTemp_DS18B20_median_filter(TempSensor_Struct *ths);

float GetTemp_DS18B20(TempSensor_Struct *ths);

void DS18B20_Init(TempSensor_Struct *ths,void (*set)(uint8_t val),uint8_t (*get)());

float GetTemp_Buffer(TempSensor_Struct *ths);

#endif
