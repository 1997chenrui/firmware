#ifndef _AD5282_H_
#define _AD5282_H_


#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct ad5282
{
    uint8_t address_write;//器件地址
    uint8_t address_read;
    uint8_t value[2];//影子寄存器

    bool (*writeByte)(uint8_t );
    uint8_t (*readByte)(bool );
    void (*startWR)(void);
    void (*stopWR)(void);
} AD5282;



void AD5282_Write(AD5282 * ad,uint8_t channel,uint8_t data);
uint8_t AD5282_Read_shadow(AD5282 * ad,uint8_t channel);
uint8_t AD5282_Read(AD5282 * ad,uint8_t channel);


void AD5282_ShutDown(AD5282 * ad,uint8_t channel);

#endif
