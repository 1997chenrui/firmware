#include "my_main.h"
#include "bus_can_f4_hal.h"
#include "bus_uart2_f4_hal.h"
#include "bus_uart3_f4_hal.h"
#include "bus_uart6_f4_hal.h"
#include "CanDriver.h"
#if MOTOR_CMD_ENABLE == 1
#include "motor_thread.h"
#endif
#include "mTMC5130Driver.h"
#include "mTMC5130Device.h"
#include "flash.h"
#include "my_callback.h"

#include "bus_cfg.h"

#include "tempctrl_thread.h"
#include "rv_thread.h"
#include "project_cfg.h"
#include "io_thread.h"


#if WS_LED_ENABLE == 1
#include "color.h"
#endif
#if UART_TRANSPOND_ENABLE == 1
#include "uart_thread.h"
#endif
#if VOLTAGE_ENABLE == 1
#include "voltage.h"
#include "voltage_cfg.h"
#endif
#if EPOS_ENABLE == 1
#include "servo_thread.h"
#endif
#if HANDLER_ENABLE == 1
#include "hander.h"
#endif
#if AD_SIGNAL_ENABLE == 1
#include "ad_signal.h"
#include "ad_signal_cfg.h"
#endif

#if LISTENANDSYNC_ENABLE == 1
#include "listenandsync.h"
#include "listenandsync_cfg.h"

#endif

#define             APP_CFG_TASK_START_PRIO                3u
static  OS_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

#if TEMP_CTRL_ENABLE == 1
#define  					TASK_TEMPERATURE_PRIO                   15u
#define  					TASK_TEMPERATURE_STK_SIZE                     512u
static  bus_os_thread   	TASK_TEMPERATURE_TCB;
static  u32  				TASK_TEMPERATURE_STK[TASK_TEMPERATURE_STK_SIZE];
static void temperature_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_TEMPERATURE_TCB, thread, TASK_TEMPERATURE_STK, TASK_TEMPERATURE_STK_SIZE, TASK_TEMPERATURE_PRIO,NULL);
}
#endif

#if IO_CMD_ENABLE == 1
#define  					TASK_IO_PRIO                   16u
#define  					TASK_IO_STK_SIZE                     512u
static  bus_os_thread   	TASK_IO_TCB;
static  u32  				TASK_IO_STK[TASK_IO_STK_SIZE];

static void io_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_IO_TCB, thread, TASK_IO_STK, TASK_IO_STK_SIZE, TASK_IO_PRIO,NULL);
}
#endif

#if UART_TRANSPOND_ENABLE == 1
#define  					TASK_UART_PRIO                         10u
#define  					TASK_UART_STK_SIZE                     512u
static  bus_os_thread   	TASK_UART_TCB;
static  u32  				TASK_UART_STK[TASK_UART_STK_SIZE];
static void uart_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_UART_TCB, thread, TASK_UART_STK, TASK_UART_STK_SIZE, TASK_UART_PRIO,NULL);
}
#endif

#if SHAKE_CMD_ENABLE == 1
#define  					TASK_SHAKE_PRIO                   17u
#define  					TASK_SHAKE_STK_SIZE                     512u
static  bus_os_thread   	TASK_SHAKE_TCB;
static  u32  				TASK_SHAKE_STK[TASK_SHAKE_STK_SIZE];

static void shake_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_SHAKE_TCB, thread, TASK_SHAKE_STK, TASK_SHAKE_STK_SIZE, TASK_SHAKE_PRIO,NULL);
}
#endif

extern void malloc_safe_insure(void);
#if MOTOR_CMD_ENABLE == 1
extern void	MotorInit(void);
#endif
static  void  AppTaskStart (void *p_arg);
static u32 local_id = 0;
int MyAdapterID;

extern TIM_HandleTypeDef htim2;
extern unsigned short pressure_read(void);



#if ABSORB_THERAD_ENABLE
extern void suck_init();
#endif

void led_error_write(void){
	ERR_LED_CTRL(1);
}

//
void my_main(void)
{
	
    printf("Now start Board.\r\n");
    local_id = Flash_GetAddress();
    ASSERT(local_id == 0, "local id error");
    printf("### Read Address = %d ###\r\n", local_id);
    __disable_irq();  
    malloc_safe_insure();
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

static void bus_new_driver_node_callback(bus_adapter* adapter, int addr){
    if(adapter->adatperId == CanAdapter.adatperId){
        CanDriver_AddFilter(addr);
    }
}
static  void  AppTaskStart (void *p_arg)
{
	
    printf("AppTaskStart\r\n");
#if MOTOR_CMD_ENABLE == 1	
	MotorInit();
#if RELAY_CMD
	OSTimeDly(2000);
	#if RELAY_CMD_A03
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);//电磁继电器默认打开
	#endif
	#if RELAY_CMD_A07
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);//电磁继电器默认打开
	#endif
#endif

#endif
	
#if LISTENANDSYNC_ENABLE == 1
    ListenAndSync_Init_();
	
#endif
	
#if IO_CMD_ENABLE == 1
    io_thread_init(io_thread_create);
#endif

#if UART_TRANSPOND_ENABLE == 1
    uart_rev_thread_init(uart_thread_create);
#endif
#if TEMP_CTRL_ENABLE == 1
    temp_thread_init(temperature_thread_create);
#endif
#if VOLTAGE_ENABLE == 1
    voltage_init();
#endif
#if RV_ENABLE == 1
    rv_thread_init();
#endif
#if EPOS_ENABLE == 1
    ServoMotor_Mod_Init();
#endif
#if HANDLER_ENABLE == 1
    hander_init();
#endif
#if AD_SIGNAL_ENABLE == 1
	ad_signal_init();
#endif
#if WS_LED_ENABLE == 1
	colormod_init();
#endif	
#if ABSORB_THERAD_ENABLE
	suck_init();
#endif

#if M1600_PROJECT ==1

	extern void bus_write_init(void);
	bus_write_init();
	
	local_id=1;

	MyAdapterID = UART3_ADAPTER_ID;
	Uart3Adapter.localId = local_id;
	Uart3Adapter.adatperId  = UART3_ADAPTER_ID;
	bus_adapter_add(&Uart3Adapter,uart3_adapter_send,uart3_adapter_init);



	MyAdapterID = UART2_ADAPTER_ID;
	Uart2Adapter.localId = local_id;
	Uart2Adapter.adatperId  = UART2_ADAPTER_ID;
	bus_adapter_add(&Uart2Adapter,uart2_adapter_send,uart2_adapter_init);

	
	CanAdapter.localId = local_id;
	CanAdapter.adatperId = CAN_ADAPTER_ID;
	bus_adapter_add(&CanAdapter,can_adapter_send,can_adapter_init);
	
	bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast, bus_new_driver_node_callback);
    {
        router_map_struct *node = New_router_map_struct();
        node->adapter = &Uart3Adapter;
        node->id = 0;
        router_list_add(node);
        CanDriver_AddFilter(0);
    }
	
#else
	CanAdapter.localId = local_id;
    CanAdapter.adatperId = CAN_ADAPTER_ID;
	MyAdapterID = CAN_ADAPTER_ID;
    bus_adapter_add(&CanAdapter,can_adapter_send,can_adapter_init);
    bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast);
	#if gw6000_v3_loudo == 1
	extern void TakeCup_Test();	
	extern void FunnelMotor_Create();
	FunnelMotor_Create();
	#endif		
	#if AD_SIGNAL_SEND_TASK==1
	extern void bus_write_init(void);
	bus_write_init();
	#endif
#endif

	
	
    while(1)
    {
        NORMAL_LED_CTRL(1);
        OSTimeDly(500);
        NORMAL_LED_CTRL(0);
        OSTimeDly(5000);	
		//TakeCup_Test();
		
    }
}
