#include "flash.h"
#include "bus.h"
#include "cmd.h"
#include "Protocols.h"
#include "ErrCode.h"
#include "stm32f4xx_hal.h"
#include "bus_uart6_f4_hal.h"
#include "async.h"




extern int MyAdapterID ; 

void Firmware_ReportEvent()
{
    printf("Firmware_ReportEvent\r\n");
    FirmWareReadyEvent_Send *send = New_FirmWareReadyEvent_Send();
    
    BytesBufferNode * node = NULL;
    uint32_t cmd = CMD_FirmWareReadyEvent;
    BytesBufferNode * cmd_node = malloc(sizeof(BytesBufferNode));
    memset(cmd_node,0,sizeof(BytesBufferNode));
    cmd_node->data = malloc(sizeof(cmd));
    memmove(cmd_node->data,&cmd,4);
    cmd_node->len = sizeof(cmd);
    
    BytesBufferNode_Append(&node,cmd_node);
    BytesBufferNode_Append(&node,send->ToBytes(send));
    
    uint8_t * tx = NULL;
    int len = BytesBufferNode_Destroy(cmd_node,&tx);
    
    uint32_t rt;
    uint32_t rt_len;
    //为保证adapterID 跟随不同项目 增加一个变量
    u32 code = bus_ita_hb_send(MyAdapterID,0,len,tx,(uint8_t*)&rt,&rt_len);
    
    free(tx);
    Delete_FirmWareReadyEvent_Send(send);
    printf("code = 0x%x,rt_len=%d\r\n",code,rt_len);
}


// 固件升级指令处理
void FirmWare_Excute(int cmd,bus_packet *packet )
{
    switch(cmd)
    {
        case CMD_FirmWareConfig     ://0x1F
        {
            FirmWareConfig_Send *send = New_FirmWareConfig_Send();
            FirmWareConfig_Return *rt = New_FirmWareConfig_Return();
            send->GetFromBytes(send,packet->data,4);
            int ver = send->m2_version; 
            rt->m1_errCode = 0;
            printf("FirmWareUpdateStart=%d\r\n",send->m1_cmd);
            
            
            BytesBufferNode * node =  rt->ToBytes(rt);
            u8 *data = NULL;
            int len = BytesBufferNode_Destroy(node,&data);
            bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);

            free(data);
            
            
            if(send->m1_cmd == 0) 
            {
                if(Flash_ContainsApp()){
                    printf("contains app\r\n");
                    FLASH_If_Init();
                    FLASH_If_Erase(FLASH_VERSION_SECTOR);//擦除sector3
                    printf("erase ok\r\n");
                    NVIC_SystemReset(); //在有App的情况下肯定是需要再次复位的
                }else{
                    printf("no contains app\r\n");
                    Firmware_ReportEvent();
                }
            }
            else if(send->m1_cmd == 1) 
            {
                Flash_SetProjectVersion(ver);
                printf("set version success \r\n");
                NVIC_SystemReset();
            }
            else
            {

            }
            Delete_FirmWareConfig_Send(send);
            Delete_FirmWareConfig_Return(rt);
            
            break;
        }
        case CMD_FirmWareTransmitFile    ://0x20
        {
            FirmWareTransmitFile_Send   *send = New_FirmWareTransmitFile_Send();
            FirmWareTransmitFile_Return *rt  =  New_FirmWareTransmitFile_Return();
            send->GetFromBytes(send,packet->data,4);
            uint32_t crc = crc32(0,send->m1_datas,send->m1_datas_Length);
            if(crc == send->m2_crc)
            {
                printf("crc ok ofst=%d\r\n",send->m3_offset);
                rt->m1_errCode = 0 ;
                if(FLASH_If_Write(FLASH_APP_START  +send->m3_offset,send->m1_datas,send->m1_datas_Length))
                {
                    rt->m1_errCode = ERR_FLASH_ERROR;
                }
            }
            else
            {
                printf("crc=0x%x\r\n",crc);
                rt->m1_errCode = ERR_CRC_ERROR;
            }
            BytesBufferNode * node =  rt->ToBytes(rt);
            u8 *data = NULL;
            int len = BytesBufferNode_Destroy(node,&data);
            bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);

            free(data);
            Delete_FirmWareTransmitFile_Send(send);
            Delete_FirmWareTransmitFile_Return(rt);
            break;
        }
        case CMD_McuReset                ://0x21
        {
            McuReset_Send *send = New_McuReset_Send();
            McuReset_Return *rt = New_McuReset_Return();
            send->GetFromBytes(send, packet->data , 4);
            rt->m1_errCode = 0;
            BytesBufferNode * node =  rt->ToBytes(rt);
            u8 *data = NULL;
            int len = BytesBufferNode_Destroy(node,&data);
            bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);

            free(data);
            Delete_McuReset_Send(send);
            Delete_McuReset_Return(rt);
					
            NVIC_SystemReset();

            break;
        }
        case CMD_FirmWareVersionGet      ://0x22
        {
            FirmWareVersionGet_Send *send = New_FirmWareVersionGet_Send();
            FirmWareVersionGet_Return *rt = New_FirmWareVersionGet_Return();
            rt->m1_errCode = 0;
            rt->m2_value = Flash_GetProjectVersion();
            BytesBufferNode * node =  rt->ToBytes(rt);
            u8 *data = NULL;
            int len = BytesBufferNode_Destroy(node,&data);
            bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);

            free(data);
            Delete_FirmWareVersionGet_Send(send);
            Delete_FirmWareVersionGet_Return(rt);
            
            break;
        }
    }
}




