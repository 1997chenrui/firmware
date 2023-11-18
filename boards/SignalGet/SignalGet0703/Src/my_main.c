#include "my_callback.h"
#include "my_main.h"
#include "bus_can_f4_hal.h"
#include "CanDriver.h"
#include "async.h"
#include "main_cfg.h"

#include "AD7699BySG0703.h"

//#include "AD7699.h"
#include "flash.h"
#include "sp_light.h"
#include "tempctrl_thread.h"
#include "project_cfg.h"
#include "ad_signal.h"
#include "lampCtl.h"

#include "voltage.h"

#include "sensor_single_ctl.h"

extern u16 *AD7699_ReadAllChannel_GW6000AD7699_ReadAllChannel_GW6000(Light_Ad_t *buffer, u16 laser_id, u16 FirstAdTime_A, u16 FirstAdTime_B);


#define             APP_CFG_TASK_START_PRIO                3u
static  OS_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

extern void malloc_safe_insure(void);
static  void  AppTaskStart (void *p_arg);
static u32 local_id = 0;

u32 cnt=0;
void led_error_write(void){
	ERR_LED_CTRL(1);
}

void my_main(void)
{
	printf("Now start Board.\r\n");
    local_id = Flash_GetAddress();
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


int MyAdapterID = 0;

static  void  AppTaskStart (void *p_arg)
{
	printf("App Thread Start. Prio = %d\r\n",APP_CFG_TASK_START_PRIO);
    Async_Init();
	ad_signal_init();
	Light_Init();
	
	voltage_init();
    MyAdapterID = CAN_ADAPTER_ID;
	CanAdapter.localId = local_id;
    CanAdapter.adatperId = MyAdapterID;
    bus_adapter_add(&CanAdapter,can_adapter_send,can_adapter_init);    //增加适配器
    bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast);        // 初始化总线

	OSTimeDly(2000);
	
//	Light_Start();//开启定时器会出现硬件异常
	//test
	#if 0
	extern void Light_Data_Get_Test(void);
	extern unsigned short Light_RemianDataCountGet(void);
	extern Wave wave[];
	#endif
	while(1)
    {
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);	
		LampCtl();
//		u16 remainCnt = Light_RemianDataCountGet();
//		printf("rc = %d\r\n",remainCnt);
//		Light_Data_Get_Test();
    }
}
