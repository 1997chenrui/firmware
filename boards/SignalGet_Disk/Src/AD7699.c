#include "AD7699.h"
#include "delayus.h"
#include "spi_driver.h"
#include "cpu.h"
#include <stdlib.h>

void getAdcTemperature(void)
{
	volatile u16 adcbuf;	
	SetNSS(0);
	SPIDRIVER_SPI_I2S_SendData(SPIDRIVER_SPI2,0xB124);//GetADCTemperature cmd
	while(!SPIDRIVER_SPI_I2S_GetFlagStatus(SPIDRIVER_SPI2,SPIDRIVER_SPI_I2S_FLAG_RXNE));
	SetNSS(1);
	SPIDRIVER_SPI_I2S_ReceiveData(SPIDRIVER_SPI2);
	delayUs(5);
	SetNSS(0);
	SPIDRIVER_SPI_I2S_SendData(SPIDRIVER_SPI2,0xB124);//GetADCTemperature cmd
	while(!SPIDRIVER_SPI_I2S_GetFlagStatus(SPIDRIVER_SPI2,SPIDRIVER_SPI_I2S_FLAG_RXNE));
	SetNSS(1);
	adcbuf = SPIDRIVER_SPI_I2S_ReceiveData(SPIDRIVER_SPI2);
	
	printf("data=%6d	ADC Temperature:%4.1f.C\r\n",adcbuf,4096.0*(float)adcbuf/65536-283+25);
}


u16 get_ad_data(u8 channel, u16 *buff, u8 buffCnt)
{
	volatile uint32_t temp_data = 0;
	
	CPU_SR_ALLOC();
	CPU_INT_DIS();
	SetNSS(0);
	temp_data = SPI2->DR; // 1111 0001 0010 0100 = 0xF124
	SPIDRIVER_SPI_I2S_SendData(SPIDRIVER_SPI2,0xf124|channel<<9); // 0xF124
	while(!SPIDRIVER_SPI_I2S_GetFlagStatus(SPIDRIVER_SPI2,SPIDRIVER_SPI_I2S_FLAG_RXNE));
	temp_data = SPI2->DR;
	SetNSS(1);
	delayUs(5);
	
	SetNSS(0);
	SPIDRIVER_SPI_I2S_SendData(SPIDRIVER_SPI2,0x7124|channel<<9);
	while(!SPIDRIVER_SPI_I2S_GetFlagStatus(SPIDRIVER_SPI2,SPIDRIVER_SPI_I2S_FLAG_RXNE));
	temp_data = SPI2->DR;
	SetNSS(1);
	delayUs(5);
	
	SetNSS(0);
	SPIDRIVER_SPI_I2S_SendData(SPIDRIVER_SPI2,0x7124|channel<<9);
	while(!SPIDRIVER_SPI_I2S_GetFlagStatus(SPIDRIVER_SPI2,SPIDRIVER_SPI_I2S_FLAG_RXNE));
	temp_data = SPI2->DR;
	SetNSS(1);
	delayUs(5);

	temp_data = 0;
	for(unsigned char i = 0; i < buffCnt; i++)
	{
		SetNSS(0);
		SPIDRIVER_SPI_I2S_SendData(SPIDRIVER_SPI2,0x7124|channel<<9);
		while(!SPIDRIVER_SPI_I2S_GetFlagStatus(SPIDRIVER_SPI2,SPIDRIVER_SPI_I2S_FLAG_RXNE));
		buff[i]=SPI2->DR;
		temp_data += SPI2->DR;
		SetNSS(1);
		delayUs(5);
	}
	CPU_INT_EN();
	return (temp_data/(buffCnt));
}



#ifdef GET_AD_BY_MID_DATA 

int cmpfunc (const void * a, const void * b)
{
   return ( *(u16*)a - *(u16*)b);
}

u16 get_ad_by_mid_data(u8 channel, u16* buff, u8 buffCnt)  //采用中值法采值
{
	volatile u16 buf = 0;
	u16 result=0;
//	SetNSS(0);
//	buf = SPI2->DR;
//	SPIDRIVER_SPI_I2S_SendData(SPIDRIVER_SPI2,0xF124|channel<<9);
//	while(!SPIDRIVER_SPI_I2S_GetFlagStatus(SPIDRIVER_SPI2,SPIDRIVER_SPI_I2S_FLAG_RXNE));
//	buf = SPI2->DR;
//	SetNSS(1);
//	delayUs(5);
//	SetNSS(0);
//	SPIDRIVER_SPI_I2S_SendData(SPIDRIVER_SPI2,0xF124|channel<<9);
//	while(!SPIDRIVER_SPI_I2S_GetFlagStatus(SPIDRIVER_SPI2,SPIDRIVER_SPI_I2S_FLAG_RXNE));
//	buf = SPI2->DR;
//	SetNSS(1);
//	delayUs(5);
	for(u8 i=0;i<buffCnt;i++)
	{
		SetNSS(0);
		SPIDRIVER_SPI_I2S_SendData(SPIDRIVER_SPI2,0xF124|channel<<9);
		while(!SPIDRIVER_SPI_I2S_GetFlagStatus(SPIDRIVER_SPI2,SPIDRIVER_SPI_I2S_FLAG_RXNE));
		buff[i] = SPI2->DR; 
		SetNSS(1);
		delayUs(5);
	}
	qsort(buff, buffCnt, sizeof(u16), cmpfunc);
//	if(buffCnt % 2 == 0)
//	{
//		result=(buff[buffCnt/2 - 1] + buff[buffCnt/2 +1])/2;
//	}
//	else
	{
		result=buff[buffCnt/2];
	}
	return result;
}

#endif


u16 get_ad_sigle_data(u8 channel)
{
	u16 buf = 0;
CPU_SR_ALLOC();
CPU_INT_DIS();
//CPU_CRITICAL_ENTER();
	SetNSS(0);
	buf = SPI2->DR;
	SPIDRIVER_SPI_I2S_SendData(SPIDRIVER_SPI2,0xF124|channel<<9);
	while(!SPIDRIVER_SPI_I2S_GetFlagStatus(SPIDRIVER_SPI2,SPIDRIVER_SPI_I2S_FLAG_RXNE));
	buf = SPI2->DR;
	SetNSS(1);
	delayUs(5);	
	SetNSS(0);
	SPIDRIVER_SPI_I2S_SendData(SPIDRIVER_SPI2,0xF124|channel<<9);
	while(!SPIDRIVER_SPI_I2S_GetFlagStatus(SPIDRIVER_SPI2,SPIDRIVER_SPI_I2S_FLAG_RXNE));
	buf = SPI2->DR;
	SetNSS(1);
	delayUs(5);
//	CPU_CRITICAL_EXIT();
CPU_INT_EN();
	buf = buf;
	return  SPI2->DR;
}
