#ifndef __DS18B20_H__
#define __DS18B20_H__
#include "os_implement.h"
#include "bus.h"
#include "macro.h"

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
	u8 TempLow;
	u8 TempHigh;
	u8 AlarmHigh;
	u8 AlarmLow;
	u8 ConfigReg;
	u8 ReservedReg0;
	u8 ReservedReg1;
	u8 ReservedReg2;
	u8 crc;
}TempSensorRAM_TypeDef;  

#define DS_PRECISION 0x7f //12Bit精度。0x1f:9Bit    0x3f:10Bit   0x5f:11Bit 
#define DS_AlarmTH  0x64  
#define DS_AlarmTL  0x8a  
#define DS_CONVERT_TICK 1000  

typedef struct _Ds_sensor_t
{
	void (*mfunc_set_dq)(u8 val);
	u8 (*mfunc_get_dq)(void);
    float current_temp;
    bus_os_sem mutex;
	u8 is_median_filter;
	float (*read_temperature)(struct _Ds_sensor_t*);
	float (*read_temperature_median_filter)(struct _Ds_sensor_t*);
	float (*read_buffer_temperature)(struct _Ds_sensor_t*);
}Ds_sensor_t;


Ds_sensor_t * ds_sensor_new(void (*set)(u8 val), u8 (*get)());

#endif
