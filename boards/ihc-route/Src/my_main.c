#include "my_callback.h"
#include "my_main.h"
#include "bus_can_f4_hal.h"
#include "bus_uart6_f4_hal.h"
#include "CanDriver.h"
#include "async.h"
#include "uart_transfer.h"
#include "main_cfg.h"
#include "bus_cfg.h"
#include "io_operation.h"
#include "color.h"
#include "powerled.h"

#define ROUTE_NODE_ID   		1
#define POWER_NODE_ID   		2
#define LIQUID_1_NODE_ID		6
#define SAMPLE_NODE_ID			7
#define REACTION_LOCAL_ID         50
#define ARM_MAIN_SP_PWM_ID        51
#define  ARM_MAIN_SP_COOL_ID         53
#define  ARM_MAIN_SP_SAMPLE_ID      55
#define SP_Regent_Cool_ID   57
#define SP_LIGHT   58
#define ARM_MAIN_SP_WASH_ID  59


#define             APP_CFG_TASK_START_PRIO                3u
static  OS_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];


extern void malloc_safe_insure(void);
static  void  AppTaskStart (void *p_arg);

extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart3;

extern uint8_t uart6_rx_buff[];
int MyAdapterID;

void my_main(void)
{
	printf("Now start BvRouteMain.\r\n");
	__disable_irq();	 //务必关掉一切中断
	malloc_safe_insure();
	//创建主任务
	OSInit();
	OSTaskCreateExt(AppTaskStart, 
					0,
					&AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
					APP_CFG_TASK_START_PRIO,
					APP_CFG_TASK_START_PRIO,
					&AppTaskStartStk[0],
					APP_CFG_TASK_START_STK_SIZE,
					0,
					(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

	OSStart();  
	__enable_irq();
}

void led_error_write(void){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
}

static void bus_new_driver_node_callback(bus_adapter* adapter, int addr){
    if(adapter->adatperId == CanAdapter.adatperId){
        CanDriver_AddFilter(addr);
    }
}

static  void  AppTaskStart (void *p_arg)
{
	uint8_t pwrcnt = 0;
	uint16_t keycnt = 0;
	uint16_t ledcnt = 0;
	uint16_t delaycnt = 50;
	
    Async_Init();
	
	HAL_UART_Receive_DMA(&huart6,uart6_rx_buff,2);

	MyAdapterID = UART6_ADAPTER_ID;
	Uart6Adapter.localId = BUS_ROUTER_SELF_ID;
	Uart6Adapter.adatperId  = UART6_ADAPTER_ID;
	bus_adapter_add(&Uart6Adapter,uart6_adapter_send,uart6_adapter_init);	//增加适配器

	CanAdapter.localId = BUS_ROUTER_SELF_ID;
	CanAdapter.adatperId = CAN_ADAPTER_ID;
	bus_adapter_add(&CanAdapter,can_adapter_send,can_adapter_init);	//增加适配器
	bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast, bus_new_driver_node_callback);		// 初始化总线
	printf("App Thread Start[%d]\r\n", CanAdapter.localId);

    {
        router_map_struct *node = New_router_map_struct();
        node->adapter = &Uart6Adapter;
        node->id = 0;
        router_list_add(node);
        CanDriver_AddFilter(0);
    }
    {
        router_map_struct *node = New_router_map_struct();
        node->adapter = &CanAdapter;
        node->id = 2;
        router_list_add(node);
    }
    {
        router_map_struct *node = New_router_map_struct();
        node->adapter = &CanAdapter;
        node->id = 3;
        router_list_add(node);
    }
	{
        router_map_struct *node = New_router_map_struct();
        node->adapter = &CanAdapter;
        node->id = 4;
        router_list_add(node);
    }
	{
        router_map_struct *node = New_router_map_struct();
        node->adapter = &CanAdapter;
        node->id = 5;
        router_list_add(node);
    }
	UartTransfer_Init();
	Io_OperationInit();
    colormod_init();
    powerled_init();
//    powerled_write_0_test();
//    for(int i = 0;i<8;i++){
//        colormod_write_color_test(i, 0x00, 0x00, 0xff);
//        bus_os_sleep(1000);
//    }
    
//	dac_write(1, 0.0f);
//	dac_write(2, 0.0f);
	
//	unsigned char j0101 = ws2812b_create(PE8);
//	unsigned char j0102 = ws2812b_create(PE9);
//	unsigned char j0103 = ws2812b_create(PE10);
//    ws2812b_write(j0102, 0x00FF00, 5);
//    ws2812b_write(j0101, 0x00FF00, 3);
	while(1)
    {
		{
			if((ledcnt++ % (500 / delaycnt)) == 0)
			{
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
			}
		}
		{
			if(pwrcnt < 0x14)
			{
				pwrcnt++;
				switch(pwrcnt)
				{
					case 0x01: HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET); break;
					case 0x0A: HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET); break;
					case 0x14: HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET); break;
					default: break;
				}
			}
		}
		{
			if(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) == GPIO_PIN_SET)
			{
				if(keycnt < (2000 / delaycnt))
				{
					keycnt++;
				}
				else
				{
					keycnt = 20000;
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
				}
			}
			else
			{
				if(keycnt > 2000)
				{
					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
				}
				keycnt = 0;
			}
		}
//		{
//			ws2812b_write(j0102, 0xFF0000, 5);
//		}
		OSTimeDly(delaycnt);
    }
}
