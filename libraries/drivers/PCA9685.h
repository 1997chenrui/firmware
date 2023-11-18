#ifndef _PCA9685_H_
#define _PCA9685_H_
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct pca9685
{
    uint8_t address_write;//器件地址
    uint8_t address_read;
	
	float freq;//
  
    bool (*writeByte)(uint8_t );
    uint8_t (*readByte)(bool );
    void (*startWR)(void);
    void (*stopWR)(void);
	
	void (*delay)(void);
	
} PCA9685;


void Pca9685_Set_Pwm_L(PCA9685 * pca,uint8_t channel,float dutyCycle);

void Pca9685_Set_Pwm_H(PCA9685 * pca,uint8_t channel,float dutyCycle);

void Pca9685_Init(PCA9685 * pca);



#endif
