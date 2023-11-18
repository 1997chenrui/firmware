#ifndef _VOLTAGE_CFG_H_
#define _VOLTAGE_CFG_H_
#include "voltage.h"


#include "soft_iic.h"

#define VOLTAGE_QUEUE_CNT 0
#define VOLTAGE_CHANNEL_CNT 4
extern void voltage_hd_init(void);


extern void hd0_voltage_write(f32 voltagef);

extern void hd1_voltage_write(f32 voltagef);

extern void hd2_voltage_write(f32 voltagef);

extern void hd3_voltage_write(f32 voltagef);

#endif

