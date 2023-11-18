#include "LS165_HC595.h"
#include "delayus.h"
#include "LS165_HC595_Cfg.h"

volatile uint32_t HC595_Data = 0;

void HC595_WriteData(uint32_t *state,uint8_t cnt,uint32_t data)
{
	*state = data;
	HC595_Write_Oe_Pin(0);
	HC595_Write_Clk_Pin(0);

	delayUs(5);
	HC595_Write_Clk_Pin(1);
	delayUs(5);
	HC595_Write_Clk_Pin(0);
	delayUs(5);
	HC595_Write_Clk_Pin(1);
	delayUs(5);
	for(uint8_t j=0; j<cnt; j++)
	{	
		HC595_Write_Oe_Pin(1);
		HC595_Write_Clk_Pin(0);
		HC595_Write_Data_Pin(data&0x01);
		data = data >> 1;
		delayUs(5);
		HC595_Write_Oe_Pin(0);
		HC595_Write_Clk_Pin(1);
		delayUs(5);
	}
}

void HC595_WriteBit(uint32_t *state,uint8_t cnt,uint8_t BitId,uint8_t val)
{
	*state &= ~(1<<BitId);
	*state |= val<<BitId;

	HC595_WriteData(state,cnt,*state);
}
void HC595_ToggleBit(uint32_t *state,uint8_t cnt,uint8_t BitId)
{
	uint8_t val = 0;
	val = ~((*state >> BitId)&1);
	HC595_WriteBit(state,cnt,BitId,val);
}


uint32_t LS165_ReadData(uint8_t cnt)
{
	u32 DATA = 0;
	u32 DATA_165 = 0;

	LS165_Write_Load_Pin(0);
	delayUs(30);
	LS165_Write_Load_Pin(1);
	
	for(uint8_t i=0; i<cnt; i++)
	{ 
		LS165_Write_Clk_Pin(0);
		delayUs(5);
		DATA_165 = LS165_Read_Data_Pin();
		LS165_Write_Clk_Pin(1);
		DATA = DATA << 1;
		DATA |= DATA_165;
		delayUs(5);
	}
	return DATA;
}
uint8_t LS165_ReadBit(uint8_t cnt,uint8_t BitId)
{
	return ((LS165_ReadData(cnt)>>BitId) & 0x01);
}
