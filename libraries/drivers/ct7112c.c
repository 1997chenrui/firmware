#include "ct7112c.h"
#include "stdio.h"
float CT7112Read_Temp(CT7112 *ct)
{
	ct->stopWR();
	ct->startWR();

	uint16_t value=0;
	bool neg_symbol=false;
	
	
	
	value=ct->readWord(0x00);
	
	if (value > 0x7fff)
    {
        neg_symbol = true;
        value = ~value;
        value &= 0xffff;
        value++;
    }
	
    value >>= 4;
	
	double temp=0;
    if(neg_symbol)
    {
        temp = (double)((float)value * -0.0625);
    }
    else
    {
        temp = (double)((float)value * 0.0625) ;
    }
//	float a=1.89;
//	float b=0.3;
	
	//printf("temp=%f\r\n",temp+b);
	
	return (float)temp;
}

