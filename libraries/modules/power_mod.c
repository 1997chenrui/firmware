#include "power_mod.h"
#include "bus.h"
#include "project_cfg.h"

#if CLOSE_SCAN_CODE == 1
#include "delayus.h"
extern void UartScanCodeClose(void);
#endif

#define QUERY_TIME_SPAN 100
#define POWER_ON_QUERY_TIME_CNT 500
#define POWER_OFF_QUERY_TIME_CNT 2000 



static void run(void);
static void run1(void);
static void run2(void);

static POWER_MOD_CTRL pfunc_ctrl;
static POWER_MOD_BTN_TRIG pfunc_read;
 Powermod_t power;

void powermod_init_with_state(POWER_MOD_CTRL ctrl, POWER_MOD_BTN_TRIG read_trig, Powermod_switch_e state){
    Powermod_t* self = &power;
    self->state = MOD_ON;
    self->switch_state = state;
    self->trig_time = 0;
    self->run = run;
	self->run1 = run1;
    self->run2 = run2;
    pfunc_ctrl = ctrl;
    pfunc_read = read_trig;
}

void powermod_init(POWER_MOD_CTRL ctrl, POWER_MOD_BTN_TRIG read_trig){
	powermod_init_with_state(ctrl, read_trig, SWITCH_OFF);

}


Powermod_t* powermod_get_as_ref(void){
    return &power;
}

/**
 * @brief 开机状态按下2s关机 关机状态按下500ms关机
 * 
 */
static void run(void){
    Powermod_t* self = &power;
    u8 trig = pfunc_read();
    switch (self->switch_state)
    {
    case SWITCH_ON:
        if(trig){
            if(self->trig_time > POWER_OFF_QUERY_TIME_CNT){
                self->switch_state = SWITCH_IDLE;
                self->trig_time = 0;
                self->state = MOD_OFF;
				printf("power close\r\n");
                pfunc_ctrl(0);
            }
        }else{
            self->trig_time = 0;
        }
        break;
    case SWITCH_OFF:
        if(trig){
            if(self->trig_time > POWER_ON_QUERY_TIME_CNT){
                self->switch_state = SWITCH_IDLE;
                self->trig_time = 0;
                self->state = MOD_ON;
				printf("power ON\r\n");
                pfunc_ctrl(1);        
            }
        }else{
            self->trig_time = 0;
        }
        break;
    case SWITCH_IDLE:
        if(!trig){
            self->switch_state = self->state == MOD_OFF ? SWITCH_OFF:SWITCH_ON;
        }
        break;
    default:
        break;
    }
    bus_os_sleep(QUERY_TIME_SPAN);
    self->trig_time += QUERY_TIME_SPAN;
}

/**
* @brief 程序运行时即默认为开机状态（由硬件开机），按下2s后关闭其他继电器，松开按钮关整机
 * 
 */
static void run1(void){
    Powermod_t* self = &power;
    u8 trig = pfunc_read();
    switch (self->switch_state)
    {
    case SWITCH_ON:
        if(trig){
            if(self->trig_time > POWER_OFF_QUERY_TIME_CNT){
                self->switch_state = SWITCH_IDLE;
                self->trig_time = 0;
				pfunc_ctrl(2);     
            }
        }else{
            self->trig_time = 0;
        }
        break;
    case SWITCH_IDLE:
		if(!trig){
			self->state = MOD_OFF;
			pfunc_ctrl(0);
		}
        break;
    default:
        break;
    }
    bus_os_sleep(QUERY_TIME_SPAN);
	
    self->trig_time += QUERY_TIME_SPAN;
}

/**
 * @brief 开机状态按下2s关机 关机状态按下500ms关机（同时关闭条码枪）
 * 
 */

static void run2(void){
    Powermod_t* self = &power;
    u8 trig = pfunc_read();
    switch (self->switch_state)
    {
    case SWITCH_ON:
        if(trig){
            if(self->trig_time > POWER_OFF_QUERY_TIME_CNT){
                
#if CLOSE_SCAN_CODE == 1
                UartScanCodeClose();
                delayMs(10);
#endif
                printf("scan_code_by_hand close succ\r\n");
                self->switch_state = SWITCH_IDLE;
                self->trig_time = 0;
                self->state = MOD_OFF;
				printf("power close\r\n");
                pfunc_ctrl(0);
            }
        }else{
            self->trig_time = 0;
        }
        break;
    case SWITCH_OFF:
        if(trig){
            if(self->trig_time > POWER_ON_QUERY_TIME_CNT){
                self->switch_state = SWITCH_IDLE;
                self->trig_time = 0;
                self->state = MOD_ON;
				printf("power ON\r\n");
                pfunc_ctrl(1);        
            }
        }else{
            self->trig_time = 0;
        }
        break;
    case SWITCH_IDLE:
        if(!trig){
            self->switch_state = self->state == MOD_OFF ? SWITCH_OFF:SWITCH_ON;
        }
        break;
    default:
        break;
    }
    bus_os_sleep(QUERY_TIME_SPAN);
    self->trig_time += QUERY_TIME_SPAN;
}
