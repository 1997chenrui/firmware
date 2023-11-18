#include "bus_usb_f4_hal.h"
#include "usbd_cdc_if.h"


extern  USBD_HandleTypeDef hUsbDeviceFS;

ubtf_struct  utbf_usb;
bus_adapter usbAdapter;

#define             USB_TASK_START_PRIO                     20u
#define             USB_TASK_STK_SIZE                       256
static  OS_STK      USB_TASK_STK[USB_TASK_STK_SIZE];

#define  USB_QUEUE_SIZE    1024
static  uint8_t     queue[USB_QUEUE_SIZE];
static  uint32_t    queue_w_idx = 0;
static  uint32_t    queue_r_idx = 0;
static  bus_os_sem  queue_mutex;

void CDC_Receive_FS_Callback (uint8_t* Buf, uint32_t *Len)
{
    for(int i =0;i<(*Len);i++)
    {
        ubtf_notify_byte_received(&utbf_usb,Buf[i]);
    }
}


static void UsbSendTask(void *arg)
{
    printf("usb send task start \r\n");
    while(1)
    {
        if(queue_w_idx > queue_r_idx)
        {
            bus_os_pend_sem(&queue_mutex,0);
            int drl = queue_w_idx - queue_r_idx;
            if(drl > USB_QUEUE_SIZE)
            {
                printf("Very big fault\r\n");
            }
            uint8_t *temp = malloc(drl);
            for(int i =0;i<drl;i++)
            {
                temp[i] = queue[(queue_r_idx + i)%USB_QUEUE_SIZE];
            }
            
            CDC_Transmit_FS(temp,drl);
            while(  ((USBD_CDC_HandleTypeDef   *)hUsbDeviceFS.pClassData)->TxState != 0 );
			free(temp);
            queue_r_idx += drl;
            if(queue_r_idx == queue_w_idx)
            {
                queue_r_idx = queue_w_idx = 0;
            }
            
            bus_os_post_sem(&queue_mutex);
        }
        else
        {
            OSTimeDly(1);       //延时1个ms
        }
    }
}


//关中断
void usb_rx_frame(uart_frame_struct * frame) 
{
	OS_CPU_SR cpu_sr;
	OS_ENTER_CRITICAL();
	bus_for_driver_call(USB_ADAPTER_ID,frame->srcId,frame->desId,frame->functionId,frame->sessionId,frame->dlc,frame->data);
	OS_EXIT_CRITICAL();
}


void usb_send_byte(u8 b) 
{
	//发送usb数据  写到buff 里面去
    queue[(queue_w_idx ++) % USB_QUEUE_SIZE ] = b;
}



u32 usb_adapter_send(void *ths, u8 desId, u8 functionId, u8 sessionId, u32 isRemote, u32 dlc, u8 *data)	
{
// 	printf("%s \r\n",__FUNCTION__);
	uart_frame_struct frm;
	frm.srcId = ((bus_adapter*)ths)->localId;
    frm.desId = desId;
    frm.functionId = functionId;
    frm.sessionId = sessionId;
    frm.dlc = dlc;
    memmove(frm.data,data,dlc);
    
    bus_os_pend_sem(&queue_mutex,0);    //  发送一个帧
	ubtf_send_frame(&utbf_usb,&frm);
    bus_os_post_sem(&queue_mutex);
	return 0;
}


u32 usb_adapter_init(void *ths, u8 localId)	
{
	//TODO
    printf("%s \r\n",__FUNCTION__);
	ubtf_set_byte_send_function(&utbf_usb,&usb_send_byte);
	ubtf_set_frame_rx_call(&utbf_usb,&usb_rx_frame);
	usbAdapter.adatperId = USB_ADAPTER_ID;
    
    
    uint8_t  err =  OSTaskCreateExt(UsbSendTask, 
                                    0,
                                    &USB_TASK_STK[USB_TASK_STK_SIZE - 1],
                                    USB_TASK_START_PRIO,
                                    USB_TASK_START_PRIO,
                                    &USB_TASK_STK[0],
                                    USB_TASK_STK_SIZE,
                                    0,
                                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    if(err != 0)
    {
        while(1)
            printf("usb task error,may the prio is used\r\n");
    }
    bus_os_create_sem(&queue_mutex,1);
    

	return 0;
}

