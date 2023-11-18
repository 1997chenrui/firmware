#include "ad5282.h"

void AD5282_Write(AD5282 * ad,uint8_t channel,uint8_t data)
{
   ad->stopWR();
   ad->startWR();
   if(channel==0)
   {
        ad->writeByte(0x00);
   }
   else
   {
        ad->writeByte(0x80);
   }
   ad->writeByte(data);

   ad->stopWR();

}

void AD5282_Rest(AD5282 * ad,uint8_t channel)
{
    uint8_t cmd=0x40;
    ad->startWR();

    if(channel!=0)
    {
        cmd|=0x80;
    }
    ad->writeByte(cmd);
    cmd=AD5282_Read_shadow(ad,channel);
    ad->writeByte(cmd);

    ad->stopWR();
}

void AD5282_ShutDown(AD5282 * ad,uint8_t channel)
{
    uint8_t cmd=0x20;
    ad->startWR();
    if(channel!=0)
    {
        cmd|=0x80;
    }
    ad->writeByte(cmd);
    cmd=AD5282_Read_shadow(ad,channel);
    ad->writeByte(cmd);

    ad->stopWR();
}


uint8_t AD5282_Read_shadow(AD5282 * ad,uint8_t channel)
{
    return ad->value[channel];
}
uint8_t AD5282_Read(AD5282 * ad,uint8_t channel)
{
    return ad->value[channel];
}





