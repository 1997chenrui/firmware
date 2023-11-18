#ifndef _SOFT_IIC_H_
#define _SOFT_IIC_H_
#include "stm32f4xx_hal.h"
#include "delayus.h"
#include "gpio.h"


typedef struct softIicIo
{
	GPIO_TypeDef * scl_port;
	uint16_t  scl_pin;
	
	GPIO_TypeDef * sda_port;
	uint16_t  sda_pin;
}SoftIIcIo;

typedef enum softIicSpeed
{
	LOW,
	MIDDLE,
	HIGH
}SoftIIcSpeed;

typedef enum softIicMode
{
	PP,
	OD	
}SoftIIcMode;
typedef struct softIic
{
	SoftIIcIo IO;
	SoftIIcSpeed speed;
	SoftIIcMode mode;
	unsigned int ack_timeout;
	
}SoftIIc;


extern void Soft_iic_start(SoftIIc *iic);

extern void Soft_iic_stop(SoftIIc *iic);


extern unsigned int Soft_iic_write(SoftIIc *iic , unsigned char data);

extern unsigned int Soft_iic_read(SoftIIc *iic ,unsigned char ack);
u8 Soft_iic_write_device_addr(SoftIIc *iic ,u8 addr);

void Soft_iic_search_device_addr(SoftIIc *iic);


#endif
