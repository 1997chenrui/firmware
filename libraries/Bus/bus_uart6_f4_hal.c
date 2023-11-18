#include "bus_uart6_f4_hal.h"
#include "uart_byte_to_frame.h"

extern 	UART_HandleTypeDef huart6;

ubtf_struct ubtf6;
bus_adapter Uart6Adapter;
u8 uart6_rx_buff[2] ;

void uart6_rx_half_data(void)
{
    ubtf_notify_byte_received(&ubtf6,uart6_rx_buff[0]);
}

void uart6_rx_data(void)
{
    ubtf_notify_byte_received(&ubtf6,uart6_rx_buff[1]);
}

//¹ØÖÐ¶Ï
void uart6_rx_frame(uart_frame_struct * frame) 
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	/*
	ASSERT(  frame->srcId != 0 );
	ASSERT(  (frame->desId != 2) && (frame->desId != 1)   );
	ASSERT(    (frame->functionId != 1) 
			&& (frame->functionId != 2) 
			&& (frame->functionId != 3) 
			&& (frame->functionId != 4) 
			&& (frame->functionId != 5) 
			&& (frame->functionId != 6) 

	);
	ASSERT(  frame->dlc  > 8  );
	*/
	bus_for_driver_call(UART6_ADAPTER_ID,frame->srcId,frame->desId,frame->functionId,frame->sessionId,frame->dlc,frame->data);
	OS_EXIT_CRITICAL();
}


void uart6_send_byte(u8 b) 
{
	HAL_UART_Transmit(&huart6,&b,1,0xff);
}



u32 uart6_adapter_send(void *ths, u8 desId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data)	
{
// 	printf("%s \r\n",__FUNCTION__);
	uart_frame_struct frm;
	frm.srcId = ((bus_adapter*)ths)->localId;
    frm.desId = desId;
    frm.functionId = functionId;
    frm.sessionId = sessionId;
    frm.dlc = dlc;
    memmove(frm.data,data,dlc);
	ubtf_send_frame(&ubtf6,&frm);

	return 0;
}


u32 uart6_adapter_init(void *ths, u8 localId)	
{
	//TODO
    printf("%s \r\n",__FUNCTION__);
	ubtf_set_byte_send_function(&ubtf6,&uart6_send_byte);
	ubtf_set_frame_rx_call(&ubtf6,&uart6_rx_frame);
	Uart6Adapter.adatperId = UART6_ADAPTER_ID;
	HAL_UART_Receive_DMA(&huart6,uart6_rx_buff,2);
	return 0;
}

