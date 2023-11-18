#ifndef _CT7112C_H_
#define _CT7112C_H_


#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct ct7112
{
    uint8_t address_write;//器件地址
    uint8_t address_read;
 

    bool (*writeWord)(uint8_t );
    uint16_t (*readWord)(uint8_t );
	
    void (*startWR)(void);
    void (*stopWR)(void);
} CT7112;




float CT7112Read_Temp(CT7112 *ct);

#endif
