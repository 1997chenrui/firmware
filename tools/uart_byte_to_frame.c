#include "uart_byte_to_frame.h"
#include "Tool.h"
#include <string.h>

/*
	Ö¡Í·  ae  Ö¡Î² a5
	ae -> aa 0e
	a5 -> aa 05
	aa -> aa 0a


*/

#define U2_STATE_HEAD   			0
#define U2_STATE_AA                 1 
#define U2_STATE_DATA               2
#define U2_STATE_END                3




static void uart_error_handle(ubtf_struct *uc, const char * err)
{
	printf("err = %s \r\n",err);
	
	
	uc->state = U2_STATE_HEAD;
}




void ubtf_set_frame_rx_call(ubtf_struct * ths, void(*rx_call)(uart_frame_struct * )  )
{
	ths->rx_call = rx_call;
}

void ubtf_set_byte_send_function(ubtf_struct * ths, void (*send_byte)(u8 )  )
{
	printf("sizeof(uart_frame_struct) = %d \r\n",sizeof(uart_frame_struct));
	ths->send_byte = send_byte;
}




void ubtf_notify_byte_received(ubtf_struct * ths, u8 data)
{
//	printf("[%d]=%02x ",ths->rx_idx,data);
	switch(ths->state)
	{
		case U2_STATE_HEAD   			 :
		{
			if(data == 0xae)
			{
				ths->state = U2_STATE_DATA;
				ths->rx_idx = 0;
			}
			break;
		}
		case U2_STATE_AA            : 
		{
			switch(data)
			{
				case 0x0e :
				{
					ths->buff[ths->rx_idx++] = 0xae;
					
					break;
				}
				case 0x05 :
				{
					ths->buff[ths->rx_idx++] = 0xa5;
					break;
				}
				case 0x0a :
				{
					ths->buff[ths->rx_idx++] = 0xaa;
					break;
				}

				default:
				{
					uart_error_handle(ths,"first is aa but fllow is err");	
				}
			}
            
            if(ths->rx_idx >= sizeof(ths->buff )  )
            {
                ths->state = U2_STATE_END;
            }
            else
            {
                ths->state = U2_STATE_DATA;
            }
			
			break;
		}
		case U2_STATE_DATA               :
		{
			if(data == 0xaa)
			{
				ths->state = U2_STATE_AA;
			}
			else if(data == 0xa5)
			{
				uart_error_handle(ths,"expect data but find a5");	
			}
			else if(data == 0xae)
			{
				uart_error_handle(ths,"expect data but find ae");	
			}
			else
			{
				ths->buff[ths->rx_idx++] = data;
				if(ths->rx_idx >= sizeof(ths->buff )  )
                {
                    ths->state = U2_STATE_END;
                }
                else
                {
                    ths->state = U2_STATE_DATA;
                }
			}
			break;
		}        
		case U2_STATE_END :
		{
			if(data == 0xa5)
			{
				//¼ì²écrc
                u32 crc = *((u32*)(ths->buff + 13));
                u32 res = crc32(0, ths->buff, 13);
                if(res == crc)
                {
                    //for(int i = 0; i<13;i++){
//                        printf("%d,",ths->buff[5]);
                    //}
                    //printf("\r\n");
                    //TODO:
                    ths->rx_call( (uart_frame_struct *) (ths->buff) );
                }
                else
                {
                    uart_error_handle(ths,"crcerr");
                }
                
				ths->state = U2_STATE_HEAD;		//×´Ì¬»ú¸´Î»
			}
			else
				uart_error_handle(ths,"end is not a5");
			break;
		}

	}

}


static void ubtf_send_buff(ubtf_struct * ths,u8 *buff,int len)
{
    u8 r1 = 0xae;
    ths->send_byte(r1);
    for(int i = 0;i<len;i++)
    {
        u8 data = buff[i];
        switch(data)
        {
            case 0xae :
            {
                r1 = 0xaa;
                ths->send_byte(r1);
                r1 = 0x0e;
                ths->send_byte(r1);
                break;
            }
            case 0xa5 :
            {
                r1 = 0xaa;
                ths->send_byte(r1);
                r1 = 0x05;
                ths->send_byte(r1);
                break;
            }
            case 0xaa :
            {
                r1 = 0xaa;
                ths->send_byte(r1);
                r1 = 0x0a;
                ths->send_byte(r1);
                break;
            }
            default :
            {
                ths->send_byte(data);
            }
        }
    }
        
    r1 = 0xa5;
    ths->send_byte(r1); //·¢ËÍÖ¡Î²
}




void ubtf_send_frame(ubtf_struct * ths,uart_frame_struct *temp)
{
	u8 buff[sizeof(uart_frame_struct) + 4 ];
	memmove(buff,temp,sizeof(uart_frame_struct));
	*((u32*)(buff + sizeof(uart_frame_struct))) = crc32(0, buff, sizeof(uart_frame_struct));
	ubtf_send_buff(ths, buff, sizeof(uart_frame_struct) + 4);
}



















