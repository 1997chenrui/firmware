#ifndef  mTMC4361Driver__H
#define mTMC4361Driver__H

#include <stdbool.h>
#include "stm32f4xx.h"

#define SPI_WRITE_TYPE  1  //对SPI接口进行写入操作
#define SPI_READ_TYPE   0  //对SPI接口进行读取操作

#define MIN(a,b) (((a)<(b)) ? (a) : (b))
#define MAX(a,b) (((a)>(b)) ? (a) : (b))

#define TMC4361_GENERAL_CONF          0x00
#define TMC4361_REFERENCE_CONF        0x01
#define TMC4361_START_CONF            0x02
#define TMC4361_INPUT_FILT_CONF       0x03
#define TMC4361_SPIOUT_CONF           0x04
#define TMC4361_CURRENT_CONF          0x05
#define TMC4361_SCALE_VALUES          0x06
#define TMC4361_ENC_IN_CONF           0x07
#define TMC4361_ENC_IN_DATA           0x08
#define TMC4361_ENC_OUT_DATA          0x09
#define TMC4361_STEP_CONF             0x0A
#define TMC4361_SPI_STATUS_SELECTION  0x0B
#define TMC4361_EVENT_CLEAR_CONF      0x0C
#define TMC4361_INTR_CONF             0x0D
#define TMC4361_EVENTS                0x0E
#define TMC4361_STATUS                0x0F

#define TMC4361_STP_LENGTH_ADD        0x10
#define TMC4361_DIR_SETUP_TIME        0x10
#define TMC4361_START_OUT_ADD         0x11
#define TMC4361_GEAR_RATIO            0x12
#define TMC4361_START_DELAY           0x13
#define TMC4361_CLK_GATING_DELAY      0x14
#define TMC4361_STDBY_DELAY           0x15
#define TMC4361_FREEWHEEL_DELAY       0x16
#define TMC4361_VDRV_SCALE_LIMIT      0x17
#define TMC4361_PWM_VMAX              0x17
#define TMC4361_UP_SCALE_DELAY        0x18
#define TMC4361_CL_UPSCALE_DELAY      0x18
#define TMC4361_HOLD_SCALE_DELAY      0x19
#define TMC4361_CL_DOWNSCALE_DELAY    0x19
#define TMC4361_DRV_SCALE_DELAY       0x1A
#define TMC4361_BOOST_TIME            0x1B
#define TMC4361_CL_BETA               0x1C
#define TMC4361_CL_GAMMA              0x1C
#define TMC4361_DAC_ADDR_A            0x1D
#define TMC4361_DAC_ADDR_B            0x1D
#define TMC4361_SPI_SWITCH_VEL        0x1D
#define TMC4361_HOME_SAFETY_MARGIN    0x1E
#define TMC4361_PWM_FREQ              0x1F
#define TMC4361_CHOPSYNC_DIV          0x1F

#define TMC4361_RAMPMODE              0x20
#define TMC4361_XACTUAL               0x21
#define TMC4361_VACTUAL               0x22
#define TMC4361_AACTUAL               0x23
#define TMC4361_VMAX                  0x24
#define TMC4361_VSTART                0x25
#define TMC4361_VSTOP                 0x26
#define TMC4361_VBREAK                0x27
#define TMC4361_AMAX                  0x28
#define TMC4361_DMAX                  0x29
#define TMC4361_ASTART                0x2A
#define TMC4361_SIGN_AACT             0x2A
#define TMC4361_DFINAL                0x2B
#define TMC4361_DSTOP                 0x2C
#define TMC4361_BOW1                  0x2D
#define TMC4361_BOW2                  0x2E
#define TMC4361_BOW3                  0x2F
#define TMC4361_BOW4                  0x30
#define TMC4361_CLK_FREQ              0x31

#define TMC4361_POS_COMP              0x32
#define TMC4361_VIRT_STOP_LEFT        0x33
#define TMC4361_VIRT_STOP_RIGHT       0x34
#define TMC4361_X_HOME                0x35
#define TMC4361_X_LATCH_RD            0x36
#define TMC4361_REV_CNT_RD            0x36
#define TMC4361_X_RANGE_WR            0x36
#define TMC4361_X_TARGET              0x37

#define TMC4361_X_PIPE0               0x38
#define TMC4361_X_PIPE1               0x39
#define TMC4361_X_PIPE2               0x3A
#define TMC4361_X_PIPE3               0x3B
#define TMC4361_X_PIPE4               0x3C
#define TMC4361_X_PIPE5               0x3D
#define TMC4361_X_PIPE6               0x3E
#define TMC4361_X_PIPE7               0x3F

#define TMC4361_SH_REG0               0x40
#define TMC4361_SH_REG1               0x41
#define TMC4361_SH_REG2               0x42
#define TMC4361_SH_REG3               0x43
#define TMC4361_SH_REG4               0x44
#define TMC4361_SH_REG5               0x45
#define TMC4361_SH_REG6               0x46
#define TMC4361_SH_REG7               0x47
#define TMC4361_SH_REG8               0x48
#define TMC4361_SH_REG9               0x49
#define TMC4361_SH_REG10              0x4A
#define TMC4361_SH_REG11              0x4B
#define TMC4361_SH_REG12              0x4C
#define TMC4361_SH_REG13              0x4D

#define TMC4361_DFREEZE               0x4E
#define TMC4361_IFREEZE               0x4E
#define TMC4361_CLK_GATING_REG        0x4F
#define TMC4361_RESET_REG             0x4F

#define TMC4361_ENC_POS               0x50
#define TMC4361_ENC_LATCH_RD          0x51
#define TMC4361_ENC_RESET_VAL_WR      0x51
#define TMC4361_ENC_POS_DEV_RD        0x52
#define TMC4361_CL_TR_TOLERANCE_WR    0x52
#define TMC4361_ENC_POS_DEV_TOL_WR    0x53
#define TMC4361_ENC_IN_RES_WR         0x54
#define TMC4361_ENC_CONST_RD          0x54
#define TMC4361_MANUAL_ENC_CONST0     0x54
#define TMC4361_ENC_OUT_RES           0x55
#define TMC4361_SER_CLK_IN_HIGH_WR    0x56
#define TMC4361_SER_CLK_IN_LOW_WR     0x56
#define TMC4361_SSI_IN_CLK_DELAY_WR   0x57
#define TMC4361_SSI_IN_WTIME_WR       0x57
#define TMC4361_SER_PTIME_WR          0x58

#define TMC4361_CL_OFFSET             0x59
#define TMC4361_PID_P_WR              0x5A
#define TMC4361_CL_VMAX_CALC_P_WR     0x5A
#define TMC4361_PID_VEL_RD            0x5A
#define TMC4361_PID_I_WR              0x5B
#define TMC4361_CL_VMAX_CALC_I_WR     0x5B
#define TMC4361_PID_ISUM_RD           0x5B
#define TMC4361_PID_D_WR              0x5C
#define TMC4361_CL_DELTA_P_WR         0x5C
#define TMC4361_PID_I_CLIP_WR         0x5D
#define TMC4361_PID_D_CLKDIV_WR       0x5D
#define TMC4361_PID_E_RD              0x5D
#define TMC4361_PID_DV_CLIP_WR        0x5E
#define TMC4361_PID_TOLERANCE_WR      0x5F
#define TMC4361_CL_TOLERANCE_WR       0x5F

#define TMC4361_FS_VEL_WR             0x60
#define TMC4361_DC_VEL_WR             0x60
#define TMC4361_CL_VMIN_EMF_WR        0x60
#define TMC4361_DC_TIME_WR            0x61
#define TMC4361_DC_SG_WR              0x61
#define TMC4361_DC_BLKTIME_WR         0x61
#define TMC4361_CL_VADD_EMF           0x61
#define TMC4361_DC_LSPTM_WR           0x62
#define TMC4361_ENC_VEL_ZERO_WR       0x62
#define TMC4361_ENC_VMEAN_WAIT_WR     0x63
#define TMC4361_ENC_VMEAN_FILTER_WR   0x63
#define TMC4361_ENC_VMEAN_INT_WR      0x63
#define TMC4361_SER_ENC_VARIATION_WR  0x63
#define TMC4361_CL_CYCLE_WR           0x63
#define TMC4361_SYNCHRO_SET           0x64
#define TMC4361_V_ENC_RD              0x65
#define TMC4361_V_ENC_MEAN_RD         0x66
#define TMC4361_VSTALL_LIMIT_WR       0x67

#define TMC4361_ADDR_TO_ENC           0x68
#define TMC4361_DATA_TO_ENC           0x69
#define TMC4361_ADDR_FROM_ENC         0x6A
#define TMC4361_DATA_FROM_ENC         0x6B
#define TMC4361_COVER_LOW_WR          0x6C
#define TMC4361_POLLING_STATUS_RD     0x6C
#define TMC4361_COVER_HIGH_WR         0x6D
#define TMC4361_POLLING_REG_WR        0x6D
#define TMC4361_COVER_DRV_LOW_RD      0x6E
#define TMC4361_COVER_DRV_HIGH_RD     0x6F

#define TMC4361_MSLUT_0_WR            0x70
#define TMC4361_MSLUT_1_WR            0x71
#define TMC4361_MSLUT_2_WR            0x72
#define TMC4361_MSLUT_3_WR            0x73
#define TMC4361_MSLUT_4_WR            0x74
#define TMC4361_MSLUT_5_WR            0x75
#define TMC4361_MSLUT_6_WR            0x76
#define TMC4361_MSLUT_7_WR            0x77
#define TMC4361_MSLUTSEL_WR           0x78
#define TMC4361_MSCNT_RD              0x79
#define TMC4361_MSOFFSET_WR           0x79
#define TMC4361_CURRENTA_RD           0x7A
#define TMC4361_CURRENTB_RD           0x7A
#define TMC4361_CURRENTA_SPI_RD       0x7B
#define TMC4361_CURRENTB_SPI_RD       0x7B
#define TMC4361_TZEROWAIT_WR          0x7B
#define TMC4361_SCALE_PARAM_RD        0x7C
#define TMC4361_CIRCULAR_DEC_WR       0x7C
#define TMC4361_ENC_COMP_XOFFSET      0x7D
#define TMC4361_ENC_COMP_YOFFSET      0x7D
#define TMC4361_ENC_COMP_AMPL         0x7D
#define TMC4361_START_SIN_WR          0x7E
#define TMC4361_START_SIN90_120_WR    0x7E
#define TMC4361_DAC_OFFSET_WR         0x7E
#define TMC4361_VERSION_NO_RD         0x7F

#define TMC4361_COVER_DONE (1<<25)

#define TMC4361_RAMP_HOLD      0  //速度模式 仅遵循 VMAX 请求
#define TMC4361_RAMP_TRAPEZ    1  //速度模式 遵循 VMAX 请求，并考虑加速和减速值。
#define TMC4361_RAMP_SSHAPE    2  //速度模式 遵循 VMAX 要求，考虑最大加速度/减速度值，并用 4个不同的弓形值参数调整这些值
#define TMC4361_RAMP_POSITION_HOLD  4 //定位模式 仅遵循 XTARGET 和 VMAX 请求
#define TMC4361_RAMP_POSITION_TRAPEZ  5 //定位模式 斜坡加减速
#define TMC4361_RAMP_POSITION_SSHAPE 6 //定位模式 S加减速

#define REGISTER_COUNT 128
// Default Register Values
#define R00  0x00006020  // GENERAL_CONF
#define R06  0xFFFFFFFF  // SCALE_VALUES
#define R07  0x00000400  // ENC_IN_CONF
#define R0A  0x00FB0C80  // STEP_CONF
#define R0B  0x82029805  // SPI_STATUS_SELECTION
#define R10  0x00040001  // STP_LENGTH_ADD
#define R12  0x01000000  // GEAR_RATIO
#define R1C  0x00FF00FF  // CL_BETA / CL_GAMMA
#define R1F  0x00000280  // PWM_FREQ / CHOPSYNC_DIV
#define R20  0x00000001  // RAMPMODE
#define R31  0x00F42400  // CLK_FREQ
#define R53  0xFFFFFFFF  // ENC_POS_DEV_TOL_WR
#define R56  0x00A000A0  // SER_CLK_IN_HIGH_WR / SER_CLK_IN_LOW_WR
#define R57  0x00F00000  // SSI_IN_CLK_DELAY_WR / SSI_IN_WTIME_WR
#define R58  0x00000190  // SER_PTIME_WR
#define R62  0x00FFFFFF  // DC_LSPTM_WR / ENC_VEL_ZERO_WR
#define R70  0xAAAAB554  // MSLUT_0_WR
#define R71  0x4A9554AA  // MSLUT_1_WR
#define R72  0x24492929  // MSLUT_2_WR
#define R73  0x10104222  // MSLUT_3_WR
#define R74  0xFBFFFFFF  // MSLUT_4_WR
#define R75  0xB5BB777D  // MSLUT_5_WR
#define R76  0x49295556  // MSLUT_6_WR
#define R77  0x00404222  // MSLUT_7_WR
#define R78  0xFFFF8056  // MSLUTSEL_WR
#define R7E  0x00F70000  // START_SIN_WR / START_SIN90_120_WR / 

#define STANDSTILL  0x18
#define POSTION_READHED 0x1

typedef enum
{
	PIN_ID_ENABLE=1,				//电机使能脚
	PIN_ID_CS,  //片选引脚
	PIN_ID_EME,//紧急停止引脚
	PIN_ID_REFL,//左停止位
	PIN_ID_REFR,//右停止位
	
	PIN_ID_SET_TOUCH_ISR,//光耦触发中断
	PIN_ID_SET_REACH_ISR,//目标位置到达中断
	PIN_ID_SET_ERROR_ISR,//芯片错误检测中断
	PIN_ID_SET_SENSOR,
	
	PIN_INIT_TOUCH_ISR,//初始化光耦触发中断
	PIN_INIT_REACH_ISR,//初始化目标位置到达中断
	PIN_INIT_ERROR_ISR,//初始化错误检测中断号
	
	READ_PIN_ID_TOUCH_ISR,//光耦触发中断
	READ_PIN_ID_REACH_ISR,//目标位置到达中断
	READ_PIN_ID_ERROR_ISR,
	READ_PIN_ID_SENSOR,
}pin_id_t;


//typedef enum sSTATE
//{
//	REFLAG=0,  //复位标志
//	DRIVER_ERROR, //出错标志
//	SG2, //stallsg2启用标志
//	STANDSTILL,//停止标志
//	VELOCITY_READHED,//速度到达标志
//	POSTION_READHED,//位置到达标志
//	STATUS_STOP_L,//左停止位
//	STATUS_STOP_R//右停止位
//}STATETYPE;

typedef struct reg
{
	bool flg;//是否打开过载检测停止功能
	unsigned int CHOPCONF;
	unsigned int IHOLD_IRUN;
	unsigned int COOLCONF;
	unsigned int p;
	unsigned int i;
}DRVIE_REGTYPE;

typedef struct sTABLE
{
	int AMAX;//第二段加速度
	int VMAX;//最高速度
	int DMAX;//起始减减加速度

	int run_currnet;//运行电流
	int hold_current;//停止时电流
	int MicoStep;//细分
	int mode;//运行模式
	int postion;//运行位置  
	
	int vbreak;
	int bow1;
	int bow2;
	int bow3;
	int bow4;
	
	DRVIE_REGTYPE *mTMCDriverReg;//一些特殊的寄存器需要特殊配置
	
	void (*mTMCDriver_WriteIo)(pin_id_t id,int value);
	int  (*mTMCDriver_ReadIo) (pin_id_t id,int,char);	  
}TABLE;


void mTMC4361DriverSetSpeed(struct sTABLE *);

void mTMC4361DriverResetPostion(struct sTABLE *);
int mTMC4361DriverGetState(struct sTABLE *ths);
int mTMC4361DriverGetPosition(struct sTABLE *);
int mTMC4361DriverGetSpeed(struct sTABLE * );
int mTMC4361DriverGetENCPosition(struct sTABLE * );


void mTMC4361DriverSetDefaultRegister(struct sTABLE *ths,char) ;//配置5130寄存器参数

char mTMC4361DriverMove(struct sTABLE *);
char mTMC4361Driver_Init(struct sTABLE *);
void mTMC4361DriverSetPostion(struct sTABLE * ,int );


#endif


