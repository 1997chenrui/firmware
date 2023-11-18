#ifndef TEMP_CTRL_THREAD_H_
#define TEMP_CTRL_THREAD_H_
#include "os_implement.h"
#include "TempSensor_DS18B20.h"
#include "TempCtrl.h"
#include "pid.h"
#include "bus.h"

typedef struct
{
	u8 is_start;
	Pid_cfg_t pid_cfg;
    void (*open)(void);
    void (*close)(void);
    void (*target_set)(f32);
    
    void (*open_report)(bus_packet* packet);
    void (*close_report)(bus_packet* packet);
}TempCtrl_Struct;

void TempCtrl_Init(void (*thread_create)(void (*thread)(void* data)));
TempCtrl_Struct* tempctrl_get_as_ref(void);

#endif
