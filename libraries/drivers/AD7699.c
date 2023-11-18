#include "AD7699.h"
#include "delayus.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_spi.h"

void AD7699_Init(AD7699CONFIG* self, SPI_HandleTypeDef * spi,u8 channel_cnt,NssFunc nssf,u8 *mapping)
{
	if(self!= NULL&&spi!=NULL&&nssf!=NULL&&mapping!=NULL)
	{
		self->adusedspi=spi;
		self->nssfunc=nssf;
		self->mapping=mapping;
		self->channel_cnt=channel_cnt;
		
		__HAL_SPI_ENABLE(self->adusedspi);
		AD7699_GetSingleData(self, 0);
		AD7699_GetSingleData(self, 0);
	}
	else
	{
		printf("This is not config of ad7699");
		for(;;);			
	}


}

static void SetNSS(AD7699CONFIG* self, int v)
{
	(*self->nssfunc)(v);
}



u16 AD7699_GetTemprature(AD7699CONFIG* self)
{
	volatile u16 adcbuf;
	SetNSS(self, 0);
	//self->adusedspi->Instance->DR = 0xB124;
	self->adusedspi->Instance->DR = 0xB124;
	while(!__HAL_SPI_GET_FLAG(self->adusedspi, SPI_FLAG_RXNE));
	SetNSS(self, 1);
	adcbuf = self->adusedspi->Instance->DR;
	delayUs(5);
	SetNSS(self, 0);
	self->adusedspi->Instance->DR = 0xB124;//GetADCTemperature cmd
	while(!__HAL_SPI_GET_FLAG(self->adusedspi, SPI_FLAG_RXNE));
	SetNSS(self, 1);
	adcbuf = self->adusedspi->Instance->DR;
//	printf("adData=%6d	ADC Temperature:%4.1f.C\r\n",adcbuf,4096.0*(float)adcbuf/65536-283+25);
	return adcbuf;
}



u16 AD7699_GetData(AD7699CONFIG* self, u8 channel, u16 average_cnt)
{
	volatile u16 buf = 0;
	u32 temp_data = 0;
    CPU_SR_ALLOC();
    CPU_INT_DIS();
	SetNSS(self, 0);
	buf = self->adusedspi->Instance->DR;
	self->adusedspi->Instance->DR = 0xF124|channel<<9;
	while(!__HAL_SPI_GET_FLAG(self->adusedspi, SPI_FLAG_RXNE));
	buf = self->adusedspi->Instance->DR;
	SetNSS(self, 1);
	delayUs(5);
	SetNSS(self, 0);
	self->adusedspi->Instance->DR = 0x7124|channel<<9;
	while(!__HAL_SPI_GET_FLAG(self->adusedspi, SPI_FLAG_RXNE));
	buf = self->adusedspi->Instance->DR;
	SetNSS(self, 1);
	delayUs(5);
	for(u16 i=0;i<average_cnt;i++)
	{
		SetNSS(self, 0);
		self->adusedspi->Instance->DR = 0x7124|channel<<9;
		while(!__HAL_SPI_GET_FLAG(self->adusedspi, SPI_FLAG_RXNE));
		temp_data += self->adusedspi->Instance->DR; 
		SetNSS(self, 1);
		delayUs(5);
        CPU_INT_EN();
	}
	return temp_data/average_cnt;
}

u16 AD7699_GetSingleData(AD7699CONFIG* self, u8 channel)
{
	u16 buf = 0;
    CPU_SR_ALLOC();
    CPU_INT_DIS();
	SetNSS(self, 0);
	self->adusedspi->Instance->DR = 0xF124|(channel<<9);
	while(!__HAL_SPI_GET_FLAG(self->adusedspi, SPI_FLAG_RXNE));
	buf = self->adusedspi->Instance->DR;
	SetNSS(self, 1);
	delayUs(2);	
	SetNSS(self, 0);
	self->adusedspi->Instance->DR = 0x7124|(channel<<9);
	while(!__HAL_SPI_GET_FLAG(self->adusedspi, SPI_FLAG_RXNE));
	buf = self->adusedspi->Instance->DR;//if(self->mapping[channel]==1)printf(" %d ",buf);
	SetNSS(self, 1);
	delayUs(2);
    SetNSS(self, 0);
	self->adusedspi->Instance->DR = 0x7124|(channel<<9);
	while(!__HAL_SPI_GET_FLAG(self->adusedspi, SPI_FLAG_RXNE));
	buf = self->adusedspi->Instance->DR;//if(self->mapping[channel]==1)printf(" %d ",buf);
	SetNSS(self, 1);
	delayUs(2);
    CPU_INT_EN();
	return  buf;
}

/**
 * @brief 根据平均次数获取所有通道的数据
 * 
 * @param self 
 * @param adData 最终获取的数据
 * @param buff 平均数据的缓存区
 * @param buffer_size 
 * @return u16 
 */
void AD7699AllRead_average(AD7699CONFIG* self,u16 *adData, u8 average_cnt)
{
	for(int i=0;i<self->channel_cnt;i++)
	{
		adData[i] =  AD7699_GetData(self, self->mapping[i], average_cnt);	
	}
}

u16 AD7699AllRead(AD7699CONFIG* self,u16 *adData)
{
	
	for(int i=0;i<self->channel_cnt;i++)
	{
		adData[i] =  AD7699_GetSingleData(self, self->mapping[i]) ;	
	}
	return 0;
}










