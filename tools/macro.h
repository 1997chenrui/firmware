#ifndef __MACRO_H__
#define __MACRO_H__

#include <stdbool.h>

extern void led_error_write(void);
	
#define     ASSERT(exp,log) \
	while(exp){        \
	    led_error_write();\
	    printf("FileName: %s  BUS e =%d,%s\r\n",__FILE__ , __LINE__ , log); \
		while(1);\
    } 
	
typedef unsigned char		u8;
typedef signed   char		s8;
typedef unsigned short		u16;
typedef signed   short		s16;
typedef unsigned int		u32;
typedef signed   int		s32;
typedef float				f32;

#endif
