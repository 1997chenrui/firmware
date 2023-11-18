#include "my_callback.h"
#include "my_main.h"
#include "bus_can_f4_hal.h"
#include "bus_uart6_f4_hal.h"
#include "CanDriver.h"
#include "async.h"
#include "bus_cfg.h"
#include "power.h"
#include "io_thread.h"
#include "io_mod.h"
#include "uart_thread.h"
#include "rv_thread.h"
#include "flash.h"
#include "project_cfg.h"
#include "color.h"
#include "voltage.h"
#if AD_SIGNAL_ENABLE == 1
#include "ad_signal.h"
#include "ad_signal_cfg.h"
#include "AD7699.h"
#include "gpio.h"

#if EXFLASH_ENABLE == 1
#include "exflash.h"
#define EXFLASH	PB6
#endif

extern AD7699CONFIG ad7699[1];

#include "MS5194T.h"
extern MS5194TCONFIG MS5194T[1];
extern u8 MS5194T_map[];
#endif


#define             APP_CFG_TASK_START_PRIO                3u
static  OS_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

#define  					TASK_POWER_PRIO                         7u
#define  					TASK_POWER_STK_SIZE                     512u
static  bus_os_thread   	TASK_POWER_TCB;
static  u32  				TASK_POWER_STK[TASK_POWER_STK_SIZE];

#if IO_CMD_ENABLE == 1
#define  					TASK_IO_PRIO                         8u
#define  					TASK_IO_STK_SIZE                     512u
static  bus_os_thread   	TASK_IO_TCB;
static  u32  				TASK_IO_STK[TASK_IO_STK_SIZE];
#endif

#if UART_TRANSPOND_ENABLE == 1
#define  					TASK_UART_PRIO                         6u
#define  					TASK_UART_STK_SIZE                     512u
static  bus_os_thread   	TASK_UART_TCB;
static  u32  				TASK_UART_STK[TASK_UART_STK_SIZE];
#endif
#if RUNZE_ENABLE == 1
#define  					TASK_ROTATE_VALVE_PRIO                         10u
#define  					TASK_ROTATE_VALVE_STK_SIZE                     512u
static  bus_os_thread   	TASK_ROTATE_VALVE_TCB;
static  u32  				TASK_ROTATE_VALVE_STK[TASK_ROTATE_VALVE_STK_SIZE];
#endif
int MyAdapterID;

extern void MX_DMA_Init(void);
extern void MX_USART6_UART_Init(void);
 
extern UART_HandleTypeDef huart6;

extern UART_HandleTypeDef huart1;

extern uint8_t uart6_rx_buff[];
extern void malloc_safe_insure(void);



static  void  AppTaskStart (void *p_arg);
static u32 local_id = 0;


static void power_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_POWER_TCB, thread, TASK_POWER_STK, TASK_POWER_STK_SIZE, TASK_POWER_PRIO,NULL);
}
#if IO_CMD_ENABLE == 1
static void io_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_IO_TCB, thread, TASK_IO_STK, TASK_IO_STK_SIZE, TASK_IO_PRIO,NULL);
}
#endif
#if UART_TRANSPOND_ENABLE == 1
static void uart_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_UART_TCB, thread, TASK_UART_STK, TASK_UART_STK_SIZE, TASK_UART_PRIO,NULL);
}
#endif

#if RUNZE_ENABLE == 1
static void rotate_valve_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_ROTATE_VALVE_TCB, thread, TASK_ROTATE_VALVE_STK, TASK_ROTATE_VALVE_STK_SIZE, TASK_ROTATE_VALVE_PRIO,NULL);
}
#endif

void my_main(void)
{
    local_id = Flash_GetAddress();;
	
	//local_id=1;
    ASSERT(local_id == 0, "local id error");

    printf("### Read Address = %d ###\r\n", local_id);
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
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
}

static void bus_new_driver_node_callback(bus_adapter* adapter, int addr){
    if(adapter->adatperId == CanAdapter.adatperId){
        CanDriver_AddFilter(addr);
    }
}


void PVD_IRQHandler(void)
{
    HAL_PWR_PVD_IRQHandler();

}
void HAL_PWR_PVDCallback(void)
{
#if	WS_LED_ENABLE ==1
	colormod_init();
	HAL_UART_Transmit(&huart1,(uint8_t*)"1\n",2,0xFFFF);
#endif
}



static  void  AppTaskStart (void *p_arg)
{
    printf("App Thread Start[%d]\r\n", local_id);
	
    Async_Init();//初始化 异步 生成其线程

#if AD_SIGNAL_ENABLE == 1
	ad_signal_init();
 #endif	
	
#if IO_CMD_ENABLE == 1	
    io_thread_init(io_thread_create);//初始化iO监听线程 并生成其线程
#endif

#if UART_TRANSPOND_ENABLE == 1//如果 串口转发功能打开
    uart_rev_thread_init(uart_thread_create);//生成一个转发线程
#endif
#if VOLTAGE_ENABLE == 1
    voltage_init();
#endif
	
#if EXFLASH_ENABLE == 1
	exflash_init(EXFLASH);
#endif


	MyAdapterID = UART6_ADAPTER_ID;
	Uart6Adapter.localId = local_id;
	Uart6Adapter.adatperId  = UART6_ADAPTER_ID;
	bus_adapter_add(&Uart6Adapter,uart6_adapter_send,uart6_adapter_init);

	CanAdapter.localId = local_id;
	CanAdapter.adatperId = CAN_ADAPTER_ID;
	bus_adapter_add(&CanAdapter,can_adapter_send,can_adapter_init);
	
	bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast, bus_new_driver_node_callback);

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
        CanDriver_AddFilter(2);
    }
    {
        router_map_struct *node = New_router_map_struct();
        node->adapter = &CanAdapter;
        node->id = 3;
        router_list_add(node);
        CanDriver_AddFilter(3);
    }
	{
        router_map_struct *node = New_router_map_struct();
        node->adapter = &CanAdapter;
        node->id = 4;
        router_list_add(node);
        CanDriver_AddFilter(4);
    }
	{
        router_map_struct *node = New_router_map_struct();
        node->adapter = &CanAdapter;
        node->id = 5;
        router_list_add(node);
        CanDriver_AddFilter(5);
    }
	{
        router_map_struct *node = New_router_map_struct();
        node->adapter = &CanAdapter;
        node->id = 6;
        router_list_add(node);
        CanDriver_AddFilter(6);
    }
	{
        router_map_struct *node = New_router_map_struct();
        node->adapter = &CanAdapter;
        node->id = 7;
        router_list_add(node);
        CanDriver_AddFilter(7);
    }	
	
	power_init(power_thread_create);//初始化 电源线程 生成其线程

	while(1)
    {
		NORMAL_LED_CTRL(0);
        OSTimeDly(500);
		NORMAL_LED_CTRL(1);
		OSTimeDly(500);
		
    }
}
