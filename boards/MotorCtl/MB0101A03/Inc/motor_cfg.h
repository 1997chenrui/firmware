#ifndef __MOTOR_CFG_H__
#define __MOTOR_CFG_H__

#include "motor_thread.h"
#include "mTMC5130Device.h"

#define MOTOR_CNT 4
#define MOTOR_THEARD_CNT    MOTOR_CNT

void MotorInit(void);
void motor_move_done_notify(u8 idx);
#endif
