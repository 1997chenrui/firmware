#ifndef _LISTENANDSYNC_CFG_H_
#define _LISTENANDSYNC_CFG_H_
#include "listenandsync.h"

#include "stm32f4xx_hal.h"
#include "macro.h"

#define LISTGPIOCNT 4
#define USED_TIM 2

extern SyncGpio listenGpio_0_sync[2];
extern ListenGpio listenGpio[LISTGPIOCNT];
extern void ListenAndSync_Init_(void);

#endif



