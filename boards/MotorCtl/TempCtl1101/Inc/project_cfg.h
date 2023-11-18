#ifndef __PROJECT_CFG_H__
#define __PROJECT_CFG_H__

/*********LedÖ¸Ê¾µÆ*******/
#define ERR_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define WARN_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define NORMAL_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)


#define TEMP_CTRL_ENABLE 1
#define MOTOR_CMD_ENABLE 1
#define IO_CMD_ENABLE 1
#define WS_LED_ENABLE  1 
#endif
