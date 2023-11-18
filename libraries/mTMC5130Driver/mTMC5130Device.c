#include "mTMC5130Device.h"
#include <stdlib.h>
#include "math_util.h"
#include "delayus.h"

//#define MC500    //用来定义此功能是否是MC500独有

#define SPEED_CORRECTION  1 //1.6  //速度修正比例
#define QUICK_RESET_BACKZERO   //定义是否进行快速复位  快速回零 
unsigned char tmc5130_motor_type=0x5;

#define setbit(x,y) x|=(1<<y) 
#define clrbit(x,y) x&=~(1<<y) 

static char motor_max = 0;

extern MOTOR motors[];
extern motor_cfg_struct motors_cfg[];

unsigned int GetTimeOut(int a,int b) //计算超时 时间 
{
	float time=0.0;
	if(a >0 && b>0)
	{
//		time=((float)(2000.0 / a) * (float)(b))+500;
		time=(((float)(b) + 2000.0) / (float)a) + 500; // wzc 20201221
	}
	else
	{
		time=1000;
	}
	return time;
}

#define SECOND_TO_MS 1000
#define MS_TO_MAKEUP 2000

#define MS_TO_MOTOR_DELAY 500

static int mTMC5130Motor_MoveEvent(MOTOR* ths,bool is_io_event, int *pos);

unsigned int timeOut_Reset(u32 coordinate, u32 acce, u32 speed)
{
	int timeOutMs = 0;

	//加减速阶段所需的时间
	float acceUtilSpeedSecond = (1.0 * speed) / acce;
	//加减速的距离	
	int acceUtilSpeedstep = (speed*speed)/(2*acce);
	//当加速距离小于要移动的距离（先加速，再匀速）
	if(acceUtilSpeedstep < coordinate)
	{//first to speed
		unsigned int acceStep = (unsigned int)((acceUtilSpeedSecond * acceUtilSpeedSecond * acce)/2);
		
		printf("astep=%d\r\n",acceStep);
		
		timeOutMs = SECOND_TO_MS * acceUtilSpeedSecond + SECOND_TO_MS * (coordinate - acceStep) / speed;
	}
	//当加速距离大于等于要移动的距离（一直加速）
	else
	{
		timeOutMs = int_sqrt(SECOND_TO_MS * SECOND_TO_MS * (coordinate << 1)/acce);
	}
	timeOutMs += MS_TO_MAKEUP;
	printf("time out = %d\r\n", timeOutMs);
	//电机移动过程中非理想状态，为确保时间充足，超时时间*2
	return timeOutMs << 1;
}

//unsigned int timeOut_Reset(u32 coordinate, u32 acce, u32 speed)
//{
//		printf("cor=%d,a=%d,sp=%d\r\n", coordinate, acce, speed);
//	
//		int timeOutMs = 0;
//		float acceUtilStopSecond = 2 * coordinate /speed;
//	
//		float acceUtilSpeedSecond = speed / acce;
//		
//		printf("ast=%f,asp=%f\r\n", acceUtilStopSecond, acceUtilSpeedSecond);
//	
//		if(acceUtilStopSecond > acceUtilSpeedSecond)
//		{//first to speed
//			unsigned int acceStep = (unsigned int)(acceUtilSpeedSecond * acceUtilSpeedSecond * acce);
//			
//			timeOutMs = SECOND_TO_MS * acceUtilSpeedSecond * 2 + SECOND_TO_MS * (coordinate - acceStep) / speed;
//		}else
//		{
//				timeOutMs = int_sqrt(SECOND_TO_MS * SECOND_TO_MS * (coordinate << 1)/acce);
//		}
//		
//		timeOutMs += MS_TO_MAKEUP;
//		printf("time out = %d\r\n", timeOutMs);
//		
//		printf("%d\r\n", timeOutMs);

//		return timeOutMs << 1;
//}

GPIO_PinState mGPIOReadPin(GPIO_TypeDef* port, uint16_t pin)
{
    int lCnt=0, hCnt=0, i = 0;
    do{
        if(HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_RESET)
        {
            lCnt++;
        } 
        else 
        {
            hCnt++;
        }
        if(lCnt>5 || hCnt>5){ break;}
        delayUs(2);
    }while(i++<10);
    return lCnt > hCnt ? GPIO_PIN_RESET : GPIO_PIN_SET;
}

// static bool is_back_zero(MOTOR* ths)
// {
// 	return ths->isBackZero != BACK_ZERO_NO;
// }

// static bool is_back_real_zero(MOTOR* ths)
// {
// 	return ths->isBackZero == BACK_ZERO_REAL;
// }

static int IS_Targeted(MOTOR *ths,int type)  ///< 在相应中断触发后 检查对应IO的状态
{
	//wangzhen 
//	int result=0;
//	if(type == READ_PIN_ID_REACH_ISR)
//	{
//		result=ths->read_io((pin_id_t)type,0,0);
//		result=!result;
//	}
//	else if(type == READ_PIN_ID_TOUCH_ISR)
//	{
//		result=ths->read_io((pin_id_t)type,0,ths->mot_cfg->zero_trig_vol);
//		result=(ths->mot_cfg->zero_trig_vol==0) ? !result:result;
//	}
//	else if(type == READ_PIN_ID_ERROR_ISR)
//	{
//		result=ths->read_io((pin_id_t)type,0,0);
//		result=!result;
//	}
//	return result;
		return 1;
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
                    //竟然会触发两次中断
					char i=Get_WaitTaskId(ths->id);
					if(motors[i].evnt_group == 0 && motors[i].wait_sem != NULL && i<motor_max)									
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
//			printf("TOUCH_SEM %d\r\n",ths->Listen_old_state);
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
		//TODO::有些引脚会公用中断，在这里关闭中断可能会导致部分中断丢失
		//switch(wait_type)
		//{
			// case WAIT_REACH_SEM:
			// case WAIT_REACH_FLG:
			// 	HAL_NVIC_DisableIRQ((IRQn_Type)ths->reach_isr_id);
			// 	break;
			// case WAIT_TOUCH_SEM:
			// 	HAL_NVIC_DisableIRQ((IRQn_Type)ths->zeor_isr_id);	
			// 	break;
			//case WAIT_ERROR_SEM:
			//	HAL_NVIC_DisableIRQ((IRQn_Type)ths->error_isr_id);	
			//	break;
			//default:break;
		//}			
		ths->wait_type=0;
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
	run_table.D1=acc/2;
	run_table.DMAX=acc/4;
	run_table.postion= 0;
	run_table.run_currnet=ths->mot_cfg->currentLevel; //停止时延用上一次设置的电流值大小  这里的配置不起作用
	run_table.hold_current=ths->mot_cfg->holdLevel;
	run_table.MicoStep=ths->mot_cfg->subdivision;//停止时延用上一次设置的电流值大小  这里的配置不起作用
	run_table.mode=TMC5130_MODE_VELNEG;
//	run_table.mTMCDriverReg->TCOOLTHRS= SGT(1048575/run_table.VMAX);//104875 = 0xfffff 是TCOOLTHRS的最大值
//	run_table.mTMCDriverReg->COOLCONF =TCOOL_THRS(run_table.mTMCDriverReg->TCOOLTHRS)<< 16 | run_table.mTMCDriverReg->COOLCONF;		
//	run_table.mTMCDriverReg->GCONF=0;
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
	run_table.A1=speed/5 >20? speed/5:20;
	run_table.V1= speed/2;
	run_table.AMAX=acc;
	run_table.VMAX=abs(speed);
	run_table.D1=acc/4;
	run_table.DMAX=acc/2;
	run_table.postion= 0;
	run_table.run_currnet=current>0 && current <32 ? current:ths->mot_cfg->currentLevel;
	run_table.hold_current=ths->mot_cfg->holdLevel;
	run_table.MicoStep=mico>0 && mico<257 ? mico:ths->mot_cfg->subdivision;
	run_table.mode=(dir == 0)  ? TMC5130_MODE_VELPOS : TMC5130_MODE_VELNEG;
//	run_table.DriverRegister->TCOOLTHRS= SGT(1048575/run_table.VMAX);
//	run_table.DriverRegister->COOLCONF =TCOOL_THRS(run_table.DriverRegister->TCOOLTHRS)<< 16 | run_table.DriverRegister->COOLCONF;	
//	run_table.DriverRegister->GCONF=4;

	return mTMC5130DriverMove(&run_table);
}


/**
*若位置相同,将会造成电机一直阻塞, 调用者需要检查返回值做处理
**/
char mTMC5130DevicMoveRealPosition(MOTOR* ths,int speed,int acc,int postion,int mico,char current)
{
	TABLE run_table;

// js项目泵的电机零位中断与位置到达中断在同一中断源, 在回零是两个中断几乎同时触发, 若在处理过程中,另一个中断触发, 会偶发导致其无法响应
// 上位机做避让
#ifdef __JS_PUMP
    if(abs(postion) < ths->mot_cfg->lost_endure_continue)
    {
        return 2;
    }
#endif
	//TODO::这里有一个问题，没有这个语句，有些电机竟然无法收到位置到达中断
    mTMC5130DeviceGetState(ths);
	int currentPosition = mTMC5130DeviceGetPostion(ths,0);
	if(currentPosition == postion)
	{
		printf("Running same position :: id = %d cPosition = %d, position = %d\r\n", ths->id, currentPosition, postion);
		return 1;
	}
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	run_table.A1=acc/4;
	run_table.V1= speed/2;
	run_table.AMAX=acc/4;
	run_table.VMAX=abs(speed) *1.32;
	run_table.D1=acc;
	run_table.DMAX=acc/4;
	run_table.postion=postion;
	run_table.run_currnet=ths->mot_cfg->currentLevel;
	run_table.hold_current=ths->mot_cfg->holdLevel;
	run_table.MicoStep=mico;
	run_table.mode=TMC5130_MODE_POSITION;
	printf("MoveReal：id = %d speed=%d, acc=%d, position=%d, current = %d\r\n",ths->id, speed, acc, postion,currentPosition);
	return mTMC5130DriverMove(&run_table);
}

/**
*若位置相同,将会造成电机一直阻塞, 调用者需要检查返回值做处理
**/
char mTMC5130DevicMoveto(MOTOR* ths,int speed,int acc,int postion,char dir,int mico,char current)  //绝对运动 没有方向  目标位置在当前位置前向就正转  反之则反转
{
	printf("MoveTo：id = %d speed=%d, acc=%d, position=%d, dir=%d,drift = %d\r\n",ths->id, speed, acc, postion, dir,ths->Zero_drift);
	if(ths->mot_cfg->dir_vol ) postion= 0-postion;
	
	postion += ths->Zero_drift;
	
	return mTMC5130DevicMoveRealPosition(ths,speed,acc,postion,mico,current);
}

char mTMC5130DevicMoveby(MOTOR* ths,int speed,int acc,int postion,char dir,int mico,char current) //相对运动
{
	printf("MoveBy：id = %d  speed=%d, acc=%d, position=%d, dir=%d, Zero_drift=%d\r\n", ths->id, speed, acc, postion, dir, ths->Zero_drift);	
	int currentPosition = mTMC5130DeviceGetPostion(ths,0);
	if(ths->mot_cfg->dir_vol ) postion= 0-postion;
	postion = (dir==0)? currentPosition+postion :currentPosition-postion;

	return mTMC5130DevicMoveRealPosition(ths,speed,acc,postion,mico,current);
}


int mTMC5130DeviceReadWriteRegister(MOTOR* ths,char addr,char cmd,int data)//直接对寄存器进行读取
{	  
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	return  mTMC5130DriverReadWriteRegister(&run_table,addr,cmd,data);
}


// 电机紧急停止, wzc 2021/02/03
int mTMC5130Motor_EmergencyStop(MOTOR* ths, bool flag)
{
	int regval = 0;
	regval = mTMC5130DeviceReadWriteRegister(ths, TMC5130_GCONF, 0, regval);
	
	if(flag == true)
	{
		regval |= 0x1 << 15; // set stop_enable bit, 0=正常操作，1=紧急停止
		ths->write_io(PIN_ID_EME, 1);
	}
	else
	{
		regval &= ~(0x1 << 15);
		ths->write_io(PIN_ID_EME, 0);
	}
	
	return mTMC5130DeviceReadWriteRegister(ths, TMC5130_GCONF, 1, regval);
}

/**
flg : 0驱动器真实位置
			1程序要求位置
			2程序要求位置与补偿量
*/
int mTMC5130DeviceGetPostion(MOTOR* ths,char flg)//获取电机的当前位置
{
	return mTMC5130DeviceGetPostionIf(ths, flg, true);
}

/**
flg : 0驱动器真实位置
			1程序要求位置
			2程序要求位置与补偿量
*/
int mTMC5130DeviceGetPostionIf(MOTOR* ths,char flg,bool mutex)//获取电机的当前位置
{
	int result=0;
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	if(flg == 0) 					 //获取驱动器当前位置  真实位置
	{	
		result=mTMC5130DriverGetPositionIf(&run_table, mutex);
	}
	else
	{		
		result=ths->mot_cfg->dir_vol ? 0-mTMC5130DriverGetPositionIf(&run_table,mutex):mTMC5130DriverGetPositionIf(&run_table,mutex);
	}
	
	return result;
}

int mTMC5130ConvertToAbsPosition(MOTOR* ths, int position)
{
    return ths->mot_cfg->dir_vol ? 0-position:position;
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

char mTMC5130Device_GetPara(MOTOR* ths)//获取驱动器所有参数
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	return  mTMC5130DriverGetPara(&run_table);
}

void mTMC5130Device_SetDefault(MOTOR* ths) //配置
{
	TABLE run_table;
	run_table.mTMCDriver_WriteIo=ths->write_io;
	run_table.mTMCDriver_ReadIo=ths->read_io;
	run_table.mTMCDriverReg=ths->drvie_reg;
	
	run_table.MicoStep=ths->mot_cfg->subdivision;
	
	run_table.hold_current=ths->mot_cfg->holdLevel   <= 0 ? 8: ths->mot_cfg->holdLevel;
	run_table.run_currnet=ths->mot_cfg->currentLevel <= 0 ? 15:ths->mot_cfg->currentLevel;
	
	run_table.hold_current=ths->mot_cfg->holdLevel > 31  ? 15:ths->mot_cfg->holdLevel;
	run_table.run_currnet=ths->mot_cfg->currentLevel >33 ? 25:ths->mot_cfg->currentLevel;	
	
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


int mTMC5130Motor_SimpleMove(MOTOR* ths, bool enable_zero_makeup)  //移动 相对移动  绝对移动
{
	int err=0, result=0;
	if(ths->isRelative == 1)
	{	
		if(ths->postion != 0)
		{
			result = mTMC5130DevicMoveby(ths,ths->speed,ths->acc,ths->postion,0,ths->micostep,ths->i_run);
		}
		else
		{
            ths->status=MOTOR_STATUS_MOVE_FINISH;	
			return MOTOR_NO_ERROR;
		}
	}
	else
	{
        result = mTMC5130DevicMoveto(ths,ths->speed,ths->acc,ths->postion,0,ths->micostep,ths->i_run);
		printf("目前驱动器真实位置=%d\r\n", mTMC5130DeviceGetPostion(ths,0));
        //电机目标位置和现在位置重复
        if(result == 1)
        {
            ths->status=MOTOR_STATUS_MOVE_FINISH;	
            return MOTOR_NO_ERROR;
        }
	}
	if(enable_zero_makeup)
	{
		//开启光耦触发中断
		ths->read_io(PIN_ID_SET_TOUCH_ISR,ths->zeor_isr_id,ths->mot_cfg->zero_trig_vol);
		err=mTMC5130DevicWait_Sem(ths,WAIT_REACH_SEM,0);
		//关中断
		ths->read_io(PIN_ID_SET_SENSOR,ths->zeor_isr_id,0);
		
		//电机运行完成正常停止
		if(err == MOTOR_STATUS_STOP)
		{
			
			if(abs(ths->Zero_drift) > ths->mot_cfg->lost_endure_stop)
            {
                // 失步步数大于失步阈值，可判定为回零失败
                printf("ERROR：%d号电机失步步数大于失步阈值，回零失败，失步=%d，阈值=%d\r\n",ths->id, ths->Zero_drift, ths->mot_cfg->lost_endure_continue);
                ths->status = MOTOR_STATUS_ZERO_FAIL;
                return MOTOR_STATUS_ZERO_FAIL;
            }

			bool zero_trigger = ths->read_io(PIN_ID_SET_SENSOR,ths->zeor_isr_id,1) == ths->mot_cfg->zero_trig_vol;
			
			int currentPosition = mTMC5130DeviceGetPostion(ths,1);
			printf("currentPosition = %d,lost_endure_continue=%d\r\n\r\n",currentPosition,ths->mot_cfg->lost_endure_continue);
			
			bool postionNotInZeroButZeroTrigger = currentPosition > 0 &&
				abs(currentPosition) >= ths->mot_cfg->zero_cordinate + ths->mot_cfg->lost_endure_continue &&
				zero_trigger;
			
			if(postionNotInZeroButZeroTrigger)
            {
                // 失步步数大于失步阈值，可判定为回零失败
                printf("ERROR：%d号电机不在零位但光耦触发\r\n",ths->id);
                ths->status = MOTOR_STATUS_ZERO_FAIL;
                return MOTOR_STATUS_ZERO_FAIL;
            }

            
            bool positionInZeroButZeroNoTrigger = 
			(currentPosition <= 0 || (abs(currentPosition) + ths->mot_cfg->lost_endure_continue) <= ths->mot_cfg->zero_cordinate) &&
				!zero_trigger;
           
		   if(positionInZeroButZeroNoTrigger)
            {
                // 失步步数大于失步阈值，可判定为回零失败
                printf("ERROR：%d号电机在零位但光耦未触发\r\n",ths->id);
                ths->status = MOTOR_STATUS_ZERO_FAIL;
                return MOTOR_STATUS_ZERO_FAIL;
            }
        }
	}
	else
	{
		err=mTMC5130DevicWait_Sem(ths,WAIT_REACH_SEM,0);
	}
	
	ths->status=MOTOR_STATUS_MOVE_FINISH;	
	
	return err == MOTOR_STATUS_STOP ? MOTOR_NO_ERROR:MOTOR_ERROR_UNKNOW;
}

int mTMC5130Motor_BackZero(MOTOR* ths)  //移动 相对移动  绝对移动
{

    u32 err_code = 0;
    
    if(ths->mot_cfg->back_zero_speed_mode)
    {
        err_code = mTMC5130Motor_Reset(ths);
    }else
    {
        //先把 零位光耦引脚初始化（没有开启外部中断） 并且读取引脚状态 和零位触发电压做比较
        bool zero_trigger = ths->read_io(PIN_ID_SET_SENSOR,ths->zeor_isr_id,1) == ths->mot_cfg->zero_trig_vol;
		//如果初始引脚状态已被触发 说明已经在零位或者在其之前
        if(zero_trigger)
        {
			//把目标距离 设为 电机配置零位距离 的2倍 先往正方向走一定的距离
            ths->postion = ths->mot_cfg->zero_cordinate<<1;
			//设为绝对移动
            ths->isRelative = 0;
			//开始做电机移动 
			printf("第一次移动\r\n");
            err_code = mTMC5130Motor_SimpleMove(ths, true);
			printf("第一次移动错误码=%d\r\n",err_code);
            //移动完成后 查看零位光耦是否触发
            zero_trigger = ths->read_io(PIN_ID_SET_SENSOR,ths->zeor_isr_id,1) == ths->mot_cfg->zero_trig_vol;
            if(zero_trigger)
            {
                err_code = MOTOR_ERROR_LOST;
            }
        }
        
        if(err_code == 0)
        {
            ths->postion = 0;
            ths->isRelative = 0;
			printf("第二次移动\r\n");
            err_code = mTMC5130Motor_SimpleMove(ths, true);
			printf("第二次移动错误码=%d\r\n",err_code);
        }
    }
    
    if(err_code == 0)
    {
        bool zero_trigger = ths->read_io(PIN_ID_SET_SENSOR,ths->zeor_isr_id,1) == ths->mot_cfg->zero_trig_vol;
        
		//如果还没有触发 零位光耦
        if(!zero_trigger)
        {
            err_code = MOTOR_ERROR_LOST;
        }

    }
    if(err_code != 0)
		
    {
		 printf("错误码%d\r\n",err_code);
         printf("INFO：%d号电机失步步数",ths->id);
    }
    
    ths -> status = (err_code == 0) ? MOTOR_STATUS_ZERO_FINISH : MOTOR_STATUS_ZERO_FAIL;
	ths->isBackZero = 0;
    return err_code;
}

void mTMC5130Motor_ZeroTouch(MOTOR* ths)
{
    if(ths->isReset)
    {
        ths->ZeorTouch = mTMC5130DeviceGetPostion(ths,0);
		printf("zero touch mot = %d, zerotouch = %d\r\n",ths->id, ths->ZeorTouch);
    }
	else if(ths->mot_cfg->enable_zero_makeup || ths->isBackZero)
    {
        ths->ZeorTouch = mTMC5130DeviceGetPostion(ths,1);
        ths->Zero_drift = ths->mot_cfg->zero_cordinate - ths->ZeorTouch;
		printf("zero touch mot = %d, zerotouch = %d, zero_cordinate = %d, zero_drift = %d\r\n",ths->id, ths->ZeorTouch, ths->mot_cfg->zero_cordinate, ths->Zero_drift);
    }
}

int mTMC5130Motor_Move(MOTOR* ths)  //移动 相对移动  绝对移动
{
	if(ths->isBackZero)
	{
		return mTMC5130Motor_BackZero(ths);
	}
	else
	{
		return mTMC5130Motor_SimpleMove(ths, ths->mot_cfg->enable_zero_makeup);
	}
}

int mTMC5130Motor_Reset(MOTOR* ths)//复位 回零
{
	int result=0;
	//复位或回零均清除 零位漂移
	ths->Zero_drift = 0;
    ths->isReset=1;

#ifdef QUICK_RESET_BACKZERO	
	ths->reset_acc=(unsigned int)ths->speed * ths->speed / ths->mot_cfg->zero_cordinate * 0.0038845;//0.0038845
	ths->status=MOTOR_STATUS_RESET_START_QUICK;//快速复位
#else
	ths->status=MOTOR_STATUS_RESET_START;
#endif
	
	ths->dir= ths->mot_cfg->dir_vol ? ths->mot_cfg->dir_vol:DRI_REVERAL(ths->mot_cfg->dir_vol);
	ths->read_io(PIN_ID_SET_SENSOR,ths->zeor_isr_id,0);

	result=ths->mReset(ths);
	
	int retst = MOTOR_NO_ERROR;
	if(result == MOTOR_STATUS_ERROR_TIME_OUT) // 光耦检测超时
	{
		mTMC5130DevicStop(ths,STOP_AMAX);
		ths->status = MOTOR_STATUS_ERROR_TIME_OUT;
        ths->isReset=0;
		retst = MOTOR_STATUS_ERROR_TIME_OUT;  //光耦检测超时后  复位也失败
		printf("reset time out.......\r\n");
        return retst;
	}

    ths->postion = 0;
    ths->isRelative = 0;
    retst = mTMC5130Motor_SimpleMove(ths, ths->mot_cfg->enable_zero_makeup);
    
    ths->status = (retst == MOTOR_NO_ERROR) ? MOTOR_STATUS_RESET_FINISH : MOTOR_STATUS_RESET_FAIL;

    if(retst != 0)
    {
        printf("RUNNING：%d号电机复位失败，BrakeStep=%d，zero_cordinate=%d\r\n", ths->id, ths->BrakeStep, ths->mot_cfg->zero_cordinate);
    }
	
    ths->isReset=0;
	return retst;
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
		margs[i].motor->micostep=margs[i].motor->micostep !=0 ? margs[i].motor->micostep:margs[i].motor->mot_cfg->subdivision;
		margs[i].motor->i_run=margs[i].motor->i_run != 0 ? margs[i].motor->i_run:margs[i].motor->mot_cfg->currentLevel;			
#endif	
		if(margs[i].m5_isRelative == 1)
		{		
			if(margs[i].m4_distance != 0)
			{							
				if(mTMC5130DevicMoveby(margs[i].motor,
										margs[i].m3_speed,
										margs[i].m2_acce,
										margs[i].m4_distance,
										0,
										margs[i].m8_subdivision,
										margs[i].m7_currentLevel
									   )!=1)
                {
                    margs[i].motor->wait_type=WAIT_REACH_FLG;
                    margs[0].motor->evnt_group=margs[0].motor->evnt_group | 1<< margs[i].m1_idx;	
                }
			}	
		}
		else
		{
//			if(margs[i].m4_distance == 0 && (margs[i].motor->status == MOTOR_STATUS_ZERO_FINISH || margs[i].motor->status == MOTOR_STATUS_RESET_FINISH))
//			{
//				break;
//			}

			if(mTMC5130DevicMoveto(margs[i].motor,margs[i].m3_speed,margs[i].m2_acce,margs[i].m4_distance,0,margs[i].m8_subdivision,margs[i].m7_currentLevel) != 1)
			{
				margs[i].motor->wait_type=WAIT_REACH_FLG;
				margs[0].motor->evnt_group=margs[0].motor->evnt_group | 1<< margs[i].m1_idx;
			}
					
		}		
	}
	if(margs[0].motor->evnt_group >0)
	{
		err=mTMC5130DevicWait_Sem(margs[0].motor,WAIT_REACH_FLG,0);
		for(char i=0;i<cnt;i++)
		{
			margs[i].motor->status=MOTOR_STATUS_MOVE_FINISH;			
		}
	}		
	return err == MOTOR_STATUS_STOP ? MOTOR_NO_ERROR:MOTOR_ERROR_UNKNOW;
}

int mTMC5130Motor_SeqMove(int cnt, motor_seq_move_args_t *seq)
{
	for(int i = 0;i < cnt;i++)
	{
		for(int j = 0;j<seq[i].args_len;j++)
		{
			motor_multi_move_args_t* arg = &seq[i].args[j];
			arg->m7_currentLevel = arg->motor->mot_cfg->currentLevel;
			arg->m8_subdivision = arg->motor->mot_cfg->subdivision;
		}
		u32 err = mTMC5130Motor_MultiMove(seq[i].args_len, seq[i].args, false);
		if(err != MOTOR_NO_ERROR)
		{
			return err;
		}
		if(seq[i].m2_delayms > 0)
		{
			OSTimeDlyHMSM(0,0,0,seq[i].m2_delayms);
		}
	}
	return MOTOR_NO_ERROR;
}

int mTMC5130Motor_MoveStopEvent(MOTOR* ths,int *pos)//监听IO事件
{
	return mTMC5130Motor_MoveEvent(ths, false, pos);
}

int mTMC5130Motor_MoveGPIOEvent(MOTOR* ths,int *pos)//监听IO事件
{
	return mTMC5130Motor_MoveEvent(ths, true, pos);
}

static int mTMC5130Motor_MoveEvent(MOTOR* ths,bool is_io_event, int *pos)//监听IO事件
{
	int result=0;
	if(ths->isRelative == 1)
	{				
		if(ths->postion != 0)
		{
			mTMC5130DevicMoveby(ths,ths->speed,ths->acc,ths->postion,0,ths->micostep,ths->i_run);
		}
		else
		{
			ths->status = MOTOR_STATUS_LISTEN_IO_FAIL;
			return MOTOR_ERROR_LISTEN_EVENT_NO_TRIGGER;
		}
	}
	else
	{
		if(mTMC5130DevicMoveto(ths,ths->speed,ths->acc,ths->postion,0,ths->micostep,ths->i_run) == 1)
		{
			ths->status = MOTOR_STATUS_LISTEN_IO_FAIL;
			return MOTOR_ERROR_LISTEN_EVENT_NO_TRIGGER;
		}
	}
	
	result= is_io_event ? ths->mListenIo(ths) : ths->mListenEvent(ths);
	*pos= mTMC5130DeviceGetPostion(ths,1);
	mTMC5130DevicStop(ths,STOP_AMAX);
	while(GET_STATE_BIT(mTMC5130DeviceGetState(ths),STANDSTILL) != 1) OSTimeDlyHMSM(0,0,0,300);
	
	if(result == MOTOR_STATUS_LISTEN_IO_FINISH)
	{	
		ths->status=MOTOR_STATUS_LISTEN_IO_FINISH;
		return MOTOR_NO_ERROR;
	}
	else
	{
		ths->status=MOTOR_STATUS_LISTEN_IO_FAIL;
		return MOTOR_ERROR_LISTEN_EVENT_NO_TRIGGER;
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
	ths->micostep=ths->micostep != 0 ? ths->micostep:ths->mot_cfg->subdivision;
	ths->i_run=ths->i_run != 0 ? ths->i_run:ths->mot_cfg->currentLevel;	
#endif

	mTMC5130DevicRotate(ths,ths->speed,ths->acc,ths->dir,ths->micostep,ths->i_run);
	err=mTMC5130DevicWait_Sem(ths,WAIT_VELOCITY_SEM,2000);

	ths->status=MOTOR_STATUS_RORATE_FINISH;
	return err == MOTOR_STATUS_STOP ? MOTOR_NO_ERROR:MOTOR_ERROR_UNKNOW; // MOTOR_STATUS_ERROR_TIME_OUT ;
}

int mTMC5130Motor_Stop(MOTOR* ths)//停止
{	
	int err=0;
	mTMC5130DevicStop(ths,ths->speed);
	err=mTMC5130DevicWait_Sem(ths,WAIT_STOP_SEM,1000);
	ths->status=MOTOR_STATUS_STOP_FINISH;
	return err == MOTOR_STATUS_STOP ? MOTOR_NO_ERROR:MOTOR_ERROR_STOP_TIME_OUT ;
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


void mTMC5130Motor_Init(int idx, \
						MOTOR *ths,\
						motor_cfg_struct * cfg,\
						DRIVER_REG *drive_reg,\
						void (*write_io)(pin_id_t ,int ),\
						int  (*read_io) (pin_id_t ,int,char),\
                        u8 (*read_event)(u8 id),\
						DEFINESTATUS (*motor_state_machine)(MOTOR *),\
						int (*motor_backzeor)(MOTOR *),\
						int (*motor_reset)(MOTOR *),\
						int (*motor_listen_io)(MOTOR *),\
                        int (*motor_listen_event)(MOTOR *),\
						int (*Move)(MOTOR* ths),\
						int (*Rotate)(MOTOR* ths),\
						int (*GetStatus)(MOTOR* ths),\
						int (*Stop)(MOTOR* ths),\
						int (*MultiMove)(int,motor_multi_move_args_t *,bool ),\
						int (*SeqMove)(int, motor_seq_move_args_t*), \
						int (*MoveGPIOEvent)(MOTOR* ,int*),\
                        int (*MoveStopEvent)(MOTOR* ,int*),\
						int (*MotorReset)(MOTOR* )\
						)
{		
	printf("motor init start %d\r\n", idx);
	uint8_t index = 0;
	uint32_t start_tick = 0,current_tick = 0;
	
	ths->id = idx;
	ths->read_io = read_io;
	ths->write_io= write_io;
    ths->read_event = read_event;
	ths->mot_cfg = cfg;  
	ths->zeor_isr_id= read_io(PIN_INIT_TOUCH_ISR,0,0);  //t_isr_id;计算光耦触发中断号
	ths->reach_isr_id=read_io(PIN_INIT_REACH_ISR,0,0); //r_isr_id;计算位置到达中断号
	ths->error_isr_id=read_io(PIN_INIT_ERROR_ISR,0,0);//计算错误检测中断号
	ths->drvie_reg=drive_reg;//驱动器配置参数  各个电机有差异  所需要的参数也各不相同
	
	ths->mTMC5130Motor_State_Machine=motor_state_machine;
	
    ths->isReset = 0;
	ths->Zero_drift = 0;
	ths->mBackZero=motor_backzeor;
	ths->mListenIo=motor_listen_io;	
    ths->mListenEvent = motor_listen_event;
	ths->mReset=motor_reset;
	
	ths->mTMC5130MotorReset=MotorReset;
	ths->mTMC5130MotorMove=Move;
	ths->mTMC5130MotorRotate=Rotate;
	ths->mTMC5130MotorGetStatus=GetStatus;
	ths->mTMC5130MotorStop=Stop;
	ths->mTMC5130Motor_MultiMove=MultiMove;
	ths->mTMC5130Motor_SeqMove = SeqMove;
	ths->mTMC5130MotorMoveGPIOEvent=MoveGPIOEvent;
    ths->mTMC5130MotorMoveStopEvent=MoveStopEvent;
	
	mTMC5130Device_Init(ths);
	motor_max++;
	start_tick = OSTime;//OSTimeGet();
	do
	{
		mTMC5130DevicStop(ths,4000);
		OSTimeDly(100);
		mTMC5130DeviceResetPostion(ths);
		index=mTMC5130DeviceGetPostion(ths,0);
        mTMC5130Device_SetDefault(ths);
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


