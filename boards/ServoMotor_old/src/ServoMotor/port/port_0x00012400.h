#ifndef _PORT_0x00012400_H_
#define _PORT_0x00012400_H_
#include "RWByte.h"
#include "servoMotor.h"



typedef struct {
    u8 idx;
	f32 acce;
	f32 speed;
	f32 overDistance;
	f32 afterDistance;
	u8 event;
} Port_0X00012400_t;

/*
	UNPACK 将总线的数据解包成需要的格式
	PACK 将需要返回的数据打包给返回数据类型
*/
#define   ADD_PORT_0X00012400() 									\
																	\
	void PORT_UNPACK_0X00012400(Port_0X00012400_t* self,u8* data)	\
	{																\
		uint8_t offset=0;											\
	    self->idx=data[offset];										\
		offset=offset+1;											\
		self->acce=ReadF32(&data[offset]);							\
		offset=offset+4;											\
		self->speed=ReadF32(&data[offset]);							\
		offset=offset+4;											\
		self->overDistance=ReadF32(&data[offset]);				    \
		offset=offset+4;											\
		self->afterDistance=ReadF32(&data[offset]);					\
		offset=offset+4;											\
	    self->event=data[offset];													\
		offset=offset+1;															\
	}																				\
	void PORT_PACK_0X00012400(SMotor_return* rt,u8 errcode,f32 triggerPosition)		\
	{																				\
		rt->returnMem[0]=errcode;													\
		WriteF32(&rt->returnMem[1],triggerPosition);								\
		rt->returnSize=1+4;															\
	}	

		
	
#endif


