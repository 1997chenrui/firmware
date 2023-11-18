#ifndef _PORT_0x00012101_H_
#define _PORT_0x00012101_H_
#include "RWByte.h"
#include "servoMotor.h"



typedef struct {
    u8 idx;
	u8 isRelative;
	f32 speed;
	f32 distance;
	u8 acce;
	u8 dcce;
	u8 b1;
	u8 b2;
	u8 b3;
	u8 b4;
	
} Port_0X00012101_t;

/*
	UNPACK 将总线的数据解包成需要的格式
	PACK 将需要返回的数据打包给返回数据类型
*/
#define   ADD_PORT_0X00012101() 									\
																	\
	void PORT_UNPACK_0X00012101(Port_0X00012101_t* self,u8* data)	\
	{																\
		uint8_t offset=0;											\
	    self->idx=data[offset];										\
		offset=offset+1;											\
		self->isRelative=data[offset];								\
		offset=offset+1;											\
		self->speed=ReadF32(&data[offset]);							\
		offset=offset+4;											\
		self->distance=ReadF32(&data[offset]);						\
		offset=offset+4;											\
		self->acce=data[offset];									\
		offset=offset+1;											\
		self->dcce=data[offset];									\
		offset=offset+1;											\
		self->b1=data[offset];										\
		offset=offset+1;											\
		self->b2=data[offset];										\
		offset=offset+1;											\
		self->b3=data[offset];										\
		offset=offset+1;											\
		self->b4=data[offset];										\
		offset=offset+1;											\
	}																\
	void PORT_PACK_0X00012101(SMotor_return* rt,u8 errcode)			\
	{																\
		rt->returnMem[0]=errcode;									\
		rt->returnSize=1;											\
	}	

		
	
#endif


