#include "my_main.h"
#include "bus_can_f4_hal.h"
#include "bus_uart3_f4_hal.h"
#include "CanDriver.h"
#include "motor_thread.h"
#include "flash.h"
#include "my_callback.h"
#include "ProtoTest.h"
#include "bus_cfg.h"

#include "procfg.h"


#if LampOldFlag ==1
#include "mTMC5130Device.h"

#include "ad5245.h"

void MotorInit(void);
extern MOTOR motors[];
#endif

#if LampNewFlag==1
#include "voltage_cfg.h"
#include "lampCtl.h"
#endif

#define             APP_CFG_TASK_START_PRIO                3u
static  OS_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

extern void malloc_safe_insure(void);
static  void  AppTaskStart (void *p_arg);
static u32 local_id = 0;

#if LampOldFlag ==1
unsigned int iic_green = 0;
unsigned int iic_purple =0;
unsigned int iic_white =0;
#endif

int MyAdapterID;

void led_error_write(void){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
}

void my_main(void)
{
    printf("Now start Board.\r\n");
    local_id = Flash_GetAddress();
    ASSERT(local_id == 0, "local id error");
    printf("### Read Address = %d ###\r\n", local_id);
    __disable_irq();     //务必关掉一切中断
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


#if LampOldFlag ==1

void Light_ExitClose(void) {
    EXTI->IMR &= (~GPIO_PIN_12);
}
void Light_ExitOpen(void) {
    EXTI->IMR |= (GPIO_PIN_12);
}

#endif
#if LampNewFlag ==1


#endif
static  void  AppTaskStart (void *p_arg)
{
    printf("AppTaskStart \r\n");
  
#if LampOldFlag ==1  
    Light_ExitClose();
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
#endif
#if VOLTAGE_ENABLE == 1
    voltage_init();
#endif
	
    CanAdapter.localId = local_id;
    CanAdapter.adatperId = CAN_ADAPTER_ID;
	MyAdapterID = CAN_ADAPTER_ID;
	bus_adapter_add(&CanAdapter,can_adapter_send,can_adapter_init);    //增加适配器	
    bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast);        // 初始化总?	
//	
	#if LampOldFlag ==1  	
	iic_purple = ad5245_create(PE11, PE12);  //紫
	ad5245_write_test(iic_purple, 0, 0);

	iic_green = ad5245_create(PE9, PE10);  // Lime
	ad5245_write_test(iic_green, 0, 0);
//	
	iic_white = ad5245_create(PE13, PE14); //白
	ad5245_write_test(iic_white, 0, 0);
    
	MotorInit();
	motors[0].speed= 44480;
	motors[0].acc=500;
	motors[0].i_run=15;
	motors[0].micostep=16;
	motors[0].dir=0;
	motors[0].mTMC5130MotorRotate(&motors[0]);
	
	Light_ExitOpen();
	#endif	

	while(1)
    {
		#if LampNewFlag ==1
			
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_4);	
			LampCtl();	
		#endif	 
		#if LampOldFlag ==1  
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_4);	
			OSTimeDly(500);
		#endif			
    }
}

