#include "ucos_ii.h"
#include "mTMC5130Driver.h"
#include <stdlib.h>
#include "os_implement.h"
#include "mTMC5130DThread.h"

extern SPI_HandleTypeDef hspi1;
static bus_os_sem  SPI_SEM;
static int CHOPCONF = R6C;

const char tmc5130_defaultRegisterAccess[] =
{
//	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
	3, 1, 1, 2, 7, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x00 - 0x0F
	2, 2, 1, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x10 - 0x1F
	3, 3, 1, 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 3, 0, 0, // 0x20 - 0x2F
	0, 0, 0, 2, 3, 1, 1, 0, 3, 3, 2, 1, 1, 0, 0, 0, // 0x30 - 0x3F
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x40 - 0x4F
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x50 - 0x5F
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 3, 2, 2, 1, // 0x60 - 0x6F
	2, 1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // 0x70 - 0x7F
};

//5130默认寄存器值
const int tmc5130_defaultRegisterResetState[] =
{
//	0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   A,   B,   C,   D,   E,   F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x00 - 0x0F
	R10, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x10 - 0x1F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x20 - 0x2F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   R3A, 0,   0,   0,   0,   0, // 0x30 - 0x3F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x40 - 0x4F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x50 - 0x5F
	R60, R61, R62, R63, R64, R65, R66, R67, R68, R69, 0,   0,   R6C, 0,   0,   0, // 0x60 - 0x6F
	R70, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, // 0x70 - 0x7F
};


__attribute__((always_inline)) void Delay_Nus(unsigned int nus)
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


__attribute__((always_inline)) static unsigned char readWrite(unsigned int data)
{
	while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE) == RESET) {};
	*((__IO uint8_t*)&hspi1.Instance->DR) = data;
	while(__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE) == RESET) {};
	return hspi1.Instance->DR;
}

__attribute__((always_inline)) int SPI_WriteRead(struct sTABLE *ths,char addr,char cmd,unsigned int data,unsigned char *pRxData) 
{
	u8 err=0;
		err=bus_os_pend_sem(&SPI_SEM,100);//使用信号量保护SPI接口 在读写过程中不允许打断
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
	Delay_Nus(50);	
	pRxData[0]=readWrite((cmd<<7 | addr) & 0xff);
	pRxData[1]= readWrite((data >> 24) & 0xff);
	pRxData[2]= readWrite((data >> 16) & 0xff);
	pRxData[3]=readWrite((data >> 8) & 0xff);
	pRxData[4]= readWrite((data) & 0xff);
	Delay_Nus(20);
	ths->mTMCDriver_WriteIo(PIN_ID_CS,1);	
#endif
	
		bus_os_post_sem(&SPI_SEM);
	
	
	if(pRxData[0]== 0 || pRxData[0]==0xff) //正常情况  从SPI返回的数据位 不能全是0或全部是1 
	{
		printf("spi error.......\r\n");
	}
	return err;
}


__attribute__((always_inline)) void mTMC5130DriverSetSpeed(struct sTABLE *ths) //速度设置
{	
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据
	SPI_WriteRead(ths,TMC5130_A1,SPI_WRITE_TYPE,ths->A1,RxData);  //起始加速度
	SPI_WriteRead(ths,TMC5130_V1,SPI_WRITE_TYPE,ths->V1,RxData);//第一段速度 
	SPI_WriteRead(ths,TMC5130_AMAX,SPI_WRITE_TYPE,ths->AMAX,RxData);//第二段加速度
	SPI_WriteRead(ths,TMC5130_VMAX,SPI_WRITE_TYPE,ths->VMAX,RxData);//最高速度
	SPI_WriteRead(ths,TMC5130_DMAX,SPI_WRITE_TYPE,ths->DMAX,RxData);//起始减减加速度
	SPI_WriteRead(ths,TMC5130_D1,SPI_WRITE_TYPE,ths->D1,RxData);//第二段减速加速度		
	SPI_WriteRead(ths,TMC5130_VSTART,SPI_WRITE_TYPE,0,RxData);//启动速度 这个速度要低于停止速度才 设置为0即可
	SPI_WriteRead(ths,TMC5130_VSTOP,SPI_WRITE_TYPE,0xA,RxData);//停止速度  这个速度一定要大于启动速度				
}

__attribute__((always_inline)) void mTMC5130DriverSetMicoStep(struct sTABLE *ths)  //细分设置
{
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据  
	char microsteps=0;	
	int steps=ths->MicoStep;
	switch(steps)
	{
	case 1:     microsteps = 8;   break;
	case 2:     microsteps= 7;    break;
	case 4:     microsteps= 6;    break;
	case 8:     microsteps = 5;   break;
	case 16:   microsteps = 4;   break;
	case 32:   microsteps = 3;   break;
	case 64:   microsteps = 2;   break;
	case 128: microsteps = 1;   break;
	case 256: microsteps = 0;   break;
	default:   microsteps=4;break; //默认16细分
	}	 
	int tmp=(ths->DriverRegister->CHOPCONF & 0xF0FFFFFF) | microsteps<<24;
	ths->DriverRegister->CHOPCONF=tmp;
	SPI_WriteRead(ths,TMC5130_CHOPCONF,SPI_WRITE_TYPE,tmp,RxData);
	
}

__attribute__((always_inline)) void mTMC5130DriverSetCurrent(struct sTABLE *ths)//设置电流 n1运行时电流  n2停止时电流
{	  
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据
	uint32_t cmd =6<<16 | ths->run_currnet<<8 | ths->hold_current;		    
	SPI_WriteRead(ths,TMC5130_IHOLD_IRUN,SPI_WRITE_TYPE,cmd,RxData);
}

void mTMC5130DriverResetPostion(struct sTABLE *ths)  //主要用来清零当前和目标寄存器
{	
	unsigned char RxData2[5]={0,0,0,0,0};//用来存放从SPI读取到的数据
	unsigned char RxData1[5]={0,0,0,0,0};//用来存放从SPI读取到的数据
	do
	{
		SPI_WriteRead(ths,TMC5130_XTARGET,SPI_WRITE_TYPE,0,RxData1);//清零当目标寄存器
		SPI_WriteRead(ths,TMC5130_XACTUAL,SPI_WRITE_TYPE,0,RxData2);//清零当前位置寄存器

		SPI_WriteRead(ths,TMC5130_XTARGET,SPI_READ_TYPE,0,RxData1);
		SPI_WriteRead(ths,TMC5130_XACTUAL,SPI_READ_TYPE,0,RxData2);
	}while((RxData1[1]+RxData1[2]+RxData1[3]+RxData1[4]+RxData2[1]+RxData2[2]+RxData2[3]+RxData2[4])  !=0 );		
}

int mTMC5130DriverGetDRVSTATUS(struct sTABLE *ths) //获取驱动器信息
{  	  
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据	
	SPI_WriteRead(ths,TMC5130_DRVSTATUS,SPI_READ_TYPE,0,RxData);
	SPI_WriteRead(ths,TMC5130_DRVSTATUS,SPI_READ_TYPE,0,RxData);		
	return (RxData[1]<<24 | RxData[2]<<16 | RxData[3]<<8 | RxData[4]);
}

 int mTMC5130DriverGetRAMPSTAT(struct sTABLE *ths) //获取驱动器标志位
{  	  
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据	
	SPI_WriteRead(ths,TMC5130_RAMPSTAT,SPI_READ_TYPE,0,RxData);
	SPI_WriteRead(ths,TMC5130_RAMPSTAT,SPI_READ_TYPE,0,RxData);				
	return (RxData[1]<<24 | RxData[2]<<16 | RxData[3]<<8 | RxData[4]);
}

 int mTMC5130DriverGetPosition(struct sTABLE *ths)//获取位置信息
{
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据	
	SPI_WriteRead(ths,TMC5130_XACTUAL,SPI_READ_TYPE,0,RxData);
	SPI_WriteRead(ths,TMC5130_XACTUAL,SPI_READ_TYPE,0,RxData);		
	return (RxData[1]<<24 | RxData[2]<<16 | RxData[3]<<8 | RxData[4]);
}


char mTMC5130DriverGetState(struct sTABLE *ths)
{
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据	
	SPI_WriteRead(ths,TMC5130_RAMPSTAT,SPI_READ_TYPE,0,RxData);
	SPI_WriteRead(ths,TMC5130_RAMPSTAT,SPI_READ_TYPE,0,RxData);		
	return RxData[0] & 0xff;
}

void mTMC5130DriverResetStopSpeed(struct sTABLE *ths)
{
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据	
	SPI_WriteRead(ths,TMC5130_VSTART,SPI_WRITE_TYPE,0,RxData);
	SPI_WriteRead(ths,TMC5130_VSTOP,SPI_WRITE_TYPE,0,RxData);
}


int mTMC5130DriverReadWriteRegister(struct sTABLE *ths,char addr,char cmd,int data)//获取寄存器信息
{
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据
	int result=0;
	if(cmd == 0) //读寄存器
	{			
		SPI_WriteRead(ths,addr,SPI_READ_TYPE,0,RxData);
		SPI_WriteRead(ths,addr,SPI_READ_TYPE,0,RxData);	
		result=(RxData[1]<<24 | RxData[2]<<16 | RxData[3]<<8 | RxData[4]);
	}	
	else  //写寄存器
	{
		SPI_WriteRead(ths,addr,SPI_WRITE_TYPE,data,RxData);
		result=RxData[0] & 0xff;
	}			
	return 	result;
}

void mTMC5130DriverSetCompare(struct sTABLE *ths)
{
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据
	SPI_WriteRead(ths,TMC5130_X_COMPARE, SPI_WRITE_TYPE, ths->postion,RxData);//位置比较寄存器
}

void mTMC5130DriverSetGCONF(struct sTABLE *ths,char data)
{
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据
	SPI_WriteRead(ths,TMC5130_GCONF, SPI_WRITE_TYPE, data,RxData);//位置比较寄存器
}

__attribute__((always_inline)) void mTMC5130DriverMove(struct sTABLE *ths)  //运动底动函数  所有运动最终都会调用这个函数
{ 
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据	
	if(ths->VMAX >0)
	{
		mTMC5130DriverSetMicoStep(ths);      //设置细分
		mTMC5130DriverSetCurrent(ths);         //设置运行时电流  
	}
	mTMC5130DriverSetSpeed(ths);          //设置速度

//	SPI_WriteRead(ths,TMC5130_TPWMTHRS,SPI_WRITE_TYPE,ths->DriverRegister->TPWMTHRS,RxData);
//	SPI_WriteRead(ths,TMC5130_TCOOLTHRS,SPI_WRITE_TYPE,ths->DriverRegister->TCOOLTHRS,RxData);
//	SPI_WriteRead(ths,TMC5130_COOLCONF,SPI_WRITE_TYPE,ths->DriverRegister->COOLCONF,RxData);	
//	SPI_WriteRead(ths,TMC5130_GCONF,SPI_WRITE_TYPE,ths->DriverRegister->GCONF,RxData);

	SPI_WriteRead(ths,TMC5130_RAMPMODE, SPI_WRITE_TYPE, ths->mode,RxData);//控制模式

	if(ths->mode == TMC5130_MODE_POSITION) //只有位置模式才需要设置位置寄存器
	{
		SPI_WriteRead(ths,TMC5130_X_COMPARE, SPI_WRITE_TYPE, ths->postion,RxData);//位置比较寄存器
		SPI_WriteRead(ths,TMC5130_XTARGET, SPI_WRITE_TYPE, ths->postion,RxData);	//位置信息写到寄存器里
	}		
}


void mTMC5130DriverSetDefaultRegister(struct sTABLE *ths,char setType) //配置5130寄存器参数
{
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据
	if(setType == 1){
		for(unsigned char i=0;i<sizeof(tmc5130_defaultRegisterResetState)/4;i++) {
			SPI_WriteRead(ths,i,SPI_WRITE_TYPE,tmc5130_defaultRegisterResetState[i],RxData);} 
	}	
	else if(setType == 0){
		for(unsigned char i=0;i<sizeof(tmc5130_defaultRegisterAccess);i++) {
			SPI_WriteRead(ths,i,SPI_WRITE_TYPE,tmc5130_defaultRegisterAccess[i],RxData);}
	}	
	else{
		for(unsigned char i=0;i<128;i++) {
			SPI_WriteRead(ths,i,SPI_WRITE_TYPE,0,RxData);}		
	}
}


void mTMC5130Driver_Init(struct sTABLE *ths)//初始化
{
	unsigned char RxData[5]={0,0,0,0,0};//用来存放从SPI读取到的数据
	ths->mTMCDriver_WriteIo(PIN_ID_ENABLE,1);
	OSTimeDlyHMSM(0,0,0,20);
	ths->mTMCDriver_WriteIo(PIN_ID_REFL,0);//状态位直接设置成1  这样在返回数据时  始终不会为0
	ths->mTMCDriver_WriteIo(PIN_ID_REFR,1);

	if(SPI_SEM.life_flag == 0)  //如果建立了信号量  就不再重新创建了
	{
		bus_os_create_sem(&SPI_SEM,1);
	}	

	SPI_WriteRead(ths,TMC5130_GSTAT,SPI_WRITE_TYPE,0,RxData);//初始化时  初始化所有寄存器

	
//    mTMC5130DriverSetDefaultRegister(ths,-1);//先把全部寄存器初始化为默认值 
	mTMC5130DriverSetDefaultRegister(ths,1);//先把全部寄存器初始化为默认值 
	
	SPI_WriteRead(ths,TMC5130_IHOLD_IRUN,SPI_WRITE_TYPE,0x61005,RxData);//设置电流 运行电流是16 停止电流是1	
	SPI_WriteRead(ths,TMC5130_SWMODE,SPI_WRITE_TYPE,0x88a,RxData);

//	SPI_WriteRead(ths,TMC5130_TPWMTHRS,SPI_WRITE_TYPE,ths->DriverRegister->TPWMTHRS,RxData);//stealthChop运行的速度上限  超过这个速度将启动spreadCycle
//	SPI_WriteRead(ths,TMC5130_TCOOLTHRS,SPI_WRITE_TYPE,ths->DriverRegister->TCOOLTHRS,RxData);//coolStep运行的速度下限 超过个速度才启用coolStep
//	SPI_WriteRead(ths,TMC5130_THIGH,SPI_WRITE_TYPE,ths->DriverRegister->THIGH,RxData);			
//	SPI_WriteRead(ths,TMC5130_VDCMIN,SPI_WRITE_TYPE,ths->DriverRegister->VDCMIN,RxData);  		

//	SPI_WriteRead(ths,TMC5130_COOLCONF,SPI_WRITE_TYPE,ths->DriverRegister->COOLCONF,RxData);			
//	SPI_WriteRead(ths,TMC5130_PWMCONF,SPI_WRITE_TYPE,ths->DriverRegister->PWMCONF,RxData);  
//	SPI_WriteRead(ths,TMC5130_CHOPCONF,SPI_WRITE_TYPE,ths->DriverRegister->CHOPCONF,RxData);			
//			
//	SPI_WriteRead(ths,TMC5130_TPOWERDOWN,SPI_WRITE_TYPE,1,RxData);//电机停止后 电机对电源的电流下降延时为0			
//	SPI_WriteRead(ths,TMC5130_TZEROWAIT,SPI_WRITE_TYPE,1,RxData);//电机启停间隔 1为1秒						

//	SPI_WriteRead(ths,TMC5130_VMAX,SPI_WRITE_TYPE,0,RxData);//
//	SPI_WriteRead(ths,TMC5130_RAMPMODE, SPI_WRITE_TYPE, TMC5130_MODE_VELPOS,RxData);//控制模式					
//	SPI_WriteRead(ths,TMC5130_XTARGET,SPI_WRITE_TYPE,0,RxData);//清零当目标寄存器
//	SPI_WriteRead(ths,TMC5130_XACTUAL,SPI_WRITE_TYPE,0,RxData);//清零当前位置寄存器	

	//SPI_WriteRead(ths,TMC5130_GCONF,SPI_WRITE_TYPE,5,RxData);
			
	OSTimeDlyHMSM(0,0,0,50);
	ths->mTMCDriver_WriteIo(PIN_ID_ENABLE,0);	
}

