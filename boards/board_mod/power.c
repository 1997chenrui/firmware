#include "power.h"
#include "power_mod.h"
#include "stm32f4xx.h"
#include "bus.h"
#include "cpu.h"
#include "power_mod.h"
#include "project_cfg.h"
#include "itf_00000040.h"

#if CLOSE_SCAN_CODE == 1
#include "delayus.h"
extern void UartScanCodeClose(void);
#endif

#if POWER_DEFAULT_ON != 1
extern UART_HandleTypeDef huart6;
#endif
extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart3;
static Itf_00000040_t* send_mod;
static bus_packet m_packet;

static void power_ctrl(u8 on);
static u8 button_trig(void);
static void power_listen_thread(void* data);
extern u8 uart6_rx_buff[];
extern Powermod_t power;
void power_init(void (*thread_create)(void (*thread)(void* data))){
#if POWER_DEFAULT_ON == 1
	power_ctrl(1);//开启 5v、电机、温控、PC电源
    powermod_init_with_state(power_ctrl, button_trig, SWITCH_ON);//初始化状态为 默认开启
#else
	powermod_init(power_ctrl, button_trig);//初始化状态为 默认关闭
#endif
    thread_create(power_listen_thread);
}

void power_cmd(bus_packet* packet){
	
//	uint8_t rt=0;
//	bus_ita_return(packet->adapterId, packet->srcId, packet->sessionId, 1, &rt);
	
    //若当前指令尚未被执行，则不响应其他指令
    if(send_mod != NULL){
        return;
    }
    send_mod = itf_00000040_new();
    send_mod->packet_parse(send_mod, packet);
    m_packet = *packet;
	

}

void power_ctrl(u8 on){
    POWER_CTRL(on);
}

static void cmd_run(){
    if(send_mod != NULL){
		//即使是 软件关机也要同步目前的电源状态
		uint8_t s= (send_mod->content.state);
        
#if CLOSE_SCAN_CODE == 1
       if(0 == s)
       {           
            UartScanCodeClose();
            delayMs(10);
       }
#endif
       printf("scan_code_by_up close succ\r\n");
        send_mod->rtn_and_free(send_mod, &m_packet, 0);
        send_mod = NULL;
		power.switch_state = SWITCH_OFF;
		power.trig_time = 0;
		power.state= MOD_OFF;
        power_ctrl(s);
    }
}

static u8 button_trig(void){
    return BTN_STATE_READ();
}


static void power_listen_thread(void* data){
    printf("### power thread start ###\r\n");
    while(1){
#if POWER_DEFAULT_ON == 1
		powermod_get_as_ref()->run1();//检测按键 松开
        
#elif POWER_DEFAULT_ON == 2
        powermod_get_as_ref()->run2();//检测按键 松开
#else
        powermod_get_as_ref()->run();//检测按键 按下
#endif
        cmd_run();//根据命令 执行一系列对 电源的开关操作
    }
}



