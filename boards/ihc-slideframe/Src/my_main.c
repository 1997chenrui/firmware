#include "my_main.h"
#include "bus_can_f4_hal.h"
#include "CanDriver.h"
#include "io_operation.h"
#include "my_callback.h"
#include "tempctrl_thread.h"
#include "bus_cfg.h"
#include "async.h"
#include "taskio_thread.h"
#include "mTMC5130Device.h"
#include "mTMC5130DThread.h"
#include "bus.h"
#include "stm32f4xx.h"
#include "mTMC5130Device.h"
#include "sensors.h"

#define             APP_CFG_TASK_START_PRIO                3u
static  OS_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

#define  					TASK_TEMPERATURE_LISTEN_PRIO                           20u
#define  					TASK_TEMPERATURE_LISTEN_STK_SIZE                     	512u
static  bus_os_thread   	TASK_TEMPERATURE_LISTEN_TCB;
static  u32  				TASK_TEMPERATURE_LISTEN_Stk[TASK_TEMPERATURE_LISTEN_STK_SIZE];

extern void malloc_safe_insure(void);
static  void  AppTaskStart (void *p_arg);
int MyAdapterID;


static void temperature_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_TEMPERATURE_LISTEN_TCB, thread, TASK_TEMPERATURE_LISTEN_Stk, TASK_TEMPERATURE_LISTEN_STK_SIZE, TASK_TEMPERATURE_LISTEN_PRIO,NULL);
}
 
void led_error_write(void){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
}

void my_main(void)
{
    printf("Now start Bv arm.\r\n");
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


extern MOTOR motors[];

static void motor_reset(int idx)
{
    motors[idx].id = idx;
    motors[idx].speed = 5000;
    motors[idx].acc=5000;
    motors[idx].i_run=25;
    motors[idx].micostep=16;
    motors[idx].mTMC5130MotorReset(&motors[idx]);
}

static void motor_move(int idx, int position)
{
    motors[idx].id=idx;
    motors[idx].speed=5000;
    motors[idx].acc=5000;
    motors[idx].isBackZero=0;
    motors[idx].isRelative=0;
    motors[idx].i_run=25;
    motors[idx].micostep=16;
    motors[idx].postion=position;	
    motors[idx].mTMC5130MotorMove(&motors[idx]);
}

static void io_test(){
	printf("%d,%d,%d,%d,%d,%d,%d,%d\r\n",
		HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0),
		HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1),
		HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2),
		HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3),
		HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4),
		HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6),
		HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_15),
		HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)
	);
	HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_5);
}


static  void  AppTaskStart (void *p_arg)
{
    printf("AppTaskStart[02H]\r\n");
	Async_Init();

	CanAdapter.localId = 2;
	CanAdapter.adatperId = CAN_ADAPTER_ID;
	MyAdapterID = CAN_ADAPTER_ID;
	bus_adapter_add(&CanAdapter,can_adapter_send,can_adapter_init);
	bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast);
	Io_OperationInit();  
	taskio_thread_init();
    temperature_sensor_init();
	TempCtrl_Init(temperature_thread_create);
	MotorInit();
	
//	motor_move(0, 1000);
//	motor_move(1, 1000);
//	motor_reset(0);
//	motor_reset(1);
	
	while(1)
    {
		
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        OSTimeDly(500); 
//		motor_reset(0);
//		motor_reset(1);
//		motor_move(0, 3000);
//		motor_move(1, 3000);
		
//		io_test();
    }  
}



