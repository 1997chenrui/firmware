/**
 * @file runze_sv04_new.c
 * @author Chenr
 * @brief runze旋转泵控制驱动，只实现了串口功能。
 * 发送方    |      接收方
 * 发送指令   ->    校验错误
 *           <-    返回错误
 *------------------------
 * 发送指令   ->    校验成功
 *                 执行
 *           <-    返回执行状态
 * @version 0.1
 * @date 2023-04-22
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "runze_sv04_new.h"
#include "tool.h"
#include <stdlib.h>
#include <string.h>

#define HEAD 0xAA
#define TAIL 0x00

// F factory 工厂指令
#define F_ADDRESS_SET_CODE 0x00
#define F_BAUDRATE_SET_CODE 0x01
#define F_AUTO_RESET_CODE 0x0E
#define F_PARAMETER_LOCK_CODE 0xFC
#define F_PARAMETER_RESET 0xFF

// N normal 正常指令
//查询指令
#define N_ADDRESS_QUERY 0x20
#define N_BAUDRATE_QUERY 0x21
#define N_AUTO_RESET_QUERY 0x2E
#define N_CURRENT_POSITION_QUERY 0x3E
#define N_CURRENT_VERSION_QUERY 0x3F
#define N_MOTOR_STATE_QUERY 0x4A

#define N_RESET_CHANNEL_QUIRE 0x5A

//执行指令
#define N_RUN_AUTO_ROAD 0x01
#define N_RESET 0x05
#define N_ORIGIN_RESET 0x4F
#define N_RUN_SET_ROAT_HOLE 0xA4
#define N_RUN_SET_ROAT_HOLE_BETWEEN 0xB4
#define N_FORCE_STOP 0x49
#define quire_state 0x90
#define QUERY_MAX_VLOCITY 0x91

#define SET_DEFAGLT_RESET_CHANNEL 0x59

#define RUN_TIME_THRESHOLD 5000
#define QUERY_OR_SET_TIME_THRESHOLD 1000

typedef enum {
    NORMAL = 0,
    FRAME_ERR = 1,
    PARAMETER_ERR,
    ZERO_ERR,
    MOTOR_BUSY,
    MOTOR_STALL,
    UNKNOWN_POSITION,
    TASK_PEND = 0xFE,
    UNKNOWN_ERR = 0xFF
} Runze_state_e;

typedef enum { TIME_OUT = 0xE0 } Runze_custom_err_e;

const u8 FACTORY_PASSWORD[4] = {0xFF, 0xEE, 0xBB, 0xAA};

static u8 is_rev_frame_check_ok(Runze_rev_frame_t *self);
static u8 rev_frame_state_read(Runze_rev_frame_t *self);
static u32 rev_frame_data_read(Runze_rev_frame_t *self);
static u8 rev_frame_data_write(Runze_rev_frame_t *self, u8 data);
static void rev_frame_clear(Runze_rev_frame_t *self);
static Runze_qurey_t current_version_query(Runze_t *);
static Runze_qurey_t current_position_query(Runze_t *self);
static Runze_qurey_t motor_state_query(Runze_t *self);
static u8 run_channel(Runze_t *self, u8 channel, Runze_rotate_dir_e dir);
static u8 run_reset(Runze_t *self);
static u8 run_auto_channel(Runze_t *self, u8 channel);
static u8 run_auto_channel_between(Runze_t *self);
static u8 run_channel_between(Runze_t *self, u8 channel, Runze_rotate_dir_e dir);
static void byte_rev(Runze_t *self, u8 data);


/**
 * @brief 从堆中创建对象
 *
 * @param max_channel 旋转阀通道数，有硬件选型决定
 * @param bytes_send 数据传输的底层接口
 * @return Runze_t* 创建的对象
 */
Runze_t *runze_new(u8 max_channel, void (*bytes_send)(u8 *data, u8 len)) {
    Runze_t *self = (Runze_t *)malloc(sizeof(Runze_t));
    self->max_channel = max_channel;
    self->address = 0;
    self->current_hole = 1;
    memset(&self->rev_frame, 0, sizeof(Runze_rev_frame_t));
    self->run_state = 0;
    self->run_data = 0;
    self->switch_state = 0;
    bus_os_create_sem(&self->sem, 0);
    self->mfunc_bytes_send = bytes_send;
    self->current_version_query = current_version_query;
    self->current_position_query = current_position_query;
    self->motor_state_query = motor_state_query;
    self->run_channel = run_channel;
    self->run_reset = run_reset;
    self->run_auto_channel = run_auto_channel;
    self->run_auto_channel_between = run_auto_channel_between;
    self->run_channel_between = run_channel_between;
    self->byte_rev = byte_rev;
    return self;
}

/**
 * @brief 对象释放
 *
 * @param self
 */
void runze_free(Runze_t *self) { free(self); }

/**
 * @brief 计算校验和
 *
 * @param data 数据
 * @param len 数据长度
 * @return u16 校验和
 */
static u8 calc_checksum(u8 *data, u8 len) {
    u16 sum = 0;
    for (u8 i = 0; i < len; i++) {
        sum += data[i];
    }
    return sum;
}
/** Runze_rev_frame_t 成员函数 **/

/**
 * @brief 从中断接收数据
 *
 * @param self
 * @param data 串口接收的数据
 */
static void byte_rev(Runze_t *self, u8 data) {
    switch (self->switch_state) {
    //寻找头
    case 0: {
        if (data == HEAD) {
            rev_frame_clear(&self->rev_frame);
            rev_frame_data_write(&self->rev_frame, data);
            self->switch_state = 1;
        }
        break;
    }
    //已找到头，数据接收中
    case 1:
        if (rev_frame_data_write(&self->rev_frame, data)) {
            if (is_rev_frame_check_ok(&self->rev_frame)) {
                bus_os_post_sem(&self->sem);
            }
            self->switch_state = 0;
        }
        break;
    default:
        break;
    }
}

/**
 * @brief 组帧并发送，这里模式已经决定了data的长度，故此处指针可以无长度
 *
 * @param is_factory true：表示工厂模式，data为4字节
 * false：表示普通模式，data为2字节
 * @param func 指令码
 * @param data 数据
 */
static void frame_make_send(Runze_t *self, u8 is_factory, u8 func, u8 *data) {
    u8 offset = 0;
    u8 buf[13] = {0};
    buf[offset++] = HEAD;
    buf[offset++] = self->address;
    buf[offset++] = func;
    if (is_factory) {
        memmove(&buf[offset], FACTORY_PASSWORD, 4);
        offset += 4;
        memmove(&buf[offset], data, 4);
        offset += 4;
    } else {
        memmove(&buf[offset], data, 4);
        offset += 4;
    }
    buf[offset] = calc_checksum(buf, offset);
    offset ++;
    self->mfunc_bytes_send(buf, offset);
    printf("send: length=%d ", offset);
    for (int i = 0; i < offset; i++) {
        printf("%x,", buf[i]);
    }
    printf("\r\n");
}

/**
 * @brief 组帧并发送
 *
 * @param is_factory true：表示工厂模式，data为4字节
 * false：表示普通模式，data为2字节
 * @param func 指令码
 * @param data 数据
 */
static u8 frame_make_send_wait(Runze_t *self, u8 is_factory, u8 func, u8 *data,
                               u16 time_out) {
    for (int i = 0; i < 3; i++) {
        frame_make_send(self, is_factory, func, data);
        if (bus_os_pend_sem(&self->sem, time_out) == 0) {
            self->run_state = rev_frame_state_read(&self->rev_frame);
            self->run_data = rev_frame_data_read(&self->rev_frame);
            //接收完成
            return self->run_data;
			
        }
    }
    return TIME_OUT;
}

/**
 * @brief 工厂指令发送
 * 
 * @param self 
 * @param func 指令码
 * @param data 数据
 * @return u8 状态
 */
static u8 factory_cmd_send(Runze_t *self, u8 func, u8 *data) {
    return frame_make_send_wait(self, 1, func, data,
                                QUERY_OR_SET_TIME_THRESHOLD);
}

/**
 * @brief 查询指令发送 
 * 
 * @param self 
 * @param func 指令码
 * @return Runze_qurey_t 返回的查询数据
 */
static Runze_qurey_t quary_cmd_send(Runze_t *self, u8 func) {
    u8 data[2] = {0, 0};
    Runze_qurey_t query;
    query.state =
        frame_make_send_wait(self, 0, func, data, QUERY_OR_SET_TIME_THRESHOLD);
    query.data = query.state == 0 ? query.data = self->run_data : 0;
    return query;
}

/**
 * @brief 执行指令发送 
 * 
 * @param self 
 * @param func 指令码
 * @param data 数据
 * @return u8 状态
 */
static u8 run_cmd_send(Runze_t *self, u8 func, u8 *data) {
    return frame_make_send_wait(self, 0, func, data, RUN_TIME_THRESHOLD);
}

/**
 * @brief 地址设置 新设备地址默认为0
 * 
 * @param self 
 * @param address 新地址
 * @return u8 状态
 */
static u8 address_set(Runze_t *self, u8 address) {
    u8 data[4] = {address, 0, 0, 0};
    u8 state = factory_cmd_send(self, F_ADDRESS_SET_CODE, data);
    if (state == 0) {
        self->address = address;
    }
    return state;
}

/**
 * @brief 波特率设置 新设备默认波特率9600
 * 
 * @param self 
 * @param boud 波特率
 * @return u8 状态
 */
static u8 baudrate_set(Runze_t *self, Runze_boudrate_e boud) {
    u8 data[4] = {boud, 0, 0, 0};
    return factory_cmd_send(self, F_BAUDRATE_SET_CODE, data);
}

/**
 * @brief 开机自动复位设置
 * 
 * @param self 
 * @param auto_reset 开机是否自动复位
 * @return u8 状态
 */
static u8 auto_reset_set(Runze_t *self, u8 auto_reset) {
    u8 data[4] = {auto_reset & 0x01, 0, 0, 0};
    return factory_cmd_send(self, F_AUTO_RESET_CODE, data);
}

/**
 * @brief 当前位置查询，正对孔返回 0x00xx,xx表示孔，两孔之间返回0xxx_xx,xx表示孔
 * 
 * @param self 
 * @return Runze_qurey_t 
 */
static Runze_qurey_t current_position_query(Runze_t *self) {
    return quary_cmd_send(self, N_CURRENT_POSITION_QUERY);
}

/**
 * @brief 版本查询 返回 0x xx_xx表示版本为xx.xx
 * 
 * @param self 
 * @return Runze_qurey_t 
 */
static Runze_qurey_t current_version_query(Runze_t *self) {
    return quary_cmd_send(self, N_CURRENT_VERSION_QUERY);
}

/**
* @brief 设置复位通道
 * 
 * @param self 
 * @return u8 state 
 */
u8 reset_channel_set(Runze_t *self) {
	
    u8 data[4] = {0, 0, 0, 10};
    u8 state = run_cmd_send(self, SET_DEFAGLT_RESET_CHANNEL, data);
	return state;
}

/**
* @brief 查询复位通道
 * 
 * @param self 
 * @return u8 state 
 */
u8 reset_channel_quire(Runze_t *self) {
	
    u8 data[4] = {0, 0, 0, 0};
    u8 state = run_cmd_send(self, N_RESET_CHANNEL_QUIRE, data);
	return state;
}

/**
 * @brief 电机状态查询 见Runze_state_e 
 * 
 * @param self 
 * @return Runze_qurey_t 
 */
static Runze_qurey_t motor_state_query(Runze_t *self) {
    return quary_cmd_send(self, N_MOTOR_STATE_QUERY);
}

/**
 * @brief 按照最优路径运行到指定通道
 * 
 * @param self 
 * @param channel 通道
 * @return u8 状态
 */
static u8 run_auto_channel(Runze_t *self, u8 channel) {
    ASSERT(channel > self->max_channel || channel <= 0, "channel error");
    u8 data[4] = {0, 0, 0, channel};
    u8 state = run_cmd_send(self, N_RUN_AUTO_ROAD, data);
    if (state == 0) {
        self->current_hole = channel;
    }
    return state;
}

/**
 * @brief 按照指定方向 运动到指定通道
 * 
 * @param self 
 * @param channel 通道
 * @param dir 方向
 * @return u8 
 */
static u8 run_channel(Runze_t *self, u8 channel, Runze_rotate_dir_e dir) {
    ASSERT(channel > self->max_channel || channel <= 0, "channel error");
    u8 first_channel;
    if (dir) {
        first_channel = channel == 1 ? self->max_channel : channel - 1;
    } else {
        first_channel = channel == self->max_channel ? 1 : channel + 1;
    }
    u8 data[2] = {channel, first_channel};
    u8 state = run_cmd_send(self, N_RUN_SET_ROAT_HOLE, data);
    if (state == 0) {
        self->current_hole = channel;
    }
    return state;
}

/**
 * @brief 按照指定方向运动到指定通道之间，即channel - 1 与 channel之间
 * 
 * @param self 
 * @param channel 
 * @param dir 
 * @return u8 
 */
static u8 run_channel_between(Runze_t *self, u8 channel, Runze_rotate_dir_e dir) {
    ASSERT(channel > self->max_channel || channel <= 0, "channel error");
    u8 first_channel = channel == 1 ? self->max_channel : channel - 1;
    u8 data[2];
    if (dir) {
        data[0] = channel;
        data[1] = first_channel;
    } else {
        data[0] = first_channel;
        data[1] = channel;
    }
    u8 state = run_cmd_send(self, N_RUN_SET_ROAT_HOLE_BETWEEN, data);
    if (state == 0) {
        self->current_hole = channel;
    }
    return state;
}

/**
 * @brief 根据当前位置移动到 当前通道-1 与 当前通道之间
 * 
 * @param self 
 * @return u8 
 */
static u8 run_auto_channel_between(Runze_t *self) {
    return run_channel_between(self, self->current_hole, RUNZE_ROTATE_CLOCK_WISE);
}

/**
 * @brief 复位 逆时针运动到光耦位置
 * 
 * @param self 
 * @return u8 
 */
static u8 run_reset(Runze_t *self) {
    u8 data[4] = {0, 0, 0, 0};
    u8 state = run_cmd_send(self, N_RESET, data);
    if(state == 0){
        self->current_hole = 1;
    }
    return state;
}

/** Runze_rev_frame_t 成员函数 **/
/**
 * @brief 判断接收的数据校验和是否正确
 *
 * @param self
 * @return u8 0 失败 1 成功
 */
static u8 is_rev_frame_check_ok(Runze_rev_frame_t *self) {
    printf("rev:");
    for (int i = 0; i < 7; i++) {
        printf("%x,", self->buffer[i]);
    }
    printf("\r\n");
    u8 check_sum = ReadU16(&self->buffer[6]);
    u16 calc_sum = calc_checksum(self->buffer, 6);
    //    printf("c=%x, l = %x\r\n", check_sum, calc_sum);
    return check_sum == calc_sum;
}

/**
 * @brief 从接收的数据中获取状态
 *
 * @param self
 * @return u8 状态 见Runze_state_e
 */
static u8 rev_frame_state_read(Runze_rev_frame_t *self) {
    return self->buffer[2];
}

/**
 * @brief 从接收的数据中获取状态
 *
 * @param self
 * @return u8 状态 见Runze_state_e
 */
static u32 rev_frame_data_read(Runze_rev_frame_t *self) {
    u32 data = self->buffer[5] << 24;
    data |= self->buffer[4] << 16;
	data |= self->buffer[3] << 8;
	data |= self->buffer[2];
    return data;
}

/**
 * @brief 向接收缓冲区写数据
 *
 * @param self
 * @param data 数据
 * @return u8 0 未接收完成 1 接收完成
 */
static u8 rev_frame_data_write(Runze_rev_frame_t *self, u8 data) {
    ASSERT(self->write_idx >= 8, "runze_rev_frame_t full");
    self->buffer[self->write_idx++] = data;
    return self->write_idx == 7;
}

/**
 * @brief 清空接收缓存区
 *
 * @param self
 */
static void rev_frame_clear(Runze_rev_frame_t *self) { self->write_idx = 0; }
