#include "ms1112.h"



uint16_t Ms1112_ReadLastValueAndStartNext(MS1112* self,uint8_t channel,uint8_t gain)
{
	//读取上一次温度
	self->stopWR();
	self->startWR();
	self->writebyte(self->address_read);
	
	uint8_t msb,lsb,reg;
	
	msb=self->readbyte(true);
	lsb=self->readbyte(true);
	reg=self->readbyte(false);
	
	(void)reg;
	uint16_t rt=(msb<<8)|lsb;
	
	
	
	//开启下一次转换
	self->stopWR();
	self->startWR();
	
	self->writebyte(self->address_write);
	
	uint8_t temp;
	/*
	|ST/DRDDY|INP1|INP0|SC|DR1|DR0|PGA1|PGA0|
	*/
	temp=0x9C|gain|(channel<<5);
	
	self->writebyte(temp);
	
	return rt;	
}

