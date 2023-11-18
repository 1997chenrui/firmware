#include "ad5304.h"


void Ad5304_WriteData(AD5304 *self,uint8_t channel,uint8_t data)
{
	
	/*********************************************************
	|A1|A0|PD-|LDAC-|D7|D6|D5|D4|D3|D2|D1|D0|D7|0|0|x|x|
	***********************************************************/
	
	uint16_t data_temp;
	uint16_t channel_temp;
	
	data_temp=(uint16_t)data <<4 ;
	channel_temp=(((uint16_t)channel<<2)|0x02)<<12 ;
	
	
	data_temp|=channel_temp;
	
	self->writeScl(0);
	self->delay(10);
	self->writeSync(1);
	self->delay(10);
	self->writeSync(0);
	self->delay(10);
	
	for(uint8_t i=0;i<16;i++)
	{
		if((data_temp<<i)&0x8000)
		{
			self->writeSda(1);
		}
		else
		{
			self->writeSda(0);
		}
		self->writeScl(1);
		self->delay(10);
		self->writeScl(0);
		self->delay(10);
	}
	self->writeSync(1);
	self->delay(100);
}

