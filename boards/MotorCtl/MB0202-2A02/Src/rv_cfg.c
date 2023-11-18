#include "rv_cfg.h"
#include "stm32f4xx.h"

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart4;

Rv_thread_t rv[RUNZE_CNT];

static bool queue0_pend(void);
static bool queue0_post(void);

static bool queue1_pend(void);
static bool queue1_post(void);

static void uart0_bytes_send(u8 *data, u8 len);
static void uart1_bytes_send(u8 *data, u8 len);

void rv_hd_init(void){
    rv[0].queue_post = queue0_post;
    rv[0].queue_pend = queue0_pend;
    rv[0].uart_bytes_send = uart0_bytes_send;
    rv[0].thread_prio = 11;
	
	rv[1].queue_post = queue1_post;
    rv[1].queue_pend = queue1_pend;
    rv[1].uart_bytes_send = uart1_bytes_send;
    rv[1].thread_prio = 12;
	
	__HAL_UART_CLEAR_FLAG(&huart3,UART_IT_RXNE);
	__HAL_UART_CLEAR_FLAG(&huart4,UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart4,UART_IT_RXNE);
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE);
}

static bool queue0_pend(void) {
    bus_os_pend_sem(&rv[0].queue_sem, 0);
    return true;
}

static bool queue1_pend(void) {
    bus_os_pend_sem(&rv[1].queue_sem, 0);
    return true;
}

static bool queue0_post(void) {
    bus_os_post_sem(&rv[0].queue_sem);
    return true;
}

static bool queue1_post(void) {
    bus_os_post_sem(&rv[1].queue_sem);
    return true;
}

static void uart0_bytes_send(u8 *data, u8 len){
	HAL_StatusTypeDef rt;
    rt=HAL_UART_Transmit(&huart4, data, len, 0xff);
	ASSERT(rt==HAL_TIMEOUT,"rv send error\r\n");
}

static void uart1_bytes_send(u8 *data, u8 len){
	HAL_StatusTypeDef rt;
    rt=HAL_UART_Transmit(&huart3, data, len, 0xff);
	ASSERT(rt==HAL_TIMEOUT,"rv send error\r\n");
}

void UART4_IRQHandler(void)
{
	volatile int data = 0;
	data = UART4->SR;
	if(data & USART_SR_RXNE )
	{
		data = UART4->DR;
        rv[0].runze->byte_rev(rv[0].runze, data);
	}
	data = UART4->DR;
}

void USART3_IRQHandler(void)
{
	volatile int data = 0;
	data = USART3->SR;
	if(data & USART_SR_RXNE )
	{
		data = USART3->DR;
        rv[1].runze->byte_rev(rv[1].runze, data);
	}
	data = USART3->DR;
}
