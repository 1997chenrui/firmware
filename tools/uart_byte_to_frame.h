#ifndef UART_BYTE_TO_FRAME_H_
#define UART_BYTE_TO_FRAME_H_

#include <stdio.h>
#pragma anon_unions
typedef unsigned char u8;
typedef unsigned int  u32;

typedef struct
{
	u8 desId;
	u8 srcId; 
	u8 functionId; 
	u8 sessionId; 
    u8 dlc;
	u8 data[8];

}uart_frame_struct;



typedef struct
{
	void (*rx_call)(uart_frame_struct * )  ;
	void (*send_byte)(u8 );

	
	u8  rx_idx;
	u8  state;
	u8  adapterId;
	u8  localId;

    u8   buff[sizeof(uart_frame_struct) + 4];    //多4个是放crc的地方

	
}ubtf_struct;

void ubtf_set_frame_rx_call(ubtf_struct * ths, void(*rx_call)(uart_frame_struct * )  );
void ubtf_set_byte_send_function(ubtf_struct * ths, void (*send_byte)(u8 )  );
void ubtf_notify_byte_received(ubtf_struct * ths, u8 data);
void ubtf_send_frame(ubtf_struct * ths,uart_frame_struct *frame);











#endif





