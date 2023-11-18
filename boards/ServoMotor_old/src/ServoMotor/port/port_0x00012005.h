#ifndef _PORT_0X00012005_H_
#define _PORT_0X00012005_H_

#include "tool.h"
#include "servoMotor.h"
#include "stdint.h"

typedef struct {
    u8 idx;
} Port_0X00012005_t;

typedef struct {
	u8 errcode;
	u8 drive_type;
	u8 maxI;
	u8 minI;
	u8 step_per_fs;
	f32 step_ratio;
	f32 zero_distance;//零位距离
	u8  restEventPol;//复位开关电平
	u8  defaultDir;//默认电机方向  
	u32 encoderAccuracy; //编码器精度
	f32 max_distance;
	f32 max_distance_re;
}Port_0X00012005_rt;
/*
	UNPACK 将总线的数据解包成需要的格式
	PACK 将需要返回的数据打包给返回数据类型
*/
#define   ADD_PORT_0X00012005()  									\
																	\
	void PORT_UNPACK_0X00012005(Port_0X00012005_t* self,u8* data)	\
	{																\
		uint8_t offset=0;											\
	    self->idx=data[offset];										\
		offset=offset+1;											\
	}																\
	void PORT_PACK_0X00012005(SMotor_return* rt,Port_0X00012005_rt const *data)			\
	{																\
		uint8_t offset=0;											\
		rt->returnMem[offset]=data->errcode;						\
		offset+=1;													\
		rt->returnMem[offset]=data->drive_type;						\
		offset+=1;													\
		rt->returnMem[offset]=data->maxI;							\
		offset+=1;													\
		rt->returnMem[offset]=data->minI;							\
		offset+=1;													\
		rt->returnMem[offset]=data->step_per_fs;					\
		offset+=1;													\
		WriteF32(&rt->returnMem[offset],data->step_ratio);			\
		offset+=4;													\
		WriteF32(&rt->returnMem[offset],data->zero_distance);		\
		offset+=4;													\
		rt->returnMem[offset]=data->restEventPol;					\
		offset+=1;													\
		rt->returnMem[offset]=data->defaultDir;						\
		offset+=1;													\
		WriteU32(&rt->returnMem[offset],data->encoderAccuracy);		\
		offset+=4;													\
		WriteF32(&rt->returnMem[offset],data->max_distance);		\
		offset+=4;													\
		WriteF32(&rt->returnMem[offset],data->max_distance_re);		\
		offset+=4;													\
		rt->returnSize=offset;										\
	}																
		
#endif


