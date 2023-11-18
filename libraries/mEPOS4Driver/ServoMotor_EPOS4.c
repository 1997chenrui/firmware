#include "ServoMotor_EPOS4.h"
#include "bus.h"
#include "errCode.h"
#include "EPOS4Driver.h"
#include <stdint.h>
#include <stdlib.h>

extern void Syn_Signal_Hight(void);
extern void Syn_Signal_Low(void);

#define TIME_OUT_COMPEN  4000
#define TIME_COMPEN(val) (val*1000+TIME_OUT_COMPEN)

int Position_Get(servo_driver_struct *motor) //位置值与PC软件看到的一致  运行也很正常
{
	int32_t result=0;	
	motor->errCode=EPOS4_ReadData(0x6064,0,&result);
	return result;
}

int Velocity_Get(servo_driver_struct *motor)
{
	int32_t result=0;
	motor->errCode=EPOS4_ReadData(0x30d3,0,&result) ;
	return result;
}

uint16_t Statusword_Get(servo_driver_struct *motor)  
{
	int32_t result=0;
	motor->errCode=EPOS4_ReadData(0x6041,0,&result);
	return result;
}


uint16_t mGetServomotorErr(servo_driver_struct *motor)  
{
	int32_t result=0;
	motor->errCode=EPOS4_ReadData(0x603f,0,&result) ;
	return result;
}

static bool TargetReached_Get(servo_driver_struct *motor,uint8_t n)
{
	int32_t result=0;
	result=Statusword_Get(motor)>> n & 1;
	motor->errCode=motor->errCode;
	return result;
}


void TargetReached_Wait(servo_driver_struct *motor, uint32_t timeout, uint32_t n)
{
	uint8_t index = 0;
	int start_tick = 0,current_tick = 0;
	start_tick = OSTimeGet();
	motor->errCode = 0;			
	do
	{		  
		current_tick = OSTimeGet();
		index = TargetReached_Get(motor,n);
		bus_os_sleep(20);
	}while(index == 0 && current_tick - start_tick < timeout);
	
	if(current_tick - start_tick > timeout)
	{
		printf("servo run move time out\r\n");
		motor->errCode = ERR_WAIT_REACH_TIME_OUT;
	}
}


void Shake_TargetReached_Wait(servo_driver_struct *motor, uint32_t timeout, uint32_t n)
{
	uint8_t index = 0;
	int start_tick = 0,current_tick = 0;
	start_tick = OSTimeGet();
	motor->errCode = 0;			
	do
	{		  
		current_tick = OSTimeGet();
		index = TargetReached_Get(motor,n);
		bus_os_sleep(10);
	}while(index == 0 && current_tick - start_tick < timeout);
	
	if(current_tick - start_tick > timeout)
	{
		printf("servo run move time out\r\n");
		motor->errCode = ERR_WAIT_REACH_TIME_OUT;
	}
}


void servo_motor_rotate_stop(servo_driver_struct *motor)
{
	motor->errCode = 0;
	do
	{
		EPOS4_WriteData(0x6040,0,0x010F);
		bus_os_sleep(50);		
	}while(TargetReached_Get(motor,10) == 0);//一直发到停止
	
	if(motor->errCode != 0)
	{
		printf("rotate stop time out\r\n");
		motor->errCode = ERR_WAIT_REACH_TIME_OUT;
	}
}


void servo_motor_reset(servo_driver_struct *motor)
{
	if(motor->flg == 0)
	{
		servo_motor_rotate_stop(motor);
		bus_os_sleep(20);
		MoveBY_Homing_Mode(2000,4000,0,300,500,0);
		bus_os_sleep(20);
		TargetReached_Wait(motor, 100000+25*6,12); //Homing procedure is completed successfully if bit 12 (=“Homing attained”) is set to “1”.
		motor->flg=1;
	}
	else
	{
		motor->errCode = 0;	
	}
}


void servo_motor_rotate_start(servo_driver_struct *motor, bool dir, int speed, uint32_t acce)
{
	motor->flg =0;
	MoveBY_Velocity_Mode(speed,acce,dir) ;
	bus_os_sleep(10);
	TargetReached_Wait(motor, TIME_COMPEN(abs(speed)/acce)+25,10);//Target velocity is reached if bit 10 is set  需要注要当停止位为1时 位置到达不可知
}


void servo_motor_move(servo_driver_struct *motor,int profileVelocity,int profileAcceleration, int profileDeceleration,int targetPosition,uint8_t absolute,uint8_t immediately)
{
	motor->flg =0;
	MoveBY_Position_Mode(profileVelocity,profileAcceleration,profileDeceleration,targetPosition,absolute,immediately);
	bus_os_sleep(10);
	TargetReached_Wait(motor,TIME_COMPEN(abs(profileVelocity)/profileAcceleration + abs(targetPosition)/abs(profileVelocity))+25*6,10);
}


void servo_motor_rotate_two_dir_on_time(servo_driver_struct *motor, bool dir, int speed, uint32_t acce,uint32_t time)
{
	motor->flg =0;
	MoveBY_Velocity_Mode(speed,acce,dir) ;
	TargetReached_Wait(motor,5000/speed *20,10);  //此处可能有超时报警  超时时间可作调整
	bus_os_sleep(time);
	servo_motor_rotate_stop(motor);	//开始停下
	TargetReached_Wait(motor,5000/speed *20,10);
	MoveBY_Velocity_Mode(speed,acce,(dir==0)?1:0) ;//开始反向
	TargetReached_Wait(motor,5000/speed *20,10);
	bus_os_sleep(time);
	servo_motor_rotate_stop(motor);	//开始停下
	TargetReached_Wait(motor,5000/speed *20,10);
}

uint32_t servo_motor_multi_rotate(int cnt,servo_multi_rotate_args_t* args) //此功没有测试
{
	servo_driver_struct *motor;
	motor->flg =0;
	for(int i = 0;i<cnt;i++)
	{
		if(args[i].isSendSyncSignal)
		{
			Syn_Signal_Hight();
		}
		MoveBY_Velocity_Mode(args[i].speed,args[i].acce,args[i].dir) ;
		TargetReached_Wait(motor,5000/args[i].speed *20,10);
		bus_os_sleep(args[i].time);
		if(args[i].isSendSyncSignal)
		{
			Syn_Signal_Low();
		}
	}
	return 0;
}


void servo_motor_rotate_shake_rotate(servo_driver_struct *motor, bool dir, int speed, uint32_t acce,uint32_t time,uint32_t step,uint32_t amplitude,float frequency,uint32_t time1,bool dir2, int speed2, uint32_t acce2)
{
	uint32_t a = 0;
	motor->flg =0;
	Syn_Signal_Hight();
	servo_motor_rotate_start(motor,dir,speed,acce);
	bus_os_sleep(time);
	Syn_Signal_Low();
	servo_motor_rotate_stop(motor);		
	servo_motor_shake(motor,step,amplitude,frequency,time1,&a);			
	servo_motor_rotate_start(motor,dir2,speed2,acce2);
}

void servo_motor_shake(servo_driver_struct *motor, uint32_t step,uint32_t amplitude,float frequency,uint32_t time,u32 * step2zero)
{
	float length=amplitude ;//*91;//每1度等于91个脉冲
	uint32_t v= (amplitude)/(1/frequency) /60 *2;
	uint32_t acc = (uint32_t)(2*frequency*v);

	//uint32_t cnt = (uint32_t)(1.0*(time/1000)*frequency);		

	uint32_t cnt = (uint32_t)(1.0*time*frequency/1000);
	motor->flg =0;
	motor->errCode = 0;
	motor->speed = v;
	motor->acce = acc;
	motor->steps = step;

	if(acc > 50000) acc=50000;

	EPOS4_WriteData(0x6060,0,0x1) ;
	OSTimeDly(10);
	EPOS4_WriteData(0x6081,0,v) ;
	OSTimeDly(10);
	EPOS4_WriteData(0x6083,0,acc) ;
	OSTimeDly(10);
	EPOS4_WriteData(0x6084,0,acc) ;
	OSTimeDly(10);
	EPOS4_WriteData(0x6085,0,10000) ;
	OSTimeDly(10);
	EPOS4_WriteData(0x6086,0,0) ;
	OSTimeDly(10);
	for(uint8_t i = 0;i<cnt;i++)
	{
		EPOS4_WriteData(0x607a,0,length);
		EPOS4_WriteData(0x6040,0,0x7f);
		EPOS4_WriteData(0x6040,0,0x0f); 
		
		Shake_TargetReached_Wait(motor, time*60*1000/20,10);
	  
		EPOS4_WriteData(0x607a,0,0-length);
		EPOS4_WriteData(0x6040,0,0x7f);
		EPOS4_WriteData(0x6040,0,0x0f); 
		
		Shake_TargetReached_Wait(motor,time*60*1000/20,10);		
	}
	TargetReached_Wait(motor,time*100,10);	
	OSTimeDly(1000);//驱动器有一个PID调节功能  刹车时有一个PID调节过程

	int cord = abs(Position_Get(motor))%step;
	*step2zero = step - cord;
}  //32468


void servo_motor_base_ctrl(servo_driver_struct *motor,uint8_t option)
{
	switch(option)
	{
		case 0:	
		{
			EPOS4_WriteData(0x6040,0,0x06);  //使能驱动器必须先使能驱动器
			OSTimeDly(10);
			EPOS4_WriteData(0x6040,0,0x0f);//使能	
		}break;						
		case 1:
		{
			EPOS4_WriteData(0x6040,0,0x06);//失能	
		}break;					
		case 2:
		{
			EPOS4_WriteData(0x6040,0,0x80);//清除错误  相当于改变控制寄存器的状态  使能状状也会改变  清除错误后驱动器失能！！！	
		}break;
		case 3:
		{
			EPOS4_WriteData(0x6040,0,0x0b);//快速停止  减速度在配置相应模式时己设置为 10000
		}break;		
		default:printf("servo_motor_base_ctrl parameter error\r\n"); break;
	}
}


void EPOS2Driver_Init(servo_driver_struct *ths, void (*send_bytes)(uint8_t *data, uint16_t length))
{
	ths->flg =0;
	OSTimeDlyHMSM(0, 0, 3, 0);//等侍驱动器初始化完成 
	
    servo_motor_base_ctrl(ths,2);	
	OSTimeDly(20);
	servo_motor_base_ctrl(ths,0);
	
	OSTimeDly(100);//  使能指令  在初始化的时侯执行   但驱动器有一个初始化时间 
}

