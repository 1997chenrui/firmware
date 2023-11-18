/*
 * TMC4361.h
 *
 *  Created on: 18.07.2017
 *      Author: LK
 */

#ifndef TMC_IC_TMC4361A_H_
#define TMC_IC_TMC4361A_H_

#include "stm32f4xx_hal.h"
#include "math.h"
#include "TMC4361A_Register.h"//TMC4361寄存器宏定义
#include "TMC4361A_Fields.h"//TMC4361常用字段宏定义
#include "stdbool.h"
//寄存器的数量
#define TMC4361A_REGISTER_COUNT   8*16

#define TMC4361A_MOTORS           1
#define TMC4361A_WRITE_BIT        0x80
#define TMC4361A_ADDRESS_MASK     0x7F
#define TMC4361A_MAX_VELOCITY     (uint32_t)4294967295uL
#define TMC4361A_MAX_ACCELERATION (uint32_t)16777215uL

#define TMC4361A_COVER_DONE (1<<25)

#define TMC4361A_RAMP_HOLD      0
#define TMC4361A_RAMP_TRAPEZ    1
#define TMC4361A_RAMP_SSHAPE    2
#define TMC4361A_RAMP_POSITION  4
#define TMC4361A_RAMP_HOLD      0

// Register access bits
/* Lower nibble is used for read/write, higher nibble is used for
 * special case registers. This makes it easy to identify the read/write
 * part of the permissions in a hexadecimal permission number.
 * The dirty bit will only ever be set at runtime, so we keep the easily
 * readable lower nibble.
 */
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




//typedef struct TMC4361TypeDef TMC4361TypeDef;

typedef void (* SpiWRFunc)(uint8_t *);


typedef struct 
{
    SPI_HandleTypeDef *spiHd;
   
	SpiWRFunc spiWR;

} TMC4361SpiInterface;

// States of a configuration
typedef enum {
	CONFIG_READY,
	CONFIG_RESET,
	CONFIG_RESTORE
} ConfigState;

typedef enum
{
	ALL,
	PART,
	
}TMCRegisterKind;
typedef struct
{
	bool coverFlag;
	TMCRegisterKind  cfgMethod;
	uint8_t address;
	uint32_t mask;
	uint32_t shift;
	uint32_t value;
} TMCRegisterConstant;

typedef struct TMC4361TypeDef
{
    TMC4361SpiInterface spi;

	bool (*getInter)(struct TMC4361TypeDef *,uint16_t,uint32_t);
	bool (*getReached)(struct TMC4361TypeDef *,uint16_t);

	bool intrPol;
	bool targetPol;
    //int32_t registerRestState[TMC4361A_REGISTER_COUNT];//重新上电后的初始状态

    int32_t shadowRegister[TMC4361A_REGISTER_COUNT];//当前寄存器状态
    uint8_t registerAccessRights[TMC4361A_REGISTER_COUNT];//寄存器对应的操作权限
    //ConfigState state;//配置的状态

    uint8_t status;//最新一次sip返回数据前8位 状态位
    
}TMC4361TypeDef;

typedef bool (*TMC4361GetInter)(TMC4361TypeDef *,uint16_t,uint32_t (*) (TMC4361TypeDef *));
typedef bool (*TMC4361GetReached)(bool,uint16_t);

typedef enum
{
	TM4361_CALI_EQUAL,
	TM4361_CALI_INVERTED,
	TM4361_CALI_ERROR

}TMC4361_CALI_RT;

void TMC4361A_Right(TMC4361TypeDef *tmc4361, int32_t velocity);

void TMC4361A_Left(TMC4361TypeDef *tmc4361, int32_t velocity);

void TMC4361A_Stop(TMC4361TypeDef *tmc4361);

void TMC4361A_MoveTo(TMC4361TypeDef *tmc4361, int32_t position, uint32_t velocityMax);

void TMC4361A_MoveBy(TMC4361TypeDef *tmc4361, int32_t *ticks, uint32_t velocityMax);

void TMC4361A_Init(TMC4361TypeDef* tmc);

uint32_t TMC4361_Event_Read(TMC4361TypeDef *tmc);

uint32_t TMC4361_Event_Clear(TMC4361TypeDef *tmc,uint32_t event);



TMC4361_CALI_RT TMC4361_Cali(TMC4361TypeDef *tmc4361);

TMC4361_CALI_RT TMC4361_SimpleCali_Start(TMC4361TypeDef *tmc4361);

int32_t TMC4361_SimpleCali_End(TMC4361TypeDef *tmc4361);

int32_t TMC4361_GetPosition(TMC4361TypeDef *tmc4361);
int32_t TMC4361_GetTargetPosition(TMC4361TypeDef *tmc4361);
int32_t TMC4361_GetEncPosition(TMC4361TypeDef *tmc4361);


typedef struct 
{
	uint8_t adder;
	uint32_t data;
}TMC4361A_REARDRIVE_REG;

typedef struct 
{
	bool output_pin_mode;//0 pp 1 od ;intr_tr_pu_pd_en
	bool intr_as_wired_and;//如果 intr_tr_pu_pd_en = 1， 0: INTR 输出线或 1:INTR 输出线与。
	bool tr_as_wired_and;//如果 intr_tr_pu_pd_en = 1，   0: TARGET _ REACHED 输出线或  1:TARGET _ REACHED 输出线与

	bool intr_pol; // 0 ：intr=0 表示中断发生  1： intr=1 表示中断发生
	bool target_reached; // 0 ：intr=1 表示到达  1： intr=0 表示到达
}TMC4361A_OUTPUT_PIN_REG;

typedef struct 
{
	bool spi_out_enable;//spi输出使能
	bool spi_val_transfer_en;//使能电流调节值传输到驱动器
	bool spi_auto_cover_en;//自动覆盖数据报 电流配置
	bool spi_cover_done_only_for_covers;//如果使能，则 覆盖事件 忽略电流数据
	bool spi_disable_polling;//是否禁用 数据查询
	uint8_t  spi_out_data_len;//数据报覆盖长度
	uint8_t  spi_out_forma;//驱动芯片类型 0c/sd模式 0d spi模式
	uint8_t  spi_out_block_time;//最后一个 SPI 输出数据报之后不发送新数据报的最短时间。

	uint16_t spi_out_low_time;//spi信号低电平时间
	uint16_t spi_out_high_time;//
	uint16_t spi_out_poll_block_mult;//

}TMC4361A_SPIOUT_REG;//spi输出寄存器设置



typedef struct 
{
	uint32_t cl_tolerance;//位置偏差容差范围
	uint32_t cl_delta_p;//PID=cl_delta_p/65536
	uint32_t cl_offset;//校准过程中内部和外部位置之间的基本偏移值 如果有预先验证的固定偏移值。则可以直接写该参数
	uint32_t cl_cycle;//编码器的两个连续调节周期之间的以时钟周期数为单位的延迟时间 闭环控制周期

	bool cl_enable;//闭环使能
	bool cl_cali_enable;//定标使能
	uint8_t  cl_beta;//补偿偏差 ENC_POS_DEV 的最大换向角

}TMC4361A_CLOSEDLOOP_REG;//闭环操作

typedef struct 
{
	bool cl_cur_scale_en;//闭环电流调节使能 总体使能

	bool cl_cur_hold_scale_en;//闭环电流静止调节使能
	bool cl_cur_drive_scale_en;//运动期间使能驱动电流的调节

	bool cl_cur_boost_on_acc_scale_en;//闭环电流调节使能 RAMP_STATE = b’01 (加速度斜坡)，使能上升电流的调节。
	bool cl_cur_boost_on_dec_scale_en;//闭环电流调节使能 如果 RAMP_STATE = b’10 (减速度斜坡)，使能上升电流的调节。
	bool cl_cur_boost_after_scale_en;//闭环电流调节使能 VACTUAL = 0 且新斜坡开始，则为临时上升电流。
	bool cl_cur_sec_scale_en;//闭环电流 调节使能 VACTUAL > VDRV_SCALE_LIMIT 时，使能第二个驱动电流的调节
	bool cl_cur_freewheeling_scale_en;//闭环电流 飞轮调节使能

	uint8_t cl_imin;//闭环最小调节值
	uint8_t cl_imax;//闭环最大调节值
	uint8_t cl_start_up;//增大电流阈值
	uint8_t cl_start_down;//减少电流阈值 设置为 0 将 CL_BETA 自动分配给该参数

	uint32_t cl_up_scale_delay;//增大电流过程的平滑度
	uint32_t cl_dn_scale_delay;//减少电流过程的平滑度

	uint32_t cl_stdby_delay;//斜坡停止到待机有效之间的延迟时间[时钟周期数]。

	//uint8_t  cl_hold_scale_val;//待机最大电流 开环 闭环时cl_start_down

	uint32_t cl_freewheel_delay;//待机阶段有效到飞轮有效之间的延迟时间[时钟周期数]。

}TMC4361A_CLOSEDLOOP_CUR_REG;//闭环电流调节


typedef struct 
{
	bool cl_backEmf_en;//闭环反电动势调节调节 使能
	uint8_t cl_gamma;//gamma 最大平衡角度
	uint32_t cl_vmin;//
	uint32_t cl_vadd;//

}TMC4361A_CLOSEDLOOP_BACKEMF_REG;//闭环反电动势补偿


typedef struct 
{
	bool cl_vlimit_en;//使能追赶速度Pi调节器
	bool cl_velocity_mode_en;//使能闭环速度模式  如果 |ENC_POS_DEV| > 768, XACTUAL 相应调整

	uint32_t cl_vmax_calc_p;//用于控制最大追赶速度的 PI 调节器中的比例 P 参数。
	uint32_t cl_vmax_calc_i;//用于控制最大追赶速度的 PI 调节器中的比例 I 参数。

	uint32_t pid_dv_clip;//设置 PID_DV_CLIP 可以避免大的速度变化；也是与最大速度 VMAX 的速度偏差限制
	uint32_t pid_i_clip;//该参数与 PID_DV_CLIP 一起使用，以限制误差补偿的速度

}TMC4361A_CLOSEDLOOP_SPEED_REG;//闭环速度调节 



typedef struct 
{
	uint8_t enc_in_mode;//编码器类型 

	bool enc_in_diff_disable;//是否禁用数字差分
	bool enc_in_invert_dir;//是否翻转编码器方向

	bool enc_const_manual;//0 自动计算 enc_const ; 1:enc_const=enc_in_res

	uint32_t enc_const;//编码器常数值
	uint32_t enc_in_res;//编码器每圈的步进数

	uint32_t enc_post_dev;//XACTUAL 和 ENC_POS 之间的偏差 实时更新

	uint32_t enc_v;//实际编码器速度 未滤波
	uint32_t enc_v_mean;//实际编码器速度 滤波

	uint8_t enc_vmean_wait;//两个连续的 编码器速度的采样延迟周期 //增量编码器 必须大于32

	uint8_t enc_vmean_filter;//滤波器指数
	uint16_t enc_vmean_int;//增量编码器 自动设置这个值为 256

	uint32_t enc_vel_zero;//一个增量编码器变化的延迟时间[时钟周期数]，在这个延时时间内，编码器没有变化，则设置 V_ENC_MEAN = 0


	bool enc_pol_n;//索引通道有效极性 N 默认0 低有效
	uint8_t enc_n_chan_sensitivity;//索引通道灵敏度
	bool enc_pol_a_for_n;
	bool enc_pol_b_for_n;
	bool enc_ignore_ab;

	bool enc_clr_latch_cont_on_n;//使能 每发生一次 N 事件，ENC_POS 信息的值被清除和/或锁存。

	bool enc_clear_on_n;//每个N事件 设置为事件，ENC_POS

	uint32_t enc_rest_val;//复位微步设置



}TMC4361A_ENC_REG;//编码器相关 


typedef struct 
{
	uint16_t setp_fs_per_rev;//电机旋转一圈的全部值
	uint8_t setp_per_fs;//细分 0x00=256

	uint32_t step_tr_tolerance;//电机到达触发 误差
}TMC4361A_STEP_REG;//电机参数有关

typedef struct 
{
	uint16_t ramp_stp_length_add;//步进信号时钟周期
	uint16_t ramp_dir_setup_time;//方向变化延迟周期

	bool ramp_step_inactive_pol;//是否改变 步进极性

	bool ramp_toggle_step;//使能 每次电平变化对应一个步进输入

	bool ramp_pol_dir_out;//是否改变 方向极性
    bool ramp_pol_dir_in;//是否改变 方向极性

	bool ramp_reverse_motor_dir;//是否改变 内部方向
	
	uint8_t ramp_stdby_clk_pin_assignment;//待机信号 输出
	uint8_t ramp_sdin_mode;//// 0 =内部步进控制
	uint8_t ramp_profile_mode;//斜坡发生器 斜坡运动轮廓  S :

	bool ramp_sd_indirect_control;// 0:STPIN/DIRIN 输入信号将直接影响 XACTUAL。1:输入信号将影响 XTARGET 寄存器值，内部斜坡发生器工作。

    bool ramp_direct_acc_val_en;//加减速度单位 0：每平方秒的脉冲   1：每个时钟周期的速度值变化
	uint32_t ramp_amax;//最大加速度值
	uint32_t ramp_dmax;//最大减速度值
	uint32_t astart;//开始加速度速度值
	uint32_t dfinal;//停止减速度速度值
	
	bool ramp_direct_bow_val_en;//bow单位 0：每平方秒的脉冲   1：每个时钟周期的速度值变化
	uint32_t ramp_bow1;//s曲线 相关
	uint32_t ramp_bow2;
	uint32_t ramp_bow3;
	uint32_t ramp_bow4;



}TMC4361A_RAMP_REG;

typedef struct 
{
	uint32_t event_status_flags;

	uint32_t event_events;

	uint32_t event_spi_status_selection;//选择8个事件作为SPI状态为返回
	uint32_t event_clrar_conf;//事件保护寄存器 被置为的事件 不会被清0

	uint32_t event_intr_conf;//中断 使能寄存器

	bool event_intr_pol;//中断输出极性

}TMC4361A_EVENT_REG;//事件寄存器


void TMC4361A_Set_RearDrive_Reg(TMC4361TypeDef *tmc4361,TMC4361A_REARDRIVE_REG *reg);
uint32_t TMC4361A_Get_RearDrive_Reg(TMC4361TypeDef *tmc4361,uint8_t adder);

void TMC4361A_Set_OutputPin_Reg(TMC4361TypeDef *tmc4361, TMC4361A_OUTPUT_PIN_REG reg);
TMC4361A_OUTPUT_PIN_REG TMC4361A_Get_OutputPin_Reg(TMC4361TypeDef *tmc4361);

//配置spi输出操作
void TMC4361A_Set_SpiOut_Reg(TMC4361TypeDef *tmc4361,TMC4361A_SPIOUT_REG reg);
TMC4361A_SPIOUT_REG TMC4361A_Get_SpiOut_Reg(TMC4361TypeDef *tmc4361);
//闭环寄存器操作
void TMC4361A_Set_ClosedLoop_Reg(TMC4361TypeDef *tmc4361, TMC4361A_CLOSEDLOOP_REG reg);
TMC4361A_CLOSEDLOOP_REG TMC4361A_Get_ClosedLoop_Reg(TMC4361TypeDef *tmc4361);
//闭环电流寄存器操作
void TMC4361A_Set_ClosedLoop_Cur_Reg(TMC4361TypeDef *tmc4361, TMC4361A_CLOSEDLOOP_CUR_REG reg);
TMC4361A_CLOSEDLOOP_CUR_REG TMC4361A_Get_ClosedLoop_Cur_Reg(TMC4361TypeDef *tmc4361);
//闭环反电动势补偿寄存器操作
void TMC4361A_Set_ClosedLoop_BackEmf_Reg(TMC4361TypeDef *tmc4361, TMC4361A_CLOSEDLOOP_BACKEMF_REG reg);
TMC4361A_CLOSEDLOOP_BACKEMF_REG TMC4361A_Get_ClosedLoop_BackEmf_Reg(TMC4361TypeDef *tmc4361);
//闭环速度环寄存器操作
void TMC4361A_Set_ClosedLoop_Speed_Reg(TMC4361TypeDef *tmc4361, TMC4361A_CLOSEDLOOP_SPEED_REG reg);
TMC4361A_CLOSEDLOOP_SPEED_REG TMC4361A_Get_ClosedLoop_Speed_Reg(TMC4361TypeDef *tmc4361);
//编码器 寄存器相关操作
void TMC4361A_Set_Enc_Reg(TMC4361TypeDef *tmc4361, TMC4361A_ENC_REG reg);
TMC4361A_ENC_REG TMC4361A_Get_Enc_Reg(TMC4361TypeDef *tmc4361);
//斜坡发生器 寄存器相关操作
void TMC4361A_Set_Ramp_Reg(TMC4361TypeDef *tmc4361, TMC4361A_RAMP_REG reg);
TMC4361A_RAMP_REG TMC4361A_Get_Ramp_Reg(TMC4361TypeDef *tmc4361);
//闭环速度环寄存器操作
void TMC4361A_Set_Event_Reg(TMC4361TypeDef *tmc4361, TMC4361A_EVENT_REG reg);
TMC4361A_EVENT_REG TMC4361A_Get_Event_Reg(TMC4361TypeDef *tmc4361);
//电机参数寄存器操作
void TMC4361A_Set_Step_Reg(TMC4361TypeDef *tmc4361, TMC4361A_STEP_REG reg);
TMC4361A_STEP_REG TMC4361A_Get_Step_Reg(TMC4361TypeDef *tmc4361);

#endif /* TMC_IC_TMC4361A_H_ */

