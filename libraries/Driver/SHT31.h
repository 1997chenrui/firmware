#ifndef __SHT31_H 
#define __SHT31_H  
#include "stdint.h"
 
/* 传感器 地址 */
#define  SHT_ADD1 0X44
#define  SHT_ADD2 0X45
/* 读写操作位 */
#define  SHT_Write_Bit  0x00
#define  SHT_Read_Bit   0x01
/* 命令码 */
#define  SHT_Cmd_MSB    0X24
#define  SHT_Cmd_LSB    0X00
/* 应答位 */
#define  SHT_ACK        0X01
#define  SHT_NACK       0X00
/* I2C 引脚映射 */
#define  SHT_SDA_Set()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET)
#define  SHT_SDA_Reset()   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET)
#define  SHT_SDA_Read()    HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_1)

#define  SHT_SCL_Set()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_SET)
#define  SHT_SCL_Reset()   HAL_GPIO_WritePin(GPIOE, GPIO_PIN_2, GPIO_PIN_RESET)
#define  SHT_SCL_Read()    HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)

//typedef struct{

//	
//}SHT31_Struct;

void delay_user(uint16_t us);
void SHT31_S_Start(void);
void SHT31_P_Stop(void);
void SHT31_ACK( uint8_t ack);
uint8_t SHT31_Test_ACK(void );
void SHT31_SentByte(uint8_t TxData);
uint8_t SHT31_ReadByte(void);

uint8_t SHT31_Read_Data(uint8_t address,uint8_t *temp,uint8_t *humi);


#endif
