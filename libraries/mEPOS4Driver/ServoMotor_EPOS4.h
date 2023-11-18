#ifndef SERVOMOTOR_EPOS4_H_
#define SERVOMOTOR_EPOS4_H_


#include "bus.h"
#include <stdint.h>
#include "os_implement.h"
#include <stdbool.h>

///*�Ĵ�������*/
//#define EPOS_IDX						0x0000
//#define EPOS_IDX1						0x0100

///*ָ��ͷ*/
//#define EPOS_RD_HEAD					0x60
//#define EPOS_WT_HEAD					0x68

///*�Ĵ����б�*/
//#define VELOCITY_ACTUAL_VALUE_AVERAGED 0x2028
//#define CURRENT_THRESHOLD_HOMING_MODE  0x2080
//#define HOME_POSITION				   0x2081
//                               
//#define CONTROL_WORD				   0x6040
//#define STATUS_WORD					   0x6041

//#define MODES_OF_OPERATION			   0x6060
//#define POSITIONA_CTUALVALUE		   0x6064
//                               
//#define TARGET_POSITION 			   0x607A
//#define HOME_OFFSET 				   0x607C
//                               
//#define PROFILE_VELOCITY 			   0x6081
//#define PROFILE_ACCELERATION		   0x6083
//#define PROFILE_DECELERATION		   0x6084
//#define MOTION_PROFILE_TYPE			   0x6086
//                               
//#define HOMING_METHOD 				   0x6098
//#define HOMING_SPEEDS				   0x6099
//#define HOMING_ACCELERATION  		   0x609A
//                               
//#define TARGET_VELOCITY				   0x60FF


//#define SERVO_CALI_STATE_HIGHT 			0
//#define SERVO_CALI_STATE_LOW			1
//#define SERVO_CALI_VALUE_THOD			50000

//static  uint8_t EPOS_RT_OK		=	0x4F;
//static  uint8_t EPOS_RT_FAILE    =   0x46;
//static  uint8_t EPOS_RT_HEAD	    =	0x00;

/*
typedef struct
{

	uint8_t opcode;
	uint8_t len;
	uint8_t data[256];		//king CRC包含在data中
}epos_packet_struct;
*/
//servo_driver_struct * ths,uint8_t head,uint16_t reg_addr,uint16_t sub_index,uint8_t *dataIn,uint8_t bytesOfdataIn
typedef struct
{
	uint8_t opcode;
	uint8_t len;
	uint16_t reg_addr;
	uint16_t sub_index;
	uint8_t data[4];			//0,2,4
	uint16_t crc;
}epos_packet_struct;


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


void EPOS2Driver_Init(servo_driver_struct *ths,void (*send_bytes)(uint8_t *data, uint16_t length));

void ShutdownAndEnableDevice(servo_driver_struct *motor);

void HomeMode_Activate(servo_driver_struct *motor);
void HomeParameter_Set(servo_driver_struct *motor, int homingAcceleration,int speedSwitch,int speedIndex,int homeOffset,int currentTreshold,int homePosition);
void HomeMethod_Set(servo_driver_struct *motor, int HomingMethod);
void Home_Start(servo_driver_struct *motor);
void ProfilePositionMode_Activate(servo_driver_struct *motor);
void PositionProfile_Set(servo_driver_struct *motor, int profileVelocity,int profileAcceleration,int profileDeceleration);
//void MoveToPosition(servo_driver_struct *motor, int targetPosition,uint8_t absolute,uint8_t immediately);
void TargetReached_Wait(servo_driver_struct *motor, uint32_t timeout,uint32_t n);
void ProfileVelocityMode_Activate(servo_driver_struct *motor);
void VelocityProfile_Set(servo_driver_struct *motor, int profileAcceleration,int profileDeceleration);
void Velocity_Move(servo_driver_struct *motor, int targetVelocity);

void VelocityMovement_GoOn(servo_driver_struct *motor);
void Quickstop(servo_driver_struct *motor);
void VelocityMode_Start(servo_driver_struct *motor, int ProfileVelocity,int profileAcceleration,int profileDeceleration);
uint8_t moveInRotationalMode(servo_driver_struct *motor,uint32_t flag,int ProfileVelocity,int profileAcceleration,int profileDeceleration,uint32_t time);

uint8_t moveInShakeMode(servo_driver_struct *motor, uint32_t step,uint32_t amplitude,float frequency,uint32_t time);
void PositionStart(servo_driver_struct *motor,int profileVelocity,int profileAcceleration,int profileDeceleration,int targetPosition, uint8_t absolute,uint8_t immediately);
int Position_Get(servo_driver_struct *motor);
int Velocity_Get(servo_driver_struct *motor);
uint16_t Statusword_Get(servo_driver_struct *motor);


uint8_t FindHome(servo_driver_struct *motor);

void servo_motor_reset(servo_driver_struct *motor);
void servo_motor_rotate_stop(servo_driver_struct *motor);
void servo_motor_rotate_start(servo_driver_struct *motor, bool dir, int speed, uint32_t acce);
void servo_motor_shake(servo_driver_struct *motor, uint32_t step,uint32_t amplitude,float frequency,uint32_t time,u32* step2zero);
void servo_motor_move(servo_driver_struct *motor,int profileVelocity,int profileAcceleration,
	int profileDeceleration,int targetPosition,uint8_t absolute,uint8_t immediately);
void servo_motor_base_ctrl(servo_driver_struct *motor,uint8_t option);
void servo_motor_rotate_two_dir_on_time(servo_driver_struct *motor, bool dir, int speed, uint32_t acce,uint32_t time);
void servo_motor_rotate_shake_rotate(servo_driver_struct *motor, bool dir, int speed, uint32_t acce,uint32_t time,
									uint32_t step,uint32_t amplitude,float frequency,uint32_t time1,
									bool dir2, int speed2, uint32_t acce2
									);
uint32_t servo_motor_multi_rotate(int cnt,servo_multi_rotate_args_t* args);


void epos_cmd_notify_byte_received(servo_driver_struct * ths, uint8_t data);


#endif
