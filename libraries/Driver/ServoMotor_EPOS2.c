#include "ServoMotor_EPOS2.h"
#include <stdlib.h>
#include <stdbool.h>
#include "bus.h"
#include "errCode.h"
#include "async.h"

static uint32_t Ready_Switch_On = 0x00;
static uint32_t Profile_Velocity_Mode = 0x03;
static uint32_t Profile_Velocity = 0x00;
static uint32_t Active_Home = 0x0006;
static uint32_t Shutdown = 0x0006;
static uint32_t Switchon_Enable = 0x000F;
static uint32_t Start_Home = 0x001F;
static uint32_t Profile_Position_Mode = 0x01;
//static uint32_t Profile_Position_Sin = 0x01;
static uint32_t Absolute_Immediately = 0x003F;
static uint32_t Absolute_Flag = 0x001F;
static uint32_t Relative_Immediately = 0x007F;
static uint32_t Relative_Flag = 0x005F;
static uint32_t Halt_Profile_Velocity_Mode = 0x010F;
static uint32_t GoOn_Profile_Velocity_Mode = 0x000F;
static uint32_t Quick_Stop = 0x000B;
static uint32_t Clear_Error = 0x0080;
       
//static uint32_t Index_Negative = 33;
static uint32_t Positive_Speed = 34;


#define TIME_OUT_COMPEN  4000
#define TIME_COMPEN(val) (val*1000+TIME_OUT_COMPEN)

#define EPOS_RT_CMD_ACK          0
#define EPOS_RT_DATA_HEAD        1
#define EPOS_RT_DATA_LEN         2      
#define EPOS_RT_DATA             3   
#define EPOS_RT_DATA_CRC         4  

//#define EPOS_BYTES_PACKET(val) 	((ths->send_packet.len + 3)<<1)
//#define	EPOS_BYTES_DATA(val) 	((ths->send_packet.len + 1)<<1)
#define EPOS_BYTES_PACKET(val) 	((val + 3)<<1)
#define	EPOS_BYTES_DATA(val) 	((val + 1)<<1)

#define EPOS_INIT_VAR()		ths->rt_state = EPOS_RT_CMD_ACK;			\
							ths->rt_data_check = 0;						\
							ths->rt_idx = 0;												

#define CHECK_SERVO_DRIVER_ERR()		if(motor->errCode != 0)			\
											return;

#define	IS_RESEND_CMD()			if(ths->errCode != 0)					\
								{										\
									if(is_resend)								\
									{									\
										Async_Excute(1,printf,"epos_send_cmd_to_driver pend err\r\n");	\
										ths->errCode = ERR_WAIT_REACH_TIME_OUT;	\
									}									\
									else								\
									{									\
										ths->errCode = 0;				\
									}									\
								}
// 									EPOS_INIT_VAR();					
// 									return;								
								
static void epos_send_cmd_to_driver(servo_driver_struct * ths,uint8_t head,uint16_t reg_addr,uint16_t sub_index,uint8_t *dataIn,uint8_t bytesOfDataIn,bool is_resend);
static inline void can_resend_epos_send_cmd_to_driver(servo_driver_struct * ths,uint8_t head,uint16_t reg_addr,uint16_t sub_index,uint8_t *dataIn,uint8_t bytesOfDataIn);

static void  EPOS_CHECK_ACK(servo_driver_struct *ths,uint8_t * data, int length)
{
	ths->is_crc_ok = 0;
    for(uint8_t i=0;i<3;i++)
	{
		ths->send_bytes(&ths->send_packet.opcode,1);
		if( bus_os_pend_sem(&ths->ack_sem, 500) == 0)
		{
			if(ths->rt_ack == EPOS_RT_OK)  
			{
				ths->send_bytes(data+1,length-1);
				if(bus_os_pend_sem(&ths->ack_sem,500) == 0)
				{
					if(ths->rt_ack == EPOS_RT_OK)  
					{		
						ths->is_crc_ok = 1;
						break;
					}
					else 
					{
						//bus_os_sleep(500);	
						//crc ack epos fail
						Async_Excute(1,printf,"caf err\r\n");                 
						continue;                      
					} 	
				}
				else
				{
					//bus_os_sleep(500);	
					//crc ack pend err
					Async_Excute(1,printf,"cap err\r\n");					
					continue;	
				}
			}			
			else
			{                                  
				//bus_os_sleep(500);
				//head ack faile				
				Async_Excute(1,printf,"haf err\r\n");					               
				continue;                      
			}								
		}
		else
		{
			//bus_os_sleep(500);	
			//head ack pend err
			Async_Excute(1,printf,"hap err\r\n");	
			continue;
		}																		
	}
	if(!ths->is_crc_ok)
	{
		ths->errCode = ERR_SERVO_DRIVER_ACK;
		return;
    }
}



static union Data_U32_Struct{
	struct 
	{
		uint16_t data1;
		uint16_t data2;
	}data;
	uint32_t data_u32;
}swap_dus;

#define sw16(x) \
	((uint16_t)(						\
	(((uint16_t)(x)& (uint16_t)0x00ff) << 8) | \
	(((uint16_t)(x)& (uint16_t)0xff00) >> 8)))


//servo_driver_struct servo_motors[SERVOMOTOR_CNT];

static bool TargetReached_Get(servo_driver_struct *motor)
{
	can_resend_epos_send_cmd_to_driver(motor,EPOS_RD_HEAD,STATUS_WORD,EPOS_IDX,(uint8_t *)(&Ready_Switch_On),0);
	//get STATUS_WORD 00 03 00 00 00 00 37 95 00 00 AB 17 
	return(motor->rt_data[7]>>2) & 1;
}
/*----------------------------------------------Home Mode----------------------------------------*/
void HomeMode_Activate(servo_driver_struct *motor)
{
	//Activate home Mode
	can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,MODES_OF_OPERATION,EPOS_IDX,(uint8_t *)(&Active_Home),4);
	
}
void HomeParameter_Set(servo_driver_struct *motor, int homingAcceleration,int speedSwitch,int speedIndex,int homeOffset,int currentTreshold,int homePosition)
{
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,HOME_OFFSET,EPOS_IDX,(uint8_t *)(&homeOffset),4);
	 CHECK_SERVO_DRIVER_ERR();
	//speedSwitch 0x6099-01
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,HOMING_SPEEDS,EPOS_IDX1,(uint8_t *)(&speedSwitch),4);
	 CHECK_SERVO_DRIVER_ERR();
				//speedIndex 0x6099-02
				// epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,0x03,HOMING_SPEEDS,0x00,0x02,speedIndex,4);
				//homingAcceleration 609A-00
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,HOMING_ACCELERATION,EPOS_IDX,(uint8_t *)(&homingAcceleration),4);
	CHECK_SERVO_DRIVER_ERR();
	//currentTreshold 0x2080-00
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CURRENT_THRESHOLD_HOMING_MODE,EPOS_IDX,(uint8_t *)(&currentTreshold),4);
	CHECK_SERVO_DRIVER_ERR();
	//homePosition 0x2081-00
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,HOME_POSITION,EPOS_IDX,(uint8_t *)(&homePosition),4);
	CHECK_SERVO_DRIVER_ERR();
}
void HomeMethod_Set(servo_driver_struct *motor, int HomingMethod)
{
	//Homing Method 0x6098-00
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,HOMING_METHOD,EPOS_IDX,(uint8_t *)(&HomingMethod),4);
}

void Home_Start(servo_driver_struct *motor)
{
	//0x6040-00 0x001F
	 epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CONTROL_WORD,EPOS_IDX,(uint8_t *)(&Start_Home),4,false);
}
/*----------------------------------------------------ProfilePositionMode-----------------------------------------*/
void ProfilePositionMode_Activate(servo_driver_struct *motor)
{
	//Modes of Operation 0x6060-00 0x01 (Profile Position Mode)
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,MODES_OF_OPERATION,EPOS_IDX,(uint8_t *)(&Profile_Position_Mode),2);
}
void PositionProfile_Set(servo_driver_struct *motor, int profileVelocity,int profileAcceleration,int profileDeceleration)
{
	/*Profile Velocity0x6081-00
	Profile Acceleration0x6083-00
	Profile Deceleration0x6084-00
	set motion profile0x6068 - 01
	*/
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,MOTION_PROFILE_TYPE,EPOS_IDX,(uint8_t *)(&Ready_Switch_On),2);
	CHECK_SERVO_DRIVER_ERR();
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,PROFILE_VELOCITY,EPOS_IDX,(uint8_t *)(&profileVelocity),4);
	CHECK_SERVO_DRIVER_ERR();
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,PROFILE_ACCELERATION,EPOS_IDX,(uint8_t *)(&profileAcceleration),4);
	CHECK_SERVO_DRIVER_ERR();
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,PROFILE_DECELERATION,EPOS_IDX,(uint8_t *)(&profileDeceleration),4);
}

//king ok
void TargetReached_Wait(servo_driver_struct *motor, uint32_t timeout)
{
	uint8_t index = 0;
	uint32_t start_tick = 0,current_tick = 0;
	start_tick = OSTimeGet();
	do
	{
		bus_os_sleep(100);
		current_tick = OSTimeGet();
		//?????00 03 00 00 00 00 37 95 00 00 AB 17 
		index = TargetReached_Get(motor);
		CHECK_SERVO_DRIVER_ERR();
	}while(index == 0 && current_tick - start_tick < timeout);
	if(current_tick - start_tick > timeout)
	{
		printf("servo move time out\r\n");
		motor->errCode = ERR_WAIT_REACH_TIME_OUT;
	}
}


/*-------------------------------ProfileVelocityMode----------------------------------*/
void ProfileVelocityMode_Activate(servo_driver_struct *motor)
{
	//Modes of Operation 6060-00
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,MODES_OF_OPERATION,EPOS_IDX,(uint8_t *)(&Profile_Velocity_Mode),2);
}
void VelocityProfile_Set(servo_driver_struct *motor, int profileAcceleration,int profileDeceleration)
{	//Max. Acceleration 0x6083-00
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,PROFILE_ACCELERATION,EPOS_IDX,(uint8_t *)(&profileAcceleration),4);
	CHECK_SERVO_DRIVER_ERR();
	//Max. Deceleration 0x6084-00
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,PROFILE_DECELERATION,EPOS_IDX,(uint8_t *)(&profileDeceleration),4);
}
void Velocity_Move(servo_driver_struct *motor, int targetVelocity)
{
	//set target velocity 0x60FF-00
	 epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,TARGET_VELOCITY,EPOS_IDX,(uint8_t *)(&targetVelocity),4,false);
}
void VelocityMovement_Halt(servo_driver_struct *motor)
{
	 epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CONTROL_WORD,EPOS_IDX,(uint8_t *)(&Halt_Profile_Velocity_Mode),2,false);
}
void VelocityMovement_GoOn(servo_driver_struct *motor)
{
	 epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CONTROL_WORD,EPOS_IDX,(uint8_t *)(&GoOn_Profile_Velocity_Mode),2,false);
}
void VelocityMode_Start(servo_driver_struct *motor, int ProfileVelocity,int profileAcceleration,int profileDeceleration)
{
	ProfileVelocityMode_Activate(motor);
	CHECK_SERVO_DRIVER_ERR();
	VelocityProfile_Set(motor,profileAcceleration,profileDeceleration);
	CHECK_SERVO_DRIVER_ERR();
	//ShutdownAndEnableDevice();
	Velocity_Move(motor,ProfileVelocity);
	CHECK_SERVO_DRIVER_ERR();
	VelocityMovement_GoOn(motor);
}

int Position_Get(servo_driver_struct *motor)
{
	int item;
	//10 01 64 60 00 02 A2 16
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_RD_HEAD,POSITIONA_CTUALVALUE,EPOS_IDX,0,0);
	//current position
	item = motor->rt_data[9] << 24 | motor->rt_data[8] << 16 | motor->rt_data[7] << 8 | motor->rt_data[6];
	return item;
}

int Velocity_Get(servo_driver_struct *motor)
{
	int item;
	//10 01 28 20 00 02 A2 16
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_RD_HEAD,VELOCITY_ACTUAL_VALUE_AVERAGED,EPOS_IDX,0,0);
	//current velocity
	item = motor->rt_data[9] << 24 | motor->rt_data[8] << 16 | motor->rt_data[7] << 8 | motor->rt_data[6];
//	printf("current speed = %d\r\n",item);
	return item;
}

int Statusword_Get(servo_driver_struct *motor)
{
	int item;
	//10 01 41 60 00 02 A2 16
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_RD_HEAD,STATUS_WORD,EPOS_IDX,0,0);
	//current state
	item = motor->rt_data[9] << 24 | motor->rt_data[8] << 16 | motor->rt_data[7] << 8 | motor->rt_data[6];
	return item;
}
int GetServomotorErr(servo_driver_struct *motor)
{
	return (Statusword_Get(motor)>>3 & 2);
}

void ClearError(servo_driver_struct *motor)
{
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CONTROL_WORD,EPOS_IDX,(uint8_t *)(&Clear_Error),2);
}

void EnableDevice(servo_driver_struct *motor)
{
	//11 3 40 60 0 1 f 0 0 0 52 ef 
	//0x6040-00 0x0006 0x000F
	//send:11 03 40 60 00 01 06 00 00 00 C3 71
	//验证:?:11,03,60,40,01,00,00,06,00,00
	//11 03 40 60 00 01 0F 00 00 00 52 EF
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CONTROL_WORD,EPOS_IDX1,(uint8_t *)(&Switchon_Enable),4);
}
void ShutdownDevice(servo_driver_struct *motor)
{
	// epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,0x03,CONTROL_WORD,EPOS_IDX1,Shutdown,2);
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CONTROL_WORD,EPOS_IDX1,(uint8_t *)(&Shutdown),4);
}
void ShutdownAndEnableDevice(servo_driver_struct *motor)
{
	ShutdownDevice(motor);
	CHECK_SERVO_DRIVER_ERR();
	EnableDevice(motor);
}

void MoveToPosition(servo_driver_struct *motor, int targetPosition,uint8_t absolute,uint8_t immediately)
{
	//Target Position 0x607A-00 Desired Position
	 can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,TARGET_POSITION,EPOS_IDX,(uint8_t *)(&targetPosition),4);
	CHECK_SERVO_DRIVER_ERR();
	if(absolute)
		if(immediately)
			//absolute pos., start immediately 0x6040-00 0x003F
			 epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CONTROL_WORD,EPOS_IDX,(uint8_t *)(&Absolute_Flag),2,false);
		else
			//absolute pos
			 epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CONTROL_WORD,EPOS_IDX,(uint8_t *)(&Absolute_Immediately),2,false);
	else
		if(immediately)
		//relative immediately 0x007F
			 epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CONTROL_WORD,EPOS_IDX,(uint8_t *)(&Relative_Immediately),2,false);
		else
			//relative 0x005F
			 epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CONTROL_WORD,EPOS_IDX,(uint8_t *)(&Relative_Flag),2,false);
}

void servo_motor_quick_stop(servo_driver_struct *motor)
{
	can_resend_epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CONTROL_WORD,EPOS_IDX,(uint8_t *)(&Quick_Stop),2);	
}

void servo_motor_reset(servo_driver_struct *motor)
{
	ClearError(motor);
	CHECK_SERVO_DRIVER_ERR();
	HomeMode_Activate(motor);
	CHECK_SERVO_DRIVER_ERR();
	HomeParameter_Set(motor,2000,4000,0,300,500,0);
	CHECK_SERVO_DRIVER_ERR();
	HomeMethod_Set(motor,Positive_Speed);
	CHECK_SERVO_DRIVER_ERR();
	ShutdownAndEnableDevice(motor);
	CHECK_SERVO_DRIVER_ERR();
	bus_os_sleep(50);
	Home_Start(motor);
	CHECK_SERVO_DRIVER_ERR();
	TargetReached_Wait(motor,10000);
	CHECK_SERVO_DRIVER_ERR();
}

void servo_motor_rotate_stop(servo_driver_struct *motor)
{
	int index;
	uint32_t speed = 0;
//	uint32_t start_tick = 0,current_tick = 0;
//	start_tick = OSTimeGet();
	motor->errCode = 0;
	VelocityMovement_Halt(motor);
	CHECK_SERVO_DRIVER_ERR();
//	do
//	{
//		current_tick = OSTimeGet();
//		bus_os_sleep(100);
//		speed = Velocity_Get(motor);
//		CHECK_SERVO_DRIVER_ERR();
//	}while(speed!=0 && current_tick - start_tick < TIME_COMPEN(abs(motor->speed)/motor->deacce));
	do
	{
		bus_os_sleep(10);
		index = TargetReached_Get(motor);
		CHECK_SERVO_DRIVER_ERR();
	}while(index == 0);
	//current_tick - start_tick > TIME_COMPEN(abs(motor->speed)/motor->deacce) || 
	if(motor->errCode != 0)
	{
		printf("rotate stop time out\r\n");
		motor->errCode = ERR_WAIT_REACH_TIME_OUT;
	}
}
void servo_motor_rotate_start(servo_driver_struct *motor, bool dir, int speed, uint32_t acce)
{
	int index = 1;
	motor->errCode = 0;
	motor->speed = speed;
	motor->acce = acce;
	dir==0?(speed = -abs(speed)):(speed = abs(speed));
	VelocityMode_Start(motor,speed,acce,acce);
	CHECK_SERVO_DRIVER_ERR();
	TargetReached_Wait(motor,TIME_COMPEN(abs(speed)/acce));
	CHECK_SERVO_DRIVER_ERR();
//	while(Velocity_Get(motor) != speed)
//	{
//		bus_os_sleep(100);
//	}
	do
	{
		bus_os_sleep(10);
		index = TargetReached_Get(motor);
		CHECK_SERVO_DRIVER_ERR();
	}while(index == 0);
	printf("speed = %d\r\n",Velocity_Get(motor));
}

void servo_motor_rotate_two_dir_on_time(servo_driver_struct *motor, bool dir, int speed, uint32_t acce,uint32_t time)
{
	int index;
	int cSpeed;
	motor->errCode = 0;
	motor->speed = speed;
	motor->acce = acce;
	dir==0?(speed = -abs(speed)):(speed = abs(speed));
	
	ProfileVelocityMode_Activate(motor);
	CHECK_SERVO_DRIVER_ERR();
	
	VelocityProfile_Set(motor,acce,acce);
	CHECK_SERVO_DRIVER_ERR();
	Velocity_Move(motor,speed);
	CHECK_SERVO_DRIVER_ERR();
	VelocityMovement_GoOn(motor);
	
	do
	{
		bus_os_sleep(10);
		index = TargetReached_Get(motor);
		CHECK_SERVO_DRIVER_ERR();
	}while(index == 0);
	
	bus_os_sleep(time);
	
	//开始停下
	VelocityMovement_Halt(motor);
	CHECK_SERVO_DRIVER_ERR();
	do
	{
		bus_os_sleep(10);
		index = TargetReached_Get(motor);
		CHECK_SERVO_DRIVER_ERR();
	}while(index==0);
	
	//开始反向
	Velocity_Move(motor,-speed);
	CHECK_SERVO_DRIVER_ERR();
	VelocityMovement_GoOn(motor);
	
	do
	{
		bus_os_sleep(10);
		index = TargetReached_Get(motor);
		CHECK_SERVO_DRIVER_ERR();
	}while(index == 0);
	bus_os_sleep(time);
	//开始停下
	VelocityMovement_Halt(motor);
	CHECK_SERVO_DRIVER_ERR();
	
	do
	{
		bus_os_sleep(10);
		index = TargetReached_Get(motor);
		CHECK_SERVO_DRIVER_ERR();
	}while(index!=0);	
	
	
}

uint32_t servo_motor_multi_rotate(int cnt,servo_multi_rotate_args_t* args)
{
	int index;
	int cSpeed;
	servo_driver_struct *motor;
	for(int i = 0;i<cnt;i++)
	{
		motor = args[i].motor;
		args[i].speed = (args[i].dir==0)?-abs(args[i].speed):abs(args[i].speed);
		ProfileVelocityMode_Activate(motor);
		if(motor->errCode != 0) return motor->errCode;
		
		VelocityProfile_Set(motor,args[i].acce,args[i].acce);
		if(motor->errCode != 0) return motor->errCode;
		Velocity_Move(motor,args[i].speed);
		if(motor->errCode != 0) return motor->errCode;
		VelocityMovement_GoOn(motor);
		
		do
		{
			bus_os_sleep(10);
			index = TargetReached_Get(motor);
			if(motor->errCode != 0) return motor->errCode;
		}while(index == 0);
		
		bus_os_sleep(args[i].time);
	}
	return 0;
}

void servo_motor_shake(servo_driver_struct *motor, uint32_t step,uint32_t amplitude,float frequency,uint32_t time,u32 * step2zero)
{
	uint32_t v= (uint32_t)(2.0*4*frequency*amplitude* 60/step);
	uint32_t acc = (uint32_t)(4.0*frequency*v);
	uint32_t deacc = acc;
	uint32_t i = 0;
	uint32_t cnt = 0;
	bool index = 0;
	int ne_amplitude = -amplitude;
	
	motor->errCode = 0;
	
	motor->speed = v;
	motor->acce = acc;
	motor->steps = step;

	cnt = (uint32_t)(1.0*time*frequency/1000);

	ProfilePositionMode_Activate(motor);
	CHECK_SERVO_DRIVER_ERR();
	PositionProfile_Set(motor,v,acc,deacc);
	CHECK_SERVO_DRIVER_ERR();
	
	epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CONTROL_WORD,EPOS_IDX,(uint8_t *)(&Relative_Flag),2,false);
	CHECK_SERVO_DRIVER_ERR();
	for(i = 0;i<cnt;i++){

		epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,TARGET_POSITION,EPOS_IDX,(uint8_t *)(&amplitude),4,false);
		epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CONTROL_WORD,EPOS_IDX,(uint8_t *)(&Relative_Immediately),2,false);
		do{
			//bus_os_sleep(50);
			index = TargetReached_Get(motor);
			CHECK_SERVO_DRIVER_ERR();
		}while(index == 0);
	
	
		epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,TARGET_POSITION,EPOS_IDX,(uint8_t *)(&ne_amplitude),4,false);
		epos_send_cmd_to_driver(motor,EPOS_WT_HEAD,CONTROL_WORD,EPOS_IDX,(uint8_t *)(&Relative_Immediately),2,false);
		do{
			//bus_os_sleep(50);
			index = TargetReached_Get(motor);
			CHECK_SERVO_DRIVER_ERR();
		}while(index == 0);
	}
	{
		int cord = abs(Position_Get(motor))%step;
		*step2zero = step - cord;
	}
}

void servo_motor_base_ctrl(servo_driver_struct *motor,uint8_t option)
{
	switch(option)
	{
		case 0:
			ShutdownDevice(motor);
			break;
		case 1:
			ShutdownDevice(motor);
			EnableDevice(motor);
			break;
		case 2:
			ClearError(motor);
			break;
		case 3:
			servo_motor_quick_stop(motor);
		default:printf("servo_motor_base_ctrl parameter error\r\n"); break;
	}
}

void servo_motor_move(servo_driver_struct *motor,int profileVelocity,int profileAcceleration,
				   int profileDeceleration,int targetPosition,
				   uint8_t absolute,uint8_t immediately)
{
	ProfilePositionMode_Activate(motor);
	CHECK_SERVO_DRIVER_ERR();
	PositionProfile_Set(motor,profileVelocity,profileAcceleration,profileDeceleration);
	CHECK_SERVO_DRIVER_ERR();
	MoveToPosition(motor,targetPosition,absolute,immediately);
	CHECK_SERVO_DRIVER_ERR();
	bus_os_sleep(50);
	TargetReached_Wait(motor,TIME_COMPEN(abs(profileVelocity)/profileAcceleration + abs(targetPosition)/abs(profileVelocity)));
	CHECK_SERVO_DRIVER_ERR();
}

void servo_motor_rotate_shake_rotate(servo_driver_struct *motor, bool dir, int speed, uint32_t acce,uint32_t time,
									uint32_t step,uint32_t amplitude,float frequency,uint32_t time1,
									bool dir2, int speed2, uint32_t acce2
									)
{
	uint32_t a = 0;
	servo_motor_rotate_start(motor,dir,speed,acce);
	bus_os_sleep(time);
	servo_motor_rotate_stop(motor);
	
	servo_motor_shake(motor,step,amplitude,frequency,time1,&a);
	
	servo_motor_rotate_start(motor,dir2,speed2,acce2);
}


static void swap_data(uint8_t *value,uint8_t datalen)
{
	memmove((uint8_t *)(&swap_dus),value,datalen);
	uint32_t temp_data;
	switch(datalen)
	{
		case 0:break;
		case 1:break;
		case 2:
			temp_data = sw16(swap_dus.data.data1);
			//printf("temp_data = %x\r\n",temp_data);
			memmove(value,(uint8_t *)&(temp_data),sizeof(uint16_t));
			break;
		case 4:
			temp_data = sw16(swap_dus.data.data1) | (sw16(swap_dus.data.data2)<<16);
			memmove(value,(uint8_t *)&(temp_data),sizeof(uint32_t));
			break;
		default:printf("swapdata parameter error\r\n");break;
	}
}

static uint16_t EPOS_CRC16(u8 * data, u16 len)
{
	uint16_t crc = 0;
	while (len-- > 0)
		crc = ccitt_table[(crc >> 8 ^ *data++) & 0xff] ^ (crc << 8);
	return crc;
}

static uint8_t * epos_packet_to_bytes(servo_driver_struct *ths)
{
	uint8_t *bytes = malloc(EPOS_BYTES_PACKET(ths->send_packet.len));
	if(bytes == NULL)
	{
		printf("epos_packet_to_bytes malloc error\r\n");
	}
	bytes[0] = ths->send_packet.opcode;
	bytes[1] = ths->send_packet.len;
	WriteU16(bytes+2,ths->send_packet.reg_addr);
	WriteU16(bytes+4,ths->send_packet.sub_index);
	
	memmove(bytes+6,ths->send_packet.data,EPOS_BYTES_DATA(ths->send_packet.len)-4);
	WriteU16(bytes+2+EPOS_BYTES_DATA(ths->send_packet.len),ths->send_packet.crc);
	return bytes;
}
static void set_packet_value(servo_driver_struct * ths,uint8_t head,uint16_t reg_addr,uint16_t sub_index,uint8_t *dataIn,uint8_t bytesOfdataIn)
{
	ths->send_packet.opcode = head;
	ths->send_packet.len = ((sizeof(reg_addr)+sizeof(sub_index)+bytesOfdataIn)>>1) - 1;
	
	ths->send_packet.reg_addr = reg_addr;
	ths->send_packet.sub_index = sub_index;
	memmove(ths->send_packet.data,dataIn,bytesOfdataIn);
}
static void swap_packet_value(servo_driver_struct * ths,uint8_t bytesOfdataIn)
{
	swap_data((uint8_t *)(&ths->send_packet.reg_addr),sizeof(ths->send_packet.reg_addr));
	swap_data((uint8_t *)(&ths->send_packet.sub_index),sizeof(ths->send_packet.sub_index));
	swap_data(ths->send_packet.data,bytesOfdataIn);
}

static inline void can_resend_epos_send_cmd_to_driver(servo_driver_struct * ths,uint8_t head,uint16_t reg_addr,uint16_t sub_index,uint8_t *dataIn,uint8_t bytesOfDataIn)
{
	for(u8 i = 0;i<10;i++)			
	{								
		epos_send_cmd_to_driver(ths,head,reg_addr,sub_index,dataIn,bytesOfDataIn,true);	
		if(ths->errCode == 0)		
		{							
			break;
		}							
	}
}	
static void epos_send_cmd_to_driver(servo_driver_struct * ths,uint8_t head,uint16_t reg_addr,uint16_t sub_index,uint8_t *dataIn,uint8_t bytesOfDataIn,bool is_resend)
{
	ths->errCode = 0;
	uint8_t *buf_temp = NULL;
	uint32_t bytesOfPacket = sizeof(head)+sizeof(uint8_t)+sizeof(reg_addr)+sizeof(sub_index)+bytesOfDataIn+sizeof(uint16_t);
	
	set_packet_value(ths,head,reg_addr,sub_index,dataIn,bytesOfDataIn);
	swap_packet_value(ths,bytesOfDataIn);
	buf_temp = epos_packet_to_bytes(ths);

	ths->send_packet.crc = EPOS_CRC16(buf_temp,EPOS_BYTES_PACKET(ths->send_packet.len)-2);
	free(buf_temp);
	swap_packet_value(ths,bytesOfDataIn);
	
	buf_temp = epos_packet_to_bytes(ths);
	EPOS_CHECK_ACK(ths,buf_temp,bytesOfPacket);
	
	free(buf_temp);
	
	IS_RESEND_CMD();

	if(bus_os_pend_sem(&ths->data_sem ,500) != 0)
	{
		ths->errCode = ERR_WAIT_REACH_TIME_OUT;
	}
		
	IS_RESEND_CMD();
	EPOS_INIT_VAR();
	//预留等待时间
	bus_os_sleep(1);
	//bus_os_sleep(20);
}


void epos_cmd_notify_byte_received(servo_driver_struct * ths, uint8_t data)
{
	switch(ths->rt_state)
	{
		case EPOS_RT_CMD_ACK:
		{
			if(data == EPOS_RT_OK || data == EPOS_RT_FAILE)
			{
// 				if(data == EPOS_RT_FAILE)
// 					Async_Excute(2,printf,"d:%x\r\n",data);
				ths->rt_ack = data;
				bus_os_post_sem(&ths->ack_sem);
			}
			else if (data == EPOS_RT_HEAD && ths->is_crc_ok == 1)
			{
				//Async_Excute(2,ths->send_bytes,&EPOS_RT_OK,1);
				ths->send_bytes(&EPOS_RT_OK,1);
				ths->rt_data_check = ccitt_table[(ths->rt_data_check >> 8 ^ data) & 0xff] ^ (ths->rt_data_check << 8);	
				ths->rt_state = EPOS_RT_DATA_LEN;
				ths->rt_data[ths->rt_idx++] = data;
			}
			break;
		}
		case EPOS_RT_DATA_LEN:
		{
			if(((data+1)<<1) > sizeof(epos_packet_struct))
			{
				Async_Excute(1,printf,"rt_idx too long\r\n");
				ths->errCode= ERR_SERVO_DRIVER_DATALEN;
				bus_os_post_sem(&ths->data_sem);
				return;
			}
			ths->rt_data[ths->rt_idx++] = data;
			ths->rt_data_check = ccitt_table[(ths->rt_data_check >> 8 ^ data) & 0xff] ^ (ths->rt_data_check << 8);
			ths->rt_state = EPOS_RT_DATA;
			break;	
		}
		case EPOS_RT_DATA:
		{
			ths->rt_data[ths->rt_idx] = data;
			if((ths->rt_idx & 0x1) == 1)
			{
				ths->rt_data_check = ccitt_table[(ths->rt_data_check >> 8 ^ ths->rt_data[ths->rt_idx]) & 0xff] ^ (ths->rt_data_check << 8);
				ths->rt_data_check = ccitt_table[(ths->rt_data_check >> 8 ^ ths->rt_data[ths->rt_idx-1]) & 0xff] ^ (ths->rt_data_check << 8);
			}
			if(ths->rt_idx == (EPOS_BYTES_DATA(ths->rt_data[1]) + 2))
			{
				ths->rt_state = EPOS_RT_DATA_CRC;
			}
			ths->rt_idx++;
			break;
		}
		case EPOS_RT_DATA_CRC:
		{
			ths->rt_data[ths->rt_idx] = data;
			
			if(ths->rt_idx == EPOS_BYTES_PACKET(ths->rt_data[1]) -1)
			{
				ths->rt_packet.crc = ReadU16(ths->rt_data + EPOS_BYTES_PACKET(ths->rt_data[1]) - 2);
				if(ths->rt_packet.crc != ths->rt_data_check)
				{
					//Async_Excute(2,ths->send_bytes,&EPOS_RT_FAILE,1);
					ths->send_bytes(&EPOS_RT_FAILE,1);
					Async_Excute(1,printf,"crc err\r\n");
					ths->errCode= ERR_SERVO_DRIVER_CRC;
					bus_os_post_sem(&ths->data_sem);
					return;
				}
				//Async_Excute(2,ths->send_bytes,&EPOS_RT_OK,1);
				ths->send_bytes(&EPOS_RT_OK,1);
				//Async_Excute(1,printf,"O\r\n");
            	bus_os_post_sem(&ths->data_sem);
				return;
			}
			ths->rt_idx++;
			break;
		}
		default:break;
	}
}

void EPOS2Driver_Init(servo_driver_struct *ths, void (*send_bytes)(uint8_t *data, uint16_t length))
{
	bus_os_create_sem(&ths->ack_sem,0);
	bus_os_create_sem(&ths->data_sem,0);
	ths->send_bytes = send_bytes;
}

