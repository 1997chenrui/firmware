#ifndef _PORT_0x00012161_H_
#define _PORT_0x00012161_H_
#include "RWByte.h"
#include "servoMotor.h"



typedef struct {
    u8 idx;
} Port_0X00012161_t;

/*
	UNPACK 将总线的数据解包成需要的格式
	PACK 将需要返回的数据打包给返回数据类型
*/
#define   ADD_PORT_0X00012161() 									\
																	\
	void PORT_UNPACK_0X00012161(Port_0X00012161_t* self,u8* data)	\
	{																\
		uint8_t offset=0;											\
	    self->idx=data[offset];										\
		offset=offset+1;											\
	}																\
	void PORT_PACK_0X00012161(SMotor_return* rt,u8 errcode)			\
	{																\
		rt->returnMem[0]=errcode;									\
		rt->returnSize=1;											\
	}	

		
	
#endif


