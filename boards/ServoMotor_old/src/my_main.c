#include "my_main.h"
#include "bus_can_f4_hal.h"
#include "bus_uart2_f4_hal.h"
#include "bus_uart3_f4_hal.h"
#include "bus_uart6_f4_hal.h"
#include "CanDriver.h"
#include "flash.h"
#include "my_callback.h"
#include "bus_cfg.h"
#include "project_cfg.h"
#include "cmsis_os2.h" 
#include "servoMotor_CFG.h"

#define             APP_CFG_TASK_START_PRIO                3u
static  OS_STK      AppTaskStartStk[APP_CFG_STARTUP_TASK_STK_SIZE];

extern void malloc_safe_insure(void);


#if IO_CMD_ENABLE == 1
#include "io_thread.h"

#define  					TASK_IO_PRIO                   16u
#define  					TASK_IO_STK_SIZE                     512u
static  bus_os_thread   	TASK_IO_TCB;
static  u32  				TASK_IO_STK[TASK_IO_STK_SIZE];

static void io_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_IO_TCB, thread, TASK_IO_STK, TASK_IO_STK_SIZE, TASK_IO_PRIO,NULL);
}
#endif

#if UART_TRANSPOND_ENABLE == 1
#include "uart_thread.h"
#define  					TASK_UART_PRIO                         16u
#define  					TASK_UART_STK_SIZE                     512u
static  bus_os_thread   	TASK_UART_TCB;
static  u32  				TASK_UART_STK[TASK_UART_STK_SIZE];

static void uart_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_UART_TCB, thread, TASK_UART_STK, TASK_UART_STK_SIZE, TASK_UART_PRIO,NULL);
}
#endif
#if MOTOR_CMD_ENABLE == 1
#include "motor_thread.h"
#include "mTMC5130Driver.h"
#include "mTMC5130Device.h"
extern void	MotorInit(void);
#endif

#if TEMP_CTRL_ENABLE == 1
#include "tempctrl_thread.h"
#define  					TASK_TEMPERATURE_PRIO                   15u
#define  					TASK_TEMPERATURE_STK_SIZE                     512u
static  bus_os_thread   	TASK_TEMPERATURE_TCB;
static  u32  				TASK_TEMPERATURE_STK[TASK_TEMPERATURE_STK_SIZE];
static void temperature_thread_create(void (*thread)(void* data)){
    bus_create_thread(&TASK_TEMPERATURE_TCB, thread, TASK_TEMPERATURE_STK, TASK_TEMPERATURE_STK_SIZE, TASK_TEMPERATURE_PRIO,NULL);
}
#endif



#if VOLTAGE_ENABLE == 1
#include "voltage.h"
#include "voltage_cfg.h"
#endif

static  void  AppTaskStart (void *p_arg);
static u32 local_id = 0;
int MyAdapterID;

extern TIM_HandleTypeDef htim2;
extern unsigned short pressure_read(void);
void led_error_write(void){
	ERR_LED_CTRL(1);
}


//
void my_main(void)
{
	
    printf("Now start Board.\r\n");
	
    local_id = Flash_GetAddress();
	//local_id=5;
    //ASSERT(local_id == 0, "local id error");
    printf("### Read Address = %d ###\r\n", local_id);
   
     malloc_safe_insure();

      osKernelInitialize();
	  char infobuf[20];
	  osVersion_t osv;
	  osStatus_t status;
	  status = osKernelGetInfo(&osv, infobuf, sizeof(infobuf));
	  if(status == osOK) {
		printf("Kernel Information: %s\r\n", infobuf);
		printf("Kernel Version    : %d\r\n", osv.kernel);
		printf("Kernel API Version: %d\r\n", osv.api);
	  }
	  
    OSTaskCreateExt(AppTaskStart, 
                    0,
                    &AppTaskStartStk[APP_CFG_STARTUP_TASK_STK_SIZE - 1],
                    APP_CFG_TASK_START_PRIO,
                    APP_CFG_TASK_START_PRIO,
                    &AppTaskStartStk[0],
                    APP_CFG_STARTUP_TASK_STK_SIZE,
                    0,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
	  

    osKernelStart(); 
    
}

static void bus_new_driver_node_callback(bus_adapter* adapter, int addr){
    if(adapter->adatperId == CanAdapter.adatperId){
        CanDriver_AddFilter(addr);
    }
}
static  void  AppTaskStart (void *p_arg)
{
    printf("AppTaskStart\r\n");
	
	osDelay(200);
	
#if STOP_SERVE_MOTOR == 1
	// 如果开启了禁用伺服电机则不初始化伺服电机
#else
	ServoMotorInit();
#endif
	
#if MOTOR_CMD_ENABLE == 1	
	MotorInit();
#endif	
	

#if UART_TRANSPOND_ENABLE == 1//如果 串口转发功能打开
    uart_rev_thread_init(uart_thread_create);//生成一个转发线程
#endif
	
#if IO_CMD_ENABLE == 1
    io_thread_init(io_thread_create);
#endif	

#if TEMP_CTRL_ENABLE == 1
    temp_thread_init(temperature_thread_create);
#endif	
	
#if VOLTAGE_ENABLE == 1
    voltage_init();
#endif
	
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



