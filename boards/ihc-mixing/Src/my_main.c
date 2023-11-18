#include "my_main.h"
#include "bus_can_f4_hal.h"
#include "CanDriver.h"
#include "io_operation.h"
#include "my_callback.h"
#include "bus_cfg.h"
#include "ServoMotor_EPOS2.h"
#include "async.h"
#include "taskio_thread.h"
#include "mTMC5130Device.h"
#include "mTMC5130DThread.h"
#include "bus.h"
#include "stm32f4xx.h"
#include "mTMC5130Device.h"
#include "dac.h"
#include "tct_mixing.h"



#define             APP_CFG_TASK_START_PRIO                3u
static  OS_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

#define  					TASK_MIXING_LISTEN_PRIO                           8u
#define  					TASK_MIXING_LISTEN_STK_SIZE                     	512u
static  bus_os_thread   	TASK_MIXING_LISTEN_TCB;
static  u32  				TASK_MIXING_LISTEN_Stk[TASK_MIXING_LISTEN_STK_SIZE];

extern void malloc_safe_insure(void);
static  void  AppTaskStart (void *p_arg);
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim2;
int MyAdapterID;

static void mix_sem_create(bus_os_sem* sem){
    bus_os_create_sem(sem, 0);
}

static void mix_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_MIXING_LISTEN_TCB, thread, TASK_MIXING_LISTEN_Stk, TASK_MIXING_LISTEN_STK_SIZE, TASK_MIXING_LISTEN_PRIO,NULL);
}

static void mix_voltage_write(float voltage){
    u32 value = (u32)(voltage/3.3*4096);
    value = value > 4096?4096:value;
    printf("mix value = %d\r\n", value);
    dac_write(value);
}

static void mix_power(u8 is_on){
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, is_on);
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
    motors[idx].speed = 1000;
    motors[idx].acc=1000;
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
	printf("%d,%d,%d,%d,%d,%d\r\n",
		HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_0),
		HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_1),
		HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_2),
		HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_3),
		HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4),
		HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6)
	);
}

static  void  AppTaskStart (void *p_arg)
{
	Async_Init();

	CanAdapter.localId = 05;
	CanAdapter.adatperId = CAN_ADAPTER_ID;
	MyAdapterID = CAN_ADAPTER_ID;
	bus_adapter_add(&CanAdapter,can_adapter_send,can_adapter_init);
	bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast);
	printf("AppTaskStart[%d]\r\n", CanAdapter.localId);
	
	Io_OperationInit();  
	taskio_thread_init();
	MotorInit();
    tctmixing_init(3.3, 500, mix_voltage_write,mix_power, mix_thread_create, mix_sem_create);
	while(1)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        OSTimeDly(500);    
    }  
}



