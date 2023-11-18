#ifndef SERVO_HW_H_
#define SERVO_HW_H_
#include "bus.h"
#include "EPOS4Device.h"

//最小单位为2字节
typedef enum
{
	EPOS_WORD = 2,
	EPOS_DBWORD = 4,
}epos_len;

typedef struct
{
    u32         cmd;
    u8          adapterId;
    u8          srcId;
    u8          sessionId;        //return 需要用到
    void       *send;            //send指针

}ServoMotor_Queue_Ele_t;

extern servo_driver_struct 		servo_motors[];

void ServoMotorThread_EnQueue(int idx,ServoMotor_Queue_Ele_t *data);
void ServoMotor_Mod_Init(void);

#endif
