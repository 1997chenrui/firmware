#include "my_main.h"
#include "bus_can_f4_hal.h"
#include "bus_uart2_f4_hal.h"
#include "CanDriver.h"
#include "motor_thread.h"
#include "mTMC5130Driver.h"
#include "mTMC5130Device.h"
#include "flash.h"
#include "my_callback.h"
#include "bus_cfg.h"
#include "tempctrl_thread.h"
#include "rv_thread.h"
#include "project_cfg.h"
#include "io_thread.h"

#define             APP_CFG_TASK_START_PRIO                3u
static  OS_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

extern void malloc_safe_insure(void);
extern void	MotorInit(void);
static  void  AppTaskStart (void *p_arg);
static u32 local_id = 0;
int MyAdapterID;

extern TIM_HandleTypeDef htim2;
extern unsigned short pressure_read(void);
void led_error_write(void){
	ERR_LED_CTRL(1);
}

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
	MotorInit();

	CanAdapter.localId = local_id;
    CanAdapter.adatperId = CAN_ADAPTER_ID;
	MyAdapterID = CAN_ADAPTER_ID;
    bus_adapter_add(&CanAdapter,can_adapter_send,can_adapter_init);
    bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast);
	
    while(1)
    {
        NORMAL_LED_CTRL(1);
        OSTimeDly(500);
        NORMAL_LED_CTRL(0);
        OSTimeDly(500);
    }
}



