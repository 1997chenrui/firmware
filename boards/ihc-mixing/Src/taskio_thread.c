#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include "Protocols.h" 
#include "bus.h"
#include "bus_uart6_f4_hal.h"
#include "taskio_thread.h"

bus_os_thread       Task_IO_Tcb;
#define             TASK_IO_PRIO           11
#define             TASK_IO_SIZE           512
static OS_STK       TaskIoStk[TASK_IO_SIZE];
static void         TaskIo(void *arg);

extern int MyAdapterID;

#define IO_CNT		1

static volatile bool  is_iotri = false;
uint8_t  is_servo_start = 0;

typedef union
{
	struct
	{
		u16 port:8;
		u16 pin:7;
		u16 value:1;
	}data;
	u16 port_pin_state;
}IoState_Struct;
	
typedef struct
{
	uint8_t  state ;
	uint8_t  is_tri;
	char     port;
	uint16_t pin;
}IO_Cfg_Struct;
typedef struct IoStruct_t
{
	IO_Cfg_Struct cfg[IO_CNT];
    void  (*read)(IO_Cfg_Struct *ths);
}IoStruct_t;

static IoStruct_t ios;

#define IO_CFG(id,pn,pt)		 \
		ios.cfg[id].pin = pn;	 \
		ios.cfg[id].port = pt; \
		ios.cfg[id].is_tri = 0;

void   read_io(IO_Cfg_Struct *ths);
void TaskIoInit(void)
{
	ios.read = &read_io;
	IO_CFG(0,3,3);
}
static uint8_t TaskIo_GPIO_ReadPin(IO_Cfg_Struct *ths,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	uint8_t val = 0;
	switch(ths->port)
	{
		case 0 :
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 5 :
		case 6 :val = HAL_GPIO_ReadPin(GPIOx,GPIO_Pin);break;
	}
	return val;
}
void   read_io(IO_Cfg_Struct *ths)
{
    GPIO_TypeDef* port = GPIOE;
    uint16_t    pin = GPIO_PIN_8;
    switch(ths->port)
    {
        case 0 : port = GPIOA; break;
        case 1 : port = GPIOB; break;
        case 2 : port = GPIOC; break;
        case 3 : port = GPIOD; break;
        case 4 : port = GPIOE; break;
		case 5 : port = GPIOF; break;
		case 6 : port = GPIOG; break;
		//extern
		case 7 : 
		case 8 : break;
		default:printf("read_io parameter error\r\n");break;
    }
    
    switch(ths->pin)
    {
        case 0 : pin = GPIO_PIN_0; break;
        case 1 : pin = GPIO_PIN_1; break;
        case 2 : pin = GPIO_PIN_2; break;
        case 3 : pin = GPIO_PIN_3; break;
        case 4 : pin = GPIO_PIN_4; break;
        case 5 : pin = GPIO_PIN_5; break;
        case 6 : pin = GPIO_PIN_6; break;
        case 7 : pin = GPIO_PIN_7; break;
        case 8 : pin = GPIO_PIN_8; break;
        case 9 : pin = GPIO_PIN_9; break;
        case 10 : pin = GPIO_PIN_10; break;
        case 11 : pin = GPIO_PIN_11; break;
        case 12 : pin = GPIO_PIN_12; break;
        case 13 : pin = GPIO_PIN_13; break;
        case 14 : pin = GPIO_PIN_14; break;
        case 15 : pin = GPIO_PIN_15; break;
        default:printf("read_io parameter error\r\n");break;
    }
    
    GPIO_PinState value  = (GPIO_PinState)TaskIo_GPIO_ReadPin(ths,port,pin);
    
    if(value != ths->state)
    {
		ths->is_tri = 1;   
		is_iotri = 1;
		printf("T:port = %d,pin = %d,state=%d\r\n",ths->port,ths->pin,value);
    }
    ths->state = value;
}

static void TaskIo_Send(IoStruct_t *ths)
{
	u32 tri_cnt = 0;
	McuIoStateChangeEvent_Send *send = New_McuIoStateChangeEvent_Send();
	for(int i = 0;i<IO_CNT;i++)
	{
		if(ths->cfg[i].is_tri)
		{
			tri_cnt++;
		}
	}
	send->m1_port_pin_state_Length = tri_cnt;
	send->m1_port_pin_state = (u16 *)malloc(tri_cnt * sizeof(u16));
	tri_cnt = 0;
	for(int i = 0;i<IO_CNT;i++)
	{
		if(ths->cfg[i].is_tri)
		{
			IoState_Struct ios;
			ios.data.port = ths->cfg[i].port;
			ios.data.pin = ths->cfg[i].pin;
			ios.data.value = ths->cfg[i].state;
			
			send->m1_port_pin_state[tri_cnt] = ios.port_pin_state;
			ths->cfg[i].is_tri = 0;
			tri_cnt++;
			printf("S:port = %d,pin = %d,state = %d\r\n",ios.data.port,ios.data.pin,ios.port_pin_state);
		}
	}
	
	BytesBufferNode *temp_node = send->ToBytes(send);
	uint8_t * tx = NULL;
	int len = BytesBufferNode_Destroy(temp_node,&tx);
	
	
	u8 * tx_send = malloc(len+4);
	WriteU32(tx_send,0x25);
	memmove(tx_send+sizeof(u32),tx,len);
	
	uint32_t rt;
	uint32_t rt_len;
	bus_ita_hb_send(MyAdapterID,0,len+4,tx_send,(uint8_t*)&rt,&rt_len);
	
	free(tx_send);
	free(tx);
	free(send->m1_port_pin_state);
	Delete_McuIoStateChangeEvent_Send(send);
}
static void TaskIo(void *arg)
{
	TaskIoInit();
	printf("Task IO start\r\n");
    while(1)
    {
        for(int i =0;i<IO_CNT;i++)
        {
            ios.read(& (ios.cfg[i] ) );
        }
		if(is_iotri)
		{
			TaskIo_Send(&ios);
			is_iotri = false;
		}
		bus_os_sleep(100);
    }
}

void taskio_thread_init(void)
{
	u32 code = 0;
	code = bus_create_thread(&Task_IO_Tcb,TaskIo,TaskIoStk,TASK_IO_SIZE,TASK_IO_PRIO,NULL);
	if(code != 0)
	{
		printf("							create task IO error\r\n");
	}
}
