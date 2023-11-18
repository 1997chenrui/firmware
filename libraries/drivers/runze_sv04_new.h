#ifndef __RUNZE_SV04_NEW_H__
#define __RUNZE_SV04_NEW_H__
#include "macro.h"
#include "bus.h"

typedef enum{
    RUNZE_B_9600 = 0,
    RUNZE_B_19200,
    RUNZE_B_38400,
    RUNZE_B_57600,
    RUNZE_B_115200,
}Runze_boudrate_e;

typedef enum {
    /**
     * @brief 顺时针
     */
    RUNZE_ROTATE_CLOCK_WISE = 0,
    /**
     * @brief 逆时针
     */
    RUNZE_ROTATE_ANTICLOCKWISE = 1,
}Runze_rotate_dir_e;

/**
 * 需要提供 
 * 校验接口 
 * 状态解析接口
 */
typedef struct{
    u8 buffer[8];
    u8 write_idx;
}Runze_rev_frame_t;

typedef struct{
    u8 state;
    u16 data;
}Runze_qurey_t;

typedef struct _Runze_t{
    u8 max_channel;
    u8 address;
    u8 current_hole;
    Runze_rev_frame_t rev_frame;
    //从接收帧解析出的状态
    u8 run_state;
    u16 run_data;
    //状态机状态
    u8 switch_state;
    bus_os_sem sem;
    void (*mfunc_bytes_send)(u8* data, u8 len);
    Runze_qurey_t (*current_version_query)(struct _Runze_t*);
    Runze_qurey_t (*current_position_query)(struct _Runze_t *self);
    Runze_qurey_t (*motor_state_query)(struct _Runze_t *self);
    u8 (*run_channel)(struct _Runze_t *self, u8 channel, Runze_rotate_dir_e dir);
    u8 (*run_reset)(struct _Runze_t *self);
    u8 (*run_auto_channel)(struct _Runze_t *self, u8 channel);
    u8 (*run_auto_channel_between)(struct _Runze_t *self);
    u8 (*run_channel_between)(struct _Runze_t *self, u8 channel, Runze_rotate_dir_e dir);
    void (*byte_rev)(struct _Runze_t *, u8);
}Runze_t;


Runze_t* runze_new(u8 max_channel, void (*bytes_send)(u8* data, u8 len));
void runze_free(Runze_t* self);
u8 reset_channel_set(Runze_t *self);
u8 reset_channel_quire(Runze_t *self);

#endif
