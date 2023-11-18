#ifndef _MS1112_H_
#define _MS1112_H_


#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct ms1112
{
    uint8_t address_write;//器件地址
    uint8_t address_read;
 

    bool (*writebyte)(uint8_t );
    uint8_t (*readbyte)(bool );
	
    void (*startWR)(void);
    void (*stopWR)(void);
} MS1112;




uint16_t Ms1112_ReadLastValueAndStartNext(MS1112* self,uint8_t channel,uint8_t gain);

#endif

