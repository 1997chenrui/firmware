#ifndef _SOFT_IIC_H_
#define _SOFT_IIC_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum soft_iic_rtError
{
	Soft_iic_normal,
	Soft_iic_timeout
}SoftIIc_rtError;

typedef struct softIic
{

	uint16_t ack_timeout;


	void (*setSclOut)(void);//如果OD 可不实现 直接赋值NULL
	void (*setSdaOut)(void);//如果OD 可不实现 直接赋值NULL
	void (*setSdaIn)(void);//如果OD 可不实现 直接赋值NULL

	void (*writeScl)(bool);

	void (*writeSda)(bool);

	bool (*readSda)(void);

	void (*delay)(uint32_t);//直接决定IIC通信速率 单位为微妙  计算公式为 ： 速率 = 1000000/ (8*2*4*delay(1))
	
}SoftIIc;


 void Soft_iic_start(SoftIIc *iic);
 void Soft_iic_stop(SoftIIc *iic);

SoftIIc_rtError Soft_iic_write(SoftIIc *iic , uint8_t data);

uint8_t Soft_iic_read(SoftIIc *iic ,bool ackEn);



#endif
