#include "stm32f4xx.h"
#include "os.h"
#include "bus.h"
#include <stdio.h>
#include "errCode.h"

#if BUS_USE_UART == 1               //使用了串口就会包含串口
    #include "bus_uart.h"
#endif
	
typedef union {
    u32 expId;
    struct{
        u32  sessionId : 8;
        u32  funcId    : 5;
        u32  srcId     : 8;
        u32  desId     : 8;
        u32  not_use   : 3;
    }atr;
}exp_id_union;    
    

//--------------- 函数声明 --------------------------
u32 bus_driver_send(u32 adapterId, u8 nodeId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data);
u32 Can1Init(u8 localId,u32 baud);
//----------------------- 网卡结构 -----------------------------
bus_adapter CanAdapter;

u32 can_adapter_send(void *this, u8 desId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data)	//??????????????????
{
	return bus_driver_send(CanAdapter.adatperId, desId, functionId, sessionId, isRemote, dlc, data);
}


u32 can_adapter_init(void *this, u8 localId)	
{
	CanAdapter.adatperId = 0;
	Can1Init(localId,1000000);
	return 0;
}



    
    
static u32 Can1_localId = 0;
static u32 Can1_baud = 0;


u32 Can1Init(u8 localId,u32 baud)
{
    Can1_localId = localId;
    Can1_baud = baud;
    
    {
        NVIC_InitTypeDef  NVIC_InitStructure;
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        
        NVIC_InitStructure.NVIC_IRQChannel = CAN1_TX_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        
        NVIC_InitStructure.NVIC_IRQChannel = CAN1_SCE_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x2;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        
        
    }   
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* CAN GPIOs configuration **************************************************/

    /* Enable GPIO clock */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    /* Connect CAN pins to AF9 */
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);

    /* Configure CAN RX and TX pins */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* CAN configuration ********************************************************/  
    /* Enable CAN clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    /* CAN register init */
    CAN_DeInit(CAN1);
    CAN_InitTypeDef  CAN_InitStructure;
    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = ENABLE;   //禁止硬件重发
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    

    /* CAN Baudrate = 1 MBps (CAN clocked at 30 MHz) */
    if(baud == 1000000)
    {
        CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
        CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
        CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
        CAN_InitStructure.CAN_Prescaler = 2;
    }
    else if(baud == 500000)
    {
        CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
        CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
        CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
        CAN_InitStructure.CAN_Prescaler = 4;
    }
    else if(baud == 250000)
    {
        CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
        CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
        CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
        CAN_InitStructure.CAN_Prescaler = 8;
    }
    else
    {
        BUS_LOG(" ERR_BUS_BAUD_ERR");
    }

    CAN_Init(CAN1, &CAN_InitStructure);

    /* CAN filter init */
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    CAN_FilterInitStructure.CAN_FilterNumber = 0;  //因为can1 can2 公用28个筛选器
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = (localId & 0xff)<<8;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xff00;  //最高位desId设置屏蔽
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
    
    CAN_FilterInitStructure.CAN_FilterNumber = 1;  //因为can1 can2 公用28个筛选器
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = (BUS_BRAODCAST_ID & 0xff)<<8;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xff00;  //最高位desId设置屏蔽
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
    
/**
  *            @arg CAN_IT_TME: Transmit mailbox empty Interrupt 
  *            @arg CAN_IT_FMP0: FIFO 0 message pending Interrupt 
  *            @arg CAN_IT_FF0: FIFO 0 full Interrupt
  *            @arg CAN_IT_FOV0: FIFO 0 overrun Interrupt
  *            @arg CAN_IT_FMP1: FIFO 1 message pending Interrupt 
  *            @arg CAN_IT_FF1: FIFO 1 full Interrupt
  *            @arg CAN_IT_FOV1: FIFO 1 overrun Interrupt
  *            @arg CAN_IT_WKU: Wake-up Interrupt
  *            @arg CAN_IT_SLK: Sleep acknowledge Interrupt  
  *            @arg CAN_IT_EWG: Error warning Interrupt
  *            @arg CAN_IT_EPV: Error passive Interrupt
  *            @arg CAN_IT_BOF: Bus-off Interrupt  
  *            @arg CAN_IT_LEC: Last error code Interrupt
  *            @arg CAN_IT_ERR: Error Interrupt
  */
    CAN_ITConfig(CAN1, CAN_IT_FMP0|     //接收状态仅仅使用 FIFO 0 不使用队列1
                       //CAN_IT_TME |
                       //CAN_IT_EWG |
                       //CAN_IT_EPV |
                       CAN_IT_BOF |
                       //CAN_IT_LEC |
                       CAN_IT_ERR 
                       ,ENABLE);

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
    
    if(CAN_GetITStatus(CAN1,CAN_IT_EWG) == SET)  //
    {
        CAN_ClearITPendingBit(CAN1,CAN_IT_EWG);
//          printf("CAN_IT_EWG\r\n");
         Can1Init(Can1_localId,Can1_baud);
    }
    if(CAN_GetITStatus(CAN1,CAN_IT_EPV) == SET)
    {
        CAN_ClearITPendingBit(CAN1,CAN_IT_EPV);
//          printf("CAN_IT_EPV\r\n");
         Can1Init(Can1_localId,Can1_baud);
    }
    if(CAN_GetITStatus(CAN1,CAN_IT_BOF) == SET)
    {
        CAN_ClearITPendingBit(CAN1,CAN_IT_BOF);
//         printf("CAN_IT_BOF\r\n");
        Can1Init(Can1_localId,Can1_baud);

    }
    if(CAN_GetITStatus(CAN1,CAN_IT_LEC) == SET)
    {
        CAN_ClearITPendingBit(CAN1,CAN_IT_LEC);
//         printf("CAN_IT_LEC\r\n");
//         Can1Init(Can1_localId,Can1_baud);
    }
    if(CAN_GetITStatus(CAN1,CAN_IT_ERR) == SET)
    {
        CAN_ClearITPendingBit(CAN1,CAN_IT_ERR);
//         printf("CAN_IT_ERR\r\n");
        Can1Init(Can1_localId,Can1_baud);
    }


}



void CAN1_RX0_IRQHandler()
{
    if(CAN_GetITStatus(CAN1,CAN_IT_FMP0) == SET)
    {
        CanRxMsg msg; 
        CAN_Receive(CAN1,0,&msg);
        //TODO bus_for_driver_call(0,msg.ExtId,msg.RTR == CAN_RTR_Remote? 1 :0 ,msg.DLC,msg.Data);
        exp_id_union u;
        u.expId = msg.ExtId;
        
        bus_for_driver_call(0,u.atr.srcId,u.atr.desId,u.atr.funcId,u.atr.sessionId,msg.DLC,msg.Data);
        return;
    }    

}


u32 bus_driver_send(u32 adapterId,u8 nodeId,u8 functionId,u8 sessionId ,u32 isRemote,u32 dlc,u8 *data)
{
    exp_id_union u;
    u.atr.desId = nodeId;
    u.atr.srcId = Can1_localId;
    u.atr.funcId = functionId;
    u.atr.sessionId = sessionId;
    
    
    if(adapterId == 0)
    {
        u16 i=0;
        CanTxMsg TxMessage;                                 // 标准标识符为（11位）
        TxMessage.ExtId=u.expId;//FExtId;                                 // 设置扩展标示符（29位）
        TxMessage.IDE=CAN_Id_Extended;         // 使用扩展标识符
        TxMessage.RTR=  CAN_RTR_DATA  ;         // 消息类型为数据帧，一帧8位          
        TxMessage.DLC=dlc;        
        for(i=0;i<dlc;i++)
        {
            TxMessage.Data[i]=data[i];                                 // 第一帧信息          
        }
        //非等到发送掉才罢休
        vu8 transmit = 0;
        do
        {
            CPU_DEF();
            CPU_PEND();
            transmit = CAN_Transmit(CAN1, &TxMessage);
            CPU_POST();
            bus_os_sleep(1);
        }
        while(transmit == CAN_TxStatus_NoMailBox);
        
        
    }
    else if(adapterId >= 2)
    { 
#if BUS_USE_UART == 1     
        code  = bus_driver_uart_send(adapterId,extId,isRemote,dlc,data);
#endif
    }

    return 0;          
}


