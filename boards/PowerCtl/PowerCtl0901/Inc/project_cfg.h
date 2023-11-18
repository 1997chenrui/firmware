#ifndef __PROJECT_CFG_H__
#define __PROJECT_CFG_H__
#include "bus_cfg.h"

/*******电源配置********/
#define MOTOR_POWER_CTRL(on) do{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, on ? GPIO_PIN_RESET : GPIO_PIN_SET);}while(0)
#define TEMPERATURE_POWER_CTRL(on) do{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, on ? GPIO_PIN_RESET : GPIO_PIN_SET);}while(0)
#define PC_POWER_CTRL(on) do{HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, on ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define LED_CTRL(on) do{HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, on? GPIO_PIN_SET: GPIO_PIN_RESET);}while(0)
#define BTN_STATE_READ() HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13) == 0

/**
* 此处开启显示屏时需要暂停串口DMA，显示屏开启时会短暂出现高频信号，造成DMA问题。
*/
#define POWER_CTRL(on) do{  \
                            if(on){ \
                                MOTOR_POWER_CTRL(on);   \
                                bus_os_sleep(100);  \
                                TEMPERATURE_POWER_CTRL(on); \
                                bus_os_sleep(100);  \
								HAL_UART_DMAPause(&huart6);\
                                PC_POWER_CTRL(on);  \
                                bus_os_sleep(1000);  \
								HAL_UART_DMAResume(&huart6);\
                                LED_CTRL(on);   \
                            }else{  \
                                MOTOR_POWER_CTRL(on);   \
                                TEMPERATURE_POWER_CTRL(on); \
								PC_POWER_CTRL(on);  \
                                LED_CTRL(on);   \
                            }   \
                    }while(0)

/*********Led指示灯*******/
#define ERR_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define WARN_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define NORMAL_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)

#define UART_TRANSPOND_ENABLE 1
#define UART3_TO_PC 					0
#define UART6_TO_PC 					1

#define IO_CMD_ENABLE  1
#define POWER_DEFAULT_ON 2
#define CLOSE_SCAN_CODE 1
#endif
