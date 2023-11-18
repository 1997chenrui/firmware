#include "my_main.h"
#include "bus_can_f4_hal.h"
#include "callback.h"
#include "CanDriver.h"
#include "async.h"
#include "flash.h"
#include "bus_cfg.h"
#include "project_cfg.h"
#include "boot_version.h"
#if BUS_UART6_PC_ENABLE == 1
#include "bus_uart6_f4_hal.h"
#endif
#if BUS_UART3_PC_ENABLE == 1
#include "bus_uart3_f4_hal.h"
#endif

#if BUS_USE_ROUTER_FUNC == 1
#include "power.h"
#define  					TASK_POWER_PRIO                         7u
#define  					TASK_POWER_STK_SIZE                     512u
static  bus_os_thread   	TASK_POWER_TCB;
static  u32  				TASK_POWER_STK[TASK_POWER_STK_SIZE];
static void power_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_POWER_TCB, thread, TASK_POWER_STK, TASK_POWER_STK_SIZE, TASK_POWER_PRIO,NULL);
}

static void bus_new_driver_node_callback(bus_adapter* adapter, int addr){
    if(adapter->adatperId == CanAdapter.adatperId){
        CanDriver_AddFilter(addr);
    }
}
#endif
#define             APP_CFG_TASK_START_PRIO                3u
static  OS_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

extern void malloc_safe_insure(void);
static  void  AppTaskStart (void *p_arg);

#if BUS_UART3_PC_ENABLE == 1
extern UART_HandleTypeDef huart3;
extern uint8_t uart3_rx_buff[];
#endif

#if BUS_UART6_PC_ENABLE == 1
extern UART_HandleTypeDef huart6;
extern uint8_t uart6_rx_buff[];
#endif



void led_error_write(void){
    ERR_LED_CTRL(1);
}

void my_main(void)
{
    
	printf("Now start Bootloader\r\n");    
    
    if(!Flash_ContainsAddress()){
        //不包含地址才写，减少flash写次数
        Flash_SetAddressAndBootVersion(LOCAL_ID, BOOT_VERSION);
        printf("### Address Set = %d ###\r\n", Flash_GetAddress());
    }
    
    //如果包含app，则一定包含地址
    if(Flash_ContainsApp())
    {
        printf("### JumpToApp ###\r\n");
        //跳转
        FirmWare_JumpToApp();
    }
    
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

int MyAdapterID;
static  void  AppTaskStart (void *p_arg)
{

#if BUS_UART3_PC_ENABLE == 1
    MyAdapterID = UART3_ADAPTER_ID;
	Uart3Adapter.localId = LOCAL_ID;
	Uart3Adapter.adatperId  = MyAdapterID;
	bus_adapter_add(&Uart3Adapter,uart3_adapter_send,uart3_adapter_init);	
#endif
	
#if BUS_UART6_PC_ENABLE == 1
	MyAdapterID = UART6_ADAPTER_ID;
	Uart6Adapter.localId = LOCAL_ID;
	Uart6Adapter.adatperId  = MyAdapterID;
	bus_adapter_add(&Uart6Adapter,uart6_adapter_send,uart6_adapter_init);	
#endif

	CanAdapter.localId = LOCAL_ID;
	CanAdapter.adatperId = CAN_ADAPTER_ID;
	bus_adapter_add(&CanAdapter,can_adapter_send,can_adapter_init);	//增加适配器
    
#if BUS_USE_ROUTER_FUNC == 1
	bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast, bus_new_driver_node_callback);		// 初始化总线
    // 0 
    router_map_struct *node = New_router_map_struct();
    node->adapter = &Uart6Adapter;
    node->id = 0;
    router_list_add(node);
    CanDriver_AddFilter(0);
#else
bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast);	// 初始化总线
#endif
    Async_Init();
    printf("this chip no app !!\r\n");
    for(uint32_t * i = (uint32_t*)FLASH_APP_START ;  i < (uint32_t*)(0x8060000); i++ )
    {
        if(*i != 0xffffffff)
        {
            //先清扫干净 再报告事件
            FLASH_If_Init();
            for(int  j = 4;j<=6;j++)
            {
                FLASH_If_Erase(j);
            }
            break;
        }
    }
    printf("erase ok\r\n");
    Async_Excute(1,Firmware_ReportEvent);

#if BUS_USE_ROUTER_FUNC == 1
    power_init(power_thread_create);
    POWER_CTRL(1);
	//RS232通信开
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
#endif
	while(1)
    {
        WARN_LED_CTRL(1);
		OSTimeDly(500);
        WARN_LED_CTRL(0);
        OSTimeDly(500);
    }

}



