#include "ucos_ii.h"
#include <string.h>
#include <stdio.h>
#include "stm32f1xx_hal.h"

typedef  OS_CPU_SR *     SEMAPHORE_ID;

int _mutex_initialize(SEMAPHORE_ID sid)
{
   /* Create a mutex semaphore */
//     sid = CreateLock(...);
    *sid = NULL;
    return 1;
}

void _mutex_acquire(SEMAPHORE_ID sid)
{
    /* Task sleep until get semaphore */
//     AcquireLock(sid, ...);
    OS_CPU_SR cpu_sr;
    OS_ENTER_CRITICAL();
    *sid = cpu_sr;
}

void _mutex_release(SEMAPHORE_ID sid)
{
    OS_CPU_SR cpu_sr = *sid;
    OS_EXIT_CRITICAL();
}

void _mutex_free(SEMAPHORE_ID sid)
{
    /* Free the semaphore. */
//     FreeLock(sid, ...);
    printf("_mutex_free\r\n");
}


void malloc_safe_insure(void)
{
    int a = (int)&_mutex_initialize + 
            (int)&_mutex_acquire +
            (int)&_mutex_release +
            (int)&_mutex_free ;
        
    printf("Start %d \r\n",a);      //确保这些线程安全的东西被引用到
    
}
void HardFault_Handler(void)
{
	while (1)
	{
		printf("HardFault_Handler\r\n");
		//NVIC_SystemReset();
		for(unsigned short nms = 0; nms < 1000; nms++)
		{
			for(unsigned short ms = 0; ms < 4050; ms++)
			{
				// 大致延时1s
			}
		}
	}
}





#include <stdio.h>
#include "stm32f1xx_hal.h"
#include <rt_misc.h>
#pragma import(__use_no_semihosting_swi)



struct __FILE
{
    int handle;
};
 
FILE __stdout;
extern UART_HandleTypeDef huart1;


int fputc(int ch, FILE *f)
{

    USART1->DR = ch;
    while( !(USART1->SR & USART_SR_TC));
    
    
    return (ch);
}


int ferror(FILE *f) {
  /* Your implementation of ferror */
  return EOF;
}


void _ttywrch(int ch) {
    uint8_t data = ch;
    HAL_UART_Transmit(&huart1,&data,1,0xffff);
}


void _sys_exit(int return_code) {
    label:  goto label;  /* endless loop */
}







// Note
// _mutex_release() releases the lock on the mutex that was acquired by _mutex_acquire(), but the mutex still exists, and can be re-locked by a subsequent call to _mutex_acquire().

// It is only when the optional wrapper function _mutex_free() is called that the mutex is destroyed. After the mutex is destroyed, it cannot be used without first calling _mutex_initialize() to set it up again.

// Show/hideSee also
// Tasks
// How to ensure re-implemented mutex functions are called

// Using the ARM C library in a multithreaded environment.

// Concepts
// Thread safety in the ARM C library

// Thread safety in the ARM C++ library.

// Copyright 2007-2008, 2011-2012 ARM. All rights reserved. ARM DUI 0378D 
// Non-Confidential ID062912 
// PDF file icon  PDF version 
// Home > The ARM C and C++ libraries > Management of locks in multithreaded applications 



