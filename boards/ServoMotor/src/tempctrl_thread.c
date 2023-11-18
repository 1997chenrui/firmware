#include "tempctrl_thread.h"
#include "ad5245.h"
#include "itf_00060000.h"
#include "itf_00060010.h"
#include "itf_00060020.h"
#include "stm32f4xx_hal.h"
#include "tempctl_cfg.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


extern void temp_hd_init(void);

static const f32 temperature_range[] = {-30, 50};
static void tempctrl_thread(void *pdata);
static void temperature_thread_close(Temp_thread_t *);
static void temperature_thread_open(Temp_thread_t *);
static void temperature_target_set(Temp_thread_t *, f32 target);
static u8 convert_to_ad5245_value(f32 code);
static u8 is_temperature_valid(f32 temperature);

//制冷、加热
extern Temp_thread_t temperature[];

void temp_thread_init(void (*thread_create)(void (*thread)(void *data))) {
    temp_hd_init();
    for (int i = 0; i < TEMP_CTRL_CNT; i++) {
        power_set_lowest(&temperature[i]);
        temperature[i].open = temperature_thread_open;
        temperature[i].close = temperature_thread_close;
        temperature[i].target_set = temperature_target_set;
    }
    thread_create(tempctrl_thread);
}

Temp_thread_t *tempctrl_get_as_ref(u8 idx) {
    ASSERT(idx >= TEMP_CTRL_CNT, "temp idx error");
    return &temperature[idx];
}

/**
 * @brief 温控指令接口
 * 
 * @param itf_code 指令码
 * @param packet 数据包
 */
void temperature_cmd_run(u32 itf_code, bus_packet *packet) {
    switch (itf_code) {
    case 0x00060000: {
        //温度读取是耗时操作，在这里只需要读取缓存温度就可以了。
        //真正的读取传感器是在温控线程中。
        Itf_00060000_t *cmd = itf_00060000_new();
        cmd->packet_parse(cmd, packet);
        Temp_thread_t *self = tempctrl_get_as_ref(cmd->content.channel_id);
        f32 value = self->read_buffer_temperature();
        cmd->rtn_and_free(cmd, packet, value);
        break;
    }
    case 0x00060010: {
        Itf_00060010_t *cmd = itf_00060010_new();
        cmd->packet_parse(cmd, packet);
        u32 state = cmd->content.state;
        for (int i = 0; i < TEMP_CTRL_CNT; i++) {
            u8 cmd = state & 0x03;
            Temp_thread_t *self = tempctrl_get_as_ref(i);
            if (cmd == 0x01) {
                temperature_thread_close(self);
            } else if (cmd == 0x02) {
                temperature_thread_open(self);
            }
            state = state >> 2;
        }

        cmd->rtn_and_free(cmd, packet, 0);
        break;
    }
    case 0x00060020: {
        Itf_00060020_t *cmd = itf_00060020_new();
        cmd->packet_parse(cmd, packet);
        Temp_thread_t *self = tempctrl_get_as_ref(cmd->content.id);
        self->target_set(self, cmd->content.target_temp);
        cmd->rtn_and_free(cmd, packet, 0);
        break;
    }
    default:
        ASSERT("1", "cmd no support");
    }
}

/**
 * @brief 温控线程
 *
 * @param pdata
 * @return none
 */
static void tempctrl_thread(void *pdata) {
    f32 adjust = 0;
    for (;;) {
        for (int i = 0; i < TEMP_CTRL_CNT; i++) {
            Temp_thread_t *self = tempctrl_get_as_ref(i);
            
            f32 current_temp = self->temperature_read();
			//printf("current temp = %3f\r\n", current_temp);
            if (!self->is_start || self->target_over(current_temp) || !is_temperature_valid(current_temp)) {
                self->hd_open(false);
            } else {
                self->hd_open(true);
                adjust = self->pid->position_ctl(self->pid, current_temp);
                f32 code = self->convert_code((float)adjust);
                printf("current temp = %3f, code=%d\r\n", current_temp, convert_to_ad5245_value(code));
                
                power_set(self, convert_to_ad5245_value(code));
            }
#ifdef TEMP_CTRL_TIME_SPAN
            bus_os_sleep(TEMP_CTRL_TIME_SPAN);
#else
            bus_os_sleep(500);
#endif
        }
    }
}

/**
 * @brief 温控关
 * 
 * @param self 
 */
static void temperature_thread_close(Temp_thread_t *self) {
    self->is_start = 0;
    self->hd_open(false);
    power_set_lowest(self);
}

/**
 * @brief 打开温控线程
 *
 * @return none
 */
static void temperature_thread_open(Temp_thread_t *self) {
    self->is_start = 1;
    self->hd_open(true);
    power_set_lowest(self);
}

/**
 * @brief 设置目标温度
 *
 * @return none
 */
static void temperature_target_set(Temp_thread_t *self, f32 target) {
    self->pid->target_set(self->pid, target);
}

/**
 * @brief 输出
 *
 * @return none
 */
void power_set(Temp_thread_t *self, u8 code) {
    ad5245_write(self->iic_id & 0x0f, 0, code);
    u8 other = (self->iic_id >> 4) & 0x0f;
    if (other != 0) {
        ad5245_write((self->iic_id >> 4) & 0x0f, 0, code);
    }
}

/**
 * @brief 输出最低功率
 *
 * @return none
 */
void power_set_lowest(Temp_thread_t *self) {
    power_set(self, self->lowest_power);
}

/**
 * @brief 将计算的code转换为ad5245的值 
 * 注意：计算出的code有可能大于255，未防止溢出，此处加限制
 * @param code 计算的code
 * @return u8 ad5245值
 */
static u8 convert_to_ad5245_value(f32 code) {
    if (code > 255) {
        return 255;
    } else if (code < 0) {
        return 0;
    } else {
        return (u8)code;
    }
}


static u8 is_temperature_valid(f32 temperature){
    return temperature > temperature_range[0] && temperature < temperature_range[1];
}
