#include "soft_iic.h"

void Soft_iic_start(SoftIIc *iic)
{
	if(iic->setSdaOut!=NULL)
	{
		iic->setSdaOut();
	}
	if(iic->setSclOut!=NULL)
	{
		iic->setSdaOut();
	}	
	iic->writeSda(true);
	iic->writeScl(true);
	iic->delay(5);
	iic->writeSda(false);
	iic->delay(5);
	iic->writeScl(false);
}

void Soft_iic_stop(SoftIIc *iic)
{
	if(iic->setSdaOut!=NULL)
	{
		iic->setSdaOut();
	}
	if(iic->setSclOut!=NULL)
	{
		iic->setSdaOut();
	}	
	iic->writeScl(true);
	iic->writeSda(false);
	iic->delay(5);
	iic->writeScl(true);
	iic->writeSda(true);
	iic->delay(5);
}

static SoftIIc_rtError Soft_iic_wait_ack(SoftIIc * iic)
{
	uint16_t timeout=0;
	if(iic->setSdaOut!=NULL)
	{
		iic->setSdaOut();
	}
	if(iic->setSclOut!=NULL)
	{
		iic->setSdaOut();
	}	
	iic->writeSda(true);
	iic->delay(2);
	iic->writeScl(true);
	iic->delay(2);

	if(iic->setSdaIn!=NULL)
	{
		iic->setSdaIn();
	}
	while (iic->readSda()==true)
	{
		if(timeout++ >iic->ack_timeout)
		{		
			Soft_iic_stop(iic);
			return Soft_iic_timeout;
		}
	}
	//iic->writeSda(true);
	iic->writeScl(false);	
	return  Soft_iic_normal;
	
}

static void Soft_iic_send_ack(SoftIIc * iic)
{
	if(iic->setSdaOut!=NULL)
	{
		iic->setSdaOut();
	}
	if(iic->setSclOut!=NULL)
	{
		iic->setSdaOut();
	}	
	iic->writeSda(false);
	iic->writeScl(false);
	iic->delay(2);
	iic->writeScl(true);
	iic->delay(2);
	iic->writeScl(false);
}

static void Soft_iic_send_nack(SoftIIc * iic)
{
	if(iic->setSdaOut!=NULL)
	{
		iic->setSdaOut();
	}
	if(iic->setSclOut!=NULL)
	{
		iic->setSdaOut();
	}	
	
	iic->writeSda(true);
	iic->writeScl(false);
	iic->delay(2);
	iic->writeScl(true);
	iic->delay(2);
	iic->writeScl(false);
}

SoftIIc_rtError Soft_iic_write(SoftIIc *iic , uint8_t data)
{
	if(iic->setSdaOut!=NULL)
	{
		iic->setSdaOut();
	}
	if(iic->setSclOut!=NULL)
	{
		iic->setSdaOut();
	}	
	iic->writeScl(false);
	for(uint8_t i = 0; i < 8; i++)
	{
		if((data&0x80 )>> 7)
			iic->writeSda(true);
		else
			iic->writeSda(false);
		
		data<<=1;
		iic->delay(5);
		iic->writeScl(true);
		iic->delay(5);
		iic->writeScl(false);
		iic->delay(5);
	}
	return Soft_iic_wait_ack(iic);

}

uint8_t Soft_iic_read(SoftIIc *iic ,bool ackEn)
{
	if(iic->setSdaIn!=NULL)
	{
		iic->setSdaIn();
	}
	if(iic->setSclOut!=NULL)
	{
		iic->setSdaOut();
	}	
	uint8_t data = 0;

	for(uint8_t i = 0; i < 8; i++)
	{
		iic->writeScl(false);
		iic->delay(2);
		iic->writeScl(true);
		data<<=1;
		if(iic->readSda()!=false)
		{
			data++;
		}
		iic->delay(1);			
	}
	
	if(ackEn)
		Soft_iic_send_ack(iic);
	else
		Soft_iic_send_nack(iic);
	return data;
}

