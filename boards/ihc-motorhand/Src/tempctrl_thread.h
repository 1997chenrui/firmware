#ifndef TEMP_CTRL_THREAD_H_
#define TEMP_CTRL_THREAD_H_
#include "os_implement.h"
#include "TempSensor_DS18B20.h"
#include "TempCtrl.h"

#define TempCtrl_Channel_Cnt 1

typedef struct
{
	u8 is_start;
	PID_Ctrl_Struct     PID_Parm;
	TempSensor_Struct   sensor0;
	TempSensor_Struct   sensor1;
	TempSensor_Struct   sensor2;
}TempCtrl_Struct;
extern TempCtrl_Struct    temp_ctrl_mod[];
void TempCtrl_Init(void);
float TempCtrl_GetTemp(int channelId, bool buffer);
void TempCtrl_Close(int channelId);
void TempCtrl_Open(int channelId);
#endif
