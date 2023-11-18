#ifndef ADCDETECTHANDLE_H_
#define ADCDETECTHANDLE_H_

#include "bus.h"
#include <stdbool.h>

void ADCDetectCmdExcute(u32 func_code  ,  bus_packet * packet );



extern volatile bool LightAutoCaliStart;



#endif

