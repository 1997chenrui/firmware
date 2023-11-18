#include "shake_thread.h"
#include "gpio.h"
#include "ad5245.h"
#include "stm32f4xx.h"

#define MAX_VOLTAGE 3.3f
#define MAX_CODE 255.0f

static Shake_t shake_obj;
static Itf_00070011_t* itf_cmd;
static u8 iic_id;
static f32 volage_range[] = {0.0f, 255.0f};

static void shake(void);
static void shake_time(u16);
static void shake_cfg(Shake_content_t* self);
static void shake_asyn(Shake_content_t* content);
static void shake_thread(void* data);
static void shake_start(void);
static void shake_stop(void);
static f32 calc_voltage(u8 persent);

void shake_thread_init(void (*thread_create)(void (*thread)(void* data))){
    Shake_t* self = &shake_obj;
    self->content.voltage = volage_range[0];
    self->content.time_ms = 800;
    iic_id = ad5245_create(PD2, PD3);
    // 打开LM控制开关
    gpio_config(PB3, eGPIO_OUT_PP_UP, 1);
    self->is_busy = 0;
    self->shake = shake;
    self->shake_time = shake_time;
    self->shake_cfg = shake_cfg;
    bus_os_create_sem(&self->sem, 0);
    thread_create(shake_thread);
}

void shake_cmd_run(bus_packet* packet){
    Itf_00070011_t* cmd = itf_00070011_new();
    itf_cmd = cmd;
    cmd->packet_parse(cmd, packet);
    Shake_content_t shake_content;
    shake_content.voltage = calc_voltage(cmd->content.persent);
    shake_content.time_ms = cmd->content.time_ms;
    shake_content.packet = *packet;
    shake_content.is_cfg = 1;
    shake_asyn(&shake_content);
}

Shake_t* shake_get_as_ref(void){
    return &shake_obj;
}

static f32 calc_voltage(u8 persent){
	if(persent > 100) persent = 100;
    return volage_range[0] + persent * (volage_range[1]  -volage_range[0]) / 100;
}

static void shake_cfg(Shake_content_t* self){
    u8 code = 0;
    f32 calc = self->voltage;
    if(calc > 255){
        code = 255;
    } else if(calc > code){
        code = (u8)calc;
    }
	code = 255 - code;
    printf("code = %d\r\n", code);
    printf("time = %d\r\n", self->time_ms);
    
    if(self->is_cfg){
        ad5245_write(iic_id, 0, code);
        bus_os_sleep(50);
        self->is_cfg = 0;
    }
    shake_start();
    if(self->time_ms > 0){
        bus_os_sleep(self->time_ms);
    }
    shake_stop();
}

static void shake(void){
    Shake_t* self = &shake_obj;
    shake_start();
    printf("time1=%d\r\n",self->content.time_ms);
    if(self->content.time_ms > 0){
        bus_os_sleep(self->content.time_ms);
    }
    shake_stop();
}

static void shake_time(u16 time_ms){
    shake_start();
    if(time_ms > 0){
        bus_os_sleep(time_ms);
    }
    shake_stop();
}

static void shake_asyn(Shake_content_t* content){
    Shake_t* self = &shake_obj;
    ASSERT(self->is_busy, "shake thread busy");
    self->is_busy = 1;
    self->content = *content;
    bus_os_post_sem(&self->sem);
}

static void shake_start(void){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}

static void shake_stop(void){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}

/** 局部函数 **/
static void shake_thread(void* data){
    printf("** shake thread start ***\r\n");
    Shake_t* self = &shake_obj;
	while(1)
	{
		bus_os_pend_sem(&self->sem, 0);
        shake_cfg(&self->content);
        if(itf_cmd != NULL){
            itf_cmd->rtn_and_free(itf_cmd, &self->content.packet, 0);
        }
        self->is_busy = 0;
	}
}
