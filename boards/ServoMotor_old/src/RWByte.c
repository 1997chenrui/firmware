#include "RWByte.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


u16 ReadU16(u8* data) 
{
	u16 rt = 0;
	rt = data[0]|(data[1]<<8);
	return rt;
}

u32 ReadU32(u8* data)
{
	u32 rt = 0;
	rt =(u32) (data[0]|(data[1]<<8)|(data[2]<<16)|(data[3]<<24));//隐式转换
	//rt = u32 (data[0]|(data[1]<<8)|(data[2]<<16)|(data[3]<<24));显式转换
	return rt;
}

float ReadF32(u8 *data)
{
    float rt = 0;
    memcpy(&rt,data,4);
    return rt;
}

void WriteF32(u8 *data, f32 code)
{
    union
    {
        u8    data[sizeof(code)];
        f32   code;
    }t;
    t.code = code;
    memmove(data,t.data,sizeof(code));
}

// void ExchangeU32(u32 *a,u32 *b)
// {

// }

void WriteU32(u8 *d,u32 code)
{
    union
    {
        u8    data[sizeof(code)];
        u32   code;
    }t;
    t.code = code;
    memmove(d,t.data,sizeof(code));

}

void WriteU16(u8 *d,u16 code)
{
    union
    {
        u8    data[sizeof(code)];
        u16   code;
    }t;
    t.code = code;
    memmove(d,t.data,sizeof(code));
    
}

void reverse_bit(u32* value, u8 offset){
    u32 t = 1 << offset;
    u32 t1 = ~(*value & t);
    *value |= t;
    *value = *value & t1;
}