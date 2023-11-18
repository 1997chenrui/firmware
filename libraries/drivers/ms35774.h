#ifndef _MS35774_H_
#define _MS35774_H_
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct{
	uint8_t microstep;
}Ms35774cfg;

typedef struct ms35774
{
	Ms35774cfg cfg;
	
	void (*setDir)(bool);//外部需要实现
	void (*setPdn)(bool);
	
}MS35774;

#endif

