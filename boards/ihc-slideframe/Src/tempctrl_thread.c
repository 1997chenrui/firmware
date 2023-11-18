#include "tempctrl_thread.h"
#include "ad5245.h"
#include "gpio.h"
#include "sensors.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <stdlib.h>

TempCtrl_Struct temp_ctrl_mod = {.is_start = 1,
                                 .pid_cfg.type = POSITION,
                                 .pid_cfg.target = 30,
                                 .pid_cfg.kp = 200,
                                 .pid_cfg.ki = 10,
                                 .pid_cfg.kd = 50,
                                 .pid_cfg.limit = 255,
                                 .pid_cfg.separate_i_range = 2};

static void TempCtrl_Thread(void *pdata);
static void close(void);
static void open(void);
static void target_set(f32 value);
static void open_report(bus_packet *packet);
static void close_report(bus_packet *packet);

Pid_t *pid;
unsigned int hiic = 0;
void TempCtrl_Init(void (*thread_create)(void (*thread)(void *data))) {
    TempCtrl_Struct *self = &temp_ctrl_mod;
    gpio_config(PE3, eGPIO_OUT_PP_UP, 0);
    hiic = ad5245_create(PE0, PE1);
    pid = pid_mod_new(self->pid_cfg);
    self->close = close;
    self->open = open;
    self->target_set = target_set;
    self->open_report = open_report;
    self->close_report = close_report;
    thread_create(TempCtrl_Thread);
}

TempCtrl_Struct *tempctrl_get_as_ref(void) { return &temp_ctrl_mod; }

static u8 convert_2_code(f32 adjust) {
    u8 code = (u8)(adjust < 0 ? 0 : adjust);
    code = 255 - code;
    code = code < 10 ? 10 : code;
    printf("adjust = %4f, code = %d\r\n", adjust, code);
    return code;
}

static void TempCtrl_Thread(void *pdata) {
    f32 adjust = 0;
    while (1) {
        f32 current_temp = temperaturesensor_get_as_ref()->read();
        if (temp_ctrl_mod.is_start) {
            adjust = pid->position_ctl(pid, current_temp);
            ad5245_write(hiic, 0, convert_2_code(adjust));
            if (adjust == 0) {
                gpio_write_pin(PE3, 0); // 关闭开关
            } else {
                gpio_write_pin(PE3, 1); // 打开开关
            }
        } else {
            gpio_write_pin(PE3, 0); // 关闭开关
        }

        bus_os_sleep(5000);
    }
}

static void close(void) {
    gpio_write_pin(PE3, 0); // 关闭开关
    temp_ctrl_mod.is_start = 0;
}

static void open(void) {
    gpio_write_pin(PE3, 1); // 打开开关
    temp_ctrl_mod.is_start = 1;
}

static void target_set(f32 value) { pid->target_set(pid, value); }

static void open_report(bus_packet *packet) {
    u8 data[4] = {0};
    f32 target = ReadF32(&packet->data[5]);
    printf("temp target = %4f\r\n", target);
    target_set(target);
    bus_ita_return(packet->adapterId, packet->srcId, packet->sessionId, 4,
                   data);
}

static void close_report(bus_packet *packet) {
    u8 data[4] = {0};
    printf("temp close\r\n");
    close();
    bus_ita_return(packet->adapterId, packet->srcId, packet->sessionId, 4,
                   data);
}
