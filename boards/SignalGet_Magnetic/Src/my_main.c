#include "my_callback.h"
#include "my_main.h"
#include "bus_can_f4_hal.h"
#include "CanDriver.h"
#include "main_cfg.h"
#include "flash.h"
#include "tempctrl_thread.h"
#include "project_cfg.h"

#include "ad5282_cfg.h"
#include "magneticBead_cfg.h"

#define  					TASK_TEMPERATURE_PRIO                   20u
#define  					TASK_TEMPERATURE_STK_SIZE                     512u
static  bus_os_thread   	TASK_TEMPERATURE_TCB;
static  u32  				TASK_TEMPERATURE_STK[TASK_TEMPERATURE_STK_SIZE];

static void temperature_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_TEMPERATURE_TCB, thread, TASK_TEMPERATURE_STK, TASK_TEMPERATURE_STK_SIZE, TASK_TEMPERATURE_PRIO,NULL);
}

#define             APP_CFG_TASK_START_PRIO                3u
static  OS_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

extern void malloc_safe_insure(void);
static  void  AppTaskStart (void *p_arg);
static u32 local_id = 0;

void led_error_write(void){
	ERR_LED_CTRL(1);
}

void my_main(void)
{
	printf("Now start Board.\r\n");
    local_id = Flash_GetAddress();
    //ASSERT(local_id == 0, "local id error");
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
extern TIM_HandleTypeDef htim8;
static  void  AppTaskStart (void *p_arg)
{
    printf("App Thread Start. Prio = %d\r\n",APP_CFG_TASK_START_PRIO);
    
    temp_thread_init(temperature_thread_create);
	AD5282_CFG_INIT();
	MagneticBeadCtrlTaskInit();
    MyAdapterID = CAN_ADAPTER_ID;
	CanAdapter.localId = local_id;
    CanAdapter.adatperId = MyAdapterID;
    bus_adapter_add(&CanAdapter,can_adapter_send,can_adapter_init);    //增加适配器
    bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast);        // 初始化总线

//	HAL_Delay(3000);
	__HAL_TIM_SET_AUTORELOAD(&htim8, 500);	
	__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 250); 
	
	HAL_Delay(3000);
	
	__HAL_TIM_SET_AUTORELOAD(&htim8, 0);	
	__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 0); 
//	
//	HAL_Delay(3000);
//	__HAL_TIM_SET_AUTORELOAD(&htim8, 300);	
//	__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 150); 
//	
//	HAL_Delay(3000);
//	__HAL_TIM_SET_AUTORELOAD(&htim8, 200);	
//	__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 100); 
	
//	HAL_Delay(3000);
//	__HAL_TIM_SET_AUTORELOAD(&htim8, 150);	
//	__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_1, 75); 
	while(1)
    {
		///HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_13);
 		NORMAL_LED_CTRL(1);
 		OSTimeDly(500);
 		NORMAL_LED_CTRL(0);
 		OSTimeDly(500);
    }
}
