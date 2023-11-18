#ifndef  mTMC5130Driver__H
#define mTMC5130Driver__H

#include <stdbool.h>
#include "stm32f4xx.h"

#define SPI_WRITE_TYPE  1  //对SPI接口进行写入操作
#define SPI_READ_TYPE   0  //对SPI接口进行读取操作

#define MIN(a,b) (((a)<(b)) ? (a) : (b))
#define MAX(a,b) (((a)>(b)) ? (a) : (b))

#define TMC5130_MAX_VELOCITY           8388096  //最大速度
#define TMC5130_MAX_ACCELERATION  65535  //最大加速度

//#define MOTOR_TYPE TMC5160

#define TMC5130_GCONF       0x00
#define TMC5130_GSTAT       0x01
#define TMC5130_IFCNT       0x02
#define TMC5130_SLAVECONF   0x03
#define TMC5130_INP_OUT     0x04
#define TMC5130_X_COMPARE   0x05

#define TMC5130_IHOLD_IRUN  0x10
#define TMC5130_TPOWERDOWN  0x11
#define TMC5130_TSTEP       0x12
#define TMC5130_TPWMTHRS    0x13
#define TMC5130_TCOOLTHRS   0x14
#define TMC5130_THIGH       0x15

#define TMC5130_RAMPMODE    0x20
#define TMC5130_XACTUAL     0x21
#define TMC5130_VACTUAL     0x22
#define TMC5130_VSTART      0x23
#define TMC5130_A1          0x24
#define TMC5130_V1          0x25
#define TMC5130_AMAX        0x26
#define TMC5130_VMAX        0x27
#define TMC5130_DMAX        0x28
#define TMC5130_D1          0x2A
#define TMC5130_VSTOP       0x2B
#define TMC5130_TZEROWAIT   0x2C
#define TMC5130_XTARGET     0x2D

#define TMC5130_VDCMIN      0x33
#define TMC5130_SWMODE      0x34
#define TMC5130_RAMPSTAT    0x35
#define TMC5130_XLATCH      0x36

#define TMC5130_ENCMODE     0x38
#define TMC5130_XENC        0x39
#define TMC5130_ENC_CONST   0x3A
#define TMC5130_ENC_STATUS  0x3B
#define TMC5130_ENC_LATCH   0x3C

#define TMC5130_MSLUT0      0x60
#define TMC5130_MSLUT1      0x61
#define TMC5130_MSLUT2      0x62
#define TMC5130_MSLUT3      0x63
#define TMC5130_MSLUT4      0x64
#define TMC5130_MSLUT5      0x65
#define TMC5130_MSLUT6      0x66
#define TMC5130_MSLUT7      0x67
#define TMC5130_MSLUTSEL    0x68
#define TMC5130_MSLUTSTART  0x69
#define TMC5130_MSCNT       0x6A
#define TMC5130_MSCURACT    0x6B

#define TMC5130_CHOPCONF    0x6C
#define TMC5130_COOLCONF    0x6D
#define TMC5130_DCCTRL      0x6E
#define TMC5130_DRVSTATUS   0x6F
#define TMC5130_PWMCONF     0x70
#define TMC5130_PWMSTATUS   0x71
#define TMC5130_ENCM_CTRL   0x72
#define TMC5130_LOST_STEPS  0x73

	// ramp modes (Register TMC5130_RAMPMODE)
#define TMC5130_MODE_POSITION  0  //位置模式
#define TMC5130_MODE_VELPOS    1  //
#define TMC5130_MODE_VELNEG    2
#define TMC5130_MODE_HOLD      3

#define R10 0x00070903  // IHOLD_IRUN
#define R2B 0xA  //
#define R3A 0x00010000  // ENC_CONST
#define R60 0xAAAAB554  // MSLUT[0]
#define R61 0x4A9554AA  // MSLUT[1]
#define R62 0x24492929  // MSLUT[2]
#define R63 0x10104222  // MSLUT[3]
#define R64 0xFBFFFFFF  // MSLUT[4]
#define R65 0xB5BB777D  // MSLUT[5]
#define R66 0x49295556  // MSLUT[6]
#define R67 0x00404222  // MSLUT[7]
#define R68 0xFFFF8056  // MSLUTSEL
#define R69 0x00F70000  // MSLUTSTART

#define R6C 0x000101D5  // CHOPCONF  101D5
#define R70 0x000504C8  // PWMCONF


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


typedef enum sSTATE
{
	REFLAG=0,  //复位标志
	DRIVER_ERROR, //出错标志
	SG2, //stallsg2启用标志
	STANDSTILL,//停止标志
	VELOCITY_READHED,//速度到达标志
	POSTION_READHED,//位置到达标志
	STATUS_STOP_L,//左停止位
	STATUS_STOP_R//右停止位
}STATETYPE;

typedef struct reg
{
	bool flg;//是否打开过载检测停止功能
	unsigned int CHOPCONF;
	unsigned int IHOLD_IRUN;
	unsigned int COOLCONF;
}DRVIE_REGTYPE;

typedef struct sTABLE
{
	int A1;  //起始加速度
	int V1;//第一段速度 
	int AMAX;//第二段加速度
	int VMAX;//最高速度
	int DMAX;//起始减减加速度
	int D1;//第二段减速加速度

	int run_currnet;//运行电流
	int hold_current;//停止时电流
	int MicoStep;//细分
	int mode;//运行模式
	int postion;//运行位置  
	
	DRVIE_REGTYPE *mTMCDriverReg;//一些特殊的寄存器需要特殊配置
	
	void (*mTMCDriver_WriteIo)(pin_id_t id,int value);
	int   (*mTMCDriver_ReadIo) (pin_id_t id,int,char);	  
}TABLE;


void mTMC5130DriverSetSpeed(struct sTABLE *);
void mTMC5130DriverSetMicoStep(struct sTABLE *);
void mTMC5130DriverSetCurrent(struct sTABLE *);
void mTMC5130DriverResetPostion(struct sTABLE *);
int mTMC5130DriverReadWriteRegister(struct sTABLE *ths,char addr,char cmd,int data);//获取寄存器信息
char mTMC5130DriverGetState(struct sTABLE *ths);
int mTMC5130DriverGetSpeed(struct sTABLE *ths);//获取当前速度
int mTMC5130DriverGetPosition(struct sTABLE *);
int mTMC5130DriverGetRAMPSTAT(struct sTABLE *ths);
char mTMC5130DriverGetPara(struct sTABLE *ths);
int mTMC5130DriverGetDRVSTATUS(struct sTABLE *ths);
int mTMC5130DriverSETSWMODE(struct sTABLE *ths,int );
void mTMC5130DriverSetPostion(struct sTABLE *ths,int pos);

void mTMC5130DriverSetDefaultRegister(struct sTABLE *ths,char) ;//配置5130寄存器参数

char mTMC5130DriverMove(struct sTABLE *);
void mTMC5130Driver_Init(struct sTABLE *);



#endif


