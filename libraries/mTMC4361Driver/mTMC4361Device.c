#include "mTMC4361Device.h"
#include <stdlib.h>
#include "ErrCode.h"


//0正向 1反向
#define PROJECT_TYPE  NFL //用来定义此功能是否是MC500独有
#define QUICK_RESET_BACKZERO 1

#define SPEED_CORRECTION  1 //1.6  //速度修正比例
#define SPEED_BACK_ZEOR   8500 //轴回零速度
unsigned char motor_type=0x5;

#define setbit(x,y) x|=(1<<y) 
#define clrbit(x,y)  x&=~(1<<y) 

static char motor_max;

extern MOTOR motors[];
extern motor_cfg_struct motors_cfg[];

unsigned int GetTimeOut(int a,int b) //计算超时 时间 
{
	float time=0.0;
	if(a >0 && b>0)
	{
		time=((float)(2000.0 / a) * (float)(b))+500;
	}
	else
	{
		time=1000;
	}
	return time;
}

static int IS_Targeted(MOTOR *ths,int type)  ///< 在相应中断触发后 检查对应IO的状态
{
	int result=0;
	if(type == READ_PIN_ID_REACH_ISR)
	{
		result=ths->read_io((pin_id_t)type,0,0);
		result=!result;
	}
	else if(type == READ_PIN_ID_TOUCH_ISR)
	{
		result=ths->read_io((pin_id_t)type,0,ths->mot_cfg->zero_trig_vol);
		result=(ths->mot_cfg->zero_trig_vol==0) ? !result:result;
	}
	else if(type == READ_PIN_ID_ERROR_ISR)
	{
		result=ths->read_io((pin_id_t)type,0,0);
		result=!result;
	}
	return result;
}

static char Get_WaitTaskId(char c)
{
	for(char i=0;i<motor_max;i++) 
	{
		if(motors[i].evnt_group >0)
		{
			clrbit(motors[i].evnt_group,c);
			return i;
		}				
	}
	return motor_max;
}


char Get_ISR_ID(u32 pin)
{
	char result=0;
	switch(pin)
	{
		case GPIO_PIN_0:result=6;break;
		case GPIO_PIN_1:result=7;break;
		case GPIO_PIN_2:result=8;break;
		case GPIO_PIN_3:result=9;break;
		case GPIO_PIN_4:result=10;break;
		case GPIO_PIN_5:
		case GPIO_PIN_6:
		case GPIO_PIN_7:
		case GPIO_PIN_8:
		case GPIO_PIN_9:result=23;break;
		case GPIO_PIN_10:
		case GPIO_PIN_11:
		case GPIO_PIN_12:
		case GPIO_PIN_13:
		case GPIO_PIN_14:
		case GPIO_PIN_15:result=40;break;
	}
	return result;
}

void mTMC4361ProcessSemPost(MOTOR *ths,int wait_type)
{
	switch(wait_type)
	{
		case WAIT_TOUCH_SEM: //光耦中断检测触发中断  复位
		{
			if(IS_Targeted(ths,READ_PIN_ID_TOUCH_ISR))
			{
				if(ths->wait_sem != NULL && ths->wait_type == WAIT_TOUCH_SEM) 
				{
					OSSemPost(ths->wait_sem);		
				}
			}
		}break;
		case WAIT_REACH_FLG://位置到达检测触发中断 motor_move
		{
			if(IS_Targeted(ths,READ_PIN_ID_REACH_ISR))
			{
				if(ths->wait_type == WAIT_REACH_FLG)
				{
					char i=Get_WaitTaskId(ths->id);
					if(motors[i].evnt_group == 0 && motors[i].wait_sem != NULL && i<=motor_max)									
					{
						OSSemPost(motors[i].wait_sem);
					}
				}
			}
		}break;	
		case WAIT_REACH_SEM://信号量组 多电机运行中使用到  到达触发中断
		{
			if(IS_Targeted(ths,READ_PIN_ID_REACH_ISR))
			{
				ths->is_reach_target=1;//位置到达信号到
				if(ths->wait_sem != NULL && ths->wait_type == WAIT_REACH_SEM)
				{
					OSSemPost(ths->wait_sem);
				}
			}
		}break;
		case WAIT_ERROR_SEM://驱动内报警出错触发中断
		{
			if(IS_Targeted(ths,READ_PIN_ID_ERROR_ISR))
			{
				if(ths->wait_sem != NULL && ths->wait_type == WAIT_ERROR_SEM)
				{
					OSSemPost(ths->wait_sem);
				}	
			}			
			break;
		} 
		default:break;
	}
}

int mTMC4361DevicWait_Sem(MOTOR* ths,char wait_type,unsigned int wait_time)
{           
	u8 err=0;	
	int result=0;
	switch(wait_type)
	{
		case WAIT_REACH_FLG:  //等待信号组中所有的变量都被清零后  发送信号量
		{
			for(char i=0;i<motor_max;i++)
			{
				if(motors[i].wait_type == WAIT_REACH_FLG)	
				{
					motors[i].read_io(PIN_ID_SET_REACH_ISR,motors[i].reach_isr_id,0);
				}
			}				
		}break;
		case WAIT_REACH_SEM:  //等待位置信号被发送
		{
			ths->read_io(PIN_ID_SET_REACH_ISR,ths->reach_isr_id,0);	
		}break;
		case WAIT_TOUCH_SEM:  //等待光耦信号被发送
		{
			ths->read_io(PIN_ID_SET_TOUCH_ISR,ths->zeor_isr_id,ths->mot_cfg->zero_trig_vol);
		}break;
		case WAIT_ERROR_SEM:
		{
			ths->read_io(PIN_ID_SET_ERROR_ISR,ths->error_isr_id,0);//打开错误检测中断
			break;
		}
		case WAIT_STOP_SEM:  //等待电机停止信号被发送
		{								   
			static char t;
			do
			{
				t++;
				OSTimeDlyHMSM(0,0,0,100);				
			}while(mTMC4361DeviceGetSpeed(ths) != 0 );
	
			if(t > wait_time/100)
			{
				ths->status=MOTOR_STATUS_ERROR_TIME_OUT;						  
			}
			goto NO_ERROR_RETURN;
		}
		case WAIT_VELOCITY_SEM://等待速度到达信号量 
		{								   
			static char t;
			do
			{
				t++;
				OSTimeDlyHMSM(0,0,0,100);				
			}while(mTMC4361DeviceGetSpeed(ths) < 10 );
	
			if(t > wait_time/100)
			{
				ths->status=MOTOR_STATUS_ERROR_TIME_OUT;						  
			}
			goto NO_ERROR_RETURN;
		}
		default:break;
	}
	
	ths->wait_type=wait_type;
	ths->wait_sem=OSSemCreate(0);

	OSSemPend(ths->wait_sem,wait_time>0 ? wait_time:0,&err);		

	if(err == OS_ERR_PEND_ABORT) //信号量被中止
	{		
		mTMC4361DevicStop(ths,STOP_AMAX);		
		ths->status=MOTOR_STATUS_ERROR_MOTORABORT;
//		ths->wait_sem=NULL;//影响后面删除sem的操作(sem无法删除),导致内存泄漏等问题
		result=MOTOR_STATUS_ERROR_MOTORABORT;
	}				
	else if(err == OS_ERR_TIMEOUT)
	{
		ths->status=MOTOR_STATUS_ERROR_TIME_OUT;
		result=MOTOR_STATUS_ERROR_TIME_OUT;
	}		
	else
	{
		ths->status=MOTOR_STATUS_STOP;
		result=MOTOR_STATUS_STOP;
	}
	NO_ERROR_RETURN:
	{
		if(ths->wait_sem != NULL)	
		{
			OSSemDel(ths->wait_sem,OS_DEL_ALWAYS,&err);
			ths->wait_sem=NULL;
		}	
//		HAL_NVIC_DisableIRQ((IRQn_Type)ths->reach_isr_id);
//		HAL_NVIC_DisableIRQ((IRQn_Type)ths->zeor_isr_id);					
		ths->wait_type=0;
		ths->is_reset=0;
	}
	return result;
}


char mTMC4361DevicStop(MOTOR* ths,int acc) //按减速度停止电机   减速度越大   停止用时越短
{   
	TABLE run_table;	
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	run_table.VMAX=0;
	run_table.DMAX=acc;
	run_table.hold_current=HOLD_CURRENT;	
	run_table.vbreak=0;
	run_table.bow1=0;
	run_table.bow2=0;
	run_table.bow3=0;
	run_table.bow4=0;
	run_table.mode=TMC4361_RAMP_TRAPEZ;//按AMAX加速  按DMAX减速
	return mTMC4361DriverMove(&run_table);
}


char mTMC4361DevicRotate(MOTOR* ths,int speed,int acc,char dir,int mico,char current)   //速度模式  按指定速度转动
{	   
	TABLE run_table;	
	dir= ths->mot_cfg->dir_vol ? dir:DRI_REVERAL(dir);
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	run_table.AMAX=acc;
	run_table.VMAX=dir>0?speed:0-speed;
	run_table.DMAX=acc;
	run_table.run_currnet=current;
	run_table.MicoStep=mico;
	run_table.bow1=200;
	run_table.bow2=3000;
	run_table.bow3=3000;
	run_table.bow4=100;	
	run_table.mode=TMC4361_RAMP_SSHAPE;
	return mTMC4361DriverMove(&run_table);
}


char mTMC4361DevicMoveto(MOTOR* ths,int speed,int acc,int postion,char dir,int mico,char current)  //绝对运动 没有方向  目标位置在当前位置前向就正转  反之则反转
{
	if(current == 5)
	{
		TABLE run_table;
		if(ths->mot_cfg->dir_vol ) postion= 0-postion;
		run_table.mTMCDriver_WriteIo=ths->write_io;
		run_table.mTMCDriver_ReadIo=ths->read_io;
		run_table.mTMCDriverReg=ths->drvie_reg;
		run_table.AMAX=acc;
		run_table.VMAX=speed;
		run_table.DMAX=acc;
		run_table.postion=(ths->mot_cfg->dir_vol )? postion : postion;
		run_table.run_currnet=current;
		run_table.MicoStep=mico;
		run_table.bow1=1000;
		run_table.bow2=5000;
		run_table.bow3=5000;
		run_table.bow4=1000;	
		run_table.mode=TMC4361_RAMP_POSITION_SSHAPE;
		return mTMC4361DriverMove(&run_table);
	}
	else if(current == 6)
	{
		int pos = abs(postion - ths->c_postion * 16);
		if(ths->c_postion *16 < postion) dir=0;else dir=1;
		TABLE run_table;
		if(ths->mot_cfg->dir_vol ) postion= 0-postion;
		run_table.mTMCDriver_WriteIo=ths->write_io;
		run_table.mTMCDriver_ReadIo=ths->read_io;
		run_table.mTMCDriverReg=ths->drvie_reg;
		run_table.AMAX=acc;
		run_table.VMAX=speed;
		run_table.DMAX=acc;
		run_table.postion=(dir==0)? mTMC4361DeviceGetPostion(ths,0)+pos :mTMC4361DeviceGetPostion(ths,0) - pos;
		run_table.run_currnet=current;
		run_table.MicoStep=mico;
		run_table.bow1=200;
		run_table.bow2=3000;
		run_table.bow3=3000;
		run_table.bow4=100;	
		run_table.mode=TMC4361_RAMP_POSITION_SSHAPE;
		ths->c_postion=postion/16;//x 2270.6308  1200.5856
		return mTMC4361DriverMove(&run_table);
	}
	return mTMC4361DevicMoveby(ths,speed,acc,postion,dir,mico,current);
}


char mTMC4361DevicMoveby(MOTOR* ths,int speed,int acc,int postion,char dir,int mico,char current) //相对运动
{
	TABLE run_table;
	//postion += ths->Zero_drift;
	if(ths->mot_cfg->dir_vol ) postion= 0-postion;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	run_table.AMAX=acc;
	run_table.VMAX=speed;
	run_table.DMAX=acc;
	run_table.postion=(dir==0)? mTMC4361DeviceGetPostion(ths,0)+postion :mTMC4361DeviceGetPostion(ths,0)-postion;
	run_table.run_currnet=current;
	run_table.MicoStep=mico;
	run_table.bow1=200;
	run_table.bow2=3000;
	run_table.bow3=3000;
	run_table.bow4=100;	
	run_table.mode=TMC4361_RAMP_POSITION_SSHAPE;
	ths->c_postion+=postion/16;
	return mTMC4361DriverMove(&run_table);
}

int mTMC4361DeviceGetPostion(MOTOR* ths,char flg)//获取电机的当前位置
{
	int result=0;
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	if(flg == 0) 					 //获取驱动器当前位置  真实位置
		result=mTMC4361DriverGetPosition(&run_table);
	else
	{		
		result = ths->c_postion;
//		result=ths->mot_cfg->dir_vol ? 0-mTMC4361DriverGetPosition(&run_table):mTMC4361DriverGetPosition(&run_table);
	}
	
	return result;
}

int mTMC4361DeviceGetENCPostion(MOTOR* ths,char flg)
{
	int result=0;
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	if(flg == 0) 					 //获取驱动器当前位置  真实位置
		result=mTMC4361DriverGetENCPosition(&run_table);
	else
	{		
		result=ths->mot_cfg->dir_vol ? 0-mTMC4361DriverGetENCPosition(&run_table):mTMC4361DriverGetENCPosition(&run_table);
	}

	return result;
}

void mTMC4361DeviceSetPostion(MOTOR* ths,int pos)//
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	mTMC4361DriverSetPostion(&run_table,pos);
}

void mTMC4361DeviceResetPostion(MOTOR* ths)//清零位置寄存器  使之等于零
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	mTMC4361DriverResetPostion(&run_table);	
}

int mTMC4361DeviceGetSpeed(MOTOR* ths)
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	return mTMC4361DriverGetSpeed(&run_table);		
}

int mTMC4361DeviceGetState(MOTOR* ths)//获取驱动器状态 
{	  
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	return  mTMC4361DriverGetState(&run_table);
}

char mTMC4361Device_Init(MOTOR* ths) //初始化
{
	TABLE run_table;
	run_table.mTMCDriverReg=ths->drvie_reg;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	return mTMC4361Driver_Init(&run_table);//初始化	
}


int mTMC4361Motor_Move(MOTOR* ths)  //移动  包括回零  相对移动  绝对移动
{
	int err=0;
	if(ths->status == MOTOR_STATUS_INIT_FAIL)
	{
		printf("motor init fail....\r\n");
		return ERR_MOTOR_INIT_FAIL;
	}
	
	if(ths == NULL)
	{
		printf("device ERR_MOTOR_ARGS_ERROR....\r\n");
		return ERR_MOTOR_ARGS_ERROR;		
	}
	
	if(ths->isBackZero == 1) /*在新的快速复位程序里，零位不是原点*/
	{	
		if(ths->status == MOTOR_STATUS_ZERO_FINISH || ths->status == MOTOR_STATUS_INIT_FINISH || ths->status == MOTOR_STATUS_RESET_FINISH )  
		{
			return ERR_MOTOR_NO;
		}
		if(mTMC4361DeviceGetPostion(ths,1) <= 0) /*电机必须在原点外面*/
		{
			return ERR_MOTOR_NO;
		}	
				
		mTMC4361DevicMoveto(ths,ths->reset_speed,ths->reset_acc,ths->Zero_drift,1,ths->micostep,5);
		err=mTMC4361DevicWait_Sem(ths,WAIT_REACH_SEM,8000);
		
		printf("back zero successful backzero=%d\r\n",ths->BackZero);
		ths->status=MOTOR_STATUS_ZERO_FINISH;
		ths->c_postion=0;
		return ERR_MOTOR_NO;	
	}
	else /*移动部分代码*/
	{
		ths->speed=ths->speed * 7;
		ths->acc=10000;
		if(ths->isRelative == 1)
		{	
			if(ths->postion != 0)
			{				
				mTMC4361DevicMoveby(ths,ths->speed,ths->acc,ths->postion * 16,0,ths->micostep,ths->i_run);
			}
			else
			{
				return ERR_MOTOR_NO;
			}
		}
		else
		{
			/*己经回零或己经复位后 电机不在零位  这与以前驱动程序不一样  为了一致  当移动动零位时就相当于复位与回零*/
			if(ths->postion == 0)
			{
				if(ths->status == MOTOR_STATUS_ZERO_FINISH || ths->status == MOTOR_STATUS_RESET_FINISH)
				{
					return ERR_MOTOR_NO;
				}
			}
			if(mTMC4361DeviceGetPostion(ths,1) != ths->postion )
			{
				mTMC4361DevicMoveto(ths,ths->speed,ths->acc,ths->postion * 16,0,ths->micostep,6);
			}
			else
			{
				return ERR_MOTOR_NO;
			}
		}	
		err=mTMC4361DevicWait_Sem(ths,WAIT_REACH_SEM,8000);
	
		ths->status=MOTOR_STATUS_MOVE_FINISH;	
	}
	return err == MOTOR_STATUS_STOP ? ERR_MOTOR_NO:MOTOR_STATUS_ERROR_TIME_OUT ;
}

int mTMC4361Motor_Reset(MOTOR* ths)//复位
{
	int result=0;
	if(ths->status == MOTOR_STATUS_INIT_FAIL)
	{
		printf("motor init fail....\r\n");
		return ERR_MOTOR_INIT_FAIL;
	}

	if(ths == NULL)
	{
		printf("device ERR_MOTOR_ARGS_ERROR....\r\n");
		return ERR_MOTOR_ARGS_ERROR;		
	}
	
	ths->Zero_drift=0;
	ths->ZeorTouch=0;
	ths->ZeroBrake=0;
	ths->BrakeStep=0;
	
	ths->dir=0;//0是正向
	
//	ths->dir= ths->mot_cfg->dir_vol ? DRI_REVERAL(ths->mot_cfg->dir_vol):ths->mot_cfg->dir_vol;
	ths->read_io(PIN_ID_SET_SENSOR,ths->zeor_isr_id,0);

//	int stop_speed=(unsigned int)ths->reset_speed * ths->reset_speed / ths->mot_cfg->zero_cordinate * 0.0038845/30;
	
	if(ths->read_io(READ_PIN_ID_SENSOR,ths->zeor_isr_id,0) == ths->mot_cfg->zero_trig_vol)  
	{
		printf("zero triger\r\n");
		mTMC4361DevicRotate(ths,ths->reset_speed/2,ths->reset_acc/2,ths->dir,ths->micostep,ths->i_run);
		while(ths->read_io(READ_PIN_ID_SENSOR,ths->zeor_isr_id,0) == ths->mot_cfg->zero_trig_vol);
		mTMC4361DevicMoveby(ths,ths->reset_speed/2,ths->reset_acc/2,ths->mot_cfg->rs_cordinate*16*2,ths->dir,ths->micostep,ths->i_run);
		if(MOTOR_STATUS_ERROR_TIME_OUT == mTMC4361DevicWait_Sem(ths,WAIT_REACH_SEM,8000))
		{
			printf("1 back zero time out.....\r\n");
			ths->status=MOTOR_STATUS_ERROR_TIME_OUT;
			goto STOP;
		}
		mTMC4361DevicMoveby(ths,ths->reset_speed,ths->reset_acc,ths->mot_cfg->rs_cordinate*16*4,DRI_REVERAL(ths->dir),ths->micostep,ths->i_run);
	}
	else
	{
		mTMC4361DevicRotate(ths,ths->reset_speed,ths->reset_acc,DRI_REVERAL(ths->dir),ths->micostep,ths->i_run); 
	}
	if(MOTOR_STATUS_ERROR_TIME_OUT == mTMC4361DevicWait_Sem(ths,WAIT_TOUCH_SEM,8000))
	{
		printf("2 back zero time out.....\r\n");
		ths->status= MOTOR_STATUS_ERROR_TIME_OUT;
		goto STOP;
	}
	mTMC4361DevicMoveby(ths,ths->reset_speed,ths->reset_acc,ths->mot_cfg->rs_cordinate*16,DRI_REVERAL(ths->dir),ths->micostep,ths->i_run);	
	if(MOTOR_STATUS_ERROR_TIME_OUT == mTMC4361DevicWait_Sem(ths,WAIT_REACH_SEM,8000))
	{
		printf("1 back zero time out.....\r\n");
		ths->status=MOTOR_STATUS_ERROR_TIME_OUT;
		goto STOP;
	}
STOP:		
//		ths->ZeorTouch = mTMC4361DeviceGetPostion(ths,0);
//	mTMC4361DevicStop(ths,stop_speed);			
//	mTMC4361DevicWait_Sem(ths,WAIT_STOP_SEM,0);
	ths->Zero_drift=mTMC4361DeviceGetPostion(ths,0);
	printf("Zero_drift=%d\r\n",ths->Zero_drift);
	ths->is_reset=1;
	ths->c_postion=0;

	if(result == MOTOR_STATUS_ERROR_TIME_OUT) //光耦检测超时
	{
		printf("reset time out.......\r\n");
		mTMC4361DevicStop(ths,STOP_AMAX);
		ths->status = MOTOR_STATUS_ERROR_TIME_OUT;
		return ERR_MOTOR_SENSOR_BROKEN;  //光耦检测超时后  复位也失败
	}
	else 
	{
		printf("the motor %d reset successful backstep=%d\r\n",ths->id,ths->BrakeStep);
		ths->status=MOTOR_STATUS_RESET_FINISH;
		return ERR_MOTOR_NO;
	}
}

int mTMC4361Motor_MultiMove(int cnt,motor_multi_move_args_t *margs,bool is_move_to_zero)//多电机移动
{
	int err=0;	
	for(char i=0;i<cnt;i++)
	{
		if(margs->motor[i].status == MOTOR_STATUS_INIT_FAIL)
		{
			printf("motor init fail....\r\n");
			return ERR_MOTOR_INIT_FAIL;
		}	
	}	
	
	if(margs == NULL)
	{
		printf("device ERR_MOTOR_ARGS_ERROR....\r\n");
		return ERR_MOTOR_ARGS_ERROR;
	}		
	
	for(char i=0;i<cnt;i++)
	{		
		margs[i].m3_speed *= 7;
		if(margs[i].m5_isRelative == 1)
		{		
			if(margs[i].m4_distance != 0)
			{
				margs[i].motor->wait_type=WAIT_REACH_FLG;
				margs[0].motor->evnt_group=margs[0].motor->evnt_group | 1<< margs[i].m1_idx;								
				mTMC4361DevicMoveby(margs[i].motor,margs[i].m3_speed,margs[i].m2_acce,margs[i].m4_distance * 16,0,margs[i].m8_subdivision,margs[i].m7_currentLevel) ;
			}	
		}
		else
		{
			if(margs[i].m4_distance == 0 && (margs[i].motor->status == MOTOR_STATUS_ZERO_FINISH || margs[i].motor->status == MOTOR_STATUS_RESET_FINISH))
			{
				return ERR_MOTOR_NO;			
			}
			if(mTMC4361DeviceGetPostion(margs[i].motor,1) != margs[i].m4_distance)
			{
				margs[i].motor->wait_type=WAIT_REACH_FLG;
				margs[0].motor->evnt_group=margs[0].motor->evnt_group | 1<< margs[i].m1_idx;
				mTMC4361DevicMoveto(margs[i].motor,margs[i].m3_speed,margs[i].m2_acce,margs[i].m4_distance * 16,0,margs[i].m8_subdivision,6);	
			}						
		}		
	}
	if(margs[0].motor->evnt_group >0)
	{
		err=mTMC4361DevicWait_Sem(margs[0].motor,WAIT_REACH_FLG,8000);
		for(char i=0;i<cnt;i++)
		{
			margs[i].motor->status=MOTOR_STATUS_MOVE_FINISH;			
		}
	}		
	return err == MOTOR_STATUS_STOP ? ERR_MOTOR_NO:MOTOR_STATUS_ERROR_TIME_OUT ;
}


int mTMC4361Motor_MoveGPIOEvent(MOTOR* ths,unsigned int *pos)//监听IO事件
{
	int result=0;

	if(ths->status == MOTOR_STATUS_INIT_FAIL)
	{
		printf("motor init fail....\r\n");
		return ERR_MOTOR_INIT_FAIL;
	}
	
	if(ths == NULL)
	{
		printf("device ERR_MOTOR_ARGS_ERROR....\r\n");
		return ERR_MOTOR_ARGS_ERROR;		
	}

	if(ths->isRelative == 1)
	{				
		if(ths->postion != 0)
		{
			mTMC4361DevicMoveby(ths,ths->speed,ths->acc,ths->postion * 16,0,ths->micostep,ths->i_run);
		}
		else
		{
			return ERR_MOTOR_NO;
		}
	}
	else
	{
		if(mTMC4361DeviceGetPostion(ths,1) != ths->postion)
		{
			mTMC4361DevicMoveto(ths,ths->speed,ths->acc,ths->postion,0,ths->micostep,ths->i_run);		
		}	
		else
		{
			return ERR_MOTOR_NO;
		}		
	}
	
//	result= ths->mListenIo(ths);
	
	do
	{
		if(ths->is_reach_target == 1) 
		{
			ths->status= MOTOR_STATUS_ERROR_TIME_OUT;
			break;
		}
		else
		{
			ths->Listen_state=HAL_GPIO_ReadPin((GPIO_TypeDef *)ths->Listen_port,1<<ths->Listen_pin);
			switch(ths->Listen_type)
			{
				case 0:if(ths->Listen_state == 0) ths->status=MOTOR_STATUS_LISTEN_IO_FINISH;break;
				case 1:if(ths->Listen_state == 1) ths->status=MOTOR_STATUS_LISTEN_IO_FINISH;break;
				case 2:if(ths->Listen_old_state == 1 && ths->Listen_state==0) ths->status=MOTOR_STATUS_LISTEN_IO_FINISH;break;
				case 3:if(ths->Listen_old_state == 0 && ths->Listen_state==1) ths->status=MOTOR_STATUS_LISTEN_IO_FINISH;break;
			}
			ths->Listen_old_state=ths->Listen_state;
			if(MOTOR_STATUS_LISTEN_IO_FINISH == ths->status)
			{
				break;
			}
		}
	}while(1);

	*pos= mTMC4361DeviceGetPostion(ths,1);

	mTMC4361DevicStop(ths,STOP_AMAX);
	mTMC4361DevicWait_Sem(ths,WAIT_STOP_SEM,8000);

	if(result == MOTOR_STATUS_LISTEN_IO_FINISH)
	{	
		ths->status=MOTOR_STATUS_LISTEN_IO_FINISH;
		return ERR_MOTOR_NO;
	}
	else
	{
		ths->status=MOTOR_STATUS_LISTEN_IO_FAIL;
		return ERR_MOTOR_LISTEN_IO_FAIL;
	}
}


int mTMC4361Motor_Rotate(MOTOR* ths)//旋转
{
	int err=0;
	if(ths->status == MOTOR_STATUS_INIT_FAIL)
	{
		printf("motor init fail....\r\n");
		return ERR_MOTOR_INIT_FAIL;
	}
	
	if(ths == NULL)
	{
		printf("device ERR_MOTOR_ARGS_ERROR....\r\n");
		return ERR_MOTOR_ARGS_ERROR;		
	}
	#if MC500
		ths->speed=ths->speed * SPEED_CORRECTION;
		ths->acc=ths->speed/2;
	#endif
	ths->micostep=ths->micostep != 0 ? ths->micostep:ths->mot_cfg->subdivision;
	ths->i_run=ths->i_run != 0 ? ths->i_run:ths->mot_cfg->currentLevel;
//	ths->dir= ths->mot_cfg->dir_vol ? DRI_REVERAL(ths->mot_cfg->dir_vol):ths->mot_cfg->dir_vol;
	
	mTMC4361DevicRotate(ths,ths->speed,ths->acc,ths->dir,ths->micostep,ths->i_run);
//	err=mTMC4361DevicWait_Sem(ths,WAIT_VELOCITY_SEM,2000);

	ths->status=MOTOR_STATUS_RORATE_FINISH;
	return err == MOTOR_STATUS_STOP ? ERR_MOTOR_NO:MOTOR_STATUS_ERROR_TIME_OUT ;
}

int mTMC4361Motor_Stop(MOTOR* ths)//停止
{	
	int err=0;
	if(ths->status == MOTOR_STATUS_INIT_FAIL)
	{
		printf("motor init fail....\r\n");
		return ERR_MOTOR_INIT_FAIL;
	}
	
	if(ths == NULL)
	{
		printf("device ERR_MOTOR_ARGS_ERROR....\r\n");
		return ERR_MOTOR_ARGS_ERROR;		
	}
	
	mTMC4361DevicStop(ths,ths->speed * ths->speed);	
	err=mTMC4361DevicWait_Sem(ths,WAIT_STOP_SEM,1000);
	ths->status=MOTOR_STATUS_STOP_FINISH;
	return err == MOTOR_STATUS_STOP ? ERR_MOTOR_NO:MOTOR_STATUS_ERROR_TIME_OUT ;
}


int mTMC4361Motor_GetStatus(MOTOR* ths)//获取状态
{
	if(ths == NULL)
	{
		printf("device ERR_MOTOR_ARGS_ERROR....\r\n");
		return ERR_MOTOR_ARGS_ERROR;		
	}
	
	ths->c_postion=mTMC4361DeviceGetPostion(ths,1);
	return ths->status;
}


void mTMC4361Motor_Init(MOTOR *ths,
						motor_cfg_struct * cfg,\
						DRVIE_REGTYPE *drive_reg,\
						void (*write_io)(pin_id_t ,int ),\
						int  (*read_io) (pin_id_t ,int,char),\
						DEFINESTATUS (*motor_state_machine)(MOTOR *),\
						int (*motor_backzeor)(MOTOR *),\
						int (*motor_reset)(MOTOR *),\
						int (*motor_listen_io)(MOTOR *),\
						int (*Move)(MOTOR* ths),\
						int (*Rotate)(MOTOR* ths),\
						int (*GetStatus)(MOTOR* ths),\
						int (*Stop)(MOTOR* ths),\
						int (*MultiMove)(int,motor_multi_move_args_t *,bool ),\
						int (*MoveGPIOEvent)(MOTOR* ,unsigned int*),\
						int (*MotorReset)(MOTOR* )\
						)
{			
	ths->read_io = read_io;
	ths->write_io= write_io;
	ths->mot_cfg = cfg;  
	ths->zeor_isr_id= read_io(PIN_INIT_TOUCH_ISR,0,0);  //t_isr_id;计算光耦触发中断号
	ths->reach_isr_id=read_io(PIN_INIT_REACH_ISR,0,0); //r_isr_id;计算位置到达中断号
	ths->error_isr_id=read_io(PIN_INIT_ERROR_ISR,0,0);//计算错误检测中断号
	ths->drvie_reg=drive_reg;//驱动器配置参数  各个电机有差异  所需要的参数也各不相同
	
//	ths->mTMC4361Motor_State_Machine=motor_state_machine;
//	
//	ths->mBackZero=motor_backzeor;
//	ths->mListenIo=motor_listen_io;	
//	ths->mReset=motor_reset;
	
	ths->mTMC4361MotorReset=MotorReset;
	ths->mTMC4361MotorMove=Move;
	ths->mTMC4361MotorRotate=Rotate;
	ths->mTMC4361MotorGetStatus=GetStatus;
	ths->mTMC4361MotorStop=Stop;
	ths->mTMC4361Motor_MultiMove=MultiMove;
	ths->mTMC4361MotorMoveGPIOEvent=MoveGPIOEvent;
	
	mTMC4361Device_Init(ths);
//	mTMC4361Device_Init(ths);
	if(mTMC4361Device_Init(ths))
	{
		ths->status = MOTOR_STATUS_INIT_FAIL;
		printf("mTMC4361Motor_Init motor %d Fail......\r\n",motor_max);
	}
	else
	{
		motor_max++;
		ths->status=MOTOR_STATUS_INIT_FINISH;
		printf("mTMC4361Motor_Init motor %d set successful......\r\n",motor_max-1);
	}
//	OSTimeDlyHMSM(0,0,1,0);
}


