#ifndef __PROJECT_CFG_H__
#define __PROJECT_CFG_H__


#define ERR_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define WARN_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)
#define NORMAL_LED_CTRL(open) do{HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, open ? GPIO_PIN_SET : GPIO_PIN_RESET);}while(0)

#define IO_CFG_FILE_NEW 1
#define IO_CMD_ENABLE 1
#define MOTOR_CMD_ENABLE 1
#define TEMP_CTRL_ENABLE 1
//#define UART2_TO_PC 					1//使用串口2 和上位机通讯
#define UART3_TO_PC 					1//使用串口3 和上位机通讯
#endif
