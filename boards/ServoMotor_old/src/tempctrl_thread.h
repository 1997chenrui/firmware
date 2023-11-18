#ifndef TEMP_CTRL_THREAD_H_
#define TEMP_CTRL_THREAD_H_
#include "os_implement.h"
#include "pid.h"
#include <stdbool.h>
#include "bus.h"

typedef struct _Temp_thread_t{
    u8 is_start;
    u8 lowest_power;
    u8 iic_id;
    Pid_cfg_t pid_cfg;
    Pid_t *pid;
    void (*hd_open)(u8 open);
    u8 (*target_over)(f32 current);
    f32 (*convert_code)(f32 adjust);
    void (*open)(struct _Temp_thread_t*);
    void (*close)(struct _Temp_thread_t*);
    void (*target_set)(struct _Temp_thread_t*, f32);
    f32 (*temperature_read)(void);
    f32 (*read_buffer_temperature)(void);
} Temp_thread_t;
void power_set(Temp_thread_t *self, u8 code);
void temp_thread_init(void (*thread_create)(void (*thread)(void *data)));
Temp_thread_t *tempctrl_get_as_ref(u8 idx);
void temperature_cmd_run(u32 itf_code, bus_packet* packet);
void power_set_lowest(Temp_thread_t *);
#endif
