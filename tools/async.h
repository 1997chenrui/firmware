#ifndef ASYNC_H_
#define ASYNC_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ucos_ii.h"


#define   ASYNC_MAX_ARG_CNT     6       //异步函数最大参数个数

typedef struct
{
    void        *handler;
    void        *content[ASYNC_MAX_ARG_CNT];

}Async_Excute_t;        //执行件


void Async_Init(void);
bool Async_Excute(int argcnt,void *handler,...);







#endif

