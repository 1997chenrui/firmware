
#include "PCA9685.h"

void Pca9685_Write(PCA9685 * pca,uint8_t adder,uint8_t data)
{
   pca->stopWR();
   pca->startWR();
   pca->writeByte(adder);
   pca->writeByte(data);
   pca->stopWR();
}

void Pca9685_Set_Pwm_L(PCA9685 * pca,uint8_t channel,float dutyCycle)
{
   pca->stopWR();
   pca->startWR();
	
	
	uint16_t on;
	uint16_t off;
	
	if(dutyCycle>100) dutyCycle=100;
	else if(dutyCycle<0) dutyCycle=0;
	

	off= (uint16_t)(40.96*(dutyCycle));
	
	on = 0;
	
	Pca9685_Write(pca,0x06+4*channel, on); // 
	
	Pca9685_Write(pca,0x07+4*channel, on>>8);//
	
	Pca9685_Write(pca,0x08+4*channel, off);//
	
	Pca9685_Write(pca,0x09+4*channel, off>>8);//
	
   pca->stopWR();
}

void Pca9685_Set_Pwm_H(PCA9685 * pca,uint8_t channel,float dutyCycle)
{
   pca->stopWR();
   pca->startWR();
	
	
	uint16_t on;
	uint16_t off;
	
	if(dutyCycle>100) dutyCycle=100;
	else if(dutyCycle<0) dutyCycle=0;
	
	
//	
	off=0;
	
	on = (uint16_t)(40.96*(dutyCycle));
	
	Pca9685_Write(pca,0x06+4*channel, on); // 
	
	Pca9685_Write(pca,0x07+4*channel, on>>8);//
	
	Pca9685_Write(pca,0x08+4*channel, off);//
	
	Pca9685_Write(pca,0x09+4*channel, off>>8);//
	
   pca->stopWR();
}

void Pca9685_Init(PCA9685 * pca)
{
   pca->stopWR();
   pca->startWR();

	Pca9685_Write(pca,0x00,0x00);
	pca->delay();
	Pca9685_Write(pca,0x00,0x10);
	pca->delay();
	
	
	double freq;
	
	freq=pca->freq;
	
	freq *= 0.9;  
	double prescaleval = 25000000;
						
	prescaleval /= 4096;
	prescaleval /= freq;
	prescaleval -= 1;

	Pca9685_Write(pca,0xFE,(uint8_t)prescaleval);
	
	Pca9685_Write(pca,0x00,0x00);
    pca->stopWR();
}

