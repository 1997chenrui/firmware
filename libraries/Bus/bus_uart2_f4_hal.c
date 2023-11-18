#include "bus_uart2_f4_hal.h"
#include "uart_byte_to_frame.h"

extern 	UART_HandleTypeDef huart2;
    
ubtf_struct ubtf2;
bus_adapter Uart2Adapter;
u8 uart2_rx_buff[2] ;


void uart2_rx_half_data(void)
{
    ubtf_notify_byte_received(&ubtf2,uart2_rx_buff[0]);
}


void uart2_rx_data(void)
{
    ubtf_notify_byte_received(&ubtf2,uart2_rx_buff[1]);
}

void uart2_rx_frame(uart_frame_struct * frame) 
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	bus_for_driver_call(UART2_ADAPTER_ID,frame->srcId,frame->desId,frame->functionId,frame->sessionId,frame->dlc,frame->data);
	OS_EXIT_CRITICAL();
}


void uart2_send_byte(u8 b) 
{
	HAL_UART_Transmit(&huart2,&b,1,0xff);
}


u32 uart2_adapter_send(void *ths, u8 desId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data)	
{
// 	printf("%s \r\n",__FUNCTION__);
	uart_frame_struct frm;
	frm.srcId = ((bus_adapter*)ths)->localId;
    frm.desId = desId;
    frm.functionId = functionId;
    frm.sessionId = sessionId;
    frm.dlc = dlc;
    memmove(frm.data,data,dlc);
	ubtf_send_frame(&ubtf2,&frm);

	return 0;
}


u32 uart2_adapter_init(void *ths, u8 localId)	
{
	//TODO
    printf("uar2 init %s \r\n",__FUNCTION__);
	ubtf_set_byte_send_function(&ubtf2,&uart2_send_byte);
	ubtf_set_frame_rx_call(&ubtf2,&uart2_rx_frame);
	Uart2Adapter.adatperId = UART2_ADAPTER_ID;
	HAL_UART_Receive_DMA(&huart2,uart2_rx_buff,2);

	return 0;
}




