#include "io_mod.h"
#include "bus.h"
#include "stdio.h"
#include <stdlib.h>
#define IO_MAX 16

#define BIT_READ(state, offset) ((state >> offset) & 0x01)
#define VALUE_SET(value, v, offset) value |= (v << (offset << 1))

static void io_write(Io_writable_t *self, const u8 id, Io_state_e state);
static void group_write(Io_writable_t *self, u32 state);
static void timing_io_write(Io_writable_t *self, void *timing, u8 len);
static Io_state_e io_read(Io_readable_t *self, u8 id);
static u16 group_read(Io_readable_t *self);
static u32 change_value(Io_readable_t *self, u16 state);
static u32 listen(Io_readable_t *self);
static Io_state_e cfg_covert_io_state(Io_cfg_t* self, Io_state_e state);

/** Io_writable_t成员函数 **/
/**
 * @brief 可写io组初始化
 *
 * @param hd_write 底层控制函数
 * @param self
 * @param io_array 可写io组
 * @param len 长度
 */
Io_writable_t *writable_io_new(void (*hd_write)(Io_cfg_t *, Io_state_e), Io_cfg_t *io_array, u8 len)
{
	
    Io_writable_t *self = (Io_writable_t *)malloc(sizeof(Io_writable_t));
    self->mfunc_hd_write = hd_write;
    self->io_array = io_array;
    self->io_array_len = len;
    for(int i = 0; i < len; i++){
        self->io_array[i].covert_io_state = cfg_covert_io_state;
    }
    self->group_write = group_write;
    self->io_write = io_write;
    self->timing_io_write = timing_io_write;
    return self;
}

/**
 * @brief 写io组
 *
 * @param self
 * @param state 每两个bit代表一个io 0：无操作 1：写失能 2：写使能
 */
static void group_write(Io_writable_t *self, u32 state) {
    int val;
    for (int i = 0; i < self->io_array_len; i++) {
        val = state & 0x03;
        if (val == 1) {
            io_write(self, i, IO_DISABLE);
        } else if (val == 2) {
            io_write(self, i, IO_ENABLE);
        }
        state = state >> 2;
    }
}

/**
 * @brief 根据id 写io
 *
 * @param self
 * @param id io对应的编号
 * @param value 写入的状态
 */
static void io_write(Io_writable_t *self, const u8 id, Io_state_e state) {
    ASSERT(id >= self->io_array_len, "io parameter error");
	
//	printf("writeIo %d\r\n",id);
//	printf("state 0x%x\r\n",state);
//	
//	printf("but,cfg= %d\r\n",self->io_array[id].valid_level);
	
	state = cfg_covert_io_state(&self->io_array[id], state);
	
//	printf("so,state 0x%x\r\n",state);
	
    

    self->mfunc_hd_write(&self->io_array[id], state);
}

/**
 * @brief 根据时序写控制io
 *
 * @param self
 * @param value 时序数据
 * @param len 时序数据长度
 */
static void timing_io_write(Io_writable_t *self, void *value, u8 len) {
    Io_timing_t *timing = (Io_timing_t *)value;
    for (int i = 0; i < len; i++) {
        group_write(self, timing[i].io_value);
        bus_os_sleep(timing[i].time_delay);
    }
}

/** Io_readable_t 成员函数 **/

/**
 * @brief 可读io初始化
 *
 * @param self
 * @param io_array io配置
 * @param len 长度
 */
Io_readable_t* readable_io_new(u8 (*hd_read)(Io_cfg_t*), u16 change_enable_cfg, Io_cfg_t *io_array, u8 len) {
    Io_readable_t* self = (Io_readable_t* )malloc(sizeof(Io_readable_t));
    self->change_enable_cfg = change_enable_cfg;
    self->mfunc_hd_read = hd_read;
    self->io_array = io_array;
    self->io_array_len = len;
    self->io_read = io_read;
    self->group_read = group_read;
    self->listen = listen;
    return self;
}

/**
 * @brief io组读
 *
 * @param self
 * @return u32 每bit代表一个io 0 失能 1：使能
 */
static u16 group_read(Io_readable_t *self) {
    u32 value = 0;
    for (int i = 0; i < self->io_array_len; i++) {
        Io_state_e io_state = io_read(self, i);
        value |= (io_state << i);
    }
    return value;
}

/**
 * @brief io读
 *
 * @param self
 * @param id io对应的id
 * @return u8 io对应的状态
 */
static Io_state_e io_read(Io_readable_t *self, u8 id) {
    ASSERT(id >= self->io_array_len, "io_read parameter error");
    Io_state_e io_state = (Io_state_e)self->mfunc_hd_read(&self->io_array[id]);
    return cfg_covert_io_state(&self->io_array[id], io_state);
}

/**
 * @brief 监听io是否翻转 两bit为一组如 [0-1] 0 不变 1 变为失能 2 变为使能
 * 
 * @param self 
 * @return u32 
 */
static u32 listen(Io_readable_t *self) {
    u32 state = self->group_read(self);
    u32 changed = change_value(self, state);
//    printf("changed = %x\r\n", changed);
    self->current_io_state = state;
    return changed;
}

static u32 change_value(Io_readable_t *self, u16 state) {
    if (state == self->current_io_state) {
        return 0;
    }
    u32 value = 0;
    u32 change = state ^ self->current_io_state;
    for (int i = 0; i < 16; i++) {
        if (!BIT_READ(self->change_enable_cfg, i)) {
            continue;
        }
        u8 b = 0;
        if (BIT_READ(change, i)) {
            b = BIT_READ(state, i) == 0 ? 1 : 2;
        } else {
            b = 0;
        }
        VALUE_SET(value, b, i);
    }
    return value;
}

static Io_state_e cfg_covert_io_state(Io_cfg_t* self, Io_state_e state){
    return state == self->valid_level ? IO_ENABLE : IO_DISABLE;
}
