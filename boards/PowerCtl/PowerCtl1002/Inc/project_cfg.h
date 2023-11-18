#ifndef __PROJECT_CFG_H__
#define __PROJECT_CFG_H__
#include "bus_cfg.h"
#include "color.h"	
	
/*********电源控制********/
//显示屏和激光打印机电源
#define MOTOR_PC_LASER_POWER_CTRL(on) do{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, on ?  GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
//PC电源
#define PC_POWER_EN_CTRL(on) do{HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, on ? GPIO_PIN_RESET : GPIO_PIN_SET );}while(0)
//#define TEMPERATURE_POWER_CTRL(on) do{HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, on ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
//激光电源
#define THE_LASER_EN_CRTL(on) do{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, on ? GPIO_PIN_RESET : GPIO_PIN_SET);}while(0)
//主电源
#define MAIN_POWER_CTRL(on) do{HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, on? GPIO_PIN_RESET : GPIO_PIN_SET);}while(0)
//5V电源
#define POWER_5V_CTRL(on) do{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, on? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
//电源按钮
//bus_os_sleep(1000);\
//HAL_UART_DMAPause(&huart6);\
bus_os_sleep(1000);\
HAL_UART_DMAResume(&huart6);\

#define BTN_STATE_READ() HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == 1
#define POWER_CTRL(on) do{ printf("power state = %d\r\n", on); \
                            if(on == 1){ \
								POWER_5V_CTRL(on); \
								bus_os_sleep(10);  \
								MOTOR_PC_LASER_POWER_CTRL(on);\
								bus_os_sleep(10);  \
                                PC_POWER_EN_CTRL(on);  \
								bus_os_sleep(10);  \
								THE_LASER_EN_CRTL(on); \
								bus_os_sleep(10);  \
								colormod_init(); \
								bus_os_sleep(10);  \
                            }else if(on == 2){	\
								bus_os_sleep(10);  \
								POWER_5V_CTRL(0);   \
								bus_os_sleep(10);  \
                                THE_LASER_EN_CRTL(0);  \
								bus_os_sleep(10);  \
								PC_POWER_EN_CTRL(0); \
								bus_os_sleep(10);  \
							}else{  \
								bus_os_sleep(10);  \
                                MAIN_POWER_CTRL(0);\
								bus_os_sleep(10);  \
                            }   \
                    }while(0)

/** 此设备电源开机由硬件控制，开关按钮只能控制关机，故默认初始状态为开机状态 **/
#define POWER_DEFAULT_ON 1

/*********Led指示灯*******/
#define ERR_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define WARN_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define NORMAL_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)

#define UART_TRANSPOND_ENABLE 1 //开启串口转发功能
#define WS_LED_ENABLE 1			//开启RGB灯功能

#define UART3_TO_PC 					0
#define UART6_TO_PC 					1//使用串口6 和上位机通讯


#define IO_CMD_ENABLE 1

#define TCT_V2 1
#define IO_CFG_FILE_NEW 1
#define AD_SIGNAL_ENABLE 1

#endif
