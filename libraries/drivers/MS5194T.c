#include "MS5194T.h"
#include "delayus.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_spi.h"

static u8 MS5194T_Read8b(MS5194TCONFIG* self, u8 REGISTER);

static u8 Spi_MS5194T_ReadWriteByte(MS5194TCONFIG* self, u8 TxData);

static void MS5194T_Write16b(MS5194TCONFIG* self, u8 REGISTER, u16 value);
	
static u32 MS5194T_Read24b(MS5194TCONFIG* self, u8 REGISTER);

void MS5194T_Init(MS5194TCONFIG* self, SPI_HandleTypeDef * spi,u8 channel_cnt,NssFunc nssf)
{
	if(self!= NULL&&spi!=NULL&&nssf!=NULL)
	{
		self->adusedspi=spi;
		self->nssfunc=nssf;
		self->channel_cnt=channel_cnt;
		
		__HAL_SPI_ENABLE(self->adusedspi);
		
		Spi_MS5194T_ReadWriteByte(self, 0xFF);
		Spi_MS5194T_ReadWriteByte(self, 0xFF);
		Spi_MS5194T_ReadWriteByte(self, 0xFF);
		Spi_MS5194T_ReadWriteByte(self, 0xFF);
		Spi_MS5194T_ReadWriteByte(self, 0xFF);
		Spi_MS5194T_ReadWriteByte(self, 0xFF);//连续写入至少32个1来复位
		
		delayMs(500);//复位后等待500us才能访问内部寄存器
		
		MS5194T_Write16b(self, W_MODE, 0x000F);
		
	}
	else
	{
		printf("This is not config of MS5194T");
		for(;;);			
	}
	MS5194T_GetData(self,0);
	printf("MS5194T initial finish!\r\n");
}

static void SetNSS(MS5194TCONFIG* self, int v)
{
	(*self->nssfunc)(v);
}

f32 MS5194T_GetData(MS5194TCONFIG* self, u8 channel_id)
{
//	u32 time1 = OSTimeGet();
	
	MS5194T_Write16b(self, W_CONFIGURATION, 0x0710|channel_id);
//	printf("config_reg_data = %x\r\n",0x0710|channel_id);
	
	while(MS5194T_Read8b(self, 0x40)&0x80);
	
	int Temp_32b = MS5194T_Read24b(self, R_DATA);
	
	while(MS5194T_Read8b(self, 0x40)&0x80);
	
	Temp_32b = MS5194T_Read24b(self, R_DATA);
	
	double value = Temp_32b;
	value /= 64.0f;
//	printf("value_%d = %f\r\n",channel_id, value);
//	printf("t = %d\r\n",OSTimeGet()-time1);
	return value;
}



static u8 Spi_MS5194T_ReadWriteByte(MS5194TCONFIG* self, u8 TxData)
{
	while(!__HAL_SPI_GET_FLAG(self->adusedspi, SPI_SR_TXE));
	self->adusedspi->Instance->DR = TxData;
	while(!__HAL_SPI_GET_FLAG(self->adusedspi, SPI_FLAG_RXNE));
	return self->adusedspi->Instance->DR;
}

static u8 MS5194T_Read8b(MS5194TCONFIG* self, u8 REGISTER)
{
	u8 temp = 0;
	self->nssfunc(0);
	Spi_MS5194T_ReadWriteByte(self, REGISTER);
	temp = Spi_MS5194T_ReadWriteByte(self, 0x80);
//	printf("temp-lsr = %d\r\n",temp);
	self->nssfunc(1);
	return temp;
}

static u16 MS5194T_Read16b(MS5194TCONFIG* self, u8 REGISTER)
{
	u16 temp = 0;
	self->nssfunc(0);
	Spi_MS5194T_ReadWriteByte(self, REGISTER);
	temp = Spi_MS5194T_ReadWriteByte(self, 0x80);
	temp = temp << 8;
	temp += Spi_MS5194T_ReadWriteByte(self, 0x80);
	self->nssfunc(1);
	return temp;
}

static u32 MS5194T_Read24b(MS5194TCONFIG* self, u8 REGISTER)
{
	u32 temp = 0;
	self->nssfunc(0);
	OSTimeDly(10);
	Spi_MS5194T_ReadWriteByte(self, REGISTER);
	temp = Spi_MS5194T_ReadWriteByte(self, 0x80);
	temp = temp << 8;
	temp += Spi_MS5194T_ReadWriteByte(self, 0x80);
	temp = temp << 8;
	temp += Spi_MS5194T_ReadWriteByte(self, 0x80);
	self->nssfunc(1);
	return temp;
}

static void MS5194T_Write16b(MS5194TCONFIG* self, u8 REGISTER, u16 value)
{
    self->nssfunc(0);
    Spi_MS5194T_ReadWriteByte(self, REGISTER);
    Spi_MS5194T_ReadWriteByte(self, value >> 8);
    Spi_MS5194T_ReadWriteByte(self, value);
    self->nssfunc(1);
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
//void MS5194TAllRead_average(MS5194TCONFIG* self,u16 *adData, u8 average_cnt)
//{
//	for(int i=0;i<self->channel_cnt;i++)
//	{
//		adData[i] =  MS5194T_GetData(self, i, average_cnt);	
//	}
//}

//u16 MS5194TAllRead(MS5194TCONFIG* self,u16 *adData)
//{
//	
//	for(int i=0;i<self->channel_cnt;i++)
//	{
//		adData[i] =  MS5194T_GetSingleData(self, i) ;	
//	}
//	return 0;
//}










