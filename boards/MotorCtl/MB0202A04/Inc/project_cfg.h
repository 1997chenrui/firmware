#ifndef __PROJECT_CFG_H__
#define __PROJECT_CFG_H__

/*********Led指示灯*******/
#define ERR_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define WARN_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define NORMAL_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)

/*********阀和泵控制*******/
#define V0401_CTRL(open) do{HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)

#define V0402_CTRL(open) do{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)

#define P0401_CTRL(open) do{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)



#define IO_CMD_ENABLE 1
#define MOTOR_CMD_ENABLE 1
#define VOLTAGE_ENABLE 0
#define VOLTAGE_THREAD_ENABLE 1
#define AD_SIGNAL_ENABLE 1
#define TCT_V2 1
#endif
