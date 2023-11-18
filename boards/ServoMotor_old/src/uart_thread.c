#include "uart_thread.h"
#include "itf_00050000.h"
#include "uart_cfg.h"

extern Uart_thread_t uart_array[];

static Itf_00050000_t* cmd;
static void uart_send(u8 id, u8 *data, u16 data_len);
static void uart_rev_thread(void *data);

void uart_rev_thread_init(void (*thread_create)(void (*thread)(void* data))) {
    cmd = Itf_00050000_new();
    for(int i = 0;i < UART_TRANSPOND_CNT;i++){//根据转发的串口数量来生成转发的线程 
        uart_array[i].transpond = uart_transpond_new(i, cmd->send);
    }
    uart_thread_hd_init();//打开扫码枪串口中断
#if 1
    uart_send_test();
#endif
    thread_create(uart_rev_thread);
}

void uart_transpond_cmd_run(bus_packet* packet){
    Itf_00050000_content_t* content = cmd->packet_parse(packet);
    uart_send(content->id, content->data, content->len);
    cmd->rtn_and_content_del(content, 0);
}

void uart_send_test(){
    //试剂条码枪开
//    u8 data[8] = {0x16, 0x42, 0x65, 0x52, 0x65, 0x51, 0x62, 0x2E};
//    uart_send(0, data, 8);
    //样本条码枪开
    u8 data[4] = {0x02, 0x2b, 0x0d, 0x0a};
    uart_send(0, data, 4);    
    //样本条码枪开
    u8 data1[4] = {0x02, 0x2b, 0x0d, 0x0a};
    uart_send(1, data1, 4);
}

static void uart_rev_thread(void *data) {
    printf("uart rev thread run.\r\n");
    while (1) {
        bus_os_sleep(DAEMON_TIME_SPACE);//挂起一定的时间
        for (int i = 0; i < UART_TRANSPOND_CNT; i++) {
            if (uart_array[i].enable) {//如果转发串口列表中的某一个串口配置为开启
                uart_array[i].transpond->pack_rev_check(uart_array[i].transpond);
				//进行相应的接收检测 并转发至总线
            }
        }
    }
}

static void uart_send(u8 id, u8 *data, u16 data_len) {
	
	printf("new uart data send:%d\r\n",data_len);
	for(u16 i=0;i<data_len;i++)
	{
		printf("%x ",data[i]);
	}
	printf("\r\n");
    USART_TypeDef *UARTX = uart_array[id].uart->Instance;
    for (u16 i = 0; i < data_len; i++) {
        UARTX->DR = (data[i] & (u16)0x01FF);
        while ((UARTX->SR & UART_FLAG_TC) == (u16)RESET) {
        }
    }
}
