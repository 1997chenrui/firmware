#ifndef TOOL_H_
#define TOOL_H_
#include "macro.h"
#include <stdbool.h>
#include <stdint.h>
u16 ReadU16(u8* data) ;
u32 ReadU32(u8* data);
void ExchangeU32(u32 *a,u32 *b);
void WriteU32(u8 *d,u32 code);
void WriteU16(u8 *d,u16 code);
int32_t ReadI32(u8* data);
float ReadF32(u8 *data);
void WriteF32(u8 *d,f32 code);
u32 crc32(u32 crc, unsigned char *buff, int len);
u16 crc16(unsigned char *q, int len);
int average(void * data,int data_size,int len);
void reverse_bit(u32* value, u8 offset);
float median_filter_float(float *pData, int nSize);



// void NVIC_Close(int cnt,...);   //关闭所有的中断除了这几个之外
// void NVIC_Open(int cnt,...);   
// void NVIC_SystickEnable(void);
// void NVIC_SystickDisable(void);



#endif

