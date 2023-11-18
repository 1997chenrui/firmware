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
#include "pwm_set.h"



#define             APP_CFG_TASK_START_PRIO                3u
static  OS_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

extern void malloc_safe_insure(void);
static  void  AppTaskStart (void *p_arg);
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim5;
int MyAdapterID;
extern TempCtrl_Struct temp_ctrl_mod[TempCtrl_Channel_Cnt];

//uint8_t  Temp_MSB,Temp_LSB,Temp_CRC,Humi_MSB,Humi_LSB ,Humi_CRC;
uint8_t   *temp=NULL, *humi=NULL ;
 
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
    motors[idx].speed = 12000;
    motors[idx].acc=12000;
    motors[idx].i_run=25;
    motors[idx].micostep=16;
    motors[idx].mTMC5130MotorReset(&motors[idx]);
}

static void motor_move(int idx, int position)
{
    motors[idx].id=idx;
    motors[idx].speed=12000;
    motors[idx].acc=12000;
    motors[idx].isBackZero=0;
    motors[idx].isRelative=0;
    motors[idx].i_run=25;
    motors[idx].micostep=16;
    motors[idx].postion=position;	
    motors[idx].mTMC5130MotorMove(&motors[idx]);
}

static void io_test(){
	printf("%d,%d,%d,%d,%d,%d,%d\r\n",
		HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5),
		HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6),
		HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7),
		HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0),
		HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1),
		HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2),
		HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5)
	
	);
//    updatePwm(1<<4|5, 1000, 50);
//    startPumpPwm(1<<4|5);
//    
//    updatePwm(2<<4|5, 1000, 50);
//    startPumpPwm(2<<4|5);
//    
//    updatePwm(3<<4|5, 1000, 50);
//    startPumpPwm(3<<4|5);
//	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
}

static  void  AppTaskStart (void *p_arg)
{
	Async_Init();

	CanAdapter.localId = 03;
	CanAdapter.adatperId = CAN_ADAPTER_ID;
	MyAdapterID = CAN_ADAPTER_ID;
	bus_adapter_add(&CanAdapter,can_adapter_send,can_adapter_init);
	bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast);
	printf("AppTaskStart[%d]\r\n", CanAdapter.localId);
	
	Io_OperationInit();  
	taskio_thread_init();
	MotorInit();

//	motor_reset(0);
//	motor_reset(1);
//	motor_reset(2);
//	motor_reset(3);

    
//    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
	
	while(1)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        OSTimeDly(500);    
//		io_test();
//		motor_reset(3);
//		motor_reset(2);
//		motor_reset(1);
//		motor_reset(0);
//		motor_move(0,5000);
//		motor_move(1,16000);
//		motor_move(2,1000);
//		motor_move(3,5000);
    }  
}



