#include "TMC4361.h"
#include "stdio.h"

#if 1
#define TMC_ACCESS_NONE        0x00
#define TMC_ACCESS_READ        0x01
#define TMC_ACCESS_WRITE       0x02
                            // 0x04 is currently unused
#define TMC_ACCESS_DIRTY       0x08  // Register has been written since reset -> shadow register is valid for restore
// Special Register bits
#define TMC_ACCESS_RW_SPECIAL  0x10  // Read and write are independent - different values and/or different functions
#define TMC_ACCESS_FLAGS       0x20  // Register has read or write to clear flags.
#define TMC_ACCESS_HW_PRESET   0x40  // Register has hardware presets (e.g. Factory calibrations) - do not write a default value
                            // 0x80 is currently unused
							

// Permission combinations
#define TMC_ACCESS_RW              (TMC_ACCESS_READ  | TMC_ACCESS_WRITE)        // 0x03 - Read and write
#define TMC_ACCESS_RW_SEPARATE     (TMC_ACCESS_RW    | TMC_ACCESS_RW_SPECIAL)   // 0x13 - Read and write, with separate values/functions
#define TMC_ACCESS_R_FLAGS         (TMC_ACCESS_READ  | TMC_ACCESS_FLAGS)        // 0x21 - Read, has flags (read to clear)
#define TMC_ACCESS_RW_FLAGS        (TMC_ACCESS_RW    | TMC_ACCESS_FLAGS)        // 0x23 - Read and write, has flags (read or write to clear)
#define TMC_ACCESS_W_PRESET        (TMC_ACCESS_WRITE | TMC_ACCESS_HW_PRESET)    // 0x42 - Write, has hardware preset - skipped in reset routine
#define TMC_ACCESS_RW_PRESET       (TMC_ACCESS_RW    | TMC_ACCESS_HW_PRESET)    // 0x43 - Read and write, has hardware presets - skipped in reset routine

// Helper macros
#define TMC_IS_READABLE(x)    ((x) & TMC_ACCESS_READ)
#define TMC_IS_WRITABLE(x)    ((x) & TMC_ACCESS_WRITE)
#define TMC_IS_DIRTY(x)       ((x) & TMC_ACCESS_DIRTY)

#define TMC_IS_PRESET(x)      ((x) & TMC_ACCESS_HW_PRESET)
#define TMC_IS_RESETTABLE(x)  (((x) & (TMC_ACCESS_W_PRESET)) == TMC_ACCESS_WRITE) // Write bit set, Hardware preset bit not set
#define TMC_IS_RESTORABLE(x)  (((x) & TMC_ACCESS_WRITE) && (!(x & TMC_ACCESS_HW_PRESET) || (x & TMC_ACCESS_DIRTY))) // Write bit set, if it's a hardware preset register, it needs to be dirty

// Generic mask/shift macros
#define FIELD_GET(data, mask, shift) \
	(((data) & (mask)) >> (shift))

#define FIELD_SET(data, mask, shift, value) \
	(((data) & (~(mask))) | (((value) << (shift)) & (mask)))


#define TMC4361A_FIELD_READ(tdef, address, mask, shift) \
	FIELD_GET(TMC4361A_ReadInt32(tdef, address), mask, shift)

#define TMC4361A_FIELD_WRITE(tdef, address, mask, shift, value) \
	(TMC4361A_WriteInt32(tdef, address, FIELD_SET(TMC4361A_ReadInt32(tdef, address), mask, shift, value)))

// Memory access helpers
// Force the compiler to access a location exactly once
#define ACCESS_ONCE(x) *((volatile typeof(x) *) (&x))
// Macro to remove write bit for shadow register array access
#define TMC_ADDRESS(x) ((x) & (TMC4361A_ADDRESS_MASK))

#define BYTE(value, n)    (((value) >> ((n) << 3)) & 0xFF)


// Helper define:
// Most register permission arrays are initialized with 128 values.
// In those fields its quite hard to have an easy overview of available
// registers. For that, ____ is defined to 0, since 4 underscores are
// very easy to distinguish from the 2-digit hexadecimal values.
// This way, the used registers (permission != ACCESS_NONE) are easily spotted
// amongst unused (permission == ACCESS_NONE) registers.
#define ____ 0x00

// Helper define:
// Default reset values are not used if the corresponding register has a
// hardware preset. Since this is not directly visible in the default
// register reset values array, N_A is used as an indicator for a preset
// value, where any value will be ignored anyways (N_A: not available).
#define N_A 0

static const uint32_t tmc4361_defaultRegisterResetState[TMC4361A_REGISTER_COUNT] ;
static const uint8_t tmc4361_defaultRegisterAccess[TMC4361A_REGISTER_COUNT] ;

#endif
//直接操作TMC4136寄存器函数
static inline int32_t TMC4361A_Field_read(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift);
static inline void TMC4361A_Field_Write(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift,uint32_t value);

static inline void TMC4361A_Field_Write_s(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift,int32_t value);

static void TMC4361A_WriteInt32(TMC4361TypeDef *tmc, uint8_t address, uint32_t value);

static uint32_t TMC4361A_ReadInt32(TMC4361TypeDef *tmc, uint8_t address);


static uint32_t TMC4361A_ReadCoverInt32(TMC4361TypeDef *tmc, uint8_t address);


void TMC4361A_SetRamp(TMC4361TypeDef *tmc4361,uint32_t acc,uint32_t dcc);
void TMC4361A_SetCL(TMC4361TypeDef *tmc4361,uint8_t min,uint8_t max);
//将默认寄存器 全部写入到TMC4361内部
static void TMC4361A_Register_Init(TMC4361TypeDef* tmc)
{
	
	 for(uint16_t i=0;i<TMC4361A_REGISTER_COUNT;i++)
    {

        tmc->shadowRegister[i]=tmc4361_defaultRegisterResetState[i];
    }


	tmc->targetFlag=true;
	tmc->status=0;

}

void TMC4361_Set_Cl_Offset(TMC4361TypeDef *tmc4361,int32_t offset);


void TMC4361A_CFG_Init(TMC4361TypeDef*tmc)
{
  //恢复TMC4361默认寄存器
 TMC4361A_Register_Init(tmc);
/*
*********************************************************************************************************
*                                      配置引脚输出
*********************************************************************************************************
*/	
  TMC4361A_OUTPUT_PIN_REG output_pin_reg={0};
  
  output_pin_reg.output_pin_mode=0;//设为推挽输出
  
  output_pin_reg.intr_pol=0;//设为 中断触发为低电平
  output_pin_reg.target_reached= 1;// 设为 到达触发为高电平
  
  TMC4361A_Set_OutputPin_Reg(tmc,output_pin_reg);
  output_pin_reg=TMC4361A_Get_OutputPin_Reg(tmc);
 /*
*********************************************************************************************************
*                                      配置 编码器和电机参数
*********************************************************************************************************
*/  
  TMC4361A_STEP_REG setpReg={0};
  setpReg.setp_fs_per_rev=tmc->cfg->setp_fs_per_rev;// 200;// 电机旋转一圈的全步值
  setpReg.setp_per_fs=tmc->cfg->setp_per_fs;//0;//细分256微步
  setpReg.step_tr_tolerance=256;//到达信号触发的容差
  setpReg.step_dev_tol=0xFFFFF;
  TMC4361A_Set_Step_Reg(tmc,setpReg); 
  //编码器
  TMC4361A_ENC_REG encReg={0};
  encReg.enc_const_manual=false;
  encReg.enc_in_mode =0x00;//ABN增量编码器
  encReg.enc_in_diff_disable=true;
  encReg.enc_in_invert_dir=tmc->cfg->enc_in_invert_dir;//编码器方向
  encReg.enc_in_res=tmc->cfg->enc_in_res;//1600;
  encReg.enc_vmean_wait=100;
  encReg.enc_vmean_filter=8;
  encReg.enc_vmean_int=256;//256;
  encReg.enc_clear_on_n=false;

  TMC4361A_Set_Enc_Reg(tmc,encReg);
  TMC4361A_WriteInt32(tmc, TMC4361A_ENC_VEL_ZERO_WR, 0x00FFFFFF);
/*
*********************************************************************************************************
*                                      配置 SPI输出 以及事件寄存器
*********************************************************************************************************
*/  
  //SPI输出
  TMC4361A_SPIOUT_REG spiReg={0};
  
  spiReg.spi_out_enable=true;//使能SPI输出
  spiReg.spi_cover_done_only_for_covers=false;
  spiReg.spi_auto_cover_en=false;//这里不自动发送覆盖数据包
  spiReg.spi_val_transfer_en=false;//电流传输
  spiReg.spi_disable_polling=false;//数据查询禁用
  
  spiReg.spi_out_forma=0x0D;
  spiReg.spi_out_data_len=0;
  spiReg.spi_out_high_time=4;
  spiReg.spi_out_low_time=4;
  spiReg.spi_out_block_time=8;
  spiReg.spi_out_poll_block_mult=0;//
  TMC4361A_Set_SpiOut_Reg(tmc,spiReg);



  //事件配置
  TMC4361A_EVENT_REG eventReg={0};
  eventReg.event_events=0x0;

////中断事件配置  
  eventReg.event_clrar_conf=tmc->cfg->event_intr_conf;
  eventReg.event_intr_conf=tmc->cfg->event_intr_conf;
  
  eventReg.event_intr_pol=false;
  eventReg.event_spi_status_selection=0x00;
  eventReg.event_status_flags=0x00;

  TMC4361A_Set_Event_Reg(tmc,eventReg);
  
  //设置 外部时钟频率寄存器
  //TMC4361A_Field_Write(tmc, TMC4361A_CLK_FREQ, TMC4361A_CLK_FREQ_MASK, TMC4361A_CLK_FREQ_SHIFT,24000000u);
 
 /*
*********************************************************************************************************
*                                      配置 后置电机驱动寄存器
*********************************************************************************************************
*/ 
 TMC4361A_Set_RearDrive_Reg(tmc,tmc->rear_reg);
  

/*
*********************************************************************************************************
*                                      设置 电流 和反电势补偿 ；是可选的 不是必须
*********************************************************************************************************
*/  
//电流配置

  TMC4361A_SetCL(tmc,tmc->cfg->cl_imin,tmc->cfg->cl_imax);
  
/*
*********************************************************************************************************
*                                      设置速度环
*********************************************************************************************************
*/
  TMC4361A_CLOSEDLOOP_SPEED_REG closedLoopSpeedReg ={0};
  closedLoopSpeedReg.cl_velocity_mode_en=false;//这里 会影响定位精度 只是为了强制保持速度的一个功能
  
  closedLoopSpeedReg.cl_vlimit_en=true;
// PID_I_CLIP ≤ PID_DV_CLIP / PID_I
  closedLoopSpeedReg.cl_vmax_calc_i=50;//pid_i 
  closedLoopSpeedReg.cl_vmax_calc_p=200;
  closedLoopSpeedReg.pid_dv_clip=1048576;
  closedLoopSpeedReg.pid_i_clip=1000;
  TMC4361A_Set_ClosedLoop_Speed_Reg(tmc,closedLoopSpeedReg);
  
//反电动势配置
  TMC4361A_CLOSEDLOOP_BACKEMF_REG closedLoopBackEmfReg={0};
  //开启反向电动势补偿之前，请务必开启速度追赶PID
  closedLoopBackEmfReg.cl_backEmf_en=true;
  closedLoopBackEmfReg.cl_gamma=255;

  closedLoopBackEmfReg.cl_vadd=1000000;//后续开放外部配置
  closedLoopBackEmfReg.cl_vmin=300000;
  
  TMC4361A_Set_ClosedLoop_BackEmf_Reg(tmc,closedLoopBackEmfReg);
 
/*
*********************************************************************************************************
*                                      配置闭环相关设置
*********************************************************************************************************
*/  
	TMC4361A_WriteInt32(tmc, TMC4361A_ENC_POS, 0);
	TMC4361A_WriteInt32(tmc, TMC4361A_XACTUAL, 0);
	TMC4361A_WriteInt32(tmc, TMC4361A_X_TARGET, 0);

	//TMC4361_Set_Cl_Offset(tmc,32);
	TMC4361A_SetRamp(tmc,1000,1000); 


	TMC4361A_CLOSEDLOOP_REG closedLoopReg={0};
	closedLoopReg=TMC4361A_Get_ClosedLoop_Reg(tmc);
	closedLoopReg.cl_beta=255;//最佳性能 
	//closedLoopReg.cl_offset=0x00;
	closedLoopReg.cl_tolerance=128;//255;//这个容差是 触发闭环换向角条件的容差
	closedLoopReg.cl_delta_p=65536;//1.  0
	//closedLoopReg.cl_cycle=5;
	
	closedLoopReg.cl_enable=0;//这里先不 开启闭环 通过后面单独开启

	TMC4361A_Set_ClosedLoop_Reg(tmc,closedLoopReg);
}






//向指定寄存器地址 写入四个8位数据
static void TMC4361A_WriteDatagram(TMC4361TypeDef* tmc,uint8_t address,uint8_t x1, uint8_t x2, uint8_t x3, uint8_t x4)
{
    int value;
	__align(4) uint8_t data[5] = { address | TMC4361A_WRITE_BIT, x1, x2, x3, x4 };
    
    tmc->spi.spiWR(data);

	tmc->status = data[0];//前8位是 状态数据报 

	value = ((uint32_t)x1 << 24) | ((uint32_t)x2 << 16) | (x3 << 8) | x4;

	// Write to the shadow register and mark the register dirty
	address = TMC_ADDRESS(address);
 
	if(address>=TMC4361A_REGISTER_COUNT)
		return;//防止数组越界

	tmc->shadowRegister[address] = value;

}

//写一个32位数据  最通用的 寄存器写入函数  还可以用在 字段的 写入
static void TMC4361A_WriteInt32(TMC4361TypeDef *tmc, uint8_t address, uint32_t value)
{
	TMC4361A_WriteDatagram(tmc, address, BYTE(value, 3), BYTE(value, 2), BYTE(value, 1), BYTE(value, 0));
}



static uint32_t TMC4361A_ReadInt32(TMC4361TypeDef *tmc, uint8_t address)
{
	int value;
	__align(4) uint8_t data[5]={0,0,0,0,0};

	address = TMC_ADDRESS(address);

    //判断一下 要读取的寄存器是否可读
    //如果不可读 返回影子寄存器的值
	if(!TMC_IS_READABLE(tmc4361_defaultRegisterAccess[address]))
		return tmc->shadowRegister[address];
	
	//需要连续读取两次 因为这次读是上次状态
	data[0] = address;
    tmc->spi.spiWR(data);

    data[0] = address;
	tmc->spi.spiWR(data);

	tmc->status = data[0];
	value = ((uint32_t)data[1] << 24) | ((uint32_t)data[2] << 16) | (data[3] << 8) | data[4];

	return value;
}







/*
调用一次该函数把TCM位置到达和tmc对象位置到达同步
建议 外部中断 中调用此函数
*/
void TMC4361_Target_Flush(TMC4361TypeDef *tmc)
{
	tmc->targetFlag=true;
}

/*
调用一次该函数把TCM寄存器和tmc对象事件同步
建议 外部中断 中调用此函数
*/

static uint32_t TMC4361_Event_Read(TMC4361TypeDef *tmc)
{
	TMC4361A_ReadInt32(tmc,0x0E);
	
	return TMC4361A_ReadInt32(tmc,0x0E);	
}


void TMC4361_Event_Get(TMC4361TypeDef *tmc,uint32_t *event)
{
	uint32_t event_new;
	event_new=TMC4361_Event_Read(tmc);

	TMC4361A_WriteInt32(tmc,0x0E,0xFFFFFFFF);//可以全部清除，因为已经保存了寄存器。
	
	
	*event=event_new;
}




/***********************************************************************************************************************

功能字段操作函数

************************************************************************************************************************/

//从寄存器读出一个指定的功能段 后 清楚无关的数据 便于使用
static inline int32_t TMC4361A_Field_get(uint32_t data, uint32_t mask, uint32_t shift)
{
	return (((data) & (mask)) >> (shift));
}

//向寄存器中一个指定的功能段写入数据前 计算合适的32位数据 不破坏原有的数据
static inline uint32_t TMC4361A_Field_set(uint32_t data, uint32_t mask, uint32_t shift,uint32_t value)
{
	return 	(((data) & (~(mask))) | (((value) << (shift)) & (mask)));
}

//从寄存器读出一个指定的功能段
static inline int32_t TMC4361A_Field_read(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift)
{
	return TMC4361A_Field_get(TMC4361A_ReadInt32(tdef, address), mask, shift);
}


//向寄存器中一个指定的功能段写入数据
 static inline void TMC4361A_Field_Write(TMC4361TypeDef* tdef, uint8_t address, uint32_t mask, uint32_t shift,uint32_t value)
{
	TMC4361A_WriteInt32(tdef, address, TMC4361A_Field_set(TMC4361A_ReadInt32(tdef, address), mask, shift, value));
}




//发送覆盖数据报
static bool TMC4361A_SendCoverDatagram(TMC4361TypeDef* tmc,uint8_t address,uint8_t x1, uint8_t x2, uint8_t x3, uint8_t x4)
{
	int value;
	bool rt=true;
	__align(4) uint8_t data[5] = { address | TMC4361A_WRITE_BIT, x1, x2, x3, x4 };
	__align(4) uint8_t cover_data_high[5] = {TMC4361A_COVER_HIGH_WR|TMC4361A_WRITE_BIT,0x00,0x00,0x00,data[0]};
	__align(4) uint8_t cover_data_low[5]  = {TMC4361A_COVER_LOW_WR|TMC4361A_WRITE_BIT,data[1],data[2],data[3],data[4]};

	

	//设定报文数据寄存器
    tmc->spi.spiWR(cover_data_high);//发送高40位  对于TMC5130来说是 是其驱动器的寄存器地址

    tmc->spi.spiWR(cover_data_low);//发送低40位   对于TMC5130来说是 是其的寄存器上数据

	tmc->status = cover_data_low[0];


	value = ((uint32_t)cover_data_high[1] << 24) | ((uint32_t)cover_data_high[2] << 16) | (cover_data_high[3] << 8) | cover_data_high[4];
	tmc->shadowRegister[TMC4361A_COVER_HIGH_WR] = value;

 	value = ((uint32_t)cover_data_low[1] << 24) | ((uint32_t)cover_data_low[2] << 16) | (cover_data_low[3] << 8) | cover_data_low[4];
	tmc->shadowRegister[TMC4361A_COVER_LOW_WR] = value;

	return rt;
}


static bool TMC4361A_WriteCoverInt32(TMC4361TypeDef *tmc, uint8_t address, uint32_t value)
{
	return TMC4361A_SendCoverDatagram(tmc, address, BYTE(value, 3), BYTE(value, 2), BYTE(value, 1), BYTE(value, 0));
}

//接收覆盖数据报
static uint32_t TMC4361A_ReadCoverInt32(TMC4361TypeDef *tmc, uint8_t address)
{
	int value;
	__align(4) uint8_t data[5];

	
	__align(4) uint8_t cover_data_high[5]={TMC4361A_COVER_DRV_HIGH_RD,0x00,0x00,0x00,0x00};
	__align(4) uint8_t cover_data_low[5]={TMC4361A_COVER_DRV_LOW_RD,0x00,0x00,0x00,0x00};
	
	
	//先通过写入数据发送报寄存器 发送给 电机驱动 想要读取的地址
	address = TMC_ADDRESS(address);
	data[0] = address;

	//需要发送两次 因为这次发送接收的是上次的数据
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

	int32_t vmax;
	int32_t rt;
	

	vmax=abs(value);

	
	if(vmax>0x3FFFFF)
	{
		printf("input Vmax is %d, it's too big\r\n",value);
		
		if(value>=0)
		{
			value= 4000000;
		}
		else
		{
			value= -4000000;
		}
	}
	
	
	rt= (value<<8);
	//printf("input Vmax=%x out Vmax=%x\r\n",value,rt);
	return (rt);
	
}


static void TMC4361A_Rotate(TMC4361TypeDef *tmc4361, int32_t velocity)
{
	TMC4361A_Field_Write(tmc4361, TMC4361A_RAMPMODE,TMC4361A_OPERATION_MODE_MASK, TMC4361A_OPERATION_MODE_SHIFT,0);

	//TMC4361A_WriteInt32(tmc4361, TMC4361A_RAMPMODE, 0x02);
	TMC4361A_WriteInt32(tmc4361, TMC4361A_VMAX, TMC4361A_DiscardVelocityDecimals(velocity));
	
}

//速度模式 右转
void TMC4361A_Right(TMC4361TypeDef *tmc4361, int32_t velocity)
{
	TMC4361A_Rotate(tmc4361, velocity);
	tmc4361->targetFlag=false;
}

//速度模式 左转
void TMC4361A_Left(TMC4361TypeDef *tmc4361, int32_t velocity)
{
	TMC4361A_Rotate(tmc4361, -velocity);
	tmc4361->targetFlag=false;
}

//停止转动
void TMC4361A_Stop(TMC4361TypeDef *tmc4361)
{
	TMC4361A_WriteInt32(tmc4361, TMC4361A_VMAX, 0);
	tmc4361->targetFlag=true;
}

//读取 内部位置（电机位置）
int32_t TMC4361_GetPosition(TMC4361TypeDef *tmc4361)
{
	return TMC4361A_ReadInt32(tmc4361,TMC4361A_XACTUAL);
}


//读取 目标位置（仅在位置模式下）
int32_t TMC4361_GetTargetPosition(TMC4361TypeDef *tmc4361)
{

	return TMC4361A_ReadInt32(tmc4361,TMC4361A_X_TARGET);
	
}


//读取 外部位置 （编码器位置）
int32_t TMC4361_GetEncPosition(TMC4361TypeDef *tmc4361)
{
	return TMC4361A_ReadInt32(tmc4361,TMC4361A_ENC_POS);
}

int32_t TMC4361_GetEnc_Speed(TMC4361TypeDef *tmc4361)
{
	return TMC4361A_ReadInt32(tmc4361,TMC4361A_V_ENC_MEAN_RD);
}


//关闭 闭环
void TMC4361_CloseCL(TMC4361TypeDef *tmc4361)
{
	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_REGULATION_MODUS_MASK, TMC4361A_REGULATION_MODUS_SHIFT,0);
}

//打开 闭环
void TMC4361_OpenCL(TMC4361TypeDef *tmc4361)
{
	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_REGULATION_MODUS_MASK, TMC4361A_REGULATION_MODUS_SHIFT,1);
}

void TMC4361_ClearEncAndPosition(TMC4361TypeDef *tmc4361)
{
	TMC4361A_WriteInt32(tmc4361, TMC4361A_VMAX, 0);
	TMC4361A_WriteInt32(tmc4361, TMC4361A_ENC_POS, 0);
	TMC4361A_WriteInt32(tmc4361, TMC4361A_XACTUAL, 0);
	TMC4361A_WriteInt32(tmc4361, TMC4361A_X_TARGET, 0);
}

//软件复位
void TMC4361_SoftRest(TMC4361TypeDef *tmc4361)
{
	TMC4361A_Field_Write(tmc4361, TMC4361A_RESET_REG, TMC4361A_RESET_REG_MASK, TMC4361A_RESET_REG_SHIFT,0x525354);
}

//位置模式 绝对移动
void TMC4361A_MoveTo(TMC4361TypeDef *tmc4361, int32_t position, int32_t velocityMax)
{
	TMC4361A_Field_Write(tmc4361, TMC4361A_RAMPMODE,TMC4361A_OPERATION_MODE_MASK, TMC4361A_OPERATION_MODE_SHIFT,1);
	TMC4361A_WriteInt32(tmc4361, TMC4361A_VMAX, TMC4361A_DiscardVelocityDecimals(velocityMax));
	TMC4361A_WriteInt32(tmc4361, TMC4361A_X_TARGET, position);
	tmc4361->targetFlag=false;
}

//位置模式 相对移动
// The function will write the absolute target position to *ticks
void TMC4361A_MoveBy(TMC4361TypeDef *tmc4361, int32_t *ticks, int32_t velocityMax)
{
	// determine actual position and add numbers of ticks to move
	
	if(*ticks>0)
	{
		velocityMax=abs(velocityMax);
	}
	else
	{
		velocityMax=abs(velocityMax)*-1;
	}
	
	*ticks += TMC4361A_ReadInt32(tmc4361, TMC4361A_XACTUAL);

	TMC4361A_MoveTo(tmc4361, *ticks, velocityMax);
}

void TMC4361A_MoveToNextFullstep(TMC4361TypeDef *tmc4361)
{
	int32_t stepCount;
	
	TMC4361A_WriteInt32(tmc4361, TMC4361A_CURRENT_CONF, 0);//Disable current scaling 关闭电流自动调节
	
    TMC4361A_WriteInt32(tmc4361, TMC4361A_RAMPMODE, 0x06);
	TMC4361A_WriteInt32(tmc4361, TMC4361A_VMAX, TMC4361A_DiscardVelocityDecimals(10));
	
	stepCount = TMC4361A_Field_read(tmc4361, TMC4361A_MSCNT_RD, TMC4361A_MSCNT_MASK, TMC4361A_MSCNT_SHIFT);
	
	stepCount = stepCount % 256;
	
	stepCount = 128 - stepCount;
	
	if(stepCount!=0)
	{
		stepCount += TMC4361A_ReadInt32(tmc4361, TMC4361A_XACTUAL);
		TMC4361A_WriteInt32(tmc4361, TMC4361A_X_TARGET, stepCount);
		tmc4361->targetFlag=false;		
	}
}

void TMC4361_SimpleCali_Start(TMC4361TypeDef *tmc4361)
{
	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CL_CALIBRATION_EN_MASK, TMC4361A_CL_CALIBRATION_EN_SHIFT,1);
}
int32_t TMC4361_SimpleCali_End(TMC4361TypeDef *tmc4361)
{
	int32_t rt=0;
	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CL_CALIBRATION_EN_MASK, TMC4361A_CL_CALIBRATION_EN_SHIFT,0);
	rt=TMC4361A_ReadInt32(tmc4361,TMC4361A_CL_OFFSET);
	
	return rt;
}

void TMC4361A_SetCL(TMC4361TypeDef *tmc4361,uint8_t min,uint8_t max)
{
  TMC4361A_CLOSEDLOOP_CUR_REG closeLoopCurReg={0};
  closeLoopCurReg.cl_cur_boost_after_scale_en=false;
  closeLoopCurReg.cl_cur_boost_on_acc_scale_en=false;
  closeLoopCurReg.cl_cur_boost_on_dec_scale_en=false;
  closeLoopCurReg.cl_cur_freewheeling_scale_en=false;
  closeLoopCurReg.cl_cur_hold_scale_en=false;
  closeLoopCurReg.cl_cur_sec_scale_en=false;
  closeLoopCurReg.cl_cur_drive_scale_en=false;
  closeLoopCurReg.cl_cur_scale_en=true;//如果使能 以上6个驱动器自动设置
  closeLoopCurReg.cl_imin=min;
  closeLoopCurReg.cl_imax=max;
  closeLoopCurReg.cl_start_up=50;
  closeLoopCurReg.cl_start_down=255;//设为0 自动加载cl——beat的值
  closeLoopCurReg.cl_up_scale_delay=4096;
  closeLoopCurReg.cl_dn_scale_delay=1048576;
  closeLoopCurReg.cl_stdby_delay=2000;//待机间隔
  TMC4361A_Set_ClosedLoop_Cur_Reg(tmc4361,closeLoopCurReg);	
  
}

void TMC4361A_SetRamp(TMC4361TypeDef *tmc4361,uint32_t acc,uint32_t dcc)
{

  TMC4361A_RAMP_REG rampReg={0};
  rampReg=TMC4361A_Get_Ramp_Reg(tmc4361);
  rampReg.ramp_dir_setup_time=6;
  rampReg.ramp_stp_length_add=4;//20;
  rampReg.ramp_profile_mode=1;//T
  rampReg.ramp_sd_indirect_control=true;
  
  rampReg.ramp_sdin_mode=0;//内部步进控制 使用内部斜坡发生器
  rampReg.ramp_sdin_mode=2;//内部步进控制 使用内部斜坡发生器
  
  rampReg.ramp_stdby_clk_pin_assignment=3;//STDBY_CLK 待机引脚信号。//该引脚没有使用
  rampReg.ramp_pol_dir_out=false;
  rampReg.ramp_pol_dir_in=false;
  rampReg.ramp_reverse_motor_dir=tmc4361->cfg->ramp_reverse_motor_dir;//电机内部方向
  rampReg.ramp_step_inactive_pol=false;//false;
  
  rampReg.ramp_toggle_step=false;
  
  
  rampReg.ramp_direct_acc_val_en=1;
  rampReg.ramp_direct_bow_val_en=1;
  
  rampReg.astart=500;
  rampReg.dfinal=2000;
  
  if(acc>4000000)
	  acc=4000000;
  if(dcc>4000000)
	  dcc=4000000;
  
  if(acc<=500)
	  acc=500;
  if(dcc<=500)
	  dcc=500;

//  rampReg.ramp_bow1=acc;
//  rampReg.ramp_bow2=acc;
//  
//  rampReg.ramp_bow3=dcc;
//  rampReg.ramp_bow4=dcc;
  
  rampReg.ramp_amax=acc;
  rampReg.ramp_dmax=dcc;
  
  TMC4361A_Set_Ramp_Reg(tmc4361,rampReg);	
}

void TMC4361A_SetRamp_advanced(TMC4361TypeDef *tmc4361,uint32_t acc,uint32_t dcc,uint32_t b1,uint32_t b2,uint32_t b3,uint32_t b4)
{
  TMC4361A_RAMP_REG rampReg={0};
  rampReg=TMC4361A_Get_Ramp_Reg(tmc4361);
  rampReg.ramp_dir_setup_time=6;
  rampReg.ramp_stp_length_add=4;//20;  
  

  
  rampReg.ramp_sd_indirect_control=true;
  
  rampReg.ramp_sdin_mode=0;//内部步进控制 使用内部斜坡发生器
  rampReg.ramp_sdin_mode=2;//内部步进控制 使用内部斜坡发生器
  
  rampReg.ramp_stdby_clk_pin_assignment=3;//STDBY_CLK 待机引脚信号。//该引脚没有使用
  rampReg.ramp_pol_dir_out=false;
  rampReg.ramp_pol_dir_in=false;
  rampReg.ramp_reverse_motor_dir=tmc4361->cfg->ramp_reverse_motor_dir;//电机内部方向
  rampReg.ramp_step_inactive_pol=false;//false;
  
  rampReg.ramp_toggle_step=false;
  
  
  rampReg.ramp_direct_acc_val_en=1;
  rampReg.ramp_direct_bow_val_en=1;
  
  rampReg.astart=10;
  rampReg.dfinal=1000;
  
  if(acc>4000000)
	  acc=4000000;
  if(dcc>4000000)
	  dcc=4000000;
  
  if(acc<=500)
	  acc=500;
  if(dcc<=500)
	  dcc=500;
  
  rampReg.ramp_amax=acc;
  rampReg.ramp_dmax=dcc;
  
  if(b1==0&&b2==0&&b3==0&&b4==0)
  {
	  //printf("Ramp 0 \r\n");
	  rampReg.ramp_profile_mode=1;//匀加速 T
  }
  else
  {
	  //printf("Ramp 2 \r\n");
	  rampReg.ramp_profile_mode=2;//2;//s
	  rampReg.ramp_bow1=b1;
	  rampReg.ramp_bow2=b2;
	  
	  rampReg.ramp_bow3=b3;
	  rampReg.ramp_bow4=b4;
  }
  
  TMC4361A_Set_Ramp_Reg(tmc4361,rampReg);	
}

//设定 ENC_POS 和 XACTUAL 之间的偏移
void TMC4361_Set_Cl_Offset(TMC4361TypeDef *tmc4361,int32_t offset)
{
	
	TMC4361A_WriteInt32(tmc4361, TMC4361A_CL_OFFSET,offset);
	printf("writr offset =%d,should is =%d\r\n",TMC4361A_ReadInt32(tmc4361,TMC4361A_CL_OFFSET),offset);
}

bool TMC4361A_Set_RearDrive_Reg(TMC4361TypeDef *tmc4361,const TMC4361A_REARDRIVE_REG *reg)
{
	uint16_t t=5000;


	bool rt=true;
	for(int i=0;reg[i].adder!=0xFF;i++)
	{
		if(TMC4361A_WriteCoverInt32(tmc4361,reg[i].adder,reg[i].data)==false)
		{
			//printf("在第%d次失败\r\n",i);
			rt= false;
		}
		else
		{
			//printf("成功%d次\r\n",i);	
		}
	while(t--);//稍微延时一下
	t=5000;
	}
	

	
	return rt;
}

uint32_t TMC4361A_Get_RearDrive_Reg(TMC4361TypeDef *tmc4361,uint8_t adder)
{
	
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
	//TMC4361A_Field_Write(tmc4361, TMC4361A_CL_TOLERANCE_WR, TMC4361A_CL_TOLERANCE_MASK, TMC4361A_CL_TOLERANCE_SHIFT,reg.cl_tolerance);

	TMC4361A_WriteInt32(tmc4361, TMC4361A_CL_TOLERANCE_WR,reg.cl_tolerance);
	
	//TMC4361A_Field_Write(tmc4361, TMC4361A_CL_DELTA_P_WR, TMC4361A_CL_DELTA_P_MASK, TMC4361A_CL_DELTA_P_SHIFT,reg.cl_delta_p);

	TMC4361A_WriteInt32(tmc4361, TMC4361A_CL_DELTA_P_WR,reg.cl_delta_p);
	
	//TMC4361A_WriteInt32(tmc4361, TMC4361A_CL_OFFSET,reg.cl_offset);
	
	//TMC4361A_Field_Write(tmc4361, TMC4361A_CL_CYCLE_WR, TMC4361A_CL_CYCLE_MASK, TMC4361A_CL_CYCLE_SHIFT,reg.cl_cycle);

	TMC4361A_Field_Write(tmc4361, TMC4361A_CL_BETA, TMC4361A_CL_BETA_MASK, TMC4361A_CL_BETA_SHIFT,reg.cl_beta);

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_CL_CALIBRATION_EN_MASK, TMC4361A_CL_CALIBRATION_EN_SHIFT,reg.cl_cali_enable);

	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_CONF, TMC4361A_REGULATION_MODUS_MASK, TMC4361A_REGULATION_MODUS_SHIFT,reg.cl_enable);

}


TMC4361A_CLOSEDLOOP_REG TMC4361A_Get_ClosedLoop_Reg(TMC4361TypeDef *tmc4361)
{

	TMC4361A_CLOSEDLOOP_REG reg={0};

//	reg.cl_tolerance=TMC4361A_Field_read(tmc4361, TMC4361A_CL_TOLERANCE_WR, TMC4361A_CL_TOLERANCE_MASK, TMC4361A_CL_TOLERANCE_SHIFT);
//	reg.cl_delta_p=TMC4361A_Field_read(tmc4361, TMC4361A_CL_DELTA_P_WR, TMC4361A_CL_DELTA_P_MASK, TMC4361A_CL_DELTA_P_SHIFT);
//	
	reg.cl_tolerance=TMC4361A_ReadInt32(tmc4361, TMC4361A_CL_TOLERANCE_WR);
	reg.cl_delta_p=TMC4361A_ReadInt32(tmc4361, TMC4361A_CL_DELTA_P_WR);

	//reg.cl_offset=TMC4361A_ReadInt32(tmc4361, TMC4361A_CL_OFFSET);
	
	//reg.cl_offset=TMC4361A_Field_read_s(tmc4361, TMC4361A_CL_OFFSET, TMC4361A_CL_OFFSET_MASK, TMC4361A_CL_OFFSET_SHIFT);

	
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

	//TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_IN_RES_WR, TMC4361A_ENC_IN_RES_MASK, TMC4361A_ENC_IN_RES_SHIFT,reg.enc_in_res);

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

//事件寄存器操作 写
void TMC4361A_Set_Event_Reg(TMC4361TypeDef *tmc4361, TMC4361A_EVENT_REG reg)
{
	//TMC4361A_WriteInt32(tmc4361,TMC4361A_STATUS,reg.event_status_flags);
	
	TMC4361A_WriteInt32(tmc4361,TMC4361A_EVENTS,reg.event_events);
	TMC4361A_WriteInt32(tmc4361,TMC4361A_SPI_STATUS_SELECTION,reg.event_spi_status_selection);

	TMC4361A_WriteInt32(tmc4361,TMC4361A_EVENT_CLEAR_CONF,reg.event_clrar_conf);
	TMC4361A_WriteInt32(tmc4361,TMC4361A_INTR_CONF,reg.event_intr_conf);

	TMC4361A_Field_Write(tmc4361, TMC4361A_GENERAL_CONF, TMC4361A_INTR_POL_MASK, TMC4361A_INTR_POL_SHIFT,reg.event_intr_pol);

	//测试发现 在每次重新配置 事件寄存器后 都会立即触发 中断 此时事件寄存器中的值为 0xc200004f 需要将其清空
	//if(TMC4361_Event_Pend(tmc4361,32,0xFF)==true)
	{
		TMC4361A_WriteInt32(tmc4361,0x0E,0xFFFFFFFF);
		TMC4361A_WriteInt32(tmc4361,0x0E,0xFFFFFFFF);
	
	}


}

//事件寄存器操作 读
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

	TMC4361A_WriteInt32(tmc4361, TMC4361A_CL_TR_TOLERANCE_WR,reg.step_tr_tolerance);
	TMC4361A_Field_Write(tmc4361, TMC4361A_ENC_POS_DEV_TOL_WR, TMC4361A_ENC_POS_DEV_TOL_MASK, TMC4361A_ENC_POS_DEV_TOL_SHIFT,reg.step_dev_tol);	
}

TMC4361A_STEP_REG TMC4361A_Get_Step_Reg(TMC4361TypeDef *tmc4361)
{

	TMC4361A_STEP_REG reg={0};

	reg.setp_per_fs=TMC4361A_Field_read(tmc4361, TMC4361A_STEP_CONF, TMC4361A_MSTEP_PER_FS_MASK, TMC4361A_MSTEP_PER_FS_SHIFT);

	reg.setp_fs_per_rev=TMC4361A_Field_read(tmc4361, TMC4361A_STEP_CONF, TMC4361A_FS_PER_REV_MASK, TMC4361A_FS_PER_REV_SHIFT);

	reg.step_tr_tolerance=TMC4361A_ReadInt32(tmc4361, TMC4361A_CL_TR_TOLERANCE_WR);
	reg.step_dev_tol=TMC4361A_Field_read(tmc4361, TMC4361A_ENC_POS_DEV_TOL_WR, TMC4361A_ENC_POS_DEV_TOL_MASK, TMC4361A_ENC_POS_DEV_TOL_SHIFT);	

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


