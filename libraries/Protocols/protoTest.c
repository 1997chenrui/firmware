#include "protoTest.h"
#include <stdio.h>
#include <stdlib.h>
#include "bus_can_f4_hal.h"
#include "ErrCode.h"
#include "Cmd.h"

static void handler(void *data);

void ProtoTest_Init(void)
{
    
}

static u32 stk1[512];
static u32 stk2[512];
//static u32 stk3[512];

void ProtoTest_Start(void)
{
    bus_os_thread tcb;
    bus_create_thread(&tcb,&handler,stk1,512,40,NULL);
    bus_create_thread(&tcb,&handler,stk2,512,41,NULL);
    //bus_create_thread(&tcb,&handler,stk3,512,42,NULL);
}


void ProtoTestExcute(u32 cmd  ,  bus_packet * packet )
{
    int idx = 0;
    packet->data +=4;
    int len = ReadU32(packet->data+idx); idx +=4;
    u32 crc1 = crc32(0,packet->data+idx,len); idx += len;
    u32 crc2 = ReadU32(packet->data+idx); idx +=4;
    if(crc1 != crc2)
    {
//         for(int i = 0;i<len;i++)
//         {
//             printf("%02x ",packet->data[4+i]);
//         }
        printf("\r\nsesion=%d err \r\n",packet->sessionId);
//         printf("crc1=0x%x \r\n",crc1);
//         printf("crc2=0x%x \r\n",crc2);
    }
        
    len = (bus_os_time_get() % 10 ) + 10;
    u8 *rt = malloc(len + 8);
    idx = 0;
    WriteU32(rt+idx,len);  idx += 4;
   
    srand(bus_os_time_get()); //用时间做种，每次产生随机数不一样
    for(int i = 0;i<len;i++)
    {
        rt[idx + i ] = rand() % 257 ; 
        //rt[idx+i] = i;
    }
    idx += len;
    u32 crc3 = crc32(0,rt+4,len);
    WriteU32(rt+idx,crc3);
    idx +=  4;

    u32 code = bus_ita_return(CAN_ADAPTER_ID, packet->srcId,packet->sessionId  ,idx,rt);
    free(rt);
    if(code)
        printf("rt =0x%x\r\n",code);
    
}

static void handler(void *data)
{
    while(1)
    {
        int len = (bus_os_time_get() % 10 ) + 10;
        u8 *send = malloc(len + 12);
        int idx = 0;
        WriteU32(send+idx,0x06);idx+=4;
        WriteU32(send+idx,len);idx+=4;
        srand(bus_os_time_get()); //用时间做种，每次产生随机数不一样
        for(int i = 0;i<len;i++)
        {
            send[idx + i ] = rand() % 257 ; 
            //send[idx + i ] = i;
        }
        idx += len;
        WriteU32(send+idx,crc32(0,send+8,len));idx+=4;
        
        u8  dataOut[102];
        u32 dataOutLen  = 0 ;
        u32 code = bus_ita_hb_send(CAN_ADAPTER_ID,0,idx,send,dataOut,&dataOutLen);
        free(send);
        if(code)
            printf("sd =0x%x\r\n",code);
        if(dataOutLen > 100 + 8)
        {
            printf("dataOutLen %d err\r\n",dataOutLen );
        }
        else
        {
            int idx2= 0 ; 
            int aLen = ReadU32(dataOut+idx2); idx2 +=4 ;
            u32 crc1 = crc32(0,dataOut+idx2,aLen); idx2 +=(dataOutLen-8) ;
            u32 crc2 = ReadU32(dataOut+idx2);
            if(crc1 == crc2)
            {
                
            }
            else
            {
                printf("rt crc err\r\n");
            }
        }
        bus_os_sleep(100);
        
    }
}






