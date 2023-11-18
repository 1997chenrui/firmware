#include "my_main.h"
#include "bus_can_f4_hal.h"
#include "my_callback.h"
#include "CanDriver.h"
#include "AD7699.h"
#include "spi_driver.h"
#include "delayus.h"
#include "async.h"
#include "flash.h"
//#include "my_node_local_id.h"
#include "bus_cfg.h"
#include "unite_node_local_id.h"
#include "auto_cali_client.h"
#include "cal.h"

extern TIM_HandleTypeDef htim5;

#define             APP_CFG_TASK_START_PRIO                3u
static  OS_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

extern void malloc_safe_insure(void);
static  void  AppTaskStart (void *p_arg);
static u32 local_id = 0;
int MyAdapterID = 0 ;
void my_main(void)
{
    printf("Now start Board.\r\n");
    local_id = Flash_GetAddress();
    //ASSERT(local_id == 0, "local id error");
    printf("### Read Address = %d ###\r\n", local_id);
    __disable_irq();     //务必关掉一切中断
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


extern auto_client auto_client_t;

void led_error_write(void){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
}


static  void  AppTaskStart (void *p_arg)
{
    printf("MC800 AppTaskStart ID:53\r\n");
	
    MyAdapterID = CAN_ADAPTER_ID;
    CanAdapter.localId = local_id;    
    CanAdapter.adatperId = MyAdapterID;
    bus_adapter_add(&CanAdapter,can_adapter_send,can_adapter_init);    //增加适配器
    bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast);        // 初始化总线
	//bus_ita_func_callback为接受命令回调函数
	
	SPIDRIVER_AD7699SPI2Config(); 
    cali_client_Iint(&auto_client_t);
    mc800_light_init();
	
    while(1)
    {
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
        OSTimeDly(500);
    }
}

