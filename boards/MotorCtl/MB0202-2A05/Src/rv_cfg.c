#include "rv_cfg.h"
#include "stm32f4xx.h"

extern UART_HandleTypeDef huart3;

Rv_thread_t rv[RUNZE_CNT];

static bool queue0_pend(void);
static bool queue0_post(void);

static void uart0_bytes_send(u8 *data, u8 len);


void rv_hd_init(void){
    rv[0].queue_post = queue0_post;
    rv[0].queue_pend = queue0_pend;
    rv[0].uart_bytes_send = uart0_bytes_send;
    rv[0].thread_prio = 11;
    
 
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE);


}

static bool queue0_pend(void) {
    bus_os_pend_sem(&rv[0].queue_sem, 0);
    return true;
}

static bool queue0_post(void) {
    bus_os_post_sem(&rv[0].queue_sem);
    return true;
}




static void uart0_bytes_send(u8 *data, u8 len){
    HAL_UART_Transmit(&huart3, data, len, 0xff);
}



void USART3_IRQHandler(void)
{
	volatile int data = 0;
	data = USART3->SR;
	if(data & USART_SR_RXNE )
	{
		data = USART3->DR;
        rv[0].runze->byte_rev(rv[0].runze, data);
	}
	data = USART3->DR;
}



