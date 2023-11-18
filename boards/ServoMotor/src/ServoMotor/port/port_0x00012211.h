#ifndef _PORT_0x00012211_H_
#define _PORT_0x00012211_H_
#include "RWByte.h"
#include "servoMotor.h"


#define Port_0X00012211_one_t_size 15

typedef struct {
	u32 count;
	u8 *data;
	u32 delayMs;
} Port_0X00012211_t;

typedef struct {
	u32 count;
	u8 *data;
} Port_0X00012211_Arg_t;

typedef struct {
    u8 idx;
	f32 acce;
	f32 speed;
	f32 distance;
	u8 isRelative;
	u8 type;
} Port_0X00012211_one_t;

/*
	UNPACK 将总线的数据解包成需要的格式
	PACK 将需要返回的数据打包给返回数据类型
*/
#define   ADD_PORT_0X00012211() 												\
																				\
	void PORT_UNPACK_0X00012211(Port_0X00012211_t* self,u8* data)				\
	{																			\
		uint8_t offset=0;														\
		self->delayMs=ReadU32(&data[offset]);									\
		offset=offset+4;														\
		self->count=ReadU32(&data[offset]);										\
		offset=offset+4;														\
		self->data=&data[offset];												\
	}																			\
	void PORT_PACK_0X00012211(SMotor_return* rt,u8 errcode)						\
	{																			\
		rt->returnMem[0]=errcode;												\
		rt->returnSize=1;														\
	}																			\
	void PORT_UNPACK_0X00012211_Arg(Port_0X00012211_Arg_t* self,u8* data)		\
	{																			\
		uint8_t offset=0;														\
		self->count=ReadU32(&data[offset]);										\
		offset=offset+4;														\
		self->data=&data[offset];												\
	}																			\
	void PORT_UNPACK_0X00012211_ONE(Port_0X00012211_one_t* self,u8* data)		\
	{																			\
		uint8_t offset=0;														\
	    self->idx=data[offset];													\
		offset=offset+1;														\
		self->acce=ReadF32(&data[offset]);										\
		offset=offset+4;														\
		self->speed=ReadF32(&data[offset]);										\
		offset=offset+4;														\
		self->distance=ReadF32(&data[offset]);									\
		offset=offset+4;														\
		self->isRelative=data[offset];											\
		offset=offset+1;														\
		self->type=data[offset];												\
		offset=offset+1;														\
	}	
		
	
#endif


