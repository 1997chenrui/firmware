#include "EPOS4Device.h"
#include "bus.h"
#include "errCode.h"
#include "EPOS4Driver.h"
#include <stdint.h>
#include <stdlib.h>

unsigned char motor_type=0x4;

extern void Syn_Signal_Hight(void);
extern void Syn_Signal_Low(void);

static void TargetReached_Wait(servo_driver_struct *motor,uint8_t wait_type)//等待目标到达 包括位置到达  复位到达  速度到达等
{
	int32_t index = 0;
	uint8_t err=0;
	uint8_t wait_time=0;
	bool r13=0,r10=0,rn=0;//状态寄存器的第13位和第10位
	motor->errCode = 0;			
	do
	{		  		
		switch(wait_type)
		{
			case WAIT_TYPE_REACH://等待位置到达
			{
				index = Servo_base_ctrl(motor,SERVO_CMD_GET_STATE,&err);
				r13=index>>13 & 1;
				r10=index>>10 & 1;
				wait_time=100;					
			}break;
			case WAIT_TYPE_QUICK_REACH://等待快速停止 用在振荡需指令里
			{
				index = Servo_base_ctrl(motor,SERVO_CMD_GET_STATE,&err);
				r13=index>>13 & 1;
				r10=index>>10 & 1;
				wait_time=10;					
			}break;			
			case WAIT_TYPE_STOP://等待停止 并设置停止
			{
				EPOS4_WriteData(0x6040,0,0x010F);
				bus_os_sleep(100);
				index = Servo_base_ctrl(motor,SERVO_CMD_GET_STATE,&err);
				r13=index>>13 & 1;
				r10=index>>10 & 1;
				wait_time=100;					
			}break;	
			case WAIT_TYPE_RESET://等待复位
			{	
				index = Servo_base_ctrl(motor,SERVO_CMD_GET_STATE,&err);
				r13=index>>13 & 1;
				r10=index>>10 & 1;
				rn=index>>12 & 1;
				wait_time=100;									
			}break;
			default:break;
		}

		if(r13 == 1 || r10 == 1 || rn==1 || err == UART_COMMUNICATION_TIMEOUT)  //电机停止或低层通讯函数进行10尝试读写后失败后 退出
		{
			break;
		}
		bus_os_sleep(wait_time);
	}while(1);
	
	if(r13 == 1)//驱动器硬件错误
	{
		Servo_base_ctrl(motor,SERVO_CMD_CLEAR_ERROR,&err);
		motor->errCode = ERR_EPOS4_ERROR;
		printf("motor error and clear error......\r\n");
	}
	else if(err == UART_COMMUNICATION_TIMEOUT)//通讯错误
	{		
		motor->errCode = ERR_WAIT_REACH_TIME_OUT;
		printf("servo run move time out\r\n");
	}
	else  
	{
		motor->errCode = 0; 
	}
}


int32_t Servo_base_ctrl(servo_driver_struct *motor,SERVO_CMD option,uint8_t *e)
{
	uint8_t err;
	int32_t result=0;	
	motor->errCode=0;
	switch(option)
	{
		case SERVO_CMD_ENABLE:	//使能
		{
			err=EPOS4_WriteData(0x6040,0,0x06);  //使能驱动器必须先使能驱动器
			OSTimeDly(10);
			err=EPOS4_WriteData(0x6040,0,0x0f);	
		}break;						
		case SERVO_CMD_DISABLE://失能	
		{
			err=EPOS4_WriteData(0x6040,0,0x06);
		}break;					
		case SERVO_CMD_CLEAR_ERROR://清除错误  相当于改变控制寄存器的状态  使能状状也会改变  清除错误后驱动器失能！！！
		{
			err=EPOS4_WriteData(0x6040,0,0x80);
			OSTimeDly(10);			
			err=EPOS4_WriteData(0x6040,0,0x06);  //使能驱动器必须先使能驱动器
			OSTimeDly(10);
			err=EPOS4_WriteData(0x6040,0,0x0f);				
		}break;
		case SERVO_CMD_STOP://快速停止  减速度在配置相应模式时己设置为 10000
		{
			err=EPOS4_WriteData(0x6040,0,0x0b);
		}break;	
		case SERVO_CMD_GET_POSTION:  //获取位置
		{
			err=EPOS4_ReadData(0x6064,0,&result);
		}break;
		case SERVO_CMD_GET_VELOCITY://获取速度
		{
			err=EPOS4_ReadData(0x30d3,0,&result) ;
		}break;
		case SERVO_CMD_GET_ERROR_ID: //获取错误码
		{
			err=EPOS4_ReadData(0x603f,0,&result);
		}break;
		case SERVO_CMD_GET_STATE://获取状态
		{
			err=EPOS4_ReadData(0x6041,0,&result);
		}break;
		case SERVO_CMD_QUICK_STOP:
		{
			TargetReached_Wait(motor,WAIT_TYPE_STOP);
		}break;
		default:printf("servo_motor_base_ctrl parameter error\r\n"); break;
	}
	*e=err;
	return result;
}


void Servo_motor_reset(servo_driver_struct *motor)
{
	uint8_t err;
	if(motor->flg == 0)
	{
		Servo_base_ctrl(motor,SERVO_CMD_QUICK_STOP,&err);
		bus_os_sleep(20);
		Move_By_Homing_Mode(2000,4000,0,300,500,0);
		bus_os_sleep(20);
		TargetReached_Wait(motor,WAIT_TYPE_RESET); //Homing procedure is completed successfully if bit 12 (=“Homing attained”) is set to “1”.
		motor->flg=1;
	}
	else
	{
		motor->errCode = 0;	
	}
}


void Servo_motor_rotate_start(servo_driver_struct *motor, bool dir, int speed, uint32_t acce)
{
	motor->flg =0;
	Move_By_Velocity_Mode(speed,acce,dir) ;
	bus_os_sleep(10);
	TargetReached_Wait(motor,WAIT_TYPE_REACH);//Target velocity is reached if bit 10 is set  需要注要当停止位为1时 位置到达不可知
}

// profileVelocity:速度, profileAcceleration:加速度, targetPosition:目标位置, absolute:是否是绝对运动, immediately:是否立即运动
void Servo_motor_move(servo_driver_struct *motor,int profileVelocity,int profileAcceleration, int profileDeceleration,int targetPosition,uint8_t absolute,uint8_t immediately)
{
	motor->flg =0;
	Move_By_Position_Mode(profileVelocity,profileAcceleration,profileDeceleration,targetPosition,absolute,immediately);
	printf("Servo_motor_move: speed=%d, acce=%d, steps=%d, absolute=%d, immediately=%d\r\n", profileVelocity, profileAcceleration, targetPosition, absolute, immediately);
	bus_os_sleep(10);
	TargetReached_Wait(motor,WAIT_TYPE_REACH);
}


void Servo_motor_rotate_two_dir_on_time(servo_driver_struct *motor, bool dir, int speed, uint32_t acce,uint32_t time)
{
	uint8_t err;
	motor->flg =0;
	Move_By_Velocity_Mode(speed,acce,dir) ;
	TargetReached_Wait(motor,WAIT_TYPE_REACH);  
	bus_os_sleep(time);
	Servo_base_ctrl(motor,SERVO_CMD_QUICK_STOP,&err);
	TargetReached_Wait(motor,WAIT_TYPE_REACH);
	Move_By_Velocity_Mode(speed,acce,(dir==0)?1:0) ;//开始反向
	TargetReached_Wait(motor,WAIT_TYPE_REACH);
	bus_os_sleep(time);
	Servo_base_ctrl(motor,SERVO_CMD_QUICK_STOP,&err);
	TargetReached_Wait(motor,WAIT_TYPE_REACH);
}

uint32_t Servo_motor_multi_rotate(int cnt,servo_multi_rotate_args_t * args) 
{		
	for(int i = 0;i<cnt;i++)
	{
		args[i].speed = (args[i].dir==0)?-abs(args[i].speed):abs(args[i].speed);		
		args[i].motor->flg =0;		
		if(args[i].isSendSyncSignal)
		{
			Syn_Signal_Hight();
		}		
		
		Move_By_Velocity_Mode(args[i].speed,args[i].acce,args[i].dir) ;
		TargetReached_Wait(args[0].motor,WAIT_TYPE_REACH);		
		bus_os_sleep(args[i].time);		
		
		if(args[i].isSendSyncSignal)
		{
			Syn_Signal_Low();
		}
	}
	TargetReached_Wait(args[0].motor,WAIT_TYPE_STOP);
	return 0;
}

void Servo_motor_rotate_shake_rotate(servo_driver_struct *motor, bool dir, int speed, uint32_t acce,uint32_t time,uint32_t step,uint32_t amplitude,float frequency,uint32_t time1,bool dir2, int speed2, uint32_t acce2)
{
	uint8_t err;
	uint32_t a = 0;
	motor->flg =0;
	Syn_Signal_Hight();
	Servo_motor_rotate_start(motor,dir,speed,acce);
	bus_os_sleep(time);
	Syn_Signal_Low();
	Servo_base_ctrl(motor,SERVO_CMD_QUICK_STOP,&err);		
	Servo_motor_shake(motor,step,amplitude,frequency,time1,&a);			
	Servo_motor_rotate_start(motor,dir2,speed2,acce2);
}

void servo_motor_rotate_shake_rotate_detect(servo_driver_struct *motor, u32 max,u32 m2_acce, u32 m3_speed,\
		u32 m4_time,u8 m5_isForward,u32 m6_time,u8 m7_frequency, u32 m8_amplitude, u32 m9_acce, u32 m10_speed, \
	u8 m11_isForward, u32 m12_time,u32 m13_acce, u32 m14_speed, u8 m15_isForward)
{
	u8 err=0;
	uint32_t v=    (uint32_t)(2.0*4*m7_frequency*m8_amplitude* 60/max);
	uint32_t acc = (uint32_t)(4.0*m7_frequency*v);
	uint32_t cnt = (uint32_t)(1.0*m6_time*m7_frequency/1000);
	
	Syn_Signal_Hight(); 
	
    Move_By_Velocity_Mode(m3_speed,m2_acce,m5_isForward) ;
	TargetReached_Wait(motor,WAIT_TYPE_REACH);
	bus_os_sleep(m4_time);
	Servo_base_ctrl(motor,SERVO_CMD_QUICK_STOP,&err);//SERVO_CMD_QUICK_STOP

    EPOS4_WriteData(0x6081,0,v);
	EPOS4_WriteData(0x6083,0,acc);
	EPOS4_WriteData(0x6084,0,acc);
	EPOS4_WriteData(0x607a,0,0);
	for(uint8_t i = 0;i<cnt;i++)
	{
		EPOS4_WriteData(0x607a,0,m8_amplitude);
		EPOS4_WriteData(0x6040,0,0x7f);
		EPOS4_WriteData(0x6040,0,0x0f);
		EPOS4_WriteData(0x6060,0,0x1);		
		TargetReached_Wait(motor,WAIT_TYPE_QUICK_REACH);
	  
		EPOS4_WriteData(0x607a,0,0-m8_amplitude);
		EPOS4_WriteData(0x6040,0,0x7f);
		EPOS4_WriteData(0x6040,0,0x0f); 		
		TargetReached_Wait(motor,WAIT_TYPE_QUICK_REACH);	
	}
	
	Move_By_Velocity_Mode(m10_speed,m9_acce,m11_isForward) ;
	TargetReached_Wait(motor,WAIT_TYPE_REACH);	
	bus_os_sleep(m12_time);
	//Servo_base_ctrl(motor,SERVO_CMD_QUICK_STOP,&err);//停止可能引起振荡	
	Move_By_Velocity_Mode(m14_speed,m13_acce,m15_isForward) ;
	TargetReached_Wait(motor,WAIT_TYPE_REACH);
	Syn_Signal_Low();	
}

void Servo_motor_shake(servo_driver_struct *motor, uint32_t step,uint32_t amplitude,float frequency,uint32_t time,u32 * step2zero)
{
	uint8_t err=0;
	float length=amplitude ;//*91;//每1度等于91个脉冲
	uint32_t v= (uint32_t)(2.0*4*frequency*amplitude* 60/step);
	uint32_t acc = (uint32_t)(4.0*frequency*v);
	uint32_t cnt = (uint32_t)(1.0*time*frequency/1000);
	
	motor->flg =0;
	motor->errCode = 0;
	motor->speed = v;
	motor->acce = acc;
	motor->steps = step;

	EPOS4_WriteData(0x6060,0,0x1);
	EPOS4_WriteData(0x6081,0,v);
	EPOS4_WriteData(0x6083,0,acc);
	EPOS4_WriteData(0x6084,0,acc);
	EPOS4_WriteData(0x607a,0,0);

	for(uint8_t i = 0;i<cnt;i++)
	{
		EPOS4_WriteData(0x607a,0,length);
		EPOS4_WriteData(0x6040,0,0x7f);
		EPOS4_WriteData(0x6040,0,0x0f);
				
		TargetReached_Wait(motor,WAIT_TYPE_QUICK_REACH);
	  
		EPOS4_WriteData(0x607a,0,0-length);
		EPOS4_WriteData(0x6040,0,0x7f);
		EPOS4_WriteData(0x6040,0,0x0f); 		
		TargetReached_Wait(motor,WAIT_TYPE_QUICK_REACH);	
	}
	TargetReached_Wait(motor,WAIT_TYPE_REACH);	
	int cord = abs(Servo_base_ctrl(motor,SERVO_CMD_GET_POSTION,&err))%step;
	*step2zero = step - cord;
} 


void EPOS4Driver_Init(servo_driver_struct *ths)
{
	uint8_t err=0;
	ths->flg =0;
	
	UART_EnableUart(); //串;口初始化
	
	OSTimeDlyHMSM(0, 0, 3, 0);//等侍驱动器初始化完成 
	
    Servo_base_ctrl(ths,SERVO_CMD_CLEAR_ERROR,&err);	
	OSTimeDly(20);
	Servo_base_ctrl(ths,SERVO_CMD_ENABLE,&err);
	
	OSTimeDly(100);//  使能指令  在初始化的时侯执行   但驱动器有一个初始化时间 
}

