#include "bus_uart3_f1_hal.h"

extern 	UART_HandleTypeDef huart3;


ubtf_struct ubtf3;
u8 uart3_rx_buff[2] ;
bus_adapter Uart3Adapter;

#define ASSERT(a ) if( (a)) while(1);

//¹ØÖÐ¶Ï
void uart3_rx_frame(uart_frame_struct * frame) 
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
	bus_for_driver_call(UART3_ADAPTER_ID,frame->srcId,frame->desId,frame->functionId,frame->sessionId,frame->dlc,frame->data);
	OS_EXIT_CRITICAL();
}


void uart3_send_byte(u8 b) 
{
	HAL_UART_Transmit(&huart3,&b,1,0xff);
}



u32 uart3_adapter_send(void *ths, u8 desId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data)	
{
// 	printf("%s \r\n",__FUNCTION__);
	uart_frame_struct frm;
	frm.srcId = ((bus_adapter*)ths)->localId;
    frm.desId = desId;
    frm.functionId = functionId;
    frm.sessionId = sessionId;
    frm.dlc = dlc;
    memmove(frm.data,data,dlc);
	ubtf_send_frame(&ubtf3,&frm);

	return 0;
}


u32 uart3_adapter_init(void *ths, u8 localId)	
{
	//TODO
    printf("%s \r\n",__FUNCTION__);
    HAL_UART_Receive_DMA(&huart3,uart3_rx_buff,2);
	ubtf_set_byte_send_function(&ubtf3,&uart3_send_byte);
	ubtf_set_frame_rx_call(&ubtf3,&uart3_rx_frame);
	Uart3Adapter.adatperId = UART3_ADAPTER_ID;

	return 0;
}

