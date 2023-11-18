#include "mTMC5130Device.h"
#include <stdlib.h>
#include "ErrCode.h"

//#define MC500    //用来定义此功能是否是MC500独有

#define SPEED_CORRECTION  1 //1.6  //速度修正比例
#define QUICK_RESET_BACKZERO   //定义是否进行快速复位  快速回零 
unsigned char motor_type=0x5;

#define setbit(x,y) x|=(1<<y) 
#define clrbit(x,y) x&=~(1<<y) 

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

void mTMC5130ProcessSemPost(MOTOR *ths,int wait_type)
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

int mTMC5130DevicWait_Sem(MOTOR* ths,char wait_type,unsigned int wait_time)
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
			while(GET_STATE_BIT(mTMC5130DeviceGetState(ths),STANDSTILL) != 1 && t < wait_time/100)
			{
				t++;
				OSTimeDlyHMSM(0,0,0,100);
			}	
			if(t > wait_time/100)
			{
				ths->status=MOTOR_STATUS_ERROR_TIME_OUT;						  
			}
			goto NO_ERROR_RETURN;
		}
		case WAIT_VELOCITY_SEM://等待速度到达信号量 
		{								   
			static char t;
			while(GET_STATE_BIT(mTMC5130DeviceGetState(ths),VELOCITY_READHED) != 1 && t < wait_time/100)
			{
				t++;
				OSTimeDlyHMSM(0,0,0,100);//间隔100个时间
			}	
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
		mTMC5130DevicStop(ths,STOP_AMAX);		
		ths->status=MOTOR_STATUS_ERROR_MOTORABORT;
		ths->wait_sem=NULL;
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
		HAL_NVIC_DisableIRQ((IRQn_Type)ths->reach_isr_id);
		HAL_NVIC_DisableIRQ((IRQn_Type)ths->zeor_isr_id);					
		ths->wait_type=0;
		ths->is_reset=0;
	}
	return result;
}


char mTMC5130DevicStop(MOTOR* ths,int acc) //按减速度停止电机   减速度越大   停止用时越短
{   
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	run_table.AMAX=acc;
	run_table.VMAX=0;
	run_table.D1=acc;
	run_table.DMAX=acc;
	run_table.postion= 0;
	run_table.run_currnet=ths->mot_cfg->currentLevel;
	run_table.hold_current=ths->mot_cfg->holdLevel;
	run_table.MicoStep=ths->mot_cfg->subdivision;//停止时延用上一次设置的电流值大小  这里的配置不起作用
	run_table.mode=TMC5130_MODE_VELNEG;
	ths->is_abort=false;
	return mTMC5130DriverMove(&run_table);
}


char mTMC5130DevicRotate(MOTOR* ths,int speed,int acc,char dir,int mico,char current)   //速度模式  按指定速度转动
{	   
	TABLE run_table;
	if(ths->mot_cfg->dir_vol ) dir= DRI_REVERAL(dir);	
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	run_table.A1=acc/4;
	run_table.V1= speed/2;
	run_table.AMAX=acc/4;
	run_table.VMAX=abs(speed);
	run_table.D1=acc;
	run_table.DMAX=acc/4;
	run_table.run_currnet=ths->mot_cfg->currentLevel;
	run_table.hold_current=ths->mot_cfg->holdLevel;
	run_table.MicoStep=mico;
	run_table.mode=(dir == 0)  ? TMC5130_MODE_VELPOS : TMC5130_MODE_VELNEG;
	return mTMC5130DriverMove(&run_table);
}


char mTMC5130DevicMoveto(MOTOR* ths,int speed,int acc,int postion,char dir,int mico,char current)  //绝对运动 没有方向  目标位置在当前位置前向就正转  反之则反转
{
	TABLE run_table;
	if(ths->mot_cfg->dir_vol ) postion= 0-postion;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	run_table.A1=acc/4;
	run_table.V1= speed/2;
	run_table.AMAX=acc/4;
	run_table.VMAX=abs(speed) *1.32;
	run_table.D1=acc;
	run_table.DMAX=acc/4;
	run_table.postion=(ths->mot_cfg->dir_vol )? postion : postion;//与Zero_drift做运算进行失步补偿
	run_table.run_currnet=ths->mot_cfg->currentLevel;
	run_table.hold_current=ths->mot_cfg->holdLevel;
	run_table.MicoStep=mico;
	run_table.mode=TMC5130_MODE_POSITION;
	return mTMC5130DriverMove(&run_table);
}


char mTMC5130DevicMoveby(MOTOR* ths,int speed,int acc,int postion,char dir,int mico,char current) //相对运动
{
	TABLE run_table;
	postion += ths->Zero_drift;	
	if(ths->mot_cfg->dir_vol ) postion= 0-postion;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	run_table.A1=acc/4;
	run_table.V1= speed/2;
	run_table.AMAX=acc/4;
	run_table.VMAX=abs(speed) *1.32;
	run_table.D1=acc;
	run_table.DMAX=acc/4;
	run_table.postion=(dir==0)? mTMC5130DeviceGetPostion(ths,0)+postion :mTMC5130DeviceGetPostion(ths,0)-postion;
	run_table.run_currnet=ths->mot_cfg->currentLevel;
	run_table.hold_current=ths->mot_cfg->holdLevel;
	run_table.MicoStep=mico;
	run_table.mode=TMC5130_MODE_POSITION;		
	return mTMC5130DriverMove(&run_table);
}

int mTMC5130DeviceReadWriteRegister(MOTOR* ths,char addr,char cmd,int data)//直接对寄存器进行读取
{	  
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	return  mTMC5130DriverReadWriteRegister(&run_table,addr,cmd,data);
}

int mTMC5130DeviceGetPostion(MOTOR* ths,char flg)//获取电机的当前位置
{
	int result=0;
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	if(flg == 0) 					 //获取驱动器当前位置  真实位置
		result=mTMC5130DriverGetPosition(&run_table);
	else
	{		
		result=ths->mot_cfg->dir_vol ? 0-mTMC5130DriverGetPosition(&run_table):mTMC5130DriverGetPosition(&run_table);
	}
	return result;
}

char mTMC5130Device_GetPara(MOTOR* ths)//获取驱动器所有参数
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	return  mTMC5130DriverGetPara(&run_table);
}

int mTMC5130DeviceGetSpeed(MOTOR* ths)
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	return mTMC5130DriverGetSpeed(&run_table);//获取当前速度
}

void mTMC5130DeviceSetPostion(MOTOR* ths,int pos)//
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	mTMC5130DriverSetPostion(&run_table,pos);
}

void mTMC5130DeviceResetPostion(MOTOR* ths)//清零位置寄存器  使之等于零
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	mTMC5130DriverResetPostion(&run_table);	
}


int mTMC5130DeviceGetDRVSTATUS(MOTOR *ths) //获取驱动器信息
{  	  
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	return mTMC5130DriverGetDRVSTATUS(&run_table);
}


int mTMC5130DeviceGetRAMPSTAT(MOTOR *ths) //获取驱动器标志位
{  	  
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	return mTMC5130DriverGetRAMPSTAT(&run_table);
}

int mTMC5130DeviceSETSWMODE(MOTOR *ths,int data) 
{  	  
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	return mTMC5130DriverSETSWMODE(&run_table,data);
}

char mTMC5130DeviceGetState(MOTOR* ths)//获取驱动器状态 
{	  
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	return  mTMC5130DriverGetState(&run_table);
}

void mTMC5130Device_SetDefault(MOTOR* ths) //配置
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	run_table.MicoStep=ths->mot_cfg->subdivision;
	run_table.hold_current=ths->mot_cfg->holdLevel <= 0 ? 15:ths->mot_cfg->holdLevel;
	run_table.run_currnet=ths->mot_cfg->currentLevel <= 0 ? 20:ths->mot_cfg->currentLevel;
	mTMC5130DriverSetMicoStep(&run_table);//设置细分
	mTMC5130DriverSetCurrent(&run_table);//设置运行时电流  及停止电流
}

void mTMC5130Device_Init(MOTOR* ths) //初始化
{
	TABLE run_table;
	run_table.mTMCDriverReg=ths->drvie_reg;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	mTMC5130Driver_Init(&run_table);//初始化	
}

int mTMC5130Motor_Move(MOTOR* ths)  //移动  包括回零  相对移动  绝对移动
{
	int err=0;
	int result=0;
#ifdef USE_FULL_ASSERT  /*错误检测*/		
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
#endif	
#ifdef MC500
	ths->speed =ths->speed * SPEED_CORRECTION;
	ths->acc=ths->speed/2;	
#endif	
	ths->micostep=ths->micostep != 0 ? ths->micostep:ths->mot_cfg->subdivision;
	ths->i_run=ths->i_run != 0 ? ths->i_run:ths->mot_cfg->currentLevel;

	if(ths->isBackZero == 1) /*在新的快速复位程序里，零位不是原点*/
	{	
		if(ths->status == MOTOR_STATUS_ZERO_FINISH || ths->status == MOTOR_STATUS_INIT_FINISH || ths->status == MOTOR_STATUS_RESET_FINISH )  
		{
			return ERR_MOTOR_NO;
		}
		if(mTMC5130DeviceGetPostion(ths,1) <= 0) /*电机必须在原点外面*/
		{
			return ERR_MOTOR_NO;
		}	
		
		ths->Zero_drift=0;
		ths->dir= ths->mot_cfg->dir_vol ? ths->mot_cfg->dir_vol:DRI_REVERAL(ths->mot_cfg->dir_vol);
		
#ifdef QUICK_RESET_BACKZERO	
	#ifdef MC500
		ths->speed=ths->reset_speed;	
	#endif	
//		ths->speed = ths->speed > ths->reset_speed ? ths->reset_speed :ths->speed;
//		ths->acc = ths->acc > ths->reset_acc ? ths->reset_acc : ths->acc;		
		ths->reset_acc=(unsigned int)ths->speed * ths->speed / ths->mot_cfg->zero_cordinate * 0.0038845;
		ths->status=MOTOR_STATUS_ZERO_START_QUICK;
	#else
		ths->status=MOTOR_STATUS_ZERO_START_THROD;
#endif
		
		result=ths->mBackZero(ths);//回零完成后 返回	
		
		if(result == MOTOR_STATUS_ERROR_TIME_OUT) //光耦触发超时
		{
			printf("lost_endure_stop\r\n");
			ths->status=MOTOR_STATUS_ZERO_FAIL;
			mTMC5130DevicStop(ths,4000);
			return ERR_MOTOR_LOST_STEP;//电机失步停止	
		}
		else 
		{
			if(abs(ths->BackZero) < ths->mot_cfg->lost_endure_continue)\
			{
				printf("back zero successful backzero=%d\r\n",ths->BackZero);
				ths->status=MOTOR_STATUS_ZERO_FINISH;
				return ERR_MOTOR_NO;
			}
			else if(abs(abs(ths->BackZero) - abs(ths->ZeorTouch)) > ths->mot_cfg->lost_endure_stop)
			{
				printf("lost_endure_stop backzero=%d\r\n",ths->BackZero);
				ths->status=MOTOR_STATUS_ZERO_FAIL;
				return ERR_MOTOR_LOST_STEP;//电机失步停止
			}	
			else
			{
				printf("back zero successful backzero=%d\r\n",ths->BackZero);
				ths->status=MOTOR_STATUS_ZERO_FINISH;
				return ERR_MOTOR_NO;					
			}
		}		
	}
	else /*移动部分代码*/
	{
		if(ths->isRelative == 1)
		{	
			if(ths->postion != 0)
			{				
				mTMC5130DevicMoveby(ths,ths->speed,ths->acc,ths->postion,0,ths->micostep,ths->i_run);
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
			if(mTMC5130DeviceGetPostion(ths,1) != ths->postion )
			{
				mTMC5130DevicMoveto(ths,ths->speed,ths->acc,ths->postion,0,ths->micostep,ths->i_run);
			}
			else
			{
				return ERR_MOTOR_NO;
			}
		}	
		err=mTMC5130DevicWait_Sem(ths,WAIT_REACH_SEM,0);
#ifdef QUICK_RESET_BACKZERO
		if(ths->Zero_drift != 0)
		{
			ths->Zero_drift=0;
			ths->write_io(PIN_ID_REFR,0);
			mTMC5130DevicStop(ths,4000);
			mTMC5130DeviceSetPostion(ths,(ths->mot_cfg->dir_vol )? 0-ths->postion : ths->postion);
			ths->write_io(PIN_ID_REFR,1);
		}
#endif		
		ths->status=MOTOR_STATUS_MOVE_FINISH;	
	}
	return err == MOTOR_STATUS_STOP ? ERR_MOTOR_NO:MOTOR_STATUS_ERROR_TIME_OUT ;
}

int mTMC5130Motor_Reset(MOTOR* ths)//复位
{
	int result=0;
#ifdef USE_FULL_ASSERT		
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
#endif	
	ths->Zero_drift=0;
	ths->ZeorTouch=0;
	ths->ZeroBrake=0;
	ths->BrakeStep=0;
#ifdef MC500
	ths->speed=ths->reset_speed;
	ths->acc=ths->reset_acc;
#endif	
#ifdef QUICK_RESET_BACKZERO	
//	ths->speed = ths->speed > ths->reset_speed ? ths->reset_speed :ths->speed;
//	ths->acc = ths->acc > ths->reset_acc ? ths->reset_acc : ths->acc;
	ths->reset_acc=(unsigned int)ths->speed * ths->speed / ths->mot_cfg->zero_cordinate * 0.0038845;
	ths->status=MOTOR_STATUS_RESET_START_QUICK;//快速复位
#else		
	ths->status=MOTOR_STATUS_RESET_START;
#endif
	ths->micostep=ths->micostep != 0 ? ths->micostep:ths->mot_cfg->subdivision;
	ths->i_run=ths->i_run != 0 ? ths->i_run:ths->mot_cfg->currentLevel;		
	ths->dir= ths->mot_cfg->dir_vol ? ths->mot_cfg->dir_vol:DRI_REVERAL(ths->mot_cfg->dir_vol);
	
	ths->read_io(PIN_ID_SET_SENSOR,ths->zeor_isr_id,0);

	result=ths->mReset(ths);
	
	if(result == MOTOR_STATUS_ERROR_TIME_OUT) //光耦检测超时
	{
		printf("reset time out.......\r\n");
		mTMC5130DevicStop(ths,STOP_AMAX);
		ths->status = MOTOR_STATUS_ERROR_TIME_OUT;
		return ERR_MOTOR_SENSOR_BROKEN;  //光耦检测超时后  复位也失败
	}
	else 
	{
		if(ths->mot_cfg->zero_cordinate >= abs(ths->BrakeStep) -10) /*刹车距离超过零位距离则报错*/
		{ 
			printf("the motor %d reset successful backstep=%d\r\n",ths->id,ths->BrakeStep);
			ths->status=MOTOR_STATUS_RESET_FINISH;
			return ERR_MOTOR_NO;
		}
		else 
		{ 
			printf("the motor %d reset fail backstep=%d\r\n",ths->id,ths->BrakeStep);
			ths->status=MOTOR_STATUS_RESET_FAIL;
			return ERR_MOTOR_RESET_FAIL;
		} 
	}
}

int mTMC5130Motor_MultiMove(int cnt,motor_multi_move_args_t *margs,bool is_move_to_zero)//多电机移动
{
	int err=0;	
#ifdef USE_FULL_ASSERT		
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
#endif	
	for(char i=0;i<cnt;i++)
	{	
#ifdef MC500
		margs[i].m3_speed=margs[i].m3_speed * SPEED_CORRECTION;
		margs[i].m2_acce=margs[i].m3_speed/2;
#endif
		margs[i].motor->micostep=margs[i].motor->micostep !=0 ? margs[i].motor->micostep:margs[i].motor->mot_cfg->subdivision;
		margs[i].motor->i_run=margs[i].motor->i_run != 0 ?margs[i].motor->i_run:margs[i].motor->mot_cfg->currentLevel;		
		
		if(margs[i].m5_isRelative == 1)
		{		
			if(margs[i].m4_distance != 0)
			{
				margs[i].motor->wait_type=WAIT_REACH_FLG;
				margs[0].motor->evnt_group=margs[0].motor->evnt_group | 1<< margs[i].m1_idx;								
				mTMC5130DevicMoveby(margs[i].motor,margs[i].m3_speed,margs[i].m2_acce,margs[i].m4_distance,0,margs[i].m8_subdivision,margs[i].m7_currentLevel) ;
			}	
		}
		else
		{
			if(margs[i].m4_distance == 0 && (margs[i].motor->status == MOTOR_STATUS_ZERO_FINISH || margs[i].motor->status == MOTOR_STATUS_RESET_FINISH))
			{
				return ERR_MOTOR_NO;			
			}
			if(mTMC5130DeviceGetPostion(margs[i].motor,1) != margs[i].m4_distance)
			{
				margs[i].motor->wait_type=WAIT_REACH_FLG;
				margs[0].motor->evnt_group=margs[0].motor->evnt_group | 1<< margs[i].m1_idx;
				mTMC5130DevicMoveto(margs[i].motor,margs[i].m3_speed,margs[i].m2_acce,margs[i].m4_distance,0,margs[i].m8_subdivision,margs[i].m7_currentLevel);	
			}						
		}		
	}
	if(margs[0].motor->evnt_group >0)
	{
		err=mTMC5130DevicWait_Sem(margs[0].motor,WAIT_REACH_FLG,0);
		for(char i=0;i<cnt;i++)
		{
			margs[i].motor->status=MOTOR_STATUS_MOVE_FINISH;
#ifdef QUICK_RESET_BACKZERO
			if(margs[i].motor->Zero_drift != 0)
			{
				margs[i].motor->Zero_drift=0;
				margs[i].motor->write_io(PIN_ID_REFR,0);
				mTMC5130DevicStop(margs[i].motor,4000);
				mTMC5130DeviceSetPostion(margs[i].motor,margs[i].motor->mot_cfg->dir_vol==1 ? 0-margs[i].m4_distance : margs[i].m4_distance);
				margs[i].motor->write_io(PIN_ID_REFR,1);
			}
#endif				
		}
	}		
	return err == MOTOR_STATUS_STOP ? ERR_MOTOR_NO:MOTOR_STATUS_ERROR_TIME_OUT ;
}


int mTMC5130Motor_MoveGPIOEvent(MOTOR* ths,unsigned int *pos)//监听IO事件
{
	int result=0;
#ifdef USE_FULL_ASSERT	
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
#endif
#ifdef MC500
	ths->speed =ths->speed * SPEED_CORRECTION;
	ths->acc=ths->speed/2;
#endif
	ths->micostep=ths->micostep != 0 ? ths->micostep:ths->mot_cfg->subdivision;
	ths->i_run=ths->i_run != 0 ? ths->i_run:ths->mot_cfg->currentLevel;
	
	if(ths->isRelative == 1)
	{				
		if(ths->postion != 0)
		{
			mTMC5130DevicMoveby(ths,ths->speed,ths->acc,ths->postion,0,ths->micostep,ths->i_run);
		}
		else
		{
			return ERR_MOTOR_NO;
		}
	}
	else
	{
		if(mTMC5130DeviceGetPostion(ths,1) != ths->postion)
		{
			mTMC5130DevicMoveto(ths,ths->speed,ths->acc,ths->postion,0,ths->micostep,ths->i_run);		
		}	
		else
		{
			return ERR_MOTOR_NO;
		}		
	}
	
	result= ths->mListenIo(ths);
	
	*pos= mTMC5130DeviceGetPostion(ths,1);

	mTMC5130DevicStop(ths,STOP_AMAX);
	while(GET_STATE_BIT(mTMC5130DeviceGetState(ths),STANDSTILL) != 1) ;
#ifdef QUICK_RESET_BACKZERO
	if(ths->Zero_drift != 0)
	{
		ths->Zero_drift=0;
		ths->write_io(PIN_ID_REFR,0);
		mTMC5130DeviceSetPostion(ths,(ths->mot_cfg->dir_vol )? 0 - *pos : *pos);
		ths->write_io(PIN_ID_REFR,1);
	}
#endif		
	if(result == MOTOR_STATUS_LISTEN_IO_FINISH)
	{	
		ths->status=MOTOR_STATUS_LISTEN_IO_FINISH;
		printf("gpio listen MOTOR_STATUS_LISTEN_IO_FINISH");
		return ERR_MOTOR_NO;
	}
	else
	{
		ths->status=MOTOR_STATUS_LISTEN_IO_FAIL;
		printf("gpio listen ERR_MOTOR_LISTEN_IO_FAIL");
		return ERR_MOTOR_LISTEN_IO_FAIL;
	}
}


int mTMC5130Motor_Rotate(MOTOR* ths)//旋转
{
	int err=0;
#ifdef USE_FULL_ASSERT		
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
#endif
#ifdef MC500
	ths->speed=ths->speed * SPEED_CORRECTION;
	ths->acc=ths->speed/2;
#endif
	ths->micostep=ths->micostep != 0 ? ths->micostep:ths->mot_cfg->subdivision;
	ths->i_run=ths->i_run != 0 ? ths->i_run:ths->mot_cfg->currentLevel;

	mTMC5130DevicRotate(ths,ths->speed,ths->acc,ths->dir,ths->micostep,ths->i_run);
	err=mTMC5130DevicWait_Sem(ths,WAIT_VELOCITY_SEM,2000);

	ths->status=MOTOR_STATUS_RORATE_FINISH;
	return err == MOTOR_STATUS_STOP ? ERR_MOTOR_NO:MOTOR_STATUS_ERROR_TIME_OUT ;
}

int mTMC5130Motor_Stop(MOTOR* ths)//停止
{	
	int err=0;
#ifdef USE_FULL_ASSERT	
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
#endif	
	mTMC5130DevicStop(ths,ths->speed * ths->speed);
	err=mTMC5130DevicWait_Sem(ths,WAIT_STOP_SEM,1000);
	ths->status=MOTOR_STATUS_STOP_FINISH;
	return err == MOTOR_STATUS_STOP ? ERR_MOTOR_NO:MOTOR_STATUS_ERROR_TIME_OUT ;
}


int mTMC5130Motor_GetStatus(MOTOR* ths)//获取状态
{
//	TABLE run_table;
//	run_table.mTMCDriver_WriteIo=ths->write_io;
//	run_table.mTMCDriver_ReadIo=ths->read_io;
#ifdef USE_FULL_ASSERT	
	if(ths == NULL)
	{
		printf("device ERR_MOTOR_ARGS_ERROR....\r\n");
		return ERR_MOTOR_ARGS_ERROR;		
	}
#endif		
	ths->DRVSTATE=mTMC5130DeviceGetState(ths);
	ths->DRVSTATUS=mTMC5130DeviceGetDRVSTATUS(ths);
	ths->RAMPSTAT=mTMC5130DeviceGetRAMPSTAT(ths);
	ths->c_postion=mTMC5130DeviceGetPostion(ths,1);
	return ths->status;
}


void mTMC5130Motor_Init(MOTOR *ths,
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
	uint8_t index = 0;
	uint32_t start_tick = 0,current_tick = 0;
	
	ths->read_io = read_io;
	ths->write_io= write_io;
	ths->mot_cfg = cfg;  
	ths->zeor_isr_id= read_io(PIN_INIT_TOUCH_ISR,0,0);  //t_isr_id;计算光耦触发中断号
	ths->reach_isr_id=read_io(PIN_INIT_REACH_ISR,0,0); //r_isr_id;计算位置到达中断号
	ths->error_isr_id=read_io(PIN_INIT_ERROR_ISR,0,0);//计算错误检测中断号
	ths->drvie_reg=drive_reg;//驱动器配置参数  各个电机有差异  所需要的参数也各不相同
	
	ths->mTMC5130Motor_State_Machine=motor_state_machine;
	
	ths->mBackZero=motor_backzeor;
	ths->mListenIo=motor_listen_io;	
	ths->mReset=motor_reset;
	
	ths->mTMC5130MotorReset=MotorReset;
	ths->mTMC5130MotorMove=Move;
	ths->mTMC5130MotorRotate=Rotate;
	ths->mTMC5130MotorGetStatus=GetStatus;
	ths->mTMC5130MotorStop=Stop;
	ths->mTMC5130Motor_MultiMove=MultiMove;
	ths->mTMC5130MotorMoveGPIOEvent=MoveGPIOEvent;
	
	mTMC5130Device_Init(ths);
	motor_max++;
	start_tick = OSTime;//OSTimeGet();
	do
	{
		mTMC5130DevicStop(ths,4000);
		OSTimeDly(100);
		mTMC5130DeviceResetPostion(ths);
		index=mTMC5130DeviceGetPostion(ths,0);
		current_tick = OSTime;
	}while(index != 0 && current_tick - start_tick < 1000);
	
	if(current_tick - start_tick > 1000)
	{		
		ths->status = MOTOR_STATUS_INIT_FAIL;
		printf("mTMC5130Motor_Init motor %d Fail......\r\n",motor_max-1);
	}
	else
	{				
		ths->status=MOTOR_STATUS_INIT_FINISH;
		printf("mTMC5130Motor_Init motor %d set successful......\r\n",motor_max-1);
	}
}


