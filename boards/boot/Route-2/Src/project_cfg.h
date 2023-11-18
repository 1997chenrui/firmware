#ifndef __PROJECT_CFG_H__
#define __PROJECT_CFG_H__


/*********电源控制********/
#define MAIN_POWER_CTRL(on) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, on? GPIO_PIN_RESET: GPIO_PIN_SET);}while(0)
#define PC_POWER_CTRL(on) do{HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, on? GPIO_PIN_RESET: GPIO_PIN_SET);}while(0)
#define BTN_STATE_READ() HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5) == 1

#define POWER_CTRL(on) do{  \
                            if(on==1){ \
								MAIN_POWER_CTRL(on);\
                                PC_POWER_CTRL(on);\
								HAL_UART_DMAPause(&huart6);\
								bus_os_sleep(1000);\
								HAL_UART_DMAResume(&huart6);\
                            }\
							else{  \
                                MAIN_POWER_CTRL(0); \
                            }   \
                    }while(0)

/** 此设备电源开机由硬件控制，开关按钮只能控制关机，故默认初始状态为开机状态 **/
#define POWER_DEFAULT_ON 2

/*********Led指示灯*******/
#define ERR_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define WARN_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define NORMAL_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)


#endif
