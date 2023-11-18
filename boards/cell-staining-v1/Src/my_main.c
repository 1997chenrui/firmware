#include "my_main.h"
#include "bus_can_f4_hal.h"
#include "CanDriver.h"
#include "mTMC5130DThread.h"
#include "io_operation.h"
#include "flash.h"
#include "my_callback.h"
#include "ProtoTest.h"
#include "bus_cfg.h"

#include "mTMC5130Device.h"
#include "uart_byte_to_frame.h"
#include "macro.h"
#include "sensor.h"
#include "io.h"
#include "tct_pressure_singal.h"
#include "io_ctrl_event.h"
#include "project_cfg.h"

#define             APP_CFG_TASK_START_PRIO                3u
static  OS_STK      AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

#if TCT_PRESSURE_SINGAL_ENABLE == 1
#define  					TASK_PRESSURE_LISTEN_PRIO                           7u
#define  					TASK_PRESSURE_LISTEN_STK_SIZE                     	512u
static  bus_os_thread   	TASK_PRESSURE_LISTEN_TCB;
static  u32  				TASK_PRESSURE_LISTEN_Stk[TASK_PRESSURE_LISTEN_STK_SIZE];
#endif

#if IO_CTRL_EVENT_ENABLE == 1
#define  					TASK_IOEVENT_LISTEN_PRIO                           8u
#define  					TASK_IOEVENT_LISTEN_STK_SIZE                     	512u
static  bus_os_thread   	TASK_IOEVENT_LISTEN_TCB;
static  u32  				TASK_IOEVENT_LISTEN_Stk[TASK_IOEVENT_LISTEN_STK_SIZE];
#endif

int MyAdapterID;
static  void  AppTaskStart (void *p_arg);

extern void malloc_safe_insure(void);
extern MOTOR motors[];
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

#if TCT_PRESSURE_SINGAL_ENABLE == 1
static void pressure_sem_create(bus_os_sem* sem){
    bus_os_create_sem(sem, 0);
}

static void pressure_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_PRESSURE_LISTEN_TCB, thread, TASK_PRESSURE_LISTEN_Stk, TASK_PRESSURE_LISTEN_STK_SIZE, TASK_PRESSURE_LISTEN_PRIO,NULL);
}
#endif

#if IO_CTRL_EVENT_ENABLE == 1
static void ioevent_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_IOEVENT_LISTEN_TCB, thread, TASK_IOEVENT_LISTEN_Stk, TASK_IOEVENT_LISTEN_STK_SIZE, TASK_IOEVENT_LISTEN_PRIO,NULL);
}
#endif

static void io_write(u32 state){
    io_get_as_ref()->write(state);
}

#if PRESSURE_SENSOR_ENABLE == 1
static u16 sensor_read(u8 idx){
    return sensor_get_as_ref()->read(idx);
}
#endif


void led_error_write(void){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
}


void my_main(void)
{
    printf("Now start Bv arm.\r\n");
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

/** 测试函数 **/
#if 0
static void motor_reset(int idx)
{
    motors[idx].id = idx;
    motors[idx].speed = 5000;
    motors[idx].acc=5000;
    motors[idx].i_run=15;
    motors[idx].micostep=16;
    motors[idx].mTMC5130MotorReset(&motors[idx]);
}

static void motor_move(int idx, int position)
{
    motors[idx].id=idx;
    motors[idx].speed=5000;
    motors[idx].acc=3450;
    motors[idx].isBackZero=0;
    motors[idx].isRelative=0;
    motors[idx].i_run=7;
    motors[idx].micostep=16;
    motors[idx].postion=position;	
    motors[idx].mTMC5130MotorMove(&motors[idx]);
}

static void motor_rotate(int idx)
{
    motors[idx].id=idx;
    motors[idx].speed=5000;
    motors[idx].acc=3450;
    motors[idx].i_run=7;
    motors[idx].micostep=16;	
    motors[idx].mTMC5130MotorRotate(&motors[idx]);
}

static void motor_stop(int idx)
{
    motors[idx].id=idx;
    motors[idx].i_run=7;
    motors[idx].micostep=16;	
    motors[idx].mTMC5130MotorStop(&motors[idx]);
}


static void io_test(bool open){
    if(open){
        io_write(0x2aaaa);
    }else{
        io_write(0x15555);
    }
}


static void sensor_test(void){
    printf("id0=%d, id1=%d,id2=%d,id3=%d\r\n",
        sensor_read(0),sensor_read(1),
        sensor_read(2),sensor_read(3)
    );
}

static void negative_test(){
    IOCtrl_cfg_t cfg;
    IOCtrl_timing_t before;
    before.io_value = 0x20569;
    before.time_delay = 0;
    IOCtrl_timing_t after;
    after.io_value = 0x2056a;
    after.time_delay = 0;
    cfg.before_len = 1;
    cfg.before = &before;
    cfg.after_len = 1;
    cfg.after = &after;
    cfg.threshold_u.split.sensor_idx = 1;
    cfg.threshold_u.split.threshold = 1400;
    cfg.threshold_u.split.threshold_enable = 1;
    cfg.threshold_u.split.trigger_way = 1;
    cfg.query_cnt = 4000;
    ioctrl_get_as_ref()->add(&cfg);
}

static void positive_test(){
    IOCtrl_cfg_t cfg;
    IOCtrl_timing_t before;
    before.io_value = 0x20569;
    before.time_delay = 0;
    IOCtrl_timing_t after;
    after.io_value = 0x2056a;
    after.time_delay = 0;
    cfg.before_len = 1;
    cfg.before = &before;
    cfg.after_len = 1;
    cfg.after = &after;
    cfg.threshold_u.split.sensor_idx = 1;
    cfg.threshold_u.split.threshold = 1400;
    cfg.threshold_u.split.threshold_enable = 1;
    cfg.threshold_u.split.trigger_way = 1;
    cfg.query_cnt = 100;
    ioctrl_get_as_ref()->add(&cfg);
}

static void pressure_singal_test(){
    Pressure_Cfg_t cfg;
    cfg.report_pressure_enable = true;
    cfg.threshold_enable = true;
    cfg.pressure_range_enable = true;
    cfg.is_pressure_bigger = true;
    cfg.sensor_idx = 0;
    cfg.diff_threthold = 1;
    cfg.listen_time_span = 1000;
    cfg.low_pressure = 100;
    cfg.high_prsspure = 3000;
    cfg.data_cnt = 100;
    pressure_get_as_ref()->add(&cfg);
}
#endif

static  void  AppTaskStart (void *p_arg)
{
    printf("AppTaskStart \r\n");
    CanAdapter.localId = LOCAL_ADDRESS;
    CanAdapter.adatperId = LOCAL_ADDRESS;
	MyAdapterID = CAN_ADAPTER_ID;
	bus_adapter_add(&CanAdapter,can_adapter_send,can_adapter_init);
    bus_app_init(bus_ita_func_callback,bus_ita_func_broadcast);

	MotorInit();
    io_init();
#if PRESSURE_SENSOR_ENABLE == 1
    sensor_init();
#endif
#if TCT_PRESSURE_SINGAL_ENABLE == 1
    pressure_singal_init(pressure_thread_create, 
        pressure_sem_create, 
        sensor_read,
        io_get_as_ref()->timing_io_write);
#endif
#if IO_CTRL_EVENT_ENABLE == 1
    ioctrl_init(ioevent_thread_create, sensor_read, io_write);
#endif
    
    while(1)
    {	
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
        OSTimeDly(500);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
        OSTimeDly(500);
    }
}

