#include "bus_uart4_f4_hal.h"


extern 	UART_HandleTypeDef huart4;
    
ubtf_struct ubtf4;
bus_adapter Uart4Adapter;


void uart4_rx_frame(uart_frame_struct * frame) 
{
	bus_for_driver_call(UART4_ADAPTER_ID,frame->srcId,frame->desId,frame->functionId,frame->sessionId,frame->dlc,frame->data);
}


void uart4_send_byte(u8 b) 
{
	HAL_UART_Transmit(&huart4,&b,1,0xff);
}



void USART4_IRQHandler(void)
{
	ubtf_notify_byte_received(&ubtf4,UART4->DR);
// 	printf("%s \r\n",__FUNCTION__);
}


u32 uart4_adapter_send(void *ths, u8 desId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data)	
{
// 	printf("%s \r\n",__FUNCTION__);
	uart_frame_struct frm;
	frm.srcId = ((bus_adapter*)ths)->localId;
    frm.desId = desId;
    frm.functionId = functionId;
    frm.sessionId = sessionId;
    frm.dlc = dlc;
    memmove(frm.data,data,dlc);
	ubtf_send_frame(&ubtf4,&frm);

	return 0;
}


u32 uart4_adapter_init(void *ths, u8 localId)	
{
	//TODO
    printf("%s \r\n",__FUNCTION__);
	ubtf_set_byte_send_function(&ubtf4,&uart4_send_byte);
	ubtf_set_frame_rx_call(&ubtf4,&uart4_rx_frame);
	Uart4Adapter.adatperId = UART4_ADAPTER_ID;
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
	return 0;
}




