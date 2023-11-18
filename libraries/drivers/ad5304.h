#ifndef _AD5304_H_
#define _AD5304_H_


#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct ad5304
{

 
	void (*writeScl)(bool);
	void (*writeSda)(bool);
	bool (*readSda)(void);
	void (*writeSync)(bool);
	
	void (*delay)(uint32_t);

} AD5304;




void Ad5304_WriteData(AD5304 *self,uint8_t channel,uint8_t data);
#endif
