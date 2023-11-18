#include "mTMC5130Device.h"
#include <stdlib.h>
#include "ucos_ii.h"
#include "ErrCode.h"
#include "stm32f4xx_hal.h"
#include "mTMC5130DThread.h"

#define setbit(x,y) x|=(1<<y) 
#define clrbit(x,y)  x&=~(1<<y) 

static char motor_max;

extern MOTOR motors[];
extern motor_cfg_struct motors_cfg[];
extern DRIVER_REG driver_reg[];

unsigned int GetTimeOut(int speed,int steep) //计算超时 时间 1.354为安全系统 500为超时时间
{
	float time=((float)(1000.0 / speed) * (float)(steep)  + 500);
	//printf("speed=%d steep=%d time=%f\r\n",speed,steep,time);
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
	return result;
}

static char Get_WaitTaskId(char c)
{
	for(char i=0;i<motor_max;i++) 
	{
		if(motors[i].evnt_group >0)
		{
			clrbit(motors[i].evnt_group,c);
			//printf("%d\r\n",motors[i].evnt_group);
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
//	printf("SemPost\r\n");
	switch(wait_type)
	{
		case WAIT_TOUCH_SEM: //光耦中断检测触发中断  复位
		{
			if(ths->wait_sem != NULL && ths->wait_type == WAIT_TOUCH_SEM && IS_Targeted(ths,READ_PIN_ID_TOUCH_ISR)) 
			{
				//ths->is_ZeroTriggered = 
//				printf("WAIT_TOUCH_SEM post: %x\r\n",ths->wait_sem);
				OSSemPost(ths->wait_sem);		
			}
		}break;
		case WAIT_REACH_FLG://信号量组 多电机运行中使用到  到达触发中断
		{
			if(ths->wait_type == WAIT_REACH_FLG && IS_Targeted(ths,READ_PIN_ID_REACH_ISR))
			{
				char i=Get_WaitTaskId(ths->id);
				if(motors[i].evnt_group == 0 && motors[i].wait_sem != NULL && i<=motor_max)									
				{
//					printf("WAIT_REACH_FLG post: %x\r\n",ths->wait_sem);
					OSSemPost(motors[i].wait_sem);
				}
			}
		}break;	
		case WAIT_REACH_SEM://位置到达检测触发中断 motor_move
		{
			if(ths->wait_sem != NULL && ths->wait_type == WAIT_REACH_SEM && IS_Targeted(ths,READ_PIN_ID_REACH_ISR))
			{
//				printf("WAIT_REACH_SEM post: %x\r\n",ths->wait_sem);
				OSSemPost(ths->wait_sem);
			}
		}break;
		case WAIT_ERROR_SEM://驱动内报警出错触发中断
		{
			printf("tmc5130 driver error......\r\n");
			break;
		} 
		default:break;
	}
}

int mTMC5130DevicWait_Sem(MOTOR* ths,char wait_type,unsigned int wait_time)
{           
	u8 err=0;	
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
		case WAIT_STOP_SEM:  //等待电机停止信号被发送
		{								   
			static char t;
			//若电机没停止并且没超时，就继续等
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
	if(ths->wait_sem==NULL)
	{
		while(1)printf("sem NULL\r\n");
	}
	printf("sem_type: %d \r\n",ths->wait_type);
//	printf("wait_time: %d\r\n",wait_time);
	OSSemPend(ths->wait_sem,wait_time>0 ? wait_time:0,&err);
//	printf("err: %d  time :%d\r\n",err,wait_time);	
//	printf("out of wait_sem_time\r\n");
	if(err == OS_ERR_PEND_ABORT) //信号量被中止
	{				  
		ths->status=MOTOR_STATUS_ERROR_MOTORABORT;
//		ths->wait_sem=NULL;//这句话导致后续在执行NO_ERROR_RETURN部分时，不会执行OSSemDel删除sem，这样就会造成占用
		mTMC5130DevicStop(ths,STOP_AMAX);
	}				
	else if(err == OS_ERR_TIMEOUT)
	{
		ths->status=MOTOR_STATUS_ERROR_TIME_OUT;				  
		mTMC5130DevicStop(ths,STOP_AMAX);
	}		
	else
	{
		ths->status=MOTOR_STATUS_NO_ERROR;
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
	}
	return ths->status;
}


void mTMC5130DevicStop(MOTOR* ths,int acc) //按减速度停止电机   减速度越大   停止用时越短
{   
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;	
	run_table.DriverRegister=ths->driver_reg;
	run_table.AMAX=acc;
	run_table.VMAX=0;
	run_table.D1=acc/2;
	run_table.DMAX=acc/4;
	run_table.postion= 0;
	run_table.run_currnet=ths->mot_cfg->currentLevel; //停止时延用上一次设置的电流值大小  这里的配置不起作用
	run_table.hold_current=ths->mot_cfg->holdLevel;
	run_table.MicoStep=ths->mot_cfg->subdivision;//停止时延用上一次设置的电流值大小  这里的配置不起作用
	run_table.mode=TMC5130_MODE_VELNEG;
	run_table.DriverRegister->TCOOLTHRS= SGT(1048575/run_table.VMAX);//104875 = 0xfffff 是TCOOLTHRS的最大值
	run_table.DriverRegister->COOLCONF =TCOOL_THRS(run_table.DriverRegister->TCOOLTHRS)<< 16 | run_table.DriverRegister->COOLCONF;		
	run_table.DriverRegister->GCONF=0;
	ths->is_abort=false;
	mTMC5130DriverMove(&run_table);
}


void mTMC5130DevicRotate(MOTOR* ths,int speed,int acc,char dir,int mico,char current)   //速度模式  按指定速度转动
{	   
	TABLE run_table;
	if(ths->mot_cfg->dir_vol ) dir= DRI_REVERAL(dir);	
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;	
	run_table.DriverRegister=ths->driver_reg;
	run_table.A1=speed/5 >20? speed/5:20;
	run_table.V1= speed/2;
	run_table.AMAX=acc/2;
	run_table.VMAX=abs(speed);
	run_table.D1=acc/4;
	run_table.DMAX=acc/2;
	run_table.postion= 0;
	run_table.run_currnet=current>0 && current <32 ? current:ths->mot_cfg->currentLevel;
	run_table.hold_current=ths->mot_cfg->holdLevel;
	run_table.MicoStep=mico>0 && mico<257 ? mico:ths->mot_cfg->subdivision;
	run_table.mode=(dir == 0)  ? TMC5130_MODE_VELPOS : TMC5130_MODE_VELNEG;
	mTMC5130DriverMove(&run_table);
}


void mTMC5130DevicMoveto(MOTOR* ths,int speed,int acc,int postion,char dir,int mico,char current)  //绝对运动 没有方向 目标位置比当前位置距零位远就正转  反之则反转
{
//	printf("current:%d position1: %d ths->Zero_drift: %d\r\n",mTMC5130DeviceGetPostion(ths,0) ,postion,ths->Zero_drift);
	TABLE run_table;
//	if(ths->mot_cfg->dir_vol ) postion= 0-postion;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;	
	run_table.DriverRegister=ths->driver_reg;
	run_table.A1=acc/5 >20? acc/5:20;
	run_table.V1= speed/2;
	run_table.AMAX=acc/2;
	run_table.VMAX=abs(speed)* 1.32;
	run_table.D1=acc/4;
	run_table.DMAX=acc/2;
	///< 区分是否为回零，若是回零则不需要补偿
	if( (ths->isBackZero == 1)&&(abs(mTMC5130DeviceGetPostion(ths,0))) )
	{
		run_table.postion= (ths->mot_cfg->dir_vol )? 0-postion:postion;
		printf("position3: %d \r\n",run_table.postion);
	}
	else
	{
		int tem_postion=postion-ths->Zero_drift;//(ths->Zero_drift>0)?postion-ths->Zero_drift : postion+ths->Zero_drift;
		run_table.postion=(ths->mot_cfg->dir_vol )? 0-tem_postion : tem_postion;
		ths->c_postion = postion+ths->Zero_drift;
		printf("position2: %d ths->Zero_drift: %d\r\n",run_table.postion,ths->Zero_drift);
	}	
	run_table.run_currnet=current>0 && current <32 ? current:ths->mot_cfg->currentLevel;
	run_table.hold_current=ths->mot_cfg->holdLevel;
	run_table.MicoStep=mico>0 && mico<257 ? mico:ths->mot_cfg->subdivision;
	run_table.mode=TMC5130_MODE_POSITION;
	
	if(TURN_TABLE && ths->is_busy != 1)//定义了转盘，同时电机不是复位状态
	{	
		run_table.postion = 0-run_table.postion;
	}
	
	mTMC5130DriverMove(&run_table);
//	ths->Zero_drift=0;
}
	
void mTMC5130DevicMoveby(MOTOR* ths,int speed,int acc,int postion,char dir,int mico,char current) //相对运动
{
//	printf("mTMC5130DevicMoveby\r\n");
	TABLE run_table;
//	printf("postion: %d\r\n",postion);
//	printf("dir: %d dir_vol: %d\r\n",dir,ths->mot_cfg->dir_vol);
	if(ths->mot_cfg->dir_vol ) postion= 0-postion;//使用ths->mot_cfg->dir_vol用为正向
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;	
	run_table.DriverRegister=ths->driver_reg;
	run_table.A1=acc/5 >20? acc/5:20;
	
	run_table.V1= speed/2;
	run_table.AMAX=acc/2;
	run_table.VMAX=abs(speed) * 1.32;
	run_table.D1=acc/4;
	run_table.DMAX=acc/2;
	
	if(TURN_TABLE && ths->is_busy != 1)
	{//定义转盘且转盘不是复位状态，此句是为了让转盘复位与运动同向
		run_table.postion=(dir==0)? mTMC5130DeviceGetPostion(ths,0)-postion:mTMC5130DeviceGetPostion(ths,0)+postion;
	}
	else 
	{
		run_table.postion=(dir==0)? mTMC5130DeviceGetPostion(ths,0)+postion:mTMC5130DeviceGetPostion(ths,0)-postion;
	}
	ths->c_postion = 0-run_table.postion;//记录当前位置
//	printf("c_p %d\r\n",ths->c_postion);
//	printf("postion: %d  get_p: %d run_table.postion:%d\r\n",postion,mTMC5130DeviceGetPostion(ths,0),run_table.postion);
	run_table.run_currnet=current>0 && current <32 ? current:ths->mot_cfg->currentLevel;
	run_table.hold_current=ths->mot_cfg->holdLevel;
	run_table.MicoStep=mico>0 && mico<257 ? mico:ths->mot_cfg->subdivision;
	run_table.mode=TMC5130_MODE_POSITION;
	printf("is_busy %d\r\n",ths->is_busy);
		
	mTMC5130DriverMove(&run_table);
//	ths->Zero_drift=0;
}


void mTMC5130DeviceOscillation(MOTOR* ths,int speed,int acc,int pulse,int time)  //振荡功能  测试使用
{	  
	for(int i=0;i<time;i++)
	{
		mTMC5130DevicMoveby(ths,speed,acc,pulse*87,0,16,15);//45
		mTMC5130DevicWait_Sem(ths,WAIT_REACH_SEM,0);	
		OSTimeDlyHMSM(0,0,1,000);
		mTMC5130DevicMoveto(ths,speed,acc,0,0,16,15);//45
		mTMC5130DevicWait_Sem(ths,WAIT_REACH_SEM,0);
		OSTimeDlyHMSM(0,0,1,000);
	}
}


void mTMC5130DeviceOverZeroMove(MOTOR* ths,int speed,int acc,int postion,char dir,int mico,char current) 
{
	mTMC5130DevicRotate(ths,speed,acc,dir,mico,current);
	mTMC5130DevicWait_Sem(ths,WAIT_TOUCH_SEM,0);
	mTMC5130DevicMoveby(ths,speed/4,speed,postion,dir,mico,current);
	mTMC5130DevicWait_Sem(ths,WAIT_REACH_SEM,0);
}

int mTMC5130DeviceGetPostion(MOTOR* ths,char flg)//获取电机的当前位置
{
	int result=0;
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;	
	run_table.DriverRegister=ths->driver_reg;
	if(flg == 0)  //获取驱动器当前位置  真实位置
		result=mTMC5130DriverGetPosition(&run_table);
	else
	{
//		if(ths->Zero_drift >0)
//		{
//			mTMC5130DevicStop(ths,4000);
//			mTMC5130DeviceReadWriteRegister(ths,TMC5130_XTARGET ,SPI_WRITE_TYPE ,ths->postion);
//			//ths->Zero_drift=0;
//		}		
		result=ths->mot_cfg->dir_vol ? 0-mTMC5130DriverGetPosition(&run_table):mTMC5130DriverGetPosition(&run_table);
//	printf("current_position=%d\r\n",result);//TIM中断里有调用，不允许使用耗时操作
	}
	return result;
}


void mTMC5130DeviceResetPostion(MOTOR* ths)//清零位置寄存器  使之等于零
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;	
	run_table.DriverRegister=ths->driver_reg;
	mTMC5130DriverResetPostion(&run_table);	
}

//void mTMC5130DeviceSetPostion(MOTOR* ths,int pos)//
//{
//	TABLE run_table;
//	run_table.mTMCDriver_WriteIo=ths->write_io;
//	run_table.mTMCDriver_ReadIo=ths->read_io;
//	run_table.mTMCDriverReg=ths->motors_reg;
//	mTMC5130DriverSetPostion(&run_table,pos);
//}

int mTMC5130DeviceGetDRVSTATUS(MOTOR *ths) //获取驱动器信息
{  	  
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;	
	run_table.DriverRegister=ths->driver_reg;
	return mTMC5130DriverGetDRVSTATUS(&run_table);
}


int mTMC5130DeviceGetRAMPSTAT(MOTOR *ths) //获取驱动器标志位
{  	  
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;	
	run_table.DriverRegister=ths->driver_reg;
	return mTMC5130DriverGetRAMPSTAT(&run_table);
}


char mTMC5130DeviceGetState(MOTOR* ths)//获取驱动器状态 
{	  
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.DriverRegister=ths->driver_reg;
	return  mTMC5130DriverGetState(&run_table);
}

///读写寄存器
int mTMC5130DeviceReadWriteRegister(MOTOR* ths,char addr,char cmd,int data)//直接对寄存器进行读取 0 读 1写
{	  
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.DriverRegister=ths->driver_reg;
	return  mTMC5130DriverReadWriteRegister(&run_table,addr,cmd,data);
}

void mTMC5130DeviceResetStopSpeed(MOTOR* ths)
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.DriverRegister=ths->driver_reg;
	mTMC5130DriverResetStopSpeed(&run_table);		
}


void mTMC5130DeviceSetMicoStep(MOTOR* ths,short Mico)//直接对寄存器进行读取
{	  
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.DriverRegister=ths->driver_reg;
	run_table.MicoStep=Mico;
	mTMC5130DriverSetMicoStep(&run_table);
}


void mTMC5130DeviceSetCurrent(MOTOR* ths,char current)//直接对寄存器进行读取
{	  
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.DriverRegister=ths->driver_reg;
	run_table.run_currnet=current;
	mTMC5130DriverSetCurrent(&run_table);
}


void mTMC5130DeviceSetGCONF(MOTOR *ths,char data)
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.DriverRegister=ths->driver_reg;
	mTMC5130DriverSetGCONF(&run_table,data);		
}


void mTMC5130DeviceSetDefaultRegister(MOTOR *ths,char setType) //配置5130寄存器参数
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.DriverRegister=ths->driver_reg;
	mTMC5130DriverSetDefaultRegister(&run_table,setType);
}


void mTMC5130Device_Init(MOTOR* ths) //初始化
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.DriverRegister=ths->driver_reg;
	mTMC5130Driver_Init(&run_table);//初始化	
}


int mTMC5130Motor_Move(MOTOR* ths)
{
	
	if(ths->isBackZero == 1)
	{
		ths->is_busy = 2;//表示电机在回零
		if(abs(mTMC5130DeviceGetPostion(ths,0)) != 0 )
		{
			if( ths->read_io(READ_PIN_ID_TOUCH_ISR,ths->zeor_isr_id,0) == ths->mot_cfg->zero_trig_vol )
			{
				mTMC5130DevicMoveto(ths,ths->speed,ths->acc,0,0,ths->micostep,ths->i_run);
			}
			else
			{
				ths->mTMC5130MotorBackZero(ths);//回零完成后 返回
				ths->is_busy = 0;//表示电机空闲
				return ths->status;
			}
		}
		else if( ths->read_io(READ_PIN_ID_TOUCH_ISR,ths->zeor_isr_id,0) == ths->mot_cfg->zero_trig_vol )
		{
			ths->status = MOTOR_STATUS_NO_ERROR;
			ths->is_busy = 0;//表示电机空闲
			return ths->status;
		}
		else if( ths->read_io(READ_PIN_ID_TOUCH_ISR,ths->zeor_isr_id,0) != ths->mot_cfg->zero_trig_vol )
		{
			ths->mTMC5130MotorReset(ths);//位置寄存器为0，但光耦未触发，则复位
		}
		else 
		{
			ths->status = MOTOR_STATUS_ZERO_FAIL;
			ths->is_busy = 0;//表示电机空闲
			return ths->status;
		}
		
	}
	else
	{
		ths->is_busy = 3;//表示电机在move
		if(ths->isRelative == 1)
		{	
			if(ths->postion != 0)
			{
//				printf("move: motor%d %d\r\n",ths->id,ths->postion);
				mTMC5130DevicMoveby(ths,ths->speed,ths->acc,ths->postion,0,ths->micostep,ths->i_run);
			}
			else
			{
				ths->is_busy = 0;//表示电机空闲
				return ths->status;
			}
		}
		else
		{
//			printf("hand_position: %d ths->postion %d\r\n",mTMC5130DeviceGetPostion(ths,0),ths->postion);
			//int com_position = (ths->Zero_drift>0) ? (ths->postion-ths->Zero_drift):(ths->postion+ths->Zero_drift);//下臂组
			//int com_position = (ths->Zero_drift>0) ? (ths->postion+ths->Zero_drift):(ths->postion-ths->Zero_drift);
			if((mTMC5130DeviceGetPostion(ths,1)) != (ths->postion-ths->Zero_drift) )//((mTMC5130DeviceGetPostion(ths,1)) != (ths->postion+ths->Zero_drift))
			{
				//printf("hand\r\n");
				mTMC5130DevicMoveto(ths,ths->speed,ths->acc,ths->postion,0,ths->micostep,ths->i_run);
				//printf(" position: %d ths->postion %d\r\n",mTMC5130DeviceGetPostion(ths,0),ths->postion);
			}
			else
			{
				printf("motor at current position\r\n");
				ths->status = MOTOR_STATUS_NO_ERROR;
				ths->is_busy = 0;//表示电机空闲
				return ths->status;
			}
		}	
	}
	mTMC5130DevicWait_Sem(ths,WAIT_REACH_SEM,10000);
	ths->is_busy = 0;//表示电机空闲
	return ths->status;
}


int mTMC5130Motor_Rotate(MOTOR* ths)
{
	mTMC5130DevicRotate(ths,ths->speed,ths->acc,ths->dir,ths->micostep,ths->i_run);
	mTMC5130DevicWait_Sem(ths,WAIT_VELOCITY_SEM,2000);
	return ths->status;
}


int mTMC5130Motor_GetStatus(MOTOR* ths)
{
	ths->DRVSTATE=mTMC5130DeviceGetState(ths);
	ths->RAMPSTAT=mTMC5130DeviceGetRAMPSTAT(ths);
	ths->DRVSTATUS=mTMC5130DeviceGetDRVSTATUS(ths);
	ths->c_postion=mTMC5130DeviceGetPostion(ths,1);
	
	printf("motor %d current_postion=%d\r\n",ths->id,ths->c_postion);
	printf("..........DRVSTATUS.......\r\n");
	printf("motor %d sg_result=%x\r\n",ths->id,ths->DRVSTATUS & 0x3ff);
	printf("motor %d fsactive=%x\r\n",ths->id,ths->DRVSTATUS >> 15 & 0x1);
	printf("motor %d cs_actual=%x\r\n",ths->id,ths->DRVSTATUS >>16 & 0x1f);
	printf("motor %d stallguard=%x\r\n",ths->id,ths->DRVSTATUS >>24 & 0x1);			
	printf("motor %d ot=%x\r\n",ths->id,ths->DRVSTATUS >>25 & 0x01);
	printf("motor %d otpw=%x\r\n",ths->id,ths->DRVSTATUS>>26 & 0x01);
	printf("motor %d s2ga=%x\r\n",ths->id,ths->DRVSTATUS>>27 & 0x01);
	printf("motor %d s2gb=%x\r\n",ths->id,ths->DRVSTATUS>>28 & 0x01);
	printf("motor %d ola=%x\r\n",ths->id,ths->DRVSTATUS>>29 & 0x01);	
	printf("motor %d olb=%x\r\n",ths->id,ths->DRVSTATUS>>30 & 0x01);
	printf("motor %d stst=%x\r\n",ths->id,ths->DRVSTATUS >>31 & 0x01);	
	printf("..........DRVSTATE..........\r\n");
	printf("motor %d reset_frag=%x\r\n",ths->id,ths->DRVSTATE & 0x01);
	printf("motor %d driver_error=%x\r\n",ths->id,ths->DRVSTATE>>1 & 0x01);
	printf("motor %d sg2=%x\r\n",ths->id,ths->DRVSTATE>>2 & 0x01);
	printf("motor %d standstill=%x\r\n",ths->id,ths->DRVSTATE >>3& 0x01);
	printf("motor %d velocity_reached=%x\r\n",ths->id,ths->DRVSTATE>>4 & 0x01);
	printf("motor %d postion_reached=%x\r\n",ths->id,ths->DRVSTATE>>5 & 0x01);
	printf("motor %d status_stop_l=%x\r\n",ths->id,ths->DRVSTATE>>6 & 0x01);
	printf("motor %d status_stop_r=%x\r\n",ths->id,ths->DRVSTATE >>7 & 0x01);
	printf("..........RAMPSTAT..........\r\n");
	printf("motor %d status_sg=%d\r\n",ths->id,ths->RAMPSTAT>>13 & 0x01);
	printf("motor %d second_move=%d\r\n",ths->id,ths->RAMPSTAT >>12 & 0x01);
	printf("motor %d t_zerowait_active=%d\r\n",ths->id,ths->RAMPSTAT >>11 & 0x01);
	printf("motor %d vzero=%d\r\n",ths->id,ths->RAMPSTAT >>10 & 0x01);
	printf("motor %d position_reached=%d\r\n",ths->id,ths->RAMPSTAT >>9 & 0x01);
	printf("motor %d velocity_reached=%d\r\n",ths->id,ths->RAMPSTAT >>8 & 0x01);
	printf("motor %d event_pos_reached=%d\r\n",ths->id,ths->RAMPSTAT >>7 & 0x01);
	printf("motor %d event_stop_sg=%d\r\n",ths->id,ths->RAMPSTAT >>6 & 0x01);
	printf("motor %d event_stop_r=%d\r\n",ths->id,ths->RAMPSTAT >>5 & 0x01);
	printf("motor %d event_stop_l =%d\r\n",ths->id,ths->RAMPSTAT >>4 & 0x01);
	printf("motor %d status_latch_r=%d\r\n",ths->id,ths->RAMPSTAT >>3 & 0x01);
	printf("motor %d status_latch_l=%d\r\n",ths->id,ths->RAMPSTAT >>2 & 0x01);
	printf("motor %d status_stop_r=%d\r\n",ths->id,ths->RAMPSTAT >>1 & 0x01);
	printf("motor %d status_stop_l=%d\r\n",ths->id,ths->RAMPSTAT >>0 & 0x01);
	return ths->status;
}


int mTMC5130Motor_Stop(MOTOR* ths)
{
	mTMC5130DevicStop(ths,ths->speed * ths->speed);
	mTMC5130DevicWait_Sem(ths,WAIT_STOP_SEM,1000);
	return ths->status;
}

int mTMC5130Motor_MultiMove(int cnt,motor_multi_move_args_t *margs,bool is_move_to_zero)
{
	int err=0,max_pos=margs[0].m4_distance;
	for(char i=0;i<cnt;i++)
	{
		if(margs[i].m5_isRelative == 1)
		{		
			if(margs[i].m4_distance != 0)
			{
				printf("Multimove: %d\r\n",margs[i].m4_distance);
				margs[i].motor->wait_type=WAIT_REACH_FLG;
				margs[0].motor->evnt_group=margs[0].motor->evnt_group | 1<< margs[i].m1_idx;				
				mTMC5130DevicMoveby(margs[i].motor,margs[i].m3_speed,margs[i].m2_acce,margs[i].m4_distance,0,margs[i].m8_subdivision,margs[i].m7_currentLevel) ;
			}
		}
		else
		{	//计算出补偿之后的position的绝对值,再比较这个值和当前位置,不一样才可以创建信号量
			int move_postion=margs[i].m4_distance-margs[i].motor->Zero_drift;
			if(abs(mTMC5130DeviceGetPostion(margs[i].motor,1)) != abs(move_postion))
			{
				if(abs(mTMC5130DeviceGetPostion(margs[i].motor,1)) != margs[i].m4_distance)	
				{	
					printf("motor%d GetPostion: %d  STEP: %d\r\n",margs[i].m1_idx ,mTMC5130DeviceGetPostion(margs[i].motor,1),margs[i].m4_distance);
					if(margs[i].m4_distance != 0)margs[i].motor->isBackZero =0;
					margs[i].motor->wait_type=WAIT_REACH_FLG;
					margs[0].motor->evnt_group=margs[0].motor->evnt_group | 1<< margs[i].m1_idx;
					mTMC5130DevicMoveto(margs[i].motor,margs[i].m3_speed,margs[i].m2_acce,margs[i].m4_distance,0,margs[i].m8_subdivision,margs[i].m7_currentLevel);	
				}
			}
		}
		if(margs[i].m4_distance > max_pos) 
		{
			max_pos=margs[i].m4_distance;
		}		
	}
	if(margs[0].motor->evnt_group >0)
	{
		err=mTMC5130DevicWait_Sem(margs[0].motor,WAIT_REACH_FLG,0);	
	}	
	return err;
}


int mTMC5130Motor_MoveGPIOEvent(MOTOR* ths,unsigned int *pos)
{
	int err=0;
	ths->is_busy = 4;//表示电机在MoveGPIO
	if(ths->isRelative == 1)
	{				
		if(ths->postion != 0)
		{
			printf("MoveGPIO_by\r\n");
			mTMC5130DevicMoveby(ths,ths->speed,ths->acc,ths->postion,0,ths->micostep,ths->i_run);
			err= ths->mTMC5130MotorListenIo(ths);
		}
	}
	else
	{
		if(abs(mTMC5130DeviceGetPostion(ths,1)) != ths->postion+ths->Zero_drift)
		{
			printf("MoveGPIO_to\r\n");
			mTMC5130DevicMoveto(ths,ths->speed,ths->acc,ths->postion,0,ths->micostep,ths->i_run);
			err= ths->mTMC5130MotorListenIo(ths);			
		}														
	}			
	*pos= mTMC5130DeviceGetPostion(ths,1);
	ths->is_busy = 0;//表示电机空闲
	return err;
}


void mTMC5130Motor_Init(MOTOR *ths,
						motor_cfg_struct * cfg,\
						DRIVER_REG *reg,\
						void (*motor_write_io)(pin_id_t ,int ),\
						int  (*motor_read_io) (pin_id_t ,int,char),\
						DEFINESTATUS (*motor_state_machine)(MOTOR *),\
						int (*motor_backzeor)(MOTOR *),\
						int (*motor_reset)(MOTOR *),\
						int (*motor_listen_io)(MOTOR *),\
						int (*Move)(MOTOR* ths),\
						int (*Rotate)(MOTOR* ths),\
						int (*GetStatus)(MOTOR* ths),\
						int (*Stop)(MOTOR* ths),\
						int (*MultiMove)(int,motor_multi_move_args_t *,bool ),\
						int (*MoveGPIOEvent)(MOTOR* ,unsigned int*)\
						)
{		
	ths->read_io = motor_read_io;
	ths->write_io= motor_write_io;
	ths->mot_cfg = cfg;  
	ths->driver_reg=reg;
	ths->zeor_isr_id= motor_read_io(PIN_INIT_TOUCH_ISR,0,0);  //t_isr_id;计算光耦触发中断号
	ths->reach_isr_id=motor_read_io(PIN_INIT_REACH_ISR,0,0); //r_isr_id;计算位置到达中断号
	ths->error_isr_id=motor_read_io(PIN_INIT_ERROR_ISR,0,0);//计算错误检测中断号
	
	ths->mTMC5130Motor_State_Machine=motor_state_machine;
	
	ths->mTMC5130MotorBackZero=motor_backzeor;
	ths->mTMC5130MotorListenIo=motor_listen_io;
	
	ths->mTMC5130MotorReset=motor_reset;
	ths->mTMC5130MotorMove=Move;
	ths->mTMC5130MotorRotate=Rotate;
	ths->mTMC5130MotorGetStatus=GetStatus;
	ths->mTMC5130MotorStop=Stop;
	ths->mTMC5130Motor_MultiMove=MultiMove;
	ths->mTMC5130MotorMoveGPIOEvent=MoveGPIOEvent;
	
	mTMC5130Device_Init(ths);
	motor_max++;
	OSTimeDlyHMSM(0,0,0,100);
	do
	{
		mTMC5130DevicStop(ths,4000);
		mTMC5130DeviceResetPostion(ths);
	}while(mTMC5130DeviceGetPostion(ths,0) != 0);
	printf("mTMC5130Motor_Init motor %d set successful......\r\n",motor_max);
}


