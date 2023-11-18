#include "ucos_ii.h"
#include "mTMC4361Driver.h"
#include <stdlib.h>
#include "os_implement.h"

extern SPI_HandleTypeDef hspi1;
static bus_os_sem  SPI_SEM;
//#define USE_HAL 1

const s32 tmc4361_defaultRegisterResetState[REGISTER_COUNT] =
{
//	0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   A,   B,   C,   D,   E,   F
	R00, 0,   0,   0,   0,   0,   R06, R07, 0,   0,   R0A, R0B, 0,   0,   0,   0,   // 00 - 0F
	R10, 0,   R12, 0,   0,   0,   0,   0,   0,   0,   0,   0,   R1C, 0,   0,   R1F, // 10 - 1F
	R20, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 20 - 2F
	0,   R31, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 30 - 3F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 40 - 4F
	0,   0,   0,   R53, 0,   0,   R56, R57, R58, 0,   0,   0,   0,   0,   0,   0,   // 50 - 5F
	0,   0,   R62, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 60 - 6F
	R70, R71, R72, R73, R74, R75, R76, R77, R78, 0,   0,   0,   0,   0,   R7E, 0    // 70 - 7F
};


static void Delay_Nus(unsigned int nus)
{		
	unsigned int ticks;
	unsigned int told,tnow,tcnt=0;
	unsigned int reload=SysTick->LOAD;					 
	ticks=nus*72; 					
	OSSchedLock();
	told=SysTick->VAL;        			
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)
				tcnt+=told-tnow;	
			else 
				tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			
		}  
	}
	OSSchedUnlock();										    
} 

static int discardVelocityDecimals(int value)
{
	if(abs(value) > 8000000)
	{
		value = (value < 0) ? -8000000 : 8000000;
	}
	return value << 8;//因为是256细分，所以要对速度乘于256
}


static unsigned char readWrite(uint8_t data)
{
	while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE) == RESET) {};
	*((__IO uint8_t*)&hspi1.Instance->DR) = data;
	while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE) == RESET) {};
	return hspi1.Instance->DR;
}

int SPI_WriteRead(struct sTABLE *ths,char addr,char cmd,unsigned int data,unsigned char *pRxData) 
{
	char err=0;
	if(ths->mTMCDriver_WriteIo ==NULL) //检测写指针是否为空
	{
		err=1;
		return err;
	}
	err=bus_os_pend_sem(&SPI_SEM,100);//使用信号量保护SPI接口 在读写过程中不允许打断
	for(char i=0;i<5;i++) pRxData[i]=0;
#ifdef USE_HAL	
	uint8_t TxData[5]={0,0,0,0,0};
	TxData[0]=(cmd<<7 | addr) & 0xff;
	TxData[1]=data>>24 &0xff;
	TxData[2]=data>>16 &0xff;
	TxData[3]=data>>8 &0xff;
	TxData[4]=data&0xff;					
	ths->mTMCDriver_WriteIo(PIN_ID_CS,0);
	Delay_Nus(20);
	HAL_SPI_TransmitReceive(&hspi1, TxData, pRxData, 5,0xff);
	Delay_Nus(20);
	ths->mTMCDriver_WriteIo(PIN_ID_CS,1);		
#else	
	__HAL_SPI_ENABLE(&hspi1);
	ths->mTMCDriver_WriteIo(PIN_ID_CS,0);
	Delay_Nus(20);	
	pRxData[0]=readWrite((cmd<<7 | addr) & 0xff);
	pRxData[1]= readWrite((data >> 24) & 0xff);
	pRxData[2]= readWrite((data >> 16) & 0xff);
	pRxData[3]=readWrite((data >> 8) & 0xff);
	pRxData[4]= readWrite((data) & 0xff);
	Delay_Nus(20);
	ths->mTMCDriver_WriteIo(PIN_ID_CS,1);	
#endif

	bus_os_post_sem(&SPI_SEM);

//	if(pRxData[0]== 0 || pRxData[0]==0xff) //正常情况  从SPI返回的数据位 不能全是0或全部是1 
//	{
//		printf("spi error.......\r\n");
//		err=1; 
//	}
	return pRxData[1]<<24 | pRxData[2]<<16 | pRxData[3]<<8 | pRxData[4];
}


char mTMC4361DriverWriteDatoTo5130(struct sTABLE *ths,unsigned char addr,int value)
{
//	short timeout=0;
	unsigned char RxData[5]={0,0,0,0,0};
	SPI_WriteRead(ths,TMC4361_INTR_CONF, SPI_WRITE_TYPE, 0x2000000,RxData);
	SPI_WriteRead(ths,TMC4361_EVENTS, SPI_READ_TYPE, 0,RxData);
    SPI_WriteRead(ths,TMC4361_COVER_HIGH_WR, SPI_WRITE_TYPE, addr | 0x80,RxData);
    SPI_WriteRead(ths,TMC4361_COVER_LOW_WR, SPI_WRITE_TYPE, value,RxData);
//	do
//	{
//		int result=SPI_WriteRead(ths,TMC4361_EVENTS, SPI_READ_TYPE,0,RxData);
//		timeout++;
//		Delay_Nus(20);
//		if(timeout>500) 
//		{
//			return 1;
//		}
//		if(result & 0x2000000)
//		{
//			break;
//		}
//	}while(1);//!(result & 0x2000000)
	SPI_WriteRead(ths,TMC4361_EVENTS, SPI_READ_TYPE, 0,RxData);
    Delay_Nus(20);
    return 0;	
}

int mTMC4361DriverReadDatoFrom5130(struct sTABLE *ths,unsigned char addr)
{
//	short timeout=0;	
	unsigned char RxData[5]={0,0,0,0,0};
//	SPI_WriteRead(ths,TMC4361_INTR_CONF, SPI_WRITE_TYPE, 0x2000000,RxData);
//	SPI_WriteRead(ths,TMC4361_EVENTS, SPI_READ_TYPE, 0,RxData);

    SPI_WriteRead(ths,TMC4361_COVER_HIGH_WR, SPI_WRITE_TYPE, addr,RxData);
    SPI_WriteRead(ths,TMC4361_COVER_LOW_WR, SPI_WRITE_TYPE, 0,RxData);
    Delay_Nus(20);
	SPI_WriteRead(ths,TMC4361_COVER_DRV_LOW_RD, SPI_READ_TYPE, 0,RxData);
	return SPI_WriteRead(ths,TMC4361_COVER_DRV_LOW_RD, SPI_READ_TYPE, 0,RxData);
}

void mTMC4361DriverSetSpeed(struct sTABLE *ths) //速度设置
{	
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据
	SPI_WriteRead(ths,TMC4361_BOW1,SPI_WRITE_TYPE,ths->bow1,RxData);  //起始加速度
	SPI_WriteRead(ths,TMC4361_BOW2,SPI_WRITE_TYPE,ths->bow2,RxData);//第一段速度 
	SPI_WriteRead(ths,TMC4361_BOW3,SPI_WRITE_TYPE,ths->bow3,RxData);//第二段加速度
	SPI_WriteRead(ths,TMC4361_BOW4,SPI_WRITE_TYPE,ths->bow4,RxData);//最高速度

	SPI_WriteRead(ths,TMC4361_VMAX,SPI_WRITE_TYPE,discardVelocityDecimals(ths->VMAX),RxData);//起始减减加速度

	SPI_WriteRead(ths,TMC4361_AMAX,SPI_WRITE_TYPE,ths->AMAX,RxData);//起始减减加速度
	SPI_WriteRead(ths,TMC4361_DMAX,SPI_WRITE_TYPE,ths->DMAX,RxData);//起始减减加速度

	SPI_WriteRead(ths,TMC4361_VBREAK,SPI_WRITE_TYPE,ths->vbreak,RxData);				
}


void mTMC4361DriverResetPostion(struct sTABLE *ths)  //主要用来清零当前和目标寄存器
{	
	static char timeout=0;
	unsigned char RxData2[5]={0,0,0,0,0};//用来存放从SPI读取到的数据
	unsigned char RxData1[5]={0,0,0,0,0};//用来存放从SPI读取到的数据
	do
	{
		SPI_WriteRead(ths,TMC4361_X_TARGET,SPI_WRITE_TYPE,0,RxData1);//清零当目标寄存器
		SPI_WriteRead(ths,TMC4361_XACTUAL,SPI_WRITE_TYPE,0,RxData2);//清零当前位置寄存器

		SPI_WriteRead(ths,TMC4361_X_TARGET,SPI_READ_TYPE,0,RxData1);
		SPI_WriteRead(ths,TMC4361_XACTUAL,SPI_READ_TYPE,0,RxData2);

		if(timeout >10) {timeout=0;break;}else timeout++;

	}while((RxData1[1]+RxData1[2]+RxData1[3]+RxData1[4]+RxData2[1]+RxData2[2]+RxData2[3]+RxData2[4])  !=0 );		
}


int mTMC4361DriverGetPosition(struct sTABLE *ths)//获取位置信息
{
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据	

	SPI_WriteRead(ths,TMC4361_ENC_POS,SPI_READ_TYPE,0,RxData);
	printf("TMC4361_ENC_POS=%d\r\n",SPI_WriteRead(ths,TMC4361_ENC_POS,SPI_READ_TYPE,0,RxData));

	SPI_WriteRead(ths,TMC4361_XACTUAL,SPI_READ_TYPE,0,RxData);
	SPI_WriteRead(ths,TMC4361_XACTUAL,SPI_READ_TYPE,0,RxData);

//	SPI_WriteRead(ths,TMC4361_ENC_POS,SPI_READ_TYPE,0,RxData);
//	SPI_WriteRead(ths,TMC4361_ENC_POS,SPI_READ_TYPE,0,RxData);	
	return (RxData[1]<<24 | RxData[2]<<16 | RxData[3]<<8 | RxData[4]);
}

int mTMC4361DriverGetENCPosition(struct sTABLE *ths)//
{
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据	
	SPI_WriteRead(ths,TMC4361_ENC_POS,SPI_READ_TYPE,0,RxData);
	SPI_WriteRead(ths,TMC4361_ENC_POS,SPI_READ_TYPE,0,RxData);	
	return (RxData[1]<<24 | RxData[2]<<16 | RxData[3]<<8 | RxData[4]);
}

void mTMC4361DriverSetPostion(struct sTABLE *ths,int pos)
{
//	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据

//	do
//	{
//		SPI_WriteRead(ths,TMC4361_XACTUAL, SPI_WRITE_TYPE, pos,RxData);//位置信息写到寄存器里
//		Delay_Nus(50);
//		SPI_WriteRead(ths,TMC4361_XACTUAL, SPI_READ_TYPE,  0,RxData);//位置信息写到寄存器里
//	}while((RxData[1]<<24 | RxData[2]<<16 | RxData[3]<<8 | RxData[4]) != pos);

//	do
//	{
//		SPI_WriteRead(ths,TMC4361_X_TARGET, SPI_WRITE_TYPE, pos,RxData);//位置信息写到寄存器里
//		Delay_Nus(50);
//		SPI_WriteRead(ths,TMC4361_X_TARGET, SPI_READ_TYPE,  0,RxData);//位置信息写到寄存器里
//	}while((RxData[1]<<24 | RxData[2]<<16 | RxData[3]<<8 | RxData[4]) != pos);
	
}

int mTMC4361DriverGetState(struct sTABLE *ths)
{
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据	
//	SPI_WriteRead(ths,TMC4361_STATUS,SPI_READ_TYPE,0,RxData);
//	SPI_WriteRead(ths,TMC4361_STATUS,SPI_READ_TYPE,0,RxData);	

//	SPI_WriteRead(ths,TMC4361_XACTUAL,SPI_READ_TYPE,0,RxData);
//	printf("TMC4361_POS_COMP=%d\r\n",SPI_WriteRead(ths,TMC4361_XACTUAL,SPI_READ_TYPE,0,RxData));

//	SPI_WriteRead(ths,0x65,SPI_READ_TYPE,0,RxData);
//	printf("65=%d\r\n",SPI_WriteRead(ths,0x65,SPI_READ_TYPE,0,RxData));

	SPI_WriteRead(ths,TMC4361_ENC_POS,SPI_READ_TYPE,0,RxData);
	SPI_WriteRead(ths,TMC4361_ENC_POS,SPI_READ_TYPE,0,RxData);
	return (RxData[1]<<24 | RxData[2]<<16 | RxData[3]<<8 | RxData[4]);
}

int mTMC4361DriverGetSpeed(struct sTABLE *ths)
{
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据	
	SPI_WriteRead(ths,TMC4361_V_ENC_RD,SPI_READ_TYPE,0,RxData);
	SPI_WriteRead(ths,TMC4361_V_ENC_RD,SPI_READ_TYPE,0,RxData);
	return (RxData[1]<<24 | RxData[2]<<16 | RxData[3]<<8 | RxData[4]);
}

char mTMC4361DriverMove(struct sTABLE *ths)  //运动底动函数  所有运动最终都会调用这个函数
{ 
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据	
//	if(ths->VMAX >0)
//	{
//		mTMC4361DriverSetMicoStep(ths);      //设置细分
//		mTMC4361DriverSetCurrent(ths);         //设置运行时电流  停止电流默认为2
//	}
	SPI_WriteRead(ths,TMC4361_RAMPMODE, SPI_WRITE_TYPE, ths->mode,RxData);//控制模式	

	if(ths->mode >= TMC4361_RAMP_POSITION_HOLD) //只有位置模式才需要设置位置寄存器
	{
		SPI_WriteRead(ths,TMC4361_POS_COMP, SPI_WRITE_TYPE, ths->postion,RxData);//位置比较寄存器
		SPI_WriteRead(ths,TMC4361_X_TARGET,SPI_WRITE_TYPE, ths->postion,RxData);
	}
	mTMC4361DriverSetSpeed(ths);          //设置速度	
	return 0;
}


void mTMC4361DriverSetDefaultRegister(struct sTABLE *ths,char setType) //配置5130寄存器参数
{
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据
	if(setType == 1)
	{
		for(unsigned char i=0;i<REGISTER_COUNT;i++) 
		{
			SPI_WriteRead(ths,i,SPI_WRITE_TYPE,tmc4361_defaultRegisterResetState[i],RxData);
		} 
	}	
}

char mTMC4361Driver_Init(struct sTABLE *ths)//初始化
{
	char err=0;
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据
	if(SPI_SEM.life_flag == 0)  //如果建立了信号量  就不再重新创建了
	{
		bus_os_create_sem(&SPI_SEM,1);
	}	
	SPI_WriteRead(ths,TMC4361_RESET_REG, SPI_WRITE_TYPE, 0x52535400,RxData);//内部复位	
//	OSTimeDlyHMSM(0,0,0,100);
	mTMC4361DriverSetDefaultRegister(ths,1);
//	OSTimeDlyHMSM(0,0,0,100);
    SPI_WriteRead(ths,TMC4361_SPIOUT_CONF, SPI_WRITE_TYPE,  0x8445022c,RxData);//配置SPI输出接口	
/*******************以下是5130驱动器的配置**********************************************/		
	err=mTMC4361DriverWriteDatoTo5130(ths,0x0,0x0);
	err=mTMC4361DriverWriteDatoTo5130(ths,0x10,0x51408);//电机线圈电流直接和极性通过串行接口编程
	err=mTMC4361DriverWriteDatoTo5130(ths,0x13,0x1388);
	err=mTMC4361DriverWriteDatoTo5130(ths,0x11,0xA);//TZEROWAIT
	err=mTMC4361DriverWriteDatoTo5130(ths,0x14,0xc8);//TCOOLTHRS=200	
	err=mTMC4361DriverWriteDatoTo5130(ths,0x15,0x32);//THIGH=50
	err=mTMC4361DriverWriteDatoTo5130(ths,0x6e,0x70025);//DCCTRL过载设置
	err=mTMC4361DriverWriteDatoTo5130(ths,0x70,0x504C8);//PWMCONF设置  404ff  0x504C8
	err=mTMC4361DriverWriteDatoTo5130(ths,0x6c,0x101d5);//斩波器配置 10223
	err=mTMC4361DriverWriteDatoTo5130(ths,0x6d,0x01080F01);//电流调节配置   0x100a220

	if(err)
	{
		printf("write 5130 err....\r\n");
		return err;//5130驱动配置出错就直接退出
	}
	
/************************以下的4361全局寄存器配置***************************************/	
	SPI_WriteRead(ths,TMC4361_STEP_CONF,           SPI_WRITE_TYPE,0xC80,RxData);	
	SPI_WriteRead(ths,TMC4361_INPUT_FILT_CONF,     SPI_WRITE_TYPE, 0x540022,RxData);//输入信号过滤	
	SPI_WriteRead(ths,TMC4361_STP_LENGTH_ADD,      SPI_WRITE_TYPE, 0x60004,RxData);
	SPI_WriteRead(ths,TMC4361_SPI_STATUS_SELECTION,SPI_WRITE_TYPE,0x0409980F,RxData);	
	SPI_WriteRead(ths,TMC4361_GENERAL_CONF,        SPI_WRITE_TYPE,0x00017006,RxData);
	SPI_WriteRead(ths,TMC4361_REFERENCE_CONF,      SPI_WRITE_TYPE,0x1800000,RxData);//
	
	SPI_WriteRead(ths,TMC4361_CURRENT_CONF, SPI_WRITE_TYPE,0x3,RxData);//电流设置
	SPI_WriteRead(ths,TMC4361_SCALE_VALUES, SPI_WRITE_TYPE,0x432f064,RxData);//电流设置开环也要配置电流 24-31位是开环时的停止电流
	
	SPI_WriteRead(ths,TMC4361_ASTART,   SPI_WRITE_TYPE, 0x0,RxData);	
	SPI_WriteRead(ths,TMC4361_VMAX,     SPI_WRITE_TYPE, 0x0,RxData);
	SPI_WriteRead(ths,TMC4361_XACTUAL,  SPI_WRITE_TYPE, 0x0,RxData);
	SPI_WriteRead(ths,TMC4361_X_TARGET, SPI_WRITE_TYPE, 0x0,RxData);
	SPI_WriteRead(ths,TMC4361_VSTOP,    SPI_WRITE_TYPE, 0x0,RxData);
	SPI_WriteRead(ths,TMC4361_VSTART,   SPI_WRITE_TYPE, 0x0,RxData);
	SPI_WriteRead(ths,TMC4361_ENC_POS,	SPI_WRITE_TYPE, 0x0,RxData);
	SPI_WriteRead(ths,TMC4361_XACTUAL,  SPI_WRITE_TYPE, 0x0,RxData);
	SPI_WriteRead(ths,TMC4361_X_TARGET, SPI_WRITE_TYPE, 0x0,RxData);
	SPI_WriteRead(ths,TMC4361_ENC_POS,	SPI_WRITE_TYPE, 0x0,RxData);
	SPI_WriteRead(ths,TMC4361_XACTUAL,  SPI_WRITE_TYPE, 0x0,RxData);
	SPI_WriteRead(ths,TMC4361_X_TARGET, SPI_WRITE_TYPE, 0x0,RxData);	
	OSTimeDlyHMSM(0,0,1,0);
//	return err;
/***************************************编码器配置及闭环设置***************************************/	
	SPI_WriteRead(ths,TMC4361_ENC_IN_RES_WR,SPI_WRITE_TYPE,0x640,RxData); // Encoder resolution = 1600

//	SPI_WriteRead(ths,TMC4361_RAMPMODE,     SPI_WRITE_TYPE,0x4,RxData); // hold + position mode
//	SPI_WriteRead(ths,TMC4361_VMAX,         SPI_WRITE_TYPE,0x100000,RxData); // slow velocity 
//	SPI_WriteRead(ths,TMC4361_PID_D_WR,     SPI_WRITE_TYPE,0x10000,RxData); // cl_p = 1.0 
//	SPI_WriteRead(ths,TMC4361_ENC_IN_CONF,  SPI_WRITE_TYPE,0x400000,RxData); // turn on closed loop 
//	SPI_WriteRead(ths,TMC4361_X_TARGET,     SPI_WRITE_TYPE,0x80,RxData); // move to full step position
//	OSTimeDlyHMSM(0,0,0,100);
//	SPI_WriteRead(ths,TMC4361_ENC_IN_CONF,  SPI_WRITE_TYPE,0x1400000,RxData); // turn on closed loop calibration
//	SPI_WriteRead(ths,TMC4361_ENC_IN_CONF,  SPI_WRITE_TYPE,0x400000,RxData); // turn off closed loop calibration
//	SPI_WriteRead(ths,TMC4361_RAMPMODE,     SPI_WRITE_TYPE,0x0,RxData); // hold + position mode	
//	SPI_WriteRead(ths,TMC4361_VMAX,         SPI_WRITE_TYPE,0x0,RxData); // v = 0

	/*59号参数是指系统固有误差，指编码器与内部计数值存在的固有偏差，
	这个偏差与实际位置偏差之减就是需要跟随的调节误差，这个误差值将会与PI作用*/
	SPI_WriteRead(ths,TMC4361_CL_TR_TOLERANCE_WR,SPI_WRITE_TYPE,0x28,RxData); //触发位置到达信号时，内部位置计算值与实际编码器之差 
	SPI_WriteRead(ths,TMC4361_CL_OFFSET,         SPI_WRITE_TYPE,0x28,RxData);//闭环校准后内部计数值与编码器位置之差

	SPI_WriteRead(ths,TMC4361_CL_BETA,           SPI_WRITE_TYPE,0xFF00FF,RxData);//闭环调节的最大换向角 
	SPI_WriteRead(ths,TMC4361_FS_VEL_WR,         SPI_WRITE_TYPE,300000,RxData);//编码器达到这个整度时反电动势开始补偿
	SPI_WriteRead(ths,TMC4361_DC_TIME_WR,        SPI_WRITE_TYPE,600000,RxData);//速度阈值，用于计算反电动势补偿达到最大角度 CL_GAMMA 时的编码器速度
	SPI_WriteRead(ths,TMC4361_DC_LSPTM_WR,       SPI_WRITE_TYPE,0xFFFFFF,RxData); //编码器速度0延时检测 在这个时间段内编码器没有输出则认为速度为0
	SPI_WriteRead(ths,TMC4361_ENC_VMEAN_WAIT_WR, SPI_WRITE_TYPE,0x2000864,RxData); //编码滤波 
	
	SPI_WriteRead(ths,TMC4361_CL_TOLERANCE_WR,   SPI_WRITE_TYPE,50,RxData);//位置允许偏差 如果 |ENC_POS_DEV| < CL_TOLERANCE ，则 CL_DELTA_P = 65536 (增益=1)

	SPI_WriteRead(ths,TMC4361_CL_DELTA_P_WR,    SPI_WRITE_TYPE,70000,RxData);//位置环P	
	SPI_WriteRead(ths,TMC4361_CL_VMAX_CALC_I_WR,SPI_WRITE_TYPE,10000,RxData);//速度环I
	SPI_WriteRead(ths,TMC4361_CL_VMAX_CALC_P_WR,SPI_WRITE_TYPE,20000,RxData);//速度环p
	
	SPI_WriteRead(ths,TMC4361_PID_I_CLIP_WR,    SPI_WRITE_TYPE,10,RxData);// PID_ISUM 的限幅值
	SPI_WriteRead(ths,TMC4361_PID_DV_CLIP_WR,   SPI_WRITE_TYPE,0x100000,RxData);//PID_VEL 的限幅值
	
	SPI_WriteRead(ths,TMC4361_SCALE_VALUES,     SPI_WRITE_TYPE,0x648008,RxData); //电流调节值 
	SPI_WriteRead(ths,TMC4361_UP_SCALE_DELAY,   SPI_WRITE_TYPE,30000,RxData); // 电流上升时间  0x10000
	SPI_WriteRead(ths,TMC4361_HOLD_SCALE_DELAY, SPI_WRITE_TYPE,10000,RxData); // 电流下降到锁住时间  0x10000
	SPI_WriteRead(ths,TMC4361_STDBY_DELAY,      SPI_WRITE_TYPE,0x8,RxData);//斜坡停止到待机有效之间的延迟时间[时钟周期数]  2
	SPI_WriteRead(ths,TMC4361_ENC_IN_CONF,      SPI_WRITE_TYPE,0xA400000,RxData); // 
	SPI_WriteRead(ths,TMC4361_CURRENT_CONF,     SPI_WRITE_TYPE,0x83,RxData);

//	OSTimeDlyHMSM(0,0,1,0);
	/*电机存在超调现象，应该是编码器位置与内部计数器位置超过允许误差，可能是机械原因造成编码器与内部位置不对，
	有可能是PI参数不对存在超调*/
    return err;
}

