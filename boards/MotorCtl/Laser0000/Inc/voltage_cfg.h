#ifndef __VOLTAGE_CFG_H__
#define __VOLTAGE_CFG_H__

#include "voltage.h"
#include "ad5245.h"
#include "project_cfg.h"
#define VOLTAGE_QUEUE_CNT 1
#define VOLTAGE_CHANNEL_CNT 2
#define VOLTAGE_IIC_CHANNEL_CNT 2
void voltage_hd_init(void);
void hd0_voltage_write_time(f32 voltage, u16 time);

#endif
