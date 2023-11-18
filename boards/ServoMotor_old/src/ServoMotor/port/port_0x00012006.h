#ifndef _PORT_0X00012006_H_
#define _PORT_0X00012006_H_

#include "tool.h"
#include "servoMotor.h"
#include "stdint.h"

typedef struct {
    u8 idx;
} Port_0X00012006_t;

typedef struct {
	u8 errcode;
	f32 speed;
	f32 position;
}Port_0X00012006_rt;
/*
	UNPACK 将总线的数据解包成需要的格式
	PACK 将需要返回的数据打包给返回数据类型
*/
#define   ADD_PORT_0X00012006()  									\
																	\
	void PORT_UNPACK_0X00012006(Port_0X00012006_t* self,u8* data)	\
	{																\
		uint8_t offset=0;											\
	    self->idx=data[offset];										\
		offset=offset+1;											\
	}																\
	void PORT_PACK_0X00012006(SMotor_return* rt,Port_0X00012006_rt const *data)			\
	{																\
		uint8_t offset=0;											\
		rt->returnMem[offset]=data->errcode;						\
		offset+=1;													\
		WriteF32(&rt->returnMem[offset],data->speed);				\
		offset+=4;													\
		WriteF32(&rt->returnMem[offset],data->position);			\
		offset+=4;													\
		rt->returnSize=offset;										\
	}																
		
#endif


