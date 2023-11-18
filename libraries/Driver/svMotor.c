#include "svMotor.h"
#include <stdbool.h>
#include "Async.h"
#include <stdlib.h>
#include "ErrCode.h"
#include "stm32f4xx_hal.h"


#define ASSERT_PTR(v)  if(v==NULL)while(1)printf("%d\r\n",__LINE__);


extern UART_HandleTypeDef huart4;
extern uint16_t CRC16(uint8_t *data, int len);
static  u16 my_crc(u8 op,u8 len,u16 *dataIn);



#define sw16(x) \
	((uint16_t)(						\
	(((uint16_t)(x)& (uint16_t)0x00ff) << 8) | \
	(((uint16_t)(x)& (uint16_t)0xff00) >> 8)))





typedef struct
{
    int state;
    u8  ack;
    bus_os_sem  sem;
    void (*send)(u8 *data,int len);

    u8 rx_buff[512];
    u8 rx_idx ; 
    u8 rx_len;
    u8 crc[2];
    bool is_crc_ok;


}SvMotor_t;

static SvMotor_t Ths;

#define FLAG_OK       'O'
#define FLAG_FALSE    'F'

#define STATE_IDLE          0
#define STATE_WAITE_O_F     1
#define STATE_ACK_FR_OP     2
#define STATE_ACK_FR_LEN    3
#define STATE_ACK_FR_DATA   4
#define STATE_ACK_FR_CRC_L    5
#define STATE_ACK_FR_CRC_H    6


/*
static void aync_cal_crc(int crc,u16 *data,int dataLen)
{
    Ths.is_crc_ok = (CRC16(data,dataLen) == crc);
}
*/

static void send_flag(u8 data)
{
    Ths.send(&data,1);
}

void SvMotor_NotifyByte(u8 data)
{
    switch(Ths.state )
    {
        case STATE_WAITE_O_F :
        {
            Ths.ack = data;
            bus_os_post_sem(& Ths.sem);
            break;
        }

        case STATE_ACK_FR_OP :
        {
            Async_Excute(2,printf,":%02x\r\n",(int)data);
            if(data == 0)
            {
                Ths.state = STATE_ACK_FR_LEN;
            }
            else
            {
                Ths.state = STATE_IDLE;
            }
            
            //TODO:回复 
            Async_Excute(1,send_flag,FLAG_OK);
            
            break;
        }

        case STATE_ACK_FR_LEN :
        {
            Async_Excute(2,printf,":%02x\r\n",(int)data);
            Ths.rx_len = data + 1;
            Ths.state = STATE_ACK_FR_DATA;
            Ths.rx_idx = 0;
            break;
        }

        case STATE_ACK_FR_DATA :
        {
            Async_Excute(2,printf,":%02x\r\n",(int)data);
            Ths.rx_buff[Ths.rx_idx ++] = data;
            if(Ths.rx_idx >= (Ths.rx_len ) *2 )
            {
                Ths.state = STATE_ACK_FR_CRC_L;
                Async_Excute(2,printf,"gc %d\r\n",(int)Ths.rx_idx);
            }
            
            break;
        }

        case STATE_ACK_FR_CRC_L :
        {
            Async_Excute(2,printf,":%02x\r\n",(int)data);
            Ths.crc[0]= data;
            Ths.state = STATE_ACK_FR_CRC_H;
            break;
        }
        case STATE_ACK_FR_CRC_H :
        {
            Async_Excute(2,printf,":%02x\r\n",(int)data);
            Ths.crc[1]= data;
            //计算crc是否正确
            Ths.is_crc_ok = ( my_crc(00,Ths.rx_len,(u16*) Ths.rx_buff )== (Ths.crc[0] | (Ths.crc[1]<<8)));
            if(! Ths.is_crc_ok)
            {
                Async_Excute(1,printf,"ce \r\n");
            }
            else
            {
                Async_Excute(1,printf,"co \r\n");
            }
            bus_os_post_sem(& Ths.sem);
            //TODO:回复
            
            
            break;
        }
            
            
        default:
        {
            Async_Excute(1,printf,"sv get err data \r\n");
            break;
        }
    }

}


static u16 my_crc(u8 op,u8 len,u16 *dataIn)
{
    u8 * send = malloc(   1   + 1 + len *2  );
    ASSERT_PTR(send);
    int idx =0;
    send[idx++] = op;
    send[idx++] = len - 1;
    for(int i = 0 ; i< len;i++)
    {
        WriteU16(send+ idx,sw16(dataIn[i ] )  );
        idx +=2;
    }
    u16 res = CRC16(send,idx);
    free(send);
    return res;
}


u32  SvMotor_DataExchange(u8 opCode,u16 *dataIn,int len,u16* dataOut,int *rxLen)
{
    u32 code = 0;

    for(int x = 0;x<3;x++)
    {
        Ths.state = STATE_WAITE_O_F ;
        bus_os_del_sem(&Ths.sem);
        bus_os_create_sem(&Ths.sem,0);
        
        Ths.send(&opCode,1);
        code = bus_os_pend_sem( & Ths.sem,500);
        if(code == 0)
        {
            
            if(Ths.ack == FLAG_OK)
            {
                 //------------------ op[1] len[1] data[len] crc[2]
                u8 * send = malloc(   1   + 1 + len *2   + 2);
                ASSERT_PTR(send);
                int i = 0;
                send[i++] = opCode;
                send[i++] = len -1 ;
                memmove(send+i,dataIn,len*2); i+= len*2;
                WriteU16(send+i, my_crc(opCode,len,dataIn) )  ; i+=2;
                Ths.send(send+1,i-1);
                free(send);
            }
            else 
            {
                code = ERR_SERVO_DRIVER_ACK ;
            }
        }
        if(code == 0)
        {
            code = bus_os_pend_sem( & Ths.sem,500);
            if(code == 0 )
            {
                if(Ths.ack == FLAG_OK)
                {
                    Ths.state = STATE_ACK_FR_OP;
                }
                else
                {
                    
                }
            }
            //开始处理回复帧
            code = bus_os_pend_sem(& Ths.sem,500);
            if(code == 0 )
            {
                if(Ths.is_crc_ok )
                {
                    code = 0;    
                    *rxLen = Ths.rx_len ;
                    memmove(dataOut,Ths.rx_buff ,Ths.rx_len * 2 );
                }
                else
                {
                    code = ERR_SERVO_DRIVER_CRC;
                }
            }
            else
            {
                printf("wt ack fr tm out \r\n");
            }
            
        }
        if(code == 0 )
        {
            break;
        }
    }
    

    Ths.state = STATE_IDLE;
    return code;

}



void SvMotor_Init(void (*send)(u8 *data,int len))
{
    Ths.send = send;
    bus_os_create_sem(& Ths.sem,0);
    __HAL_UART_ENABLE(&huart4);
	__HAL_UART_ENABLE_IT(&huart4, UART_IT_RXNE);
}






