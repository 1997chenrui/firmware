#ifndef SERVOMOTOR_EPOS4_H_
#define SERVOMOTOR_EPOS4_H_


#include "bus.h"
#include <stdint.h>
#include "os_implement.h"
#include <stdbool.h>

#define WAIT_TYPE_REACH 0x10  //等待位置到达
#define WAIT_TYPE_QUICK_REACH 0x11 //等待快速位置到达 用在振荡指令里 
#define WAIT_TYPE_STOP  0x12  //等待停止
#define WAIT_TYPE_RESET 0x13 //等待复位完成


typedef enum _SERVO_CMD
{
	SERVO_CMD_ENABLE=0,  //使能
	SERVO_CMD_DISABLE,
	SERVO_CMD_CLEAR_ERROR,//清除错误
	SERVO_CMD_STOP,
	SERVO_CMD_GET_POSTION,//获取位置
	SERVO_CMD_GET_VELOCITY,//获取速度
	SERVO_CMD_GET_ERROR_ID,//获取错误码
	SERVO_CMD_GET_STATE,//获取状态
	SERVO_CMD_QUICK_STOP,//快速停止
}SERVO_CMD;


typedef struct servo_driver_struct
{
	uint8_t 			dir;
	int 				steps;
	int 				speed;
	uint32_t			acce;
	uint32_t			deacce;
	
	bus_os_sem 			ack_sem;
	bus_os_sem 			data_sem;

	uint16_t status;
    uint8_t   flg;
	
    uint32_t			 errCode;
	void (*send_bytes)(uint8_t *data,uint16_t len);
}servo_driver_struct;

extern servo_driver_struct servo_motors[];

typedef struct
{
	servo_driver_struct 	*motor;
	uint8_t		  			dir;
	int 		  			speed;
	uint32_t 	  			acce;
	uint32_t 	  			time;
    bool					isSendSyncSignal;
}servo_multi_rotate_args_t;


typedef struct
{
	servo_driver_struct 	*motor;
	uint8_t		  			dir;
	uint32_t				step;
	int 		  			speed;
	uint32_t 	  			acce;
	uint32_t 	  			time;
}servo_multi_move_args_t;


void EPOS4Driver_Init(servo_driver_struct *);


int32_t Servo_base_ctrl(servo_driver_struct *,SERVO_CMD ,uint8_t * );

void Servo_motor_reset(servo_driver_struct *motor);

void Servo_motor_rotate_start(servo_driver_struct *, bool , int , uint32_t );
void Servo_motor_shake(servo_driver_struct *, uint32_t ,uint32_t ,float ,uint32_t ,u32* );
void Servo_motor_move(servo_driver_struct *,int ,int ,int ,int ,uint8_t ,uint8_t );

void Servo_motor_rotate_two_dir_on_time(servo_driver_struct *, bool , int , uint32_t ,uint32_t );
void Servo_motor_rotate_shake_rotate(servo_driver_struct *, bool , int , uint32_t ,uint32_t ,uint32_t ,uint32_t ,float ,uint32_t ,bool , int , uint32_t	);
uint32_t Servo_motor_multi_rotate(int ,servo_multi_rotate_args_t* );
void servo_motor_rotate_shake_rotate_detect(servo_driver_struct *,u32,u32 ,u32,u32,	u8 ,u32,u8,u32,u32,u32,	u8,u32,u32,u32,u8 );	

#endif
