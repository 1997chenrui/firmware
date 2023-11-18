#include "stm32f4xx.h"
#include "soft_iic.h"
#include "delayus.h"

static void Soft_iic_set_sda_out(SoftIIc *iic)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = iic->IO.sda_pin;
  if(iic->mode==OD)
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  else if(iic->mode==PP)
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(iic->IO.sda_port, &GPIO_InitStruct);
	
}
static void Soft_iic_set_sda_in(SoftIIc *iic)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = iic->IO.sda_pin;
  if(iic->mode==OD)
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  else if(iic->mode==PP)
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(iic->IO.sda_port, &GPIO_InitStruct);
}

static void Soft_iic_set_scl_out(SoftIIc *iic)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = iic->IO.scl_pin;
    if(iic->mode==OD)
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  else if(iic->mode==PP)
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(iic->IO.scl_port, &GPIO_InitStruct);
	
}


void Soft_iic_delay(SoftIIc *iic,u16 time)
{
	if(iic->speed==LOW)
	{
		delayUs(time*10);
	}
	else if(iic->speed==MIDDLE)
	{
		delayUs(time*5);
	}
	else if(iic->speed==HIGH)
	{
		delayUs(time);
	}
}
void Soft_iic_start(SoftIIc *iic)
{
	Soft_iic_set_sda_out(iic);
	Soft_iic_set_scl_out(iic);
	
	HAL_GPIO_WritePin(iic->IO.sda_port,iic->IO.sda_pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_SET);
	
	Soft_iic_delay(iic,5);
	HAL_GPIO_WritePin(iic->IO.sda_port,iic->IO.sda_pin,GPIO_PIN_RESET);
	Soft_iic_delay(iic,5);
	HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_RESET);

}

void Soft_iic_stop(SoftIIc *iic)
{
	Soft_iic_set_sda_out(iic);
	Soft_iic_set_scl_out(iic);
	HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(iic->IO.sda_port,iic->IO.sda_pin,GPIO_PIN_RESET);
	
	Soft_iic_delay(iic,5);
	
	HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(iic->IO.sda_port,iic->IO.sda_pin,GPIO_PIN_SET);
	Soft_iic_delay(iic,5);
}

unsigned int Soft_iic_wait_ack(SoftIIc * iic)
{
	unsigned int timeout = 0;
	
		
	Soft_iic_set_sda_out(iic);
	Soft_iic_set_scl_out(iic);

	HAL_GPIO_WritePin(iic->IO.sda_port,iic->IO.sda_pin,GPIO_PIN_SET);
	Soft_iic_delay(iic,2);

	HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_SET);
	Soft_iic_delay(iic,2);



	Soft_iic_set_sda_in(iic);
	while(HAL_GPIO_ReadPin(iic->IO.sda_port,iic->IO.sda_pin) == GPIO_PIN_SET)
	{
		if(timeout++ >iic->ack_timeout)
		{		
			Soft_iic_stop(iic);
			return 1;
		}
		
	}
//	HAL_GPIO_WritePin(iic->IO.sda_port,iic->IO.sda_pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_RESET);
	
	return  0;
	
}

static void Soft_iic_send_ack(SoftIIc * iic)
{
	Soft_iic_set_sda_out(iic);
	Soft_iic_set_scl_out(iic);
	HAL_GPIO_WritePin(iic->IO.sda_port,iic->IO.sda_pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_RESET);
	Soft_iic_delay(iic,2);
	HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_SET);
	Soft_iic_delay(iic,2);
	HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_RESET);
}

static void Soft_iic_send_nack(SoftIIc * iic)
{
	Soft_iic_set_sda_out(iic);
	Soft_iic_set_scl_out(iic);
	HAL_GPIO_WritePin(iic->IO.sda_port,iic->IO.sda_pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_RESET);
	Soft_iic_delay(iic,2);
	HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_SET);
	Soft_iic_delay(iic,2);
	HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_RESET);
}

unsigned int Soft_iic_write(SoftIIc *iic , unsigned char data)
{
	Soft_iic_set_sda_out(iic);
	Soft_iic_set_scl_out(iic);
	HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_RESET);
	for(unsigned char i = 0; i < 8; i++)
	{
		if((data&0x80 )>> 7)
			HAL_GPIO_WritePin(iic->IO.sda_port,iic->IO.sda_pin,GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(iic->IO.sda_port,iic->IO.sda_pin,GPIO_PIN_RESET);
		
		data<<=1;
		Soft_iic_delay(iic,5);
		HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_SET);
		Soft_iic_delay(iic,5);
		HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_RESET);
		Soft_iic_delay(iic,5);
	}
	return Soft_iic_wait_ack(iic);
}

unsigned int Soft_iic_read(SoftIIc *iic ,unsigned char ack)
{
	Soft_iic_set_sda_in(iic);
	Soft_iic_set_scl_out(iic);
	unsigned char data = 0;
	for(unsigned char i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_RESET);
		Soft_iic_delay(iic,2);
		HAL_GPIO_WritePin(iic->IO.scl_port,iic->IO.scl_pin,GPIO_PIN_SET);
		data<<=1;
		if(HAL_GPIO_ReadPin(iic->IO.sda_port,iic->IO.sda_pin)!=0)
		{
			data++;
		}
		
		delayUs(1);
		if(ack)
			Soft_iic_send_ack(iic);
		else
			Soft_iic_send_nack(iic);
		
	}
	return data;
}

//u8 Soft_iic_write_device_addr(SoftIIc *iic ,u8 addr)//只写地址
//{
//	u8 Read_ACK = 0;
//	Soft_iic_start(iic);
//	Soft_iic_write(iic,addr);
//	Read_ACK = Soft_iic_wait_ack(iic);
//	Soft_iic_stop(iic);
//		
//	return(Read_ACK);
//}

//void Soft_iic_search_device_addr(SoftIIc *iic)//在总线上搜寻挂载的器件地址
//{
//	u8 result = 0;
//	u8 j = 0;
//	for(j = 0;j < 128; j++)
//	{
//		if((j % 16) == 0)
//		{
//			printf("\r\n");
//		}
//		result = Soft_iic_write_device_addr(iic,j << 1);
//		if(result == 0)
//		{
//			printf(" %X ",j << 1);//%X 十六进制输出，大写；%x 小写
//		}
//		else
//		{
//			printf(" -- ");
//		}
//	}
//}

