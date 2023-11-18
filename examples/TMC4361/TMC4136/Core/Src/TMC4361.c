/*
 * TMC4361.c
 *
 *  Created on: 18.07.2017
 *      Author: LK
 */

#include "TMC4361.h"
#include "stdlib.h"
#include "math.h"


static const uint32_t tmc4361_defaultRegisterResetState[TMC4361A_REGISTER_COUNT];
static const uint8_t tmc4361_defaultRegisterAccess[TMC4361A_REGISTER_COUNT];


//直接操作TMC4136寄存器函数
static inline uint32_t TMC4361A_Field_read(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift);
static inline void TMC4361A_Field_Write(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift,uint32_t value);

static inline int32_t TMC4361A_Field_read_s(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift);
static inline void TMC4361A_Field_Write_s(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift,int32_t value);

static void TMC4361A_WriteInt32(TMC4361TypeDef *tmc, uint8_t address, uint32_t value);
static void TMC4361A_WriteInt32S(TMC4361TypeDef *tmc, uint8_t address, int32_t value);
static uint32_t TMC4361A_ReadInt32(TMC4361TypeDef *tmc, uint8_t address);
static int32_t TMC4361A_ReadInt32S(TMC4361TypeDef *tmc, uint8_t address);
//通过覆盖数据报功能 操作后置 电机驱动寄存器函数
//static inline uint32_t TMC4361A_Field_read_cover(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift);
//static inline void TMC4361A_Field_Write_cover(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift,uint32_t value);
static void TMC4361A_WriteCoverInt32(TMC4361TypeDef *tmc, uint8_t address, uint32_t value);
static uint32_t TMC4361A_ReadCoverInt32(TMC4361TypeDef *tmc, uint8_t address);





void TMC4361A_Init(TMC4361TypeDef* tmc)
{
	
//    for(int i=0;i<TMC4361A_REGISTER_COUNT;i++)
//    {
//       TMC4361A_WriteInt32(tmc,i,tmc4361_defaultRegisterResetState[i]);//清空寄存器
//    }
//	TMC4361A_ReadInt32(tmc,0x0E);
	 for(int i=0;i<TMC4361A_REGISTER_COUNT;i++)
    {

        tmc->shadowRegister[i]=tmc4361_defaultRegisterResetState[i];
		tmc->registerAccessRights[i]=tmc4361_defaultRegisterAccess[i];
    }

  
}



//从寄存器读出一个指定的功能段 后 清楚无关的数据 便于使用
static inline uint32_t TMC4361A_Field_get(uint32_t data, uint32_t mask, uint32_t shift)
{
	return (((data) & (mask)) >> (shift));
}

//向寄存器中一个指定的功能段写入数据前 计算合适的32位数据 不破坏原有的数据
static inline uint32_t TMC4361A_Field_set(uint32_t data, uint32_t mask, uint32_t shift,uint32_t value)
{
	return 	(((data) & (~(mask))) | (((value) << (shift)) & (mask)));
}




//向指定寄存器地址 写入四个8位数据
static void TMC4361A_WriteDatagram(TMC4361TypeDef* tmc,uint8_t address,uint8_t x1, uint8_t x2, uint8_t x3, uint8_t x4)
{
    int value;
	uint8_t data[5] = { address | TMC4361A_WRITE_BIT, x1, x2, x3, x4 };
    
    tmc->spi.spiWR(data);

	tmc->status = data[0];

	value = ((uint32_t)x1 << 24) | ((uint32_t)x2 << 16) | (x3 << 8) | x4;


	// Write to the shadow register and mark the register dirty
	address = TMC_ADDRESS(address);
 
	if(address>=TMC4361A_REGISTER_COUNT)
		return;//防止数组越界

	tmc->shadowRegister[address] = value;
	tmc->registerAccessRights[address] |= TMC_ACCESS_DIRTY;
}

static void TMC4361A_WriteInt32(TMC4361TypeDef *tmc, uint8_t address, uint32_t value)
{
	TMC4361A_WriteDatagram(tmc, address, BYTE(value, 3), BYTE(value, 2), BYTE(value, 1), BYTE(value, 0));
}


static void TMC4361A_WriteInt32S(TMC4361TypeDef *tmc, uint8_t address, int32_t value)
{
	TMC4361A_WriteDatagram(tmc, address, BYTE(value, 3), BYTE(value, 2), BYTE(value, 1), BYTE(value, 0));
}


static uint32_t TMC4361A_ReadInt32(TMC4361TypeDef *tmc, uint8_t address)
{
	int value;
	uint8_t data[5]={0,0,0,0,0};

	address = TMC_ADDRESS(address);

    //判断一下 要读取的寄存器是否可读
    //如果不可读 返回影子寄存器的值
	if(!TMC_IS_READABLE(tmc->registerAccessRights[address]))
		return tmc->shadowRegister[address];

//需要连续读取两次 这次读是上次状态
	data[0] = address;
    tmc->spi.spiWR(data);
    data[0] = address;
	tmc->spi.spiWR(data);

	tmc->status = data[0];
	value = ((uint32_t)data[1] << 24) | ((uint32_t)data[2] << 16) | (data[3] << 8) | data[4];

	return value;
}

//带符号
static int32_t TMC4361A_ReadInt32S(TMC4361TypeDef *tmc, uint8_t address)
{
	int value;
	uint8_t data[5]={0,0,0,0,0};

	address = TMC_ADDRESS(address);

    //判断一下 要读取的寄存器是否可读
    //如果不可读 返回影子寄存器的值
	if(!TMC_IS_READABLE(tmc->registerAccessRights[address]))
		return tmc->shadowRegister[address];

//需要连续读取两次 这次读是上次状态
	data[0] = address;
    tmc->spi.spiWR(data);
    data[0] = address;
	tmc->spi.spiWR(data);

	tmc->status = data[0];
	value = ((uint32_t)data[1] << 24) | ((uint32_t)data[2] << 16) | (data[3] << 8) | data[4];

	return value;
}

uint32_t TMC4361_Event_Read(TMC4361TypeDef *tmc)
{
	return TMC4361A_ReadInt32(tmc,0x0E);	
}

uint32_t TMC4361_Event_Clear(TMC4361TypeDef *tmc,uint32_t event)
{
	TMC4361A_WriteInt32(tmc,0x0E,(event));

	return TMC4361A_ReadInt32(tmc,0x0E);	
}

static uint32_t TMC4361A_Read_Clear_Events(TMC4361TypeDef *tmc)
{
	return TMC4361A_ReadInt32(tmc,0x0E);
}

static void TMC4361A_Clear_Events(TMC4361TypeDef *tmc ,uint8_t b)
{
	TMC4361A_WriteInt32(tmc,0x0E,(1<<b));
}


//从寄存器读出一个指定的功能段
static inline uint32_t TMC4361A_Field_read(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift)
{
	return TMC4361A_Field_get(TMC4361A_ReadInt32(tdef, address), mask, shift);
	
}
//从寄存器读出一个指定的功能段
static inline int32_t TMC4361A_Field_read_s(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift)
{
	return TMC4361A_Field_get(TMC4361A_ReadInt32S(tdef, address), mask, shift);
	
}

//向寄存器中一个指定的功能段写入数据
static inline void TMC4361A_Field_Write(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift,uint32_t value)
{
	
	TMC4361A_WriteInt32(tdef, address, TMC4361A_Field_set(TMC4361A_ReadInt32(tdef, address), mask, shift, value));
}

//向寄存器中一个指定的功能段写入数据
static inline void TMC4361A_Field_Write_s(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift,int32_t value)
{
	
	TMC4361A_WriteInt32S(tdef, address, TMC4361A_Field_set(TMC4361A_ReadInt32(tdef, address), mask, shift, value));
}


//发送覆盖数据报
static void TMC4361A_SendCoverDatagram(TMC4361TypeDef* tmc,uint8_t address,uint8_t x1, uint8_t x2, uint8_t x3, uint8_t x4)
{
	int value;
	uint8_t data[5] = { address | TMC4361A_WRITE_BIT, x1, x2, x3, x4 };
    

	
	uint8_t cover_data_high[5]={TMC4361A_COVER_HIGH_WR|TMC4361A_WRITE_BIT,0x00,0x00,0x00,data[0]};
	uint8_t cover_data_low[5]={TMC4361A_COVER_LOW_WR|TMC4361A_WRITE_BIT,data[1],data[2],data[3],data[4]};

	//设定报文数据寄存器
    tmc->spi.spiWR(cover_data_high);//发送高40位  一般来说 是 后置驱动器的寄存器地址

    tmc->spi.spiWR(cover_data_low);//发送低40位   一般来说 是 后置驱动器的寄存器上数据

	tmc->status = cover_data_low[0];

	bool rt=tmc->getInter(tmc,50,TMC4361A_COVER_DONE_MASK);

	if(rt==false)
	{
		return;
	}

	TMC4361A_Clear_Events(tmc,TMC4361A_COVER_DONE_SHIFT);

	value = ((uint32_t)cover_data_high[1] << 24) | ((uint32_t)cover_data_high[2] << 16) | (cover_data_high[3] << 8) | cover_data_high[4];
	tmc->shadowRegister[TMC4361A_COVER_HIGH_WR] = value;
	tmc->registerAccessRights[TMC4361A_COVER_HIGH_WR] |= TMC_ACCESS_DIRTY;	
	
 	value = ((uint32_t)cover_data_low[1] << 24) | ((uint32_t)cover_data_low[2] << 16) | (cover_data_low[3] << 8) | cover_data_low[4];
	tmc->shadowRegister[TMC4361A_COVER_LOW_WR] = value;
	tmc->registerAccessRights[TMC4361A_COVER_LOW_WR] |= TMC_ACCESS_DIRTY;

}

static void TMC4361A_WriteCoverInt32(TMC4361TypeDef *tmc, uint8_t address, uint32_t value)
{
	TMC4361A_SendCoverDatagram(tmc, address, BYTE(value, 3), BYTE(value, 2), BYTE(value, 1), BYTE(value, 0));
}

//接收覆盖数据报
static uint32_t TMC4361A_ReadCoverInt32(TMC4361TypeDef *tmc, uint8_t address)
{
	int value;
	uint8_t data[5];

	
	uint8_t cover_data_high[5]={TMC4361A_COVER_DRV_HIGH_RD,0x00,0x00,0x00,0x00};
	uint8_t cover_data_low[5]={TMC4361A_COVER_DRV_LOW_RD,0x00,0x00,0x00,0x00};
	
	
	//先通过写入数据发送报寄存器 发送给 电机驱动 想要读取的地址
	address = TMC_ADDRESS(address);
	data[0] = address;

	//需要发送两次 因为这次发是上次的数据
	TMC4361A_SendCoverDatagram(tmc, address, 0, 0, 0,0);

	TMC4361A_SendCoverDatagram(tmc, address, 0, 0, 0,0);
	
	//电机驱动会将返回数据报 保存在 数据报接收寄存器上
	
	//需要连读两个寄存器 高32和低32	
	
	cover_data_high[0]=TMC_ADDRESS(cover_data_high[0]);
	
	 tmc->spi.spiWR(cover_data_high);

 
	//其实也可以不读高32寄存器 因为在40位数据报覆盖模式中 仅有8位地址
	
	
	cover_data_low[0]=TMC_ADDRESS(cover_data_low[0]);
	tmc->spi.spiWR(cover_data_low);
	tmc->status = data[0];
	value = ((uint32_t)cover_data_low[1] << 24) | ((uint32_t)cover_data_low[2] << 16) | (cover_data_low[3] << 8) | cover_data_low[4];

	return value;
}




////从覆盖数据报寄存器读出一个指定的功能段
//static inline uint32_t TMC4361A_Field_read_cover(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift)
//{
//	return TMC4361A_Field_get(TMC4361A_ReadCoverInt32(tdef, address), mask, shift);
//	
//}
////向覆盖数据报寄存器中一个指定的功能段写入数据
//static inline void TMC4361A_Field_Write_cover(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift,uint32_t value)
//{
//	
//	TMC4361A_WriteCoverInt32(tdef, address, TMC4361A_Field_set(TMC4361A_ReadInt32(tdef, address), mask, shift, value));
//}



static int32_t TMC4361A_DiscardVelocityDecimals(int32_t value)
{
	// if(abs(value) > 8000000)
	// {
	// 	value = (value < 0) ? -8000000 : 8000000;
	// }
	return value << 8;
}


static void TMC4361A_Rotate(TMC4361TypeDef *tmc4361, int32_t velocity)
{
	// Disable Position Mode
	TMC4361A_Field_Write(tmc4361, TMC4361A_RAMPMODE, TMC4361A_OPERATION_MODE_MASK, TMC4361A_OPERATION_MODE_SHIFT, 0);

	TMC4361A_WriteInt32(tmc4361, TMC4361A_VMAX, TMC4361A_DiscardVelocityDecimals(velocity));
}

//速度模式 左转
void TMC4361A_Right(TMC4361TypeDef *tmc4361, int32_t velocity)
{
	TMC4361A_Rotate(tmc4361, velocity);
}

//速度模式 右转
void TMC4361A_Left(TMC4361TypeDef *tmc4361, int32_t velocity)
{
	TMC4361A_Rotate(tmc4361, -velocity);
}

//停止转动
void TMC4361A_Stop(TMC4361TypeDef *tmc4361)
{
	TMC4361A_Rotate(tmc4361, 0);
}

//读取 内部位置（电机位置）
int32_t TMC4361_GetPosition(TMC4361TypeDef *tmc4361)
{
	return TMC4361A_ReadInt32S(tmc4361,TMC4361A_XACTUAL);
}

//读取 目标位置（仅在位置模式下）
int32_t TMC4361_GetTargetPosition(TMC4361TypeDef *tmc4361)
{
	return TMC4361A_ReadInt32S(tmc4361,TMC4361A_X_TARGET);
}

//读取 外部位置 （编码器位置）
int32_t TMC4361_GetEncPosition(TMC4361TypeDef *tmc4361)
{
	return TMC4361A_ReadInt32S(tmc4361,TMC4361A_ENC_POS);
}

//位置模式 绝对移动
void TMC4361A_MoveTo(TMC4361TypeDef *tmc4361, int32_t position, uint32_t velocityMax)
{

	// Enable Position Mode
	TMC4361A_Field_Write(tmc4361, TMC4361A_RAMPMODE, TMC4361A_OPERATION_MODE_MASK, TMC4361A_OPERATION_MODE_SHIFT, 1);

	TMC4361A_WriteInt32(tmc4361, TMC4361A_VMAX, TMC4361A_DiscardVelocityDecimals(velocityMax));

	TMC4361A_WriteInt32S(tmc4361, TMC4361A_X_TARGET, position);
}

//位置模式 相对移动
// The function will write the absolute target position to *ticks
void TMC4361A_MoveBy(TMC4361TypeDef *tmc4361, int32_t *ticks, uint32_t velocityMax)
{
	// determine actual position and add numbers of ticks to move
	*ticks += TMC4361A_ReadInt32S(tmc4361, TMC4361A_XACTUAL);

	TMC4361A_MoveTo(tmc4361, *ticks, velocityMax);
}


static uint8_t TMC4361A_MoveToNextFullstep(TMC4361TypeDef *tmc4361)
{
	int32_t stepCount;

	// Motor must be stopped
	if(TMC4361A_ReadInt32S(tmc4361, TMC4361A_VACTUAL) != 0)
	{
		return 0;
	}

	// Position mode, hold mode, low velocity
	TMC4361A_WriteInt32S(tmc4361, TMC4361A_RAMPMODE, 4);
	TMC4361A_WriteInt32S(tmc4361, TMC4361A_VMAX, 10000 << 8);

	// Current step count
	stepCount = TMC4361A_Field_read_s(tmc4361, TMC4361A_MSCNT_RD, TMC4361A_MSCNT_MASK, TMC4361A_MSCNT_SHIFT);
	// Get microstep value of step count (lowest 8 bits)
	stepCount = stepCount % 256;
	// Assume: 256 microsteps -> Fullsteps are at 128 + n*256
	stepCount = 128 - stepCount;

	if(stepCount == 0)
	{
		// Fullstep reached
		return 1;
	}

	// Fullstep not reached -> calculate next fullstep position
	stepCount += TMC4361A_ReadInt32S(tmc4361, TMC4361A_XACTUAL);
	// Move to next fullstep position
	TMC4361A_WriteInt32S(tmc4361, TMC4361A_X_TARGET, stepCount);

	bool rt=false;
	rt = tmc4361->getReached(tmc4361,200);
	if(rt==false)
	{
		return 0;
	}

	return 1;
}

TMC4361_CALI_RT TMC4361_SimpleCali_Start(TMC4361TypeDef *tmc4361)
{

	//先暂时把电流设最大
	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_BETA, TMC4361A_CL_BETA_MASK, TMC4361A_CL_BETA_SHIFT,255);
	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_GAMMA, TMC4361A_CL_GAMMA_MASK, TMC4361A_CL_GAMMA_SHIFT,255);
	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_DELTA_P_WR, TMC4361A_CL_DELTA_P_MASK, TMC4361A_CL_DELTA_P_SHIFT,65536);
	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_TOLERANCE_WR, TMC4361A_CL_TOLERANCE_MASK, TMC4361A_CL_TOLERANCE_SHIFT,40);

	TMC4361A_WriteInt32(tmc4361, TMC4361A_CURRENT_CONF, 0);//Disable current scaling 关闭电流自动调节


	//移动到下个整步
	if(TMC4361A_MoveToNextFullstep(tmc4361)==0)
	{
		return TM4361_CALI_ERROR;
	}

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CL_CALIBRATION_EN_MASK, TMC4361A_CL_CALIBRATION_EN_SHIFT,1);
	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_REGULATION_MODUS_MASK, TMC4361A_REGULATION_MODUS_SHIFT,1);
	return TM4361_CALI_EQUAL;

}

int32_t TMC4361_SimpleCali_End(TMC4361TypeDef *tmc4361)
{
	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CL_CALIBRATION_EN_MASK, TMC4361A_CL_CALIBRATION_EN_SHIFT,0);

	// TMC4361A_Field_Write_s(tmc4361,TMC4361A_ENC_POS,TMC4361A_ENC_POS_MASK,TMC4361A_ENC_POS_SHIFT,0);
	// TMC4361A_WriteInt32S(tmc4361, TMC4361A_X_TARGET, 0);
	// TMC4361A_WriteInt32S(tmc4361,TMC4361A_XACTUAL,0);
	return TMC4361A_ReadInt32S(tmc4361,TMC4361A_CL_OFFSET);
}

TMC4361_CALI_RT TMC4361_Cali(TMC4361TypeDef *tmc4361)
{
	volatile int32_t xActual =TMC4361A_Field_read(tmc4361,TMC4361A_XACTUAL,TMC4361A_XACTUAL_MASK,TMC4361A_XACTUAL_SHIFT);
	volatile bool rt;
	TMC4361A_Field_Write_s(tmc4361,TMC4361A_ENC_POS,TMC4361A_ENC_POS_MASK,TMC4361A_ENC_POS_SHIFT,xActual);

	TMC4361A_Field_Write_s(tmc4361, TMC4361A_RAMPMODE, TMC4361A_OPERATION_MODE_MASK, TMC4361A_OPERATION_MODE_SHIFT, 1);
	TMC4361A_WriteInt32(tmc4361, TMC4361A_VMAX,0x271000);//速度设为10kpps
	
	int32_t xTarget=xActual+51200;
	TMC4361A_WriteInt32S(tmc4361, TMC4361A_X_TARGET, xTarget);//走二百个整步

	rt = tmc4361->getReached(tmc4361,2000);

	if(rt==false)
	{
		
		return TM4361_CALI_ERROR;
	}

	int32_t encPost =TMC4361A_Field_read_s(tmc4361,TMC4361A_ENC_POS,TMC4361A_ENC_POS_MASK,TMC4361A_ENC_POS_SHIFT);
	
	if(abs((int32_t)encPost-(int32_t)xTarget)>90000)
	{
		
		return TM4361_CALI_INVERTED;
	}

	xActual =TMC4361A_Field_read_s(tmc4361,TMC4361A_XACTUAL,TMC4361A_XACTUAL_MASK,TMC4361A_XACTUAL_SHIFT);
	
	TMC4361A_Field_Write_s(tmc4361,TMC4361A_ENC_POS,TMC4361A_ENC_POS_MASK,TMC4361A_ENC_POS_SHIFT,xActual);

	//先暂时把电流设最大
	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_BETA, TMC4361A_CL_BETA_MASK, TMC4361A_CL_BETA_SHIFT,255);
	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_GAMMA, TMC4361A_CL_GAMMA_MASK, TMC4361A_CL_GAMMA_SHIFT,255);
	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_DELTA_P_WR, TMC4361A_CL_DELTA_P_MASK, TMC4361A_CL_DELTA_P_SHIFT,65536);
	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_TOLERANCE_WR, TMC4361A_CL_TOLERANCE_MASK, TMC4361A_CL_TOLERANCE_SHIFT,40);

	TMC4361A_WriteInt32(tmc4361, TMC4361A_CURRENT_CONF, 0);//Disable current scaling 关闭电流自动调节

	volatile int32_t encPosDev;
	uint32_t devMin=0xFFFFFFFF,devMax=0;
	uint32_t mscnt=0;
	for(uint16_t i=0;i<400;i++)
	{
		mscnt=TMC4361A_Field_read(tmc4361, TMC4361A_MSCNT_RD, TMC4361A_MSCNT_MASK, TMC4361A_MSCNT_SHIFT);
		xActual =TMC4361A_Field_read_s(tmc4361,TMC4361A_XACTUAL,TMC4361A_XACTUAL_MASK,TMC4361A_XACTUAL_SHIFT);
		xTarget=xActual+384-(mscnt%256);
		TMC4361A_WriteInt32S(tmc4361, TMC4361A_X_TARGET, xTarget);//走到下个整步

		rt = tmc4361->getReached(tmc4361,200);
		if(rt==false)
		{
			return TM4361_CALI_ERROR;
		}
		encPosDev=TMC4361A_Field_read_s(tmc4361,TMC4361A_ENC_POS_DEV_RD,TMC4361A_ENC_POS_DEV_MASK,TMC4361A_ENC_POS_DEV_SHIFT);

		if(encPosDev<devMin)
		{
			devMin=encPosDev;
		}

		if(devMax>encPosDev)
		{
			devMax=encPosDev;
		}

		xTarget+=256;
		TMC4361A_WriteInt32S(tmc4361, TMC4361A_X_TARGET, xTarget);//走到一个整步
		rt = tmc4361->getReached(tmc4361,200);
		if(rt==false)
		{
			return TM4361_CALI_ERROR;
		}
	}

	uint32_t devSearch=(devMax-devMin)/2;

	int32_t calibPos=0;
	for(uint16_t i=0;i<400;i++)
	{
		xTarget-=256;//往反方向走一个整步
		TMC4361A_WriteInt32S(tmc4361, TMC4361A_X_TARGET, xTarget);//走到一个整步
		rt = tmc4361->getReached(tmc4361,200);
		if(rt==false)
		{
			return TM4361_CALI_ERROR;
		}
		encPosDev=TMC4361A_Field_read_s(tmc4361,TMC4361A_ENC_POS_DEV_RD,TMC4361A_ENC_POS_DEV_MASK,TMC4361A_ENC_POS_DEV_SHIFT);

		if(abs((int32_t)encPosDev-(int32_t)devSearch)<100)
		{
			calibPos=xTarget;
		}

	}

	xTarget=calibPos;
	TMC4361A_WriteInt32S(tmc4361, TMC4361A_X_TARGET, xTarget);
	rt = tmc4361->getReached(tmc4361,2000);
	if(rt==false)
	{
		return TM4361_CALI_ERROR;
	}
	
	TMC4361A_WriteInt32(tmc4361, TMC4361A_VMAX, 0);//速度设为0kpps
	TMC4361A_WriteInt32(tmc4361, TMC4361A_CL_VMAX_CALC_P_WR, 1000);
	TMC4361A_WriteInt32(tmc4361, TMC4361A_CL_VMAX_CALC_I_WR, 50);
	TMC4361A_WriteInt32(tmc4361, TMC4361A_PID_DV_CLIP_WR, 50000);
	TMC4361A_WriteInt32(tmc4361, TMC4361A_PID_I_CLIP_WR, 1000);

	

	mscnt=TMC4361A_Field_read(tmc4361, TMC4361A_MSCNT_RD, TMC4361A_MSCNT_MASK, TMC4361A_MSCNT_SHIFT);
	TMC4361A_Field_Write_s(tmc4361,TMC4361A_ENC_POS,TMC4361A_ENC_POS_MASK,TMC4361A_ENC_POS_SHIFT,mscnt);

	TMC4361A_WriteInt32S(tmc4361, TMC4361A_X_TARGET, 0);
	TMC4361A_WriteInt32S(tmc4361,TMC4361A_XACTUAL,0);


	return TM4361_CALI_EQUAL;
}


void TMC4361A_Set_RearDrive_Reg(TMC4361TypeDef *tmc4361,TMC4361A_REARDRIVE_REG *reg)
{
	for(int i=0;reg[i].adder!=0xFF;i++)
	{
		TMC4361A_WriteCoverInt32(tmc4361,reg[i].adder,reg[i].data);
	}
	TMC4361A_Read_Clear_Events(tmc4361);
}

uint32_t TMC4361A_Get_RearDrive_Reg(TMC4361TypeDef *tmc4361,uint8_t adder)
{
	TMC4361A_Read_Clear_Events(tmc4361);
	return TMC4361A_ReadCoverInt32(tmc4361,adder);
}



//输出引脚配置
void TMC4361A_Set_OutputPin_Reg(TMC4361TypeDef *tmc4361, TMC4361A_OUTPUT_PIN_REG reg)
{
	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_INTR_TR_PU_PD_EN_MASK, TMC4361A_INTR_TR_PU_PD_EN_SHIFT,reg.output_pin_mode);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_INTR_AS_WIRED_AND_MASK, TMC4361A_INTR_AS_WIRED_AND_SHIFT,reg.intr_as_wired_and);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_TR_AS_WIRED_AND_MASK, TMC4361A_TR_AS_WIRED_AND_SHIFT,reg.tr_as_wired_and);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_INTR_POL_MASK, TMC4361A_INTR_POL_SHIFT,reg.intr_pol);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_INVERT_POL_TARGET_REACHED_MASK, TMC4361A_INVERT_POL_TARGET_REACHED_SHIFT,!reg.target_reached);

}

TMC4361A_OUTPUT_PIN_REG TMC4361A_Get_OutputPin_Reg(TMC4361TypeDef *tmc4361)
{

	TMC4361A_OUTPUT_PIN_REG reg={0};

	reg.output_pin_mode=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_INTR_TR_PU_PD_EN_MASK, TMC4361A_INTR_TR_PU_PD_EN_SHIFT);

	reg.intr_as_wired_and=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_INTR_AS_WIRED_AND_MASK, TMC4361A_INTR_AS_WIRED_AND_SHIFT);

	reg.tr_as_wired_and=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_TR_AS_WIRED_AND_MASK, TMC4361A_TR_AS_WIRED_AND_SHIFT);

	reg.intr_pol=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_INTR_POL_MASK, TMC4361A_INTR_POL_SHIFT);

	reg.target_reached=!TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_INVERT_POL_TARGET_REACHED_MASK, TMC4361A_INVERT_POL_TARGET_REACHED_SHIFT);

	return reg;
}

//配置spi输出操作
void TMC4361A_Set_SpiOut_Reg(TMC4361TypeDef *tmc4361,TMC4361A_SPIOUT_REG reg)
{

	//使能SPI输出通信
	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_SERIAL_ENC_OUT_ENABLE_MASK, TMC4361A_SERIAL_ENC_OUT_ENABLE_SHIFT,!reg.spi_out_enable);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_DISABLE_POLLING_MASK, TMC4361A_DISABLE_POLLING_SHIFT,reg.spi_disable_polling);
	//配置SPI输出通信方案：时钟低电平长度
	TMC4361A_Field_Write(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_SPI_OUT_LOW_TIME_MASK, TMC4361A_SPI_OUT_LOW_TIME_SHIFT,reg.spi_out_low_time);
	//配置SPI输出通信方案：时钟高电平长度
	TMC4361A_Field_Write(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_SPI_OUT_HIGH_TIME_MASK, TMC4361A_SPI_OUT_HIGH_TIME_SHIFT,reg.spi_out_high_time);

	//配置SPI输出通信方案:连续查询两个数据报间隔时间因子
	TMC4361A_Field_Write(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_POLL_BLOCK_EXP_MASK, TMC4361A_POLL_BLOCK_EXP_SHIFT,reg.spi_out_poll_block_mult);

	//配置SPI输出通信方案:两次数据报间隔最短时间
	TMC4361A_Field_Write(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_SPI_OUT_BLOCK_TIME_MASK, TMC4361A_SPI_OUT_BLOCK_TIME_SHIFT,reg.spi_out_block_time);


	//配置SPI输出通信方案：覆盖数据包发送位数 对TMC芯片设为0 自动选择
	TMC4361A_Field_Write(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_COVER_DATA_LENGTH_MASK, TMC4361A_COVER_DATA_LENGTH_SHIFT,reg.spi_out_data_len);


	//配置SPI输出通信方案：设定TCM驱动器型号 TMC2130 （TMC5130通用）
	TMC4361A_Field_Write(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_SPI_OUTPUT_FORMAT_MASK, TMC4361A_SPI_OUTPUT_FORMAT_SHIFT,reg.spi_out_forma);


	TMC4361A_Field_Write(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_AUTOREPEAT_COVER_EN_MASK, TMC4361A_AUTOREPEAT_COVER_EN_SHIFT,reg.spi_auto_cover_en);

	TMC4361A_Field_Write(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_COVER_DONE_ONLY_FOR_COVER_MASK, TMC4361A_COVER_DONE_ONLY_FOR_COVER_SHIFT,reg.spi_cover_done_only_for_covers);

	TMC4361A_Field_Write(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_SCALE_VALE_TRANSFER_EN_MASK, TMC4361A_SCALE_VALE_TRANSFER_EN_SHIFT,reg.spi_val_transfer_en);

}

TMC4361A_SPIOUT_REG TMC4361A_Get_SpiOut_Reg(TMC4361TypeDef *tmc4361)
{
	TMC4361A_SPIOUT_REG  reg={0};
	//获取SPI输出通信方案：获取TCM驱动器型号 TMC2130 （TMC5130通用）
	reg.spi_out_forma=TMC4361A_Field_read(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_SPI_OUTPUT_FORMAT_MASK, TMC4361A_SPI_OUTPUT_FORMAT_SHIFT);

	reg.spi_disable_polling=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_DISABLE_POLLING_MASK, TMC4361A_DISABLE_POLLING_SHIFT);

	//获取SPI输出通信方案：覆盖数据包发送位数 对TMC芯片设为0 自动选择
	reg.spi_out_data_len=TMC4361A_Field_read(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_COVER_DATA_LENGTH_MASK, TMC4361A_COVER_DATA_LENGTH_SHIFT);
	//获取SPI输出通信方案：时钟低电平长度
	reg.spi_out_low_time=TMC4361A_Field_read(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_SPI_OUT_LOW_TIME_MASK, TMC4361A_SPI_OUT_LOW_TIME_SHIFT);
	//获取SPI输出通信方案：时钟高电平长度
	reg.spi_out_high_time=TMC4361A_Field_read(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_SPI_OUT_HIGH_TIME_MASK, TMC4361A_SPI_OUT_HIGH_TIME_SHIFT);
	//获取SPI输出通信方案:连续查询两个数据报间隔时间因子
	reg.spi_out_poll_block_mult=TMC4361A_Field_read(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_POLL_BLOCK_EXP_MASK, TMC4361A_POLL_BLOCK_EXP_SHIFT);
	//获取SPI输出通信
	reg.spi_out_enable= !TMC4361A_Field_read(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_SERIAL_ENC_OUT_ENABLE_MASK, TMC4361A_SERIAL_ENC_OUT_ENABLE_SHIFT);

	reg.spi_out_block_time=TMC4361A_Field_read(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_SPI_OUT_BLOCK_TIME_MASK, TMC4361A_SPI_OUT_BLOCK_TIME_SHIFT);

	reg.spi_auto_cover_en=TMC4361A_Field_read(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_AUTOREPEAT_COVER_EN_MASK, TMC4361A_AUTOREPEAT_COVER_EN_SHIFT);

	reg.spi_cover_done_only_for_covers=TMC4361A_Field_read(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_COVER_DONE_ONLY_FOR_COVER_MASK, TMC4361A_COVER_DONE_ONLY_FOR_COVER_SHIFT);
	reg.spi_val_transfer_en=TMC4361A_Field_read(tmc4361, TMC4361A_SPIOUT_CONF, TMC4361A_SCALE_VALE_TRANSFER_EN_MASK, TMC4361A_SCALE_VALE_TRANSFER_EN_SHIFT);

	return reg;
}


//闭环寄存器操作
void TMC4361A_Set_ClosedLoop_Reg(TMC4361TypeDef *tmc4361, TMC4361A_CLOSEDLOOP_REG reg)
{
	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_TOLERANCE_WR, TMC4361A_CL_TOLERANCE_MASK, TMC4361A_CL_TOLERANCE_SHIFT,reg.cl_tolerance);

	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_DELTA_P_WR, TMC4361A_CL_DELTA_P_MASK, TMC4361A_CL_DELTA_P_SHIFT,reg.cl_delta_p);

	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_OFFSET, TMC4361A_CL_OFFSET_MASK, TMC4361A_CL_OFFSET_SHIFT,reg.cl_offset);

	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_CYCLE_WR, TMC4361A_CL_CYCLE_MASK, TMC4361A_CL_CYCLE_SHIFT,reg.cl_cycle);

	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_BETA, TMC4361A_CL_BETA_MASK, TMC4361A_CL_BETA_SHIFT,reg.cl_beta);

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CL_CALIBRATION_EN_MASK, TMC4361A_CL_CALIBRATION_EN_SHIFT,reg.cl_cali_enable);

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_REGULATION_MODUS_MASK, TMC4361A_REGULATION_MODUS_SHIFT,reg.cl_enable);

}

TMC4361A_CLOSEDLOOP_REG TMC4361A_Get_ClosedLoop_Reg(TMC4361TypeDef *tmc4361)
{

	TMC4361A_CLOSEDLOOP_REG reg={0};

	reg.cl_tolerance=TMC4361A_Field_read(tmc4361, TMC4361A_CL_TOLERANCE_WR, TMC4361A_CL_TOLERANCE_MASK, TMC4361A_CL_TOLERANCE_SHIFT);

	reg.cl_delta_p=TMC4361A_Field_read(tmc4361, TMC4361A_CL_DELTA_P_WR, TMC4361A_CL_DELTA_P_MASK, TMC4361A_CL_DELTA_P_SHIFT);

	reg.cl_offset=TMC4361A_Field_read(tmc4361, TMC4361A_CL_OFFSET, TMC4361A_CL_OFFSET_MASK, TMC4361A_CL_OFFSET_SHIFT);

	reg.cl_cycle=TMC4361A_Field_read(tmc4361, TMC4361A_CL_CYCLE_WR, TMC4361A_CL_CYCLE_MASK, TMC4361A_CL_CYCLE_SHIFT);

	reg.cl_beta=TMC4361A_Field_read(tmc4361, TMC4361A_CL_BETA, TMC4361A_CL_BETA_MASK, TMC4361A_CL_BETA_SHIFT);

	reg.cl_cali_enable=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CL_CALIBRATION_EN_MASK, TMC4361A_CL_CALIBRATION_EN_SHIFT);
	reg.cl_enable=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_REGULATION_MODUS_MASK, TMC4361A_REGULATION_MODUS_SHIFT);

	return reg;
}

//闭环电流寄存器操作
void TMC4361A_Set_ClosedLoop_Cur_Reg(TMC4361TypeDef *tmc4361, TMC4361A_CLOSEDLOOP_CUR_REG reg)
{

	TMC4361A_Field_Write(tmc4361, TMC4361A_SCALE_VALUES, TMC4361A_CL_IMIN_MASK, TMC4361A_CL_IMIN_SHIFT,reg.cl_imin);

	TMC4361A_Field_Write(tmc4361, TMC4361A_SCALE_VALUES, TMC4361A_CL_IMAX_MASK, TMC4361A_CL_IMAX_SHIFT,reg.cl_imax);

	TMC4361A_Field_Write(tmc4361, TMC4361A_SCALE_VALUES, TMC4361A_CL_START_UP_MASK, TMC4361A_CL_START_UP_SHIFT,reg.cl_start_up);

	TMC4361A_Field_Write(tmc4361, TMC4361A_SCALE_VALUES, TMC4361A_CL_START_DN_MASK, TMC4361A_CL_START_DN_SHIFT,reg.cl_start_down);

	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_UPSCALE_DELAY, TMC4361A_CL_UPSCALE_DELAY_MASK, TMC4361A_CL_UPSCALE_DELAY_SHIFT,reg.cl_up_scale_delay);

	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_DOWNSCALE_DELAY, TMC4361A_CL_DNSCALE_DELAY_MASK, TMC4361A_CL_DNSCALE_DELAY_SHIFT,reg.cl_dn_scale_delay);

	TMC4361A_Field_Write(tmc4361, TMC4361A_STDBY_DELAY, TMC4361A_STDBY_DELAY_MASK, TMC4361A_STDBY_DELAY_SHIFT,reg.cl_stdby_delay);

	TMC4361A_Field_Write(tmc4361, TMC4361A_FREEWHEEL_DELAY, TMC4361A_FREEWHEEL_DELAY_MASK, TMC4361A_FREEWHEEL_DELAY_SHIFT,reg.cl_freewheel_delay);

	TMC4361A_Field_Write(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_HOLD_CURRENT_SCALE_EN_MASK, TMC4361A_HOLD_CURRENT_SCALE_EN_SHIFT,reg.cl_cur_hold_scale_en);
	TMC4361A_Field_Write(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_DRIVE_CURRENT_SCALE_EN_MASK, TMC4361A_DRIVE_CURRENT_SCALE_EN_SHIFT,reg.cl_cur_drive_scale_en);
	TMC4361A_Field_Write(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_BOOST_CURRENT_ON_ACC_EN_MASK, TMC4361A_BOOST_CURRENT_ON_ACC_EN_SHIFT,reg.cl_cur_boost_on_acc_scale_en);
	TMC4361A_Field_Write(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_BOOST_CURRENT_ON_DEC_EN_MASK, TMC4361A_BOOST_CURRENT_ON_DEC_EN_SHIFT,reg.cl_cur_boost_on_dec_scale_en);

	TMC4361A_Field_Write(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_BOOST_CURRENT_AFTER_START_EN_MASK, TMC4361A_BOOST_CURRENT_AFTER_START_EN_SHIFT,reg.cl_cur_boost_after_scale_en);
	TMC4361A_Field_Write(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_SEC_DRIVE_CURRENT_SCALE_EN_MASK, TMC4361A_SEC_DRIVE_CURRENT_SCALE_EN_SHIFT,reg.cl_cur_sec_scale_en);

	TMC4361A_Field_Write(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_FREEWHEELING_EN_MASK, TMC4361A_FREEWHEELING_EN_SHIFT,reg.cl_cur_freewheeling_scale_en);
	TMC4361A_Field_Write(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_CLOSED_LOOP_SCALE_EN_MASK, TMC4361A_CLOSED_LOOP_SCALE_EN_SHIFT,reg.cl_cur_scale_en);

}

TMC4361A_CLOSEDLOOP_CUR_REG TMC4361A_Get_ClosedLoop_Cur_Reg(TMC4361TypeDef *tmc4361)
{

	TMC4361A_CLOSEDLOOP_CUR_REG reg={0};

	reg.cl_imin=TMC4361A_Field_read(tmc4361, TMC4361A_SCALE_VALUES, TMC4361A_CL_IMIN_MASK, TMC4361A_CL_IMIN_SHIFT);

	reg.cl_imax=TMC4361A_Field_read(tmc4361, TMC4361A_SCALE_VALUES, TMC4361A_CL_IMAX_MASK, TMC4361A_CL_IMAX_SHIFT);

	reg.cl_start_up=TMC4361A_Field_read(tmc4361, TMC4361A_SCALE_VALUES, TMC4361A_CL_START_UP_MASK, TMC4361A_CL_START_UP_SHIFT);

	reg.cl_start_down=TMC4361A_Field_read(tmc4361, TMC4361A_SCALE_VALUES, TMC4361A_CL_START_DN_MASK, TMC4361A_CL_START_DN_SHIFT);

	reg.cl_up_scale_delay=TMC4361A_Field_read(tmc4361, TMC4361A_CL_UPSCALE_DELAY, TMC4361A_CL_UPSCALE_DELAY_MASK, TMC4361A_CL_UPSCALE_DELAY_SHIFT);

	reg.cl_dn_scale_delay=TMC4361A_Field_read(tmc4361, TMC4361A_CL_DOWNSCALE_DELAY, TMC4361A_CL_DNSCALE_DELAY_MASK, TMC4361A_CL_DNSCALE_DELAY_SHIFT);

	reg.cl_stdby_delay=TMC4361A_Field_read(tmc4361, TMC4361A_STDBY_DELAY, TMC4361A_STDBY_DELAY_MASK, TMC4361A_STDBY_DELAY_SHIFT);

	reg.cl_freewheel_delay=TMC4361A_Field_read(tmc4361, TMC4361A_FREEWHEEL_DELAY, TMC4361A_FREEWHEEL_DELAY_MASK, TMC4361A_FREEWHEEL_DELAY_SHIFT);


	reg.cl_cur_hold_scale_en=TMC4361A_Field_read(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_HOLD_CURRENT_SCALE_EN_MASK, TMC4361A_HOLD_CURRENT_SCALE_EN_SHIFT);
	reg.cl_cur_drive_scale_en=TMC4361A_Field_read(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_DRIVE_CURRENT_SCALE_EN_MASK, TMC4361A_DRIVE_CURRENT_SCALE_EN_SHIFT);
	reg.cl_cur_boost_on_acc_scale_en=TMC4361A_Field_read(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_BOOST_CURRENT_ON_ACC_EN_MASK, TMC4361A_BOOST_CURRENT_ON_ACC_EN_SHIFT);
	reg.cl_cur_boost_on_dec_scale_en=TMC4361A_Field_read(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_BOOST_CURRENT_ON_DEC_EN_MASK, TMC4361A_BOOST_CURRENT_ON_DEC_EN_SHIFT);

	reg.cl_cur_boost_after_scale_en=TMC4361A_Field_read(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_BOOST_CURRENT_AFTER_START_EN_MASK, TMC4361A_BOOST_CURRENT_AFTER_START_EN_SHIFT);
	reg.cl_cur_sec_scale_en=TMC4361A_Field_read(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_SEC_DRIVE_CURRENT_SCALE_EN_MASK, TMC4361A_SEC_DRIVE_CURRENT_SCALE_EN_SHIFT);

	reg.cl_cur_freewheeling_scale_en=TMC4361A_Field_read(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_FREEWHEELING_EN_MASK, TMC4361A_FREEWHEELING_EN_SHIFT);
	reg.cl_cur_scale_en=TMC4361A_Field_read(tmc4361, TMC4361A_CURRENT_CONF, TMC4361A_CLOSED_LOOP_SCALE_EN_MASK, TMC4361A_CLOSED_LOOP_SCALE_EN_SHIFT);

	return reg;
}

//闭环反电动势补偿寄存器操作
void TMC4361A_Set_ClosedLoop_BackEmf_Reg(TMC4361TypeDef *tmc4361, TMC4361A_CLOSEDLOOP_BACKEMF_REG reg)
{
	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_GAMMA, TMC4361A_CL_GAMMA_MASK, TMC4361A_CL_GAMMA_SHIFT,reg.cl_gamma);

	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_VMIN_EMF_WR, TMC4361A_CL_VMIN_EMF_MASK, TMC4361A_CL_VMIN_EMF_SHIFT,reg.cl_vmin);

	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_VADD_EMF, TMC4361A_CL_VADD_EMF_MASK, TMC4361A_CL_VADD_EMF_SHIFT,reg.cl_vadd);

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CL_EMF_EN_MASK, TMC4361A_CL_EMF_EN_SHIFT,reg.cl_backEmf_en);

}

TMC4361A_CLOSEDLOOP_BACKEMF_REG TMC4361A_Get_ClosedLoop_BackEmf_Reg(TMC4361TypeDef *tmc4361)
{

	TMC4361A_CLOSEDLOOP_BACKEMF_REG reg={0};

	reg.cl_gamma=TMC4361A_Field_read(tmc4361, TMC4361A_CL_GAMMA, TMC4361A_CL_GAMMA_MASK, TMC4361A_CL_GAMMA_SHIFT);

	reg.cl_vmin=TMC4361A_Field_read(tmc4361, TMC4361A_CL_VMIN_EMF_WR, TMC4361A_CL_VMIN_EMF_MASK, TMC4361A_CL_VMIN_EMF_SHIFT);

	reg.cl_vadd=TMC4361A_Field_read(tmc4361, TMC4361A_CL_VADD_EMF, TMC4361A_CL_VADD_EMF_MASK, TMC4361A_CL_VADD_EMF_SHIFT);

	reg.cl_backEmf_en=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CL_EMF_EN_MASK, TMC4361A_CL_EMF_EN_SHIFT);

	return reg;
}


//闭环速度环寄存器操作
void TMC4361A_Set_ClosedLoop_Speed_Reg(TMC4361TypeDef *tmc4361, TMC4361A_CLOSEDLOOP_SPEED_REG reg)
{
	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_VMAX_CALC_P_WR, TMC4361A_CL_VMAX_CALC_P_MASK, TMC4361A_CL_VMAX_CALC_P_SHIFT,reg.cl_vmax_calc_p);
	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_VMAX_CALC_I_WR, TMC4361A_CL_VMAX_CALC_I_MASK, TMC4361A_CL_VMAX_CALC_I_SHIFT,reg.cl_vmax_calc_i);

	TMC4361A_Field_Write(tmc4361, TMC4361A_PID_DV_CLIP_WR, TMC4361A_PID_DV_CLIP_MASK, TMC4361A_PID_DV_CLIP_SHIFT,reg.pid_dv_clip);

	TMC4361A_Field_Write(tmc4361, TMC4361A_PID_I_CLIP_WR, TMC4361A_PID_I_CLIP_MASK, TMC4361A_PID_I_CLIP_SHIFT,reg.pid_i_clip);

	TMC4361A_WriteInt32(tmc4361,TMC4361A_CL_VMAX_CALC_P_WR,reg.cl_vmax_calc_p);
	TMC4361A_WriteInt32(tmc4361,TMC4361A_CL_VMAX_CALC_I_WR,reg.cl_vmax_calc_i);
	TMC4361A_WriteInt32(tmc4361,TMC4361A_PID_DV_CLIP_WR,reg.pid_dv_clip);
	TMC4361A_WriteInt32(tmc4361,TMC4361A_PID_I_CLIP_WR,reg.pid_i_clip);

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CL_VLIMIT_EN_MASK, TMC4361A_CL_VLIMIT_EN_SHIFT,reg.cl_vlimit_en);
	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CL_VELOCITY_MODE_EN_MASK, TMC4361A_CL_VELOCITY_MODE_EN_SHIFT,reg.cl_velocity_mode_en);
}

TMC4361A_CLOSEDLOOP_SPEED_REG TMC4361A_Get_ClosedLoop_Speed_Reg(TMC4361TypeDef *tmc4361)
{

	TMC4361A_CLOSEDLOOP_SPEED_REG reg={0};

	reg.cl_vmax_calc_p=TMC4361A_Field_read(tmc4361, TMC4361A_CL_VMAX_CALC_P_WR, TMC4361A_CL_VMAX_CALC_P_MASK, TMC4361A_CL_VMAX_CALC_P_SHIFT);
	reg.cl_vmax_calc_i=TMC4361A_Field_read(tmc4361, TMC4361A_CL_VMAX_CALC_I_WR, TMC4361A_CL_VMAX_CALC_I_MASK, TMC4361A_CL_VMAX_CALC_I_SHIFT);

	reg.pid_dv_clip=TMC4361A_Field_read(tmc4361, TMC4361A_PID_DV_CLIP_WR, TMC4361A_PID_DV_CLIP_MASK, TMC4361A_PID_DV_CLIP_SHIFT);

	reg.pid_i_clip=TMC4361A_Field_read(tmc4361, TMC4361A_PID_I_CLIP_WR, TMC4361A_PID_I_CLIP_MASK, TMC4361A_PID_I_CLIP_SHIFT);

	reg.cl_vlimit_en=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CL_VLIMIT_EN_MASK, TMC4361A_CL_VLIMIT_EN_SHIFT);
	reg.cl_velocity_mode_en=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CL_VELOCITY_MODE_EN_MASK, TMC4361A_CL_VELOCITY_MODE_EN_SHIFT);

	return reg;
}



//编码器 寄存器相关操作
void TMC4361A_Set_Enc_Reg(TMC4361TypeDef *tmc4361, TMC4361A_ENC_REG reg)
{

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_VMEAN_FILTER_WR, TMC4361A_ENC_VMEAN_INT_MASK, TMC4361A_ENC_VMEAN_INT_SHIFT,reg.enc_vmean_int);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_SERIAL_ENC_IN_MODE_MASK, TMC4361A_SERIAL_ENC_IN_MODE_SHIFT,reg.enc_in_mode);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_DIFF_ENC_IN_DISABLE_MASK, TMC4361A_DIFF_ENC_IN_DISABLE_SHIFT,reg.enc_in_diff_disable);

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_INVERT_ENC_DIR_MASK, TMC4361A_INVERT_ENC_DIR_SHIFT,reg.enc_in_invert_dir);

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_RES_WR, TMC4361A_ENC_IN_RES_MASK, TMC4361A_ENC_IN_RES_SHIFT,reg.enc_in_res);

    //TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_RES_WR, TMC4361A_ENC_CONST_MASK, TMC4361A_ENC_CONST_SHIFT,reg.enc_const);

	TMC4361A_Field_Write(tmc4361, TMC4361A_MANUAL_ENC_CONST0,TMC4361A_MANUAL_ENC_CONST_MASK, TMC4361A_MANUAL_ENC_CONST_SHIFT,reg.enc_const_manual);

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_VMEAN_WAIT_WR, TMC4361A_ENC_VMEAN_WAIT_MASK, TMC4361A_ENC_VMEAN_WAIT_SHIFT,reg.enc_vmean_wait);
	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_VMEAN_FILTER_WR, TMC4361A_ENC_VMEAN_FILTER_MASK, TMC4361A_ENC_VMEAN_FILTER_SHIFT,reg.enc_vmean_filter);

	TMC4361A_WriteInt32(tmc4361,0x54,((uint32_t)reg.enc_const_manual<<31)| reg.enc_in_res);//这个寄存器需要一起写

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_POL_N_MASK, TMC4361A_POL_N_SHIFT,reg.enc_pol_n);

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_N_CHAN_SENSITIVITY_MASK, TMC4361A_N_CHAN_SENSITIVITY_SHIFT,reg.enc_n_chan_sensitivity);
	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_POL_A_FOR_N_MASK, TMC4361A_POL_A_FOR_N_SHIFT,reg.enc_pol_a_for_n);
	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_POL_B_FOR_N_MASK, TMC4361A_POL_B_FOR_N_SHIFT,reg.enc_pol_b_for_n);

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_IGNORE_AB_MASK, TMC4361A_IGNORE_AB_SHIFT,reg.enc_ignore_ab);

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CLR_LATCH_CONT_ON_N_MASK, TMC4361A_CLR_LATCH_CONT_ON_N_SHIFT,reg.enc_clr_latch_cont_on_n);
	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CLEAR_ON_N_MASK, TMC4361A_CLEAR_ON_N_SHIFT,reg.enc_clear_on_n);

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_RESET_VAL_WR, TMC4361A_ENC_RESET_VAL_MASK, TMC4361A_ENC_RESET_VAL_SHIFT,reg.enc_rest_val);



}

TMC4361A_ENC_REG TMC4361A_Get_Enc_Reg(TMC4361TypeDef *tmc4361)
{

	TMC4361A_ENC_REG reg={0};

	reg.enc_vmean_int=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_VMEAN_FILTER_WR, TMC4361A_ENC_VMEAN_INT_MASK, TMC4361A_ENC_VMEAN_INT_SHIFT);

	reg.enc_in_mode=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_SERIAL_ENC_IN_MODE_MASK, TMC4361A_SERIAL_ENC_IN_MODE_SHIFT);

	reg.enc_in_diff_disable=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_DIFF_ENC_IN_DISABLE_MASK, TMC4361A_DIFF_ENC_IN_DISABLE_SHIFT);

	reg.enc_in_invert_dir=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_INVERT_ENC_DIR_MASK, TMC4361A_INVERT_ENC_DIR_SHIFT);

	reg.enc_const=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_CONST_RD, TMC4361A_ENC_CONST_MASK, TMC4361A_ENC_CONST_SHIFT);
	
	reg.enc_in_res=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_IN_RES_WR, TMC4361A_ENC_IN_RES_MASK, TMC4361A_ENC_IN_RES_SHIFT);

	reg.enc_const_manual=TMC4361A_Field_read(tmc4361, TMC4361A_MANUAL_ENC_CONST0,TMC4361A_MANUAL_ENC_CONST_MASK, TMC4361A_MANUAL_ENC_CONST_SHIFT);

	reg.enc_vmean_wait=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_VMEAN_WAIT_WR, TMC4361A_ENC_VMEAN_WAIT_MASK, TMC4361A_ENC_VMEAN_WAIT_SHIFT);
	reg.enc_vmean_filter=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_VMEAN_FILTER_WR, TMC4361A_ENC_VMEAN_FILTER_MASK, TMC4361A_ENC_VMEAN_FILTER_SHIFT);



	reg.enc_pol_n=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_POL_N_MASK, TMC4361A_POL_N_SHIFT);

	reg.enc_n_chan_sensitivity=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_N_CHAN_SENSITIVITY_MASK, TMC4361A_N_CHAN_SENSITIVITY_SHIFT);
	reg.enc_pol_a_for_n=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_POL_A_FOR_N_MASK, TMC4361A_POL_A_FOR_N_SHIFT);
	reg.enc_pol_b_for_n=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_POL_B_FOR_N_MASK, TMC4361A_POL_B_FOR_N_SHIFT);

	reg.enc_ignore_ab=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_IGNORE_AB_MASK, TMC4361A_IGNORE_AB_SHIFT);

	reg.enc_clr_latch_cont_on_n=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CLR_LATCH_CONT_ON_N_MASK, TMC4361A_CLR_LATCH_CONT_ON_N_SHIFT);
	reg.enc_clear_on_n=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CLEAR_ON_N_MASK, TMC4361A_CLEAR_ON_N_SHIFT);

	reg.enc_rest_val=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_RESET_VAL_WR, TMC4361A_ENC_RESET_VAL_MASK, TMC4361A_ENC_RESET_VAL_SHIFT);

	reg.enc_post_dev=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_POS, TMC4361A_ENC_POS_MASK, TMC4361A_ENC_POS_SHIFT);
	reg.enc_v=TMC4361A_Field_read(tmc4361, TMC4361A_V_ENC_RD, TMC4361A_V_ENC_MASK, TMC4361A_V_ENC_SHIFT);
	reg.enc_v_mean=TMC4361A_Field_read(tmc4361, TMC4361A_V_ENC_MEAN_RD, TMC4361A_V_ENC_MEAN_MASK, TMC4361A_ENC_RESET_VAL_SHIFT);
	return reg;
}


//斜坡发生器 寄存器相关操作
void TMC4361A_Set_Ramp_Reg(TMC4361TypeDef *tmc4361, TMC4361A_RAMP_REG reg)
{
	TMC4361A_Field_Write(tmc4361, TMC4361A_STP_LENGTH_ADD, TMC4361A_STP_LENGTH_ADD_MASK, TMC4361A_STP_LENGTH_ADD_SHIFT,reg.ramp_stp_length_add);

	TMC4361A_Field_Write(tmc4361, TMC4361A_DIR_SETUP_TIME, TMC4361A_DIR_SETUP_TIME_MASK, TMC4361A_DIR_SETUP_TIME_SHIFT,reg.ramp_dir_setup_time);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_SDIN_MODE_MASK, TMC4361A_SDIN_MODE_SHIFT,reg.ramp_sdin_mode);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_STEP_INACTIVE_POL_MASK, TMC4361A_STEP_INACTIVE_POL_SHIFT,reg.ramp_step_inactive_pol);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_TOGGLE_STEP_MASK, TMC4361A_TOGGLE_STEP_SHIFT,reg.ramp_toggle_step);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_POL_DIR_IN_MASK, TMC4361A_POL_DIR_IN_SHIFT,reg.ramp_pol_dir_in);
	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_POL_DIR_OUT_MASK, TMC4361A_POL_DIR_OUT_SHIFT,reg.ramp_pol_dir_out);

    TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_SD_INDIRECT_CONTROL_MASK, TMC4361A_SD_INDIRECT_CONTROL_SHIFT,reg.ramp_sd_indirect_control);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_SDIN_MODE_MASK, TMC4361A_SDIN_MODE_SHIFT,reg.ramp_reverse_motor_dir);

	TMC4361A_Field_Write(tmc4361, TMC4361A_RAMPMODE,TMC4361A_RAMP_PROFILE_MASK, TMC4361A_RAMP_PROFILE_SHIFT,reg.ramp_profile_mode);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF,TMC4361A_STDBY_CLK_PIN_ASSIGNMENT_MASK, TMC4361A_STDBY_CLK_PIN_ASSIGNMENT_SHIFT,reg.ramp_stdby_clk_pin_assignment);


	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_DIRECT_ACC_VAL_EN_MASK, TMC4361A_DIRECT_ACC_VAL_EN_SHIFT,reg.ramp_direct_acc_val_en);
	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_DIRECT_BOW_VAL_EN_MASK, TMC4361A_DIRECT_BOW_VAL_EN_SHIFT,reg.ramp_direct_bow_val_en);

	TMC4361A_Field_Write(tmc4361, TMC4361A_AMAX, TMC4361A_SH_REG1_AMAX_MASK, TMC4361A_SH_REG1_AMAX_SHIFT,reg.ramp_amax);
	TMC4361A_Field_Write(tmc4361, TMC4361A_DMAX, TMC4361A_SH_REG2_DMAX_MASK, TMC4361A_SH_REG2_DMAX_SHIFT,reg.ramp_dmax);

	TMC4361A_Field_Write(tmc4361, TMC4361A_BOW1, TMC4361A_SH_REG3_BOW1_MASK, TMC4361A_SH_REG3_BOW1_SHIFT,reg.ramp_bow1);
	TMC4361A_Field_Write(tmc4361, TMC4361A_BOW2, TMC4361A_SH_REG4_BOW2_MASK, TMC4361A_SH_REG4_BOW2_SHIFT,reg.ramp_bow2);
	TMC4361A_Field_Write(tmc4361, TMC4361A_BOW3, TMC4361A_SH_REG5_BOW3_MASK, TMC4361A_SH_REG5_BOW3_SHIFT,reg.ramp_bow3);
	TMC4361A_Field_Write(tmc4361, TMC4361A_BOW4, TMC4361A_SH_REG6_BOW4_MASK, TMC4361A_SH_REG6_BOW4_SHIFT,reg.ramp_bow4);

	TMC4361A_Field_Write(tmc4361, TMC4361A_ASTART, TMC4361A_SH_REG3_ASTART_MASK, TMC4361A_SH_REG3_ASTART_SHIFT,reg.astart);
	TMC4361A_Field_Write(tmc4361, TMC4361A_DFINAL, TMC4361A_SH_REG4_DFINAL_MASK, TMC4361A_SH_REG4_DFINAL_SHIFT,reg.dfinal);
}

TMC4361A_RAMP_REG TMC4361A_Get_Ramp_Reg(TMC4361TypeDef *tmc4361)
{

	TMC4361A_RAMP_REG reg={0};


	reg.ramp_stp_length_add=TMC4361A_Field_read(tmc4361, TMC4361A_STP_LENGTH_ADD, TMC4361A_STP_LENGTH_ADD_MASK, TMC4361A_STP_LENGTH_ADD_SHIFT);

	reg.ramp_dir_setup_time=TMC4361A_Field_read(tmc4361, TMC4361A_DIR_SETUP_TIME, TMC4361A_DIR_SETUP_TIME_MASK, TMC4361A_DIR_SETUP_TIME_SHIFT);

    reg.ramp_sdin_mode=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_SDIN_MODE_MASK, TMC4361A_SDIN_MODE_SHIFT);
	reg.ramp_step_inactive_pol=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_STEP_INACTIVE_POL_MASK, TMC4361A_STEP_INACTIVE_POL_SHIFT);

	reg.ramp_toggle_step=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_TOGGLE_STEP_MASK, TMC4361A_TOGGLE_STEP_SHIFT);

	reg.ramp_pol_dir_in=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_POL_DIR_IN_MASK, TMC4361A_POL_DIR_IN_SHIFT);
	reg.ramp_pol_dir_out=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_POL_DIR_OUT_MASK, TMC4361A_POL_DIR_OUT_SHIFT);

	reg.ramp_sd_indirect_control=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_SD_INDIRECT_CONTROL_MASK, TMC4361A_SD_INDIRECT_CONTROL_SHIFT);

	reg.ramp_reverse_motor_dir=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_SDIN_MODE_MASK, TMC4361A_SDIN_MODE_SHIFT);

	reg.ramp_stdby_clk_pin_assignment=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF,TMC4361A_STDBY_CLK_PIN_ASSIGNMENT_MASK, TMC4361A_STDBY_CLK_PIN_ASSIGNMENT_SHIFT);

	reg.ramp_profile_mode=TMC4361A_Field_read(tmc4361, TMC4361A_RAMPMODE,TMC4361A_RAMP_PROFILE_MASK, TMC4361A_RAMP_PROFILE_SHIFT);

	reg.ramp_direct_acc_val_en=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_DIRECT_ACC_VAL_EN_MASK, TMC4361A_DIRECT_ACC_VAL_EN_SHIFT);
	reg.ramp_direct_bow_val_en=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_DIRECT_BOW_VAL_EN_MASK, TMC4361A_DIRECT_BOW_VAL_EN_SHIFT);

	reg.ramp_amax=TMC4361A_Field_read(tmc4361, TMC4361A_AMAX, TMC4361A_SH_REG1_AMAX_MASK, TMC4361A_SH_REG1_AMAX_SHIFT);
	reg.ramp_dmax=TMC4361A_Field_read(tmc4361, TMC4361A_DMAX, TMC4361A_SH_REG2_DMAX_MASK, TMC4361A_SH_REG2_DMAX_SHIFT);

	reg.ramp_bow1=TMC4361A_Field_read(tmc4361, TMC4361A_BOW1, TMC4361A_SH_REG3_BOW1_MASK, TMC4361A_SH_REG3_BOW1_SHIFT);
	reg.ramp_bow2=TMC4361A_Field_read(tmc4361, TMC4361A_BOW2, TMC4361A_SH_REG4_BOW2_MASK, TMC4361A_SH_REG4_BOW2_SHIFT);
	reg.ramp_bow3=TMC4361A_Field_read(tmc4361, TMC4361A_BOW3, TMC4361A_SH_REG5_BOW3_MASK, TMC4361A_SH_REG5_BOW3_SHIFT);
	reg.ramp_bow4=TMC4361A_Field_read(tmc4361, TMC4361A_BOW4, TMC4361A_SH_REG6_BOW4_MASK, TMC4361A_SH_REG6_BOW4_SHIFT);

	reg.astart=TMC4361A_Field_read(tmc4361, TMC4361A_ASTART, TMC4361A_SH_REG3_ASTART_MASK, TMC4361A_SH_REG3_ASTART_SHIFT);
	reg.dfinal=TMC4361A_Field_read(tmc4361, TMC4361A_DFINAL, TMC4361A_SH_REG4_DFINAL_MASK, TMC4361A_SH_REG4_DFINAL_SHIFT);
	return reg;
}

//闭环速度环寄存器操作
void TMC4361A_Set_Event_Reg(TMC4361TypeDef *tmc4361, TMC4361A_EVENT_REG reg)
{
	//TMC4361A_WriteInt32(tmc4361,TMC4361A_STATUS,reg.event_status_flags);
	TMC4361A_WriteInt32(tmc4361,TMC4361A_EVENTS,reg.event_events);
	TMC4361A_WriteInt32(tmc4361,TMC4361A_SPI_STATUS_SELECTION,reg.event_spi_status_selection);

	TMC4361A_WriteInt32(tmc4361,TMC4361A_EVENT_CLEAR_CONF,reg.event_clrar_conf);
	TMC4361A_WriteInt32(tmc4361,TMC4361A_INTR_CONF,reg.event_intr_conf);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_INTR_POL_MASK, TMC4361A_INTR_POL_SHIFT,reg.event_intr_pol);
}

TMC4361A_EVENT_REG TMC4361A_Get_Event_Reg(TMC4361TypeDef *tmc4361)
{

	TMC4361A_EVENT_REG reg={0};
	reg.event_status_flags=TMC4361A_ReadInt32(tmc4361,TMC4361A_STATUS);
	reg.event_events=TMC4361A_ReadInt32(tmc4361,TMC4361A_EVENTS);
	reg.event_spi_status_selection=TMC4361A_ReadInt32(tmc4361,TMC4361A_SPI_STATUS_SELECTION);

	reg.event_clrar_conf=TMC4361A_ReadInt32(tmc4361,TMC4361A_EVENT_CLEAR_CONF);
	reg.event_intr_conf=TMC4361A_ReadInt32(tmc4361,TMC4361A_INTR_CONF);

	reg.event_intr_pol=TMC4361A_Field_read(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_INTR_POL_MASK, TMC4361A_INTR_POL_SHIFT);

	return reg;
}

//电机参数寄存器操作
void TMC4361A_Set_Step_Reg(TMC4361TypeDef *tmc4361, TMC4361A_STEP_REG reg)
{
	TMC4361A_Field_Write(tmc4361, TMC4361A_STEP_CONF, TMC4361A_MSTEP_PER_FS_MASK, TMC4361A_MSTEP_PER_FS_SHIFT,reg.setp_per_fs);
	TMC4361A_Field_Write(tmc4361, TMC4361A_STEP_CONF, TMC4361A_FS_PER_REV_MASK, TMC4361A_FS_PER_REV_SHIFT,reg.setp_fs_per_rev);

	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_TR_TOLERANCE_WR, TMC4361A_CL_TR_TOLERANCE_MASK, TMC4361A_CL_TR_TOLERANCE_SHIFT,reg.step_tr_tolerance);
	
	
}

TMC4361A_STEP_REG TMC4361A_Get_Step_Reg(TMC4361TypeDef *tmc4361)
{

	TMC4361A_STEP_REG reg={0};

	reg.setp_per_fs=TMC4361A_Field_read(tmc4361, TMC4361A_STEP_CONF, TMC4361A_MSTEP_PER_FS_MASK, TMC4361A_MSTEP_PER_FS_SHIFT);

	reg.setp_fs_per_rev=TMC4361A_Field_read(tmc4361, TMC4361A_STEP_CONF, TMC4361A_FS_PER_REV_MASK, TMC4361A_FS_PER_REV_SHIFT);

	reg.step_tr_tolerance=TMC4361A_Field_read(tmc4361, TMC4361A_CL_TR_TOLERANCE_WR, TMC4361A_CL_TR_TOLERANCE_MASK, TMC4361A_CL_TR_TOLERANCE_SHIFT);
	return reg;
}


#define R10 0x00040001  // STP_LENGTH_ADD
#define R20 0x00000001  // RAMPMODE
static const uint32_t tmc4361_defaultRegisterResetState[TMC4361A_REGISTER_COUNT] =
{
//	0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   A,   B,   C,   D,   E,   F
	N_A, 0,   0,   0,   0,   0,   N_A, N_A, 0,   0,   N_A, N_A, 0,   0,   0,   0,   // 0x00 - 0x0F
	R10, 0,   N_A, 0,   0,   0,   0,   N_A, 0,   0,   0,   0,   N_A, 0,   0,   N_A, // 0x10 - 0x1F
	R20, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 0x20 - 0x2F
	0,   N_A, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 0x30 - 0x3F
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 0x40 - 0x4F
	0,   0,   0,   N_A, 0,   0,   N_A, N_A, N_A, 0,   0,   0,   0,   0,   0,   0,   // 0x50 - 0x5F
	0,   0,   N_A, 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   // 0x60 - 0x6F
	N_A, N_A, N_A, N_A, N_A, N_A, N_A, N_A, N_A, 0,   0,   N_A, N_A, 0,   N_A, 0    // 0x70 - 0x7F
};

// Undefine the default register values.
// This prevents warnings in case multiple TMC-API chip headers are included at once
#undef R10
#undef R20

// Register access permissions:
//     0x00: none (reserved)  保留
//     0x01: read             只读
//     0x02: write            只写
//     0x03: read/write       读写
//     0x13: read/write, separate functions/values for reading or writing
//     0x42: write, has hardware presets on reset
//     0x43: read/write, has hardware presets on reset
//     0x53: read/write, has hardware presets on reset, separate functions/values for reading or writing
static const uint8_t tmc4361_defaultRegisterAccess[TMC4361A_REGISTER_COUNT] =
{
//  0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
	0x43, 0x03, 0x03, 0x03, 0x03, 0x03, 0x43, 0x43, 0x03, 0x03, 0x43, 0x43, 0x03, 0x03, 0x13, 0x01, // 0x00 - 0x0F
	0x03, 0x03, 0x43, 0x03, 0x03, 0x03, 0x03, 0x43, 0x03, 0x03, 0x03, 0x03, 0x43, 0x03, 0x03, 0x43, // 0x10 - 0x1F
	0x03, 0x03, 0x01, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, // 0x20 - 0x2F
	0x03, 0x43, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, // 0x30 - 0x3F
	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, // 0x40 - 0x4F
	0x03, 0x13, 0x13, 0x42, 0x13, 0x02, 0x42, 0x42, 0x42, 0x03, 0x13, 0x13, 0x02, 0x13, 0x02, 0x02, // 0x50 - 0x5F
	0x02, 0x02, 0x42, 0x02, ____, 0x01, 0x01, 0x02, 0x02, 0x02, 0x01, 0x01, 0x02, 0x02, 0x01, 0x01, // 0x60 - 0x6F
	0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x01, 0x01, 0x53, 0x53, 0x02, 0x42, 0x01  // 0x70 - 0x7F
};






