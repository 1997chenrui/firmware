#ifndef __PROJECT_CFG_H__
#define __PROJECT_CFG_H__

/*******电源配置********/
#define MOTOR_POWER_CTRL(on) do{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, on ? GPIO_PIN_RESET : GPIO_PIN_SET);}while(0)
#define TEMPERATURE_POWER_CTRL(on) do{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, on ? GPIO_PIN_RESET : GPIO_PIN_SET);}while(0)
#define PC_POWER_CTRL(on) do{HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, on ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define LED_CTRL(on) do{HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, on? GPIO_PIN_SET: GPIO_PIN_RESET);}while(0)
#define BTN_STATE_READ() HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_13) == 0

#define POWER_CTRL(on) do{  \
                            if(on){ \
                                MOTOR_POWER_CTRL(on);   \
                                bus_os_sleep(100);  \
                                TEMPERATURE_POWER_CTRL(on); \
                                bus_os_sleep(100);  \
                                PC_POWER_CTRL(on);  \
                                bus_os_sleep(100);  \
                                LED_CTRL(on);   \
                            }else{  \
                                MOTOR_POWER_CTRL(on);   \
                                TEMPERATURE_POWER_CTRL(on); \
                                PC_POWER_CTRL(on);  \
                                LED_CTRL(on);   \
                            }   \
                    }while(0)

/*********Led指示灯*******/
#define ERR_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define WARN_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define NORMAL_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)

/**** 功能开关 ****/
#define RUNZE_ENABLE  1

#endif
