#include "uart_transfer.h"
#include "Protocols.h"
#include "data_struct.h"
#include "stm32f4xx_hal.h"
#include "main_cfg.h"
#include "bus.h"
#define UART_SEND_THREAD_STACK_SIZE 				256
#define UART_SEND_THREAD_PRIO						10

#define UARTRECEIVE_DAEMONTHREAD_STACK_SIZE 	    256
#define UARTRECEIVE_DAEMONTHREAD_PRIO				12

               
void UartReceive_Daemon_Thread(void *arg);

#ifdef UART1_RT_EN
extern UART_HandleTypeDef huart1;
UartReceive_Struct uartrs1 = {.fifo.empty = 1};
#endif
#ifdef UART2_RT_EN
extern UART_HandleTypeDef huart2;
UartReceive_Struct uartrs2 = {.fifo.empty = 1};
#endif
#ifdef UART3_RT_EN
extern UART_HandleTypeDef huart3;
UartReceive_Struct uartrs3 = {.fifo.empty = 1};
#endif
#ifdef UART4_RT_EN
extern UART_HandleTypeDef huart4;
UartReceive_Struct uartrs4 = {.fifo.empty = 1};
#endif
#ifdef UART5_RT_EN
extern UART_HandleTypeDef huart5;
UartReceive_Struct uartrs5 = {.fifo.empty = 1};
#endif
#ifdef UART6_RT_EN
extern UART_HandleTypeDef huart6;
UartReceive_Struct uartrs6 = {.fifo.empty = 1};
#endif
	
static struct
{
    bus_os_thread            tcb;
    bus_os_sem               sem;
    u32                    	 stack[UARTRECEIVE_DAEMONTHREAD_STACK_SIZE];
}UartReceive_DaemonThread_Cfg;

static struct
{
    bus_os_thread            tcb;
    bus_os_sem               sem;
    u32                    	 stack[UART_SEND_THREAD_STACK_SIZE];
}UartSend_Cfg;

SeqQueue_Struct	uartsend_queqe;
static bool uartsend_queue_pend(void);                                       
static bool uartsend_queue_post(void); 

#define  UART_SEND_THRAD_CREATE()        \
            bus_create_thread(&UartSend_Cfg.tcb,UartSend_Thread,         \
                                UartSend_Cfg.stack, UART_SEND_THREAD_STACK_SIZE, UART_SEND_THREAD_PRIO,NULL);\
            bus_os_create_sem(&UartSend_Cfg.sem,0);            \
            SeqQueue_Init(&uartsend_queqe,sizeof(UartSend_Queue_Ele_t),&uartsend_queue_pend,&uartsend_queue_post)

void UartSend_Thread(void *arg);

#define  UART_RECEIVE_DAEMONTHRAD_CREATE()        \
            bus_create_thread(&UartReceive_DaemonThread_Cfg.tcb,UartReceive_Daemon_Thread,         \
                                UartReceive_DaemonThread_Cfg.stack, UARTRECEIVE_DAEMONTHREAD_STACK_SIZE, UARTRECEIVE_DAEMONTHREAD_PRIO,NULL);
						

void UartTransfer_Init(void)
{
	UART_SEND_THRAD_CREATE();
	UART_RECEIVE_DAEMONTHRAD_CREATE();
#ifdef UART1_RT_EN
    __HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);
#endif
#ifdef UART2_RT_EN
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_RXNE);
#endif
#ifdef UART3_RT_EN
    __HAL_UART_ENABLE_IT(&huart3,UART_IT_RXNE);
#endif
#ifdef UART4_RT_EN
	__HAL_UART_ENABLE_IT(&huart4,UART_IT_RXNE);
#endif
#ifdef UART5_RT_EN
	__HAL_UART_ENABLE_IT(&huart5,UART_IT_RXNE);
#endif
#ifdef UART6_RT_EN
	__HAL_UART_ENABLE_IT(&huart6,UART_IT_RXNE);
#endif
}

u8 uartWriteFifo(UartFifo_Struct * ths, u8* buff, u16 length)
{
	if (UARTFIFO_CNT - ths->size < length)
	{
		ths->size = UARTFIFO_CNT;
		ths->full = 1;
		printf("[error]fifo full\r\n");
		return 1;
	}
	else
	{
		if (ths->wr_p + length > UARTFIFO_CNT - 1)
		{
			memmove(ths->buff + ths->wr_p, buff, UARTFIFO_CNT - 1 - ths->wr_p);
			memmove(ths->buff, buff + UARTFIFO_CNT - 1 - ths->wr_p, ths->wr_p + length - (UARTFIFO_CNT - 1));
			ths->wr_p = ths->wr_p + length - (UARTFIFO_CNT - 1);
		}
		else
		{
			memmove(ths->buff + ths->wr_p, buff, length);
			ths->wr_p += length;
		}
		ths->size += length;
		ths->empty = 0;
		if (ths->size == UARTFIFO_CNT)
		{
			ths->full = 1;
			
			
		}
		
	}
	return 0;
}
u16 uartReadFifo(UartFifo_Struct* ths, u8 *buff, u16 length)
{
	if (ths->size < length)
	{
		ths->size = 0;
		ths->empty = 1;
		printf("           [error]fifo empty\r\n");
		return 1;
	}
	else
	{
		if (ths->rd_p + length > UARTFIFO_CNT - 1)
		{
			memmove(buff, ths->buff + ths->rd_p, UARTFIFO_CNT - 1 - ths->rd_p);
			memmove(buff + UARTFIFO_CNT - 1 - ths->rd_p, ths->buff, ths->rd_p + length - (UARTFIFO_CNT - 1));
			ths->rd_p = ths->rd_p + length - (UARTFIFO_CNT - 1);
		}
		else
		{
			memmove(buff, ths->buff + ths->rd_p, length);
			ths->rd_p += length;
		}
		ths->size -= length;
		if (ths->wr_p == ths->rd_p)
		{
			ths->empty = 1;
		}
	}
	return 0;
}

static void UartSendBytes(USART_TypeDef *UARTX, u8 *data,uint32_t data_len)
{
	for(uint32_t i = 0;i<data_len;i++)
	{
		UARTX->DR = (data[i] & (uint16_t)0x01FF);
		while((UARTX->SR & UART_FLAG_TC) == (uint16_t)RESET){}
	}
}

static USART_TypeDef * Id2Uart(u8 id)
{
	USART_TypeDef *temp_uart = NULL;
	switch(id)
	{
		case 1:temp_uart = USART1;break;
		case 2:temp_uart = USART2;break;
		case 3:temp_uart = USART3;break;
		case 4:temp_uart = UART4;break;
		case 5:temp_uart = UART5;break;
		case 6:temp_uart = USART6;break;
		default:printf("uart parameter error\r\n");break;
	}
	return temp_uart;
}
void UartReceiveEvent(UartReceive_Struct *ths,u8 uartId,u8 data)
{
	uartWriteFifo(&ths->fifo, &data, 1);
	ths->daemon_time = 0;
}

static void  UartReceiveTransfer(u8 uartId,u8 *data,u32 len)
{
	u32 byteOfData = 4+sizeof(len)+1+len;
	u32 bytesOfDataOut = 0;
	u32 dataOut = 0;
	u8 *send = malloc(byteOfData);
	if(send == NULL)
	{
		printf("UartReceiveTransfer malloc error\r\n");
		return;
	}
	WriteU32(send,0x34);
	send[4] = uartId;
	WriteU32(send+sizeof(u32)+1,len);
	memmove(send+sizeof(u32)+1+sizeof(len),data,len);
	UART_TRANSFER_SEND(byteOfData,send,(u8*)(&dataOut),&bytesOfDataOut);
    for(int i = 0;i<byteOfData;i++)
    {
        printf("%X ",*(send+i));
    }
    printf("\r\n send over \r\n");
	free(send);
}

void UartReceiveTriggle(UartReceive_Struct *ths,u8 uartId)
{
	// sunly添加BOX项目需要结束符来区分此条数据结束   
	if(ths->daemon_time>DAEMON_TIME_CNT || ths->fifo.size>DAEMON_SEND_THRESHOLD)
	{
		if(ths->fifo.size>DAEMON_SEND_THRESHOLD)
		{	
			u8 *temp_buff = (u8 *)malloc(DAEMON_SEND_THRESHOLD);
			uartReadFifo(&ths->fifo, temp_buff, DAEMON_SEND_THRESHOLD);
			
			UartReceiveTransfer(uartId,temp_buff,DAEMON_SEND_THRESHOLD);
			free(temp_buff);
		}
		/******当超时，且fifo.size大于设定值时，转发数据并加上结束符*******/
		else if(ths->daemon_time>DAEMON_TIME_CNT && ths->fifo.size>DAEMON_SEND_THRESHOLD)
		{
			u8 *temp_buff = (u8 *)malloc(DAEMON_SEND_THRESHOLD);
			uartReadFifo(&ths->fifo, temp_buff, DAEMON_SEND_THRESHOLD);
			
			UartReceiveTransfer(uartId,temp_buff,DAEMON_SEND_THRESHOLD);
			free(temp_buff);
			u8  UartReceiveEndFLAG[5] = {0,0,'\n',0,0};
			uartWriteFifo(&ths->fifo,UartReceiveEndFLAG,5);
			u8 *temp1_buff = (u8 *)malloc(ths->fifo.size);
			u32 temp_size = ths->fifo.size;
			uartReadFifo(&ths->fifo, temp1_buff, temp_size);
			UartReceiveTransfer(uartId,temp1_buff,temp_size);
			free(temp1_buff);
		}

		else if(ths->daemon_time>DAEMON_TIME_CNT && ths->fifo.size>0)
		{		
			u8  UartReceiveEndFLAG[5] = {0,0,'\n',0,0};
			uartWriteFifo(&ths->fifo,UartReceiveEndFLAG,5);
			u8 *temp_buff = (u8 *)malloc(ths->fifo.size);
			u32 temp_size = ths->fifo.size;
			uartReadFifo(&ths->fifo, temp_buff, temp_size);
			UartReceiveTransfer(uartId,temp_buff,temp_size);
			free(temp_buff);
		}
	}
	else
	{
		ths->daemon_time += DAEMON_TIME_SPACE;
	}
}

void UartReceive_Daemon_Thread(void *arg)
{
	printf("UartReceive_Daemon_Thread Start. Prio = %d\r\n",UARTRECEIVE_DAEMONTHREAD_PRIO);
	while(1)
	{
		bus_os_sleep(DAEMON_TIME_SPACE);
#ifdef UART1_RT_EN
		UartReceiveTriggle(&uartrs1,1);
#endif
#ifdef UART2_RT_EN	
		UartReceiveTriggle(&uartrs2,2);
#endif
#ifdef UART3_RT_EN		
		UartReceiveTriggle(&uartrs3,3);
#endif
#ifdef UART4_RT_EN		
		UartReceiveTriggle(&uartrs4,4);
#endif
#ifdef UART5_RT_EN		
		UartReceiveTriggle(&uartrs5,5);
#endif
#ifdef UART6_RT_EN		
		UartReceiveTriggle(&uartrs6,6);
#endif
	}
}


			


static bool uartsend_queue_pend()            
{                                        
	bus_os_pend_sem(&UartSend_Cfg.sem,0);    
	return true;                        
}                                        
static bool uartsend_queue_post()         
{                                        
	bus_os_post_sem(&UartSend_Cfg.sem);    
	return true;                        
}

void UartSend_Init()
{
	UART_SEND_THRAD_CREATE();
}


void UartSend_Queue_Enter(UartSend_Queue_Ele_t *ele)
{
    SeqQueue_Enter(&uartsend_queqe,ele);
}


void UartSend_Thread(void *arg)
{
	printf("UartSend_Thread Start. Prio = %d\r\n",UART_SEND_THREAD_PRIO);
	BytesBufferNode *rt_node = NULL;
	while(1)
	{
		SeqQueue_Waite(&uartsend_queqe);
		UartSend_Queue_Ele_t ele;
		SeqQueue_Delete(&uartsend_queqe,&ele);

		u32 rt_buff_len;
		u8 *rt_buff;
		UartTransmit_Send* uart = (UartTransmit_Send *)ele.send;
		UartTransmit_Return* rt = New_UartTransmit_Return();

		UartSendBytes(Id2Uart(uart->m1_uart_id), uart->m2_data, uart->m2_data_Length);

		rt->m1_errCode = 0;
		rt_node = rt->ToBytes(rt);
		rt_buff_len = BytesBufferNode_Destroy(rt_node, &rt_buff);
		bus_ita_return(ele.adapterId, ele.srcId,ele.sessionId, rt_buff_len, rt_buff);

		free(rt_buff);
		Delete_UartTransmit_Send(uart);
		Delete_UartTransmit_Return(rt);
	}
}
