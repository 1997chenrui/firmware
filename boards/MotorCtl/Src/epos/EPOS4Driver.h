#ifndef __EPOSDRIVER__
#define __EPOSDRIVER__

#include "stm32f4xx_hal.h"

#define UART_COMMUNICATION_TIMEOUT 140

#define PROFILE_POSITION_MODE    0x01   //位置模式
#define PROFILE_VELOCITY_MODE    0x03   //速度模式
#define PROFILE__HOMING_MODE     0x06

#define MODES_OPERATION   0X6060     //-00    //模式选择
#define VELOCITY    0X6081         //速度
#define ACCELERATION   0X6083    //加速度
#define DECELERATION  0X6084    //减速度
#define QUICK_STOP_DECELERATION  0X6085 //快速停止
#define MOTION_PROFILE_TYPE  0X6086   //电机类型

#define TARGET_POSITION 0X607A   //目标位置

#define CONTROLWORD  0X6040   //  控制寄存器
#define STATUS    0X6041   //  状太寄存器

#define SHUTDOWN  0X0006    //停止
#define SWITCH_ON_ENABLE 0X000F  //使能
#define ABSOLUTE_POSITION 0X001F    //绝对位移
#define ABSOLUTE_POSITION_START_IMMEDIATELY  0X003F  //绝对位移 立即执行
#define RELATIVE_POSITION_START_IMMEDIATELY 0X007F   //相对位移  立即执行
#define RELATIVE_POSITION 0X005F  //相对位移

#define Ready_Switch_On  0x00;
#define Profile_Velocity_Mode 0x03;
#define Profile_Velocity  0x00;
#define Active_Home 0x0006;
#define Shutdown  0x0006;
#define Switchon_Enable  0x000F;
#define Start_Home  0x001F;
#define Profile_Position_Mode   0x01;
#define Absolute_Immediately  0x003F;
#define Absolute_Flag   0x001F;
#define Relative_Immediately   0x007F;
#define Relative_Flag   0x005F;
#define Halt_Profile_Velocity_Mode   0x010F;
#define GoOn_Profile_Velocity_Mode  0x000F;
#define Quick_Stop 0x000B;
#define Clear_Error   0x0080;  
#define Index_Negative   33;
#define Positive_Speed   34;

uint8_t UART_EnableUart(void); //串口初始化

uint8_t EPOS4_WriteData(uint16_t , uint8_t ,int32_t);
uint8_t EPOS4_ReadData(uint16_t , uint8_t ,int32_t *);

uint8_t Move_By_Position_Mode(int ,int ,int ,int ,uint8_t ,uint8_t ) ;  //位置位置下运动
uint8_t Move_By_Velocity_Mode(int ,int ,int ) ;  //速度模式下运动
uint8_t Move_By_Homing_Mode(int ,int ,int ,int ,int ,int ) ;
#endif

