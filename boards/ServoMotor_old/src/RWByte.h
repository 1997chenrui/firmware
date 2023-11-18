#ifndef _R_W_BYTE_H_
#define _R_W_BYTE_H_
#include <stdbool.h>
#include <stdint.h>
typedef unsigned char		u8;
typedef signed   char		s8;
typedef unsigned short		u16;
typedef signed   short		s16;
typedef unsigned int		u32;
typedef signed   int		s32;
typedef float				f32;

u16 ReadU16(u8* data) ;
u32 ReadU32(u8* data);
s32 ReadS32(u8* data);
void ExchangeU32(u32 *a,u32 *b);
void WriteU32(u8 *d,u32 code);
void WriteS32(u8 *d,s32 code);
void WriteU16(u8 *d,u16 code);
float ReadF32(u8 *data);
void WriteF32(u8 *d,f32 code);
#endif