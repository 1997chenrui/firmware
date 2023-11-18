#ifndef SERVOMOTOR_EPOS2_H_
#define SERVOMOTOR_EPOS2_H_

#include "bus.h"
#include <stdint.h>
#include "os_implement.h"
#include <stdbool.h>


#define EPOS_IDX						0x0000
#define EPOS_IDX1						0x0100

#define EPOS_RD_HEAD					0x10
#define EPOS_WT_HEAD					0x11

#define VELOCITY_ACTUAL_VALUE_AVERAGED 0x2028
#define CURRENT_THRESHOLD_HOMING_MODE  0x2080
#define HOME_POSITION				   0x2081
                               
#define CONTROL_WORD				   0x6040
#define STATUS_WORD					   0x6041

#define MODES_OF_OPERATION			   0x6060
#define POSITIONA_CTUALVALUE		   0x6064
                               
#define TARGET_POSITION 			   0x607A
#define HOME_OFFSET 				   0x607C
                               
#define PROFILE_VELOCITY 			   0x6081
#define PROFILE_ACCELERATION		   0x6083
#define PROFILE_DECELERATION		   0x6084
#define MOTION_PROFILE_TYPE			   0x6086
                               
#define HOMING_METHOD 				   0x6098
#define HOMING_SPEEDS				   0x6099
#define HOMING_ACCELERATION  		   0x609A
                               
#define TARGET_VELOCITY				   0x60FF


#define SERVO_CALI_STATE_HIGHT 			0
#define SERVO_CALI_STATE_LOW			1
#define SERVO_CALI_VALUE_THOD			50000

static  uint8_t EPOS_RT_OK		=	0x4F;
static  uint8_t EPOS_RT_FAILE    =   0x46;
static  uint8_t EPOS_RT_HEAD	    =	0x00;


static unsigned short ccitt_table[256] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
	0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
	0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
	0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
	0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
	0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
	0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
	0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
	0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
	0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
	0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
	0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
	0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
	0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
	0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
	0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
	0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
	0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
	0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
	0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
	0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
	0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
	0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};
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
	epos_packet_struct	send_packet;
	epos_packet_struct	rt_packet;
	uint8_t  rt_data[sizeof(epos_packet_struct)];
	uint8_t  rt_idx;
	uint8_t  rt_state;
	uint8_t  rt_ack;
	uint16_t rt_data_check;
	
	volatile uint8_t	 is_crc_ok:1;
	uint8_t 			 is_move_cmd:1;		//包括Move Rotate
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
void EnableDevice(servo_driver_struct *motor);
void ShutdownDevice(servo_driver_struct *motor);
void ShutdownAndEnableDevice(servo_driver_struct *motor);
void ClearError(servo_driver_struct *motor);
void HomeMode_Activate(servo_driver_struct *motor);
void HomeParameter_Set(servo_driver_struct *motor, int homingAcceleration,int speedSwitch,int speedIndex,int homeOffset,int currentTreshold,int homePosition);
void HomeMethod_Set(servo_driver_struct *motor, int HomingMethod);
void Home_Start(servo_driver_struct *motor);
void ProfilePositionMode_Activate(servo_driver_struct *motor);
void PositionProfile_Set(servo_driver_struct *motor, int profileVelocity,int profileAcceleration,int profileDeceleration);
//void MoveToPosition(servo_driver_struct *motor, int targetPosition,uint8_t absolute,uint8_t immediately);
void TargetReached_Wait(servo_driver_struct *motor, uint32_t timeout);
void ProfileVelocityMode_Activate(servo_driver_struct *motor);
void VelocityProfile_Set(servo_driver_struct *motor, int profileAcceleration,int profileDeceleration);
void Velocity_Move(servo_driver_struct *motor, int targetVelocity);
void VelocityMovement_Halt(servo_driver_struct *motor);
void VelocityMovement_GoOn(servo_driver_struct *motor);
void Quickstop(servo_driver_struct *motor);
void VelocityMode_Start(servo_driver_struct *motor, int ProfileVelocity,int profileAcceleration,int profileDeceleration);
uint8_t moveInRotationalMode(servo_driver_struct *motor,uint32_t flag,int ProfileVelocity,int profileAcceleration,int profileDeceleration,uint32_t time);

uint8_t moveInShakeMode(servo_driver_struct *motor, uint32_t step,uint32_t amplitude,float frequency,uint32_t time);
void PositionStart(servo_driver_struct *motor,int profileVelocity,int profileAcceleration,int profileDeceleration,int targetPosition, uint8_t absolute,uint8_t immediately);
int Position_Get(servo_driver_struct *motor);
int Velocity_Get(servo_driver_struct *motor);
int Statusword_Get(servo_driver_struct *motor);
void servo_motor_quick_stop(servo_driver_struct *motor);

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
