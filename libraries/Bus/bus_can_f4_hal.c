#include "bus_can_f4_hal.h"
#include "CanDriver.h"
#include "stm32f4xx_hal.h"
#include "CanDriver.h"

u8 Can1_localId;
bus_adapter CanAdapter;
u32 bus_driver_send(bus_adapter* ths, u8 nodeId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data);

u32 can_adapter_send(void* ths, u8 desId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data)
{
	return bus_driver_send((bus_adapter* )ths, desId, functionId, sessionId, isRemote, dlc, data);
}

u32 can_adapter_init(void* ths, u8 localId)	
{
	CanAdapter.adatperId = CAN_ADAPTER_ID;
	Can1_localId = localId;

	CanDriver_Can1Init(localId,CAN1_BAUD,CAN1_BD_ID);
	return 0;
}

// void CAN1_TX_IRQHandler()
// {
//     OS_ERR e;
//     CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
//     OSMutexPost(bus_get_mutex(0),OS_OPT_POST_NONE,&e);
// }

void CAN1_SCE_IRQHandler()
{
    
    if(CanDriver_CAN_GetITStatus(CAN1,CANDRIVER_CAN_IT_EWG) == SET)  //
    {
        CanDriver_ClearITPendingBit(CAN1,CANDRIVER_CAN_IT_EWG);
//          printf("CAN_IT_EWG\r\n");
        //CanDriver_Can1Init(Can1_localId,CAN1_BAUD,0XFF);
    }
    if(CanDriver_CAN_GetITStatus(CAN1,CANDRIVER_CAN_IT_EPV) == SET)
    {
        CanDriver_ClearITPendingBit(CAN1,CANDRIVER_CAN_IT_EPV);
//          printf("CAN_IT_EPV\r\n");
        //CanDriver_Can1Init(Can1_localId,CAN1_BAUD,0XFF);
    }
    if(CanDriver_CAN_GetITStatus(CAN1,CANDRIVER_CAN_IT_BOF) == SET)
    {
        CanDriver_ClearITPendingBit(CAN1,CANDRIVER_CAN_IT_BOF);
//         printf("CAN_IT_BOF\r\n");
        //CanDriver_Can1Init(Can1_localId,CAN1_BAUD,0XFF);

    }
    if(CanDriver_CAN_GetITStatus(CAN1,CANDRIVER_CAN_IT_LEC) == SET)
    {
        CanDriver_ClearITPendingBit(CAN1,CANDRIVER_CAN_IT_LEC);
//         printf("CAN_IT_LEC\r\n");
//         Can1Init(Can1_localId,Can1_baud);
    }
    if(CanDriver_CAN_GetITStatus(CAN1,CANDRIVER_CAN_IT_ERR) == SET)
    {
        CanDriver_ClearITPendingBit(CAN1,CANDRIVER_CAN_IT_ERR);
//         printf("CAN_IT_ERR\r\n");
        //CanDriver_Can1Init(Can1_localId,CAN1_BAUD,0XFF);
    }


}

void CAN1_RX0_IRQHandler()
{
	if(CanDriver_CAN_GetITStatus(CAN1,CANDRIVER_CAN_IT_FF0) == SET){
		//printf("		[err]CAN_IT_FF0\r\n");
	}
	if(CanDriver_CAN_GetITStatus(CAN1,CANDRIVER_CAN_IT_FOV0) == SET){
		//printf("		[err]CAN_IT_FOV0\r\n");
	}
    while(CanDriver_CAN_GetITStatus(CAN1,CANDRIVER_CAN_IT_FMP0) == SET)
    {
        CanDriver_CanTxMsg msg; 
        CanDriver_CAN_Receive(CAN1,0,&msg);
        exp_id_union u;
        u.expId = msg.ExtId;
        bus_for_driver_call(0,u.atr.srcId,u.atr.desId,u.atr.funcId,u.atr.sessionId,msg.DLC,msg.Data);
        return;
    }    
}

u32 bus_driver_send(bus_adapter* ths, u8 nodeId,u8 funcId,u8 sessionId ,u32 isRemote,u32 dlc,u8 *data)//发送帧数据
{
    exp_id_union u;
    u.atr.desId = nodeId;
    u.atr.srcId = ths->localId;
    u.atr.funcId = funcId;
    u.atr.sessionId = sessionId;
    
    if(ths->adatperId == 0)
    {
        u16 i=0;
        CanDriver_CanTxMsg TxMessage;                                 // 标准标识符为（11位）
        TxMessage.ExtId=u.expId;//FExtId;                                 // 设置扩展标示符（29位）
        TxMessage.IDE=CanDriver_CAN_Id_Extended;         // 使用扩展标识符
        TxMessage.RTR=  CanDriver_CAN_RTR_Data  ;         // 消息类型为数据帧，一帧8位          
        TxMessage.DLC=dlc;        
        for(i=0;i<dlc;i++)
        {
            TxMessage.Data[i]=data[i];                                 // 第一帧信息          
        }
		
		return CanDriver_CAN_Transmit(CAN1, &TxMessage);
		
//        //非等到发送掉才罢休
//        u8 transmit = 0;
//        do
//        {
//            CPU_DEF();
//            CPU_PEND();
//            transmit = CanDriver_CAN_Transmit(CAN1, &TxMessage);
//            CPU_POST();
//            bus_os_sleep(1);
//        }
//        while(transmit == CanDriver_CAN_TxStatus_NoMailBox);
    }
    return 0;
}
