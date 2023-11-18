#ifndef __PROJECT_CFG_H__
#define __PROJECT_CFG_H__
#include "bus_cfg.h"
	
/*********电源控制********/

#define PC_POWER_CTRL(on) do{HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, on ? GPIO_PIN_RESET : GPIO_PIN_SET);}while(0)
#define MAIN_POWER_CTRL(on) do{HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, on? GPIO_PIN_RESET: GPIO_PIN_SET);}while(0)


#define POWER_5V_CTRL(on) do{HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, on? GPIO_PIN_SET: GPIO_PIN_RESET);}while(0)




#define BTN_STATE_READ() HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4) == 1


#define POWER_CTRL(on) do{ printf("power state = %d\r\n", on); \
                            if(on==1){ \
								MAIN_POWER_CTRL(on);\
								PC_POWER_CTRL(on);\
								HAL_UART_DMAPause(&huart6);\
								bus_os_sleep(1000);\
								HAL_UART_DMAResume(&huart6);\
								POWER_5V_CTRL(on); \
                            }\
							else if(on==2){  \
								PC_POWER_CTRL(0);   \
                            }\
							else{  \
                                MAIN_POWER_CTRL(0); \
                            }   \
                    }while(0)

/** 此设备电源开机由硬件控制，开关按钮只能控制关机，故默认初始状态为开机状态 **/
#define POWER_DEFAULT_ON 1

/*********Led指示灯*******/
#define ERR_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define WARN_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define NORMAL_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)

#define UART_TRANSPOND_ENABLE 1 //开启串口转发功能

#define UART3_TO_PC 					0
#define UART6_TO_PC 					1//使用串口6 和上位机通讯


#define T0804 1
#define GW6000_V3 1

#endif
