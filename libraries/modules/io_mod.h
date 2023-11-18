#ifndef __IO_MOD_H__
#define __IO_MOD_H__
#include "macro.h"

typedef enum{
    // IO控制方式
    IO_TYPE_IO = 0,
    // PWM控制方式
    IO_TYPE_PWM,
	// PWM控制方式互补通道
	IO_TYPE_PWM_N,
	// PCA_9685PWM
	IO_TYPE_PCA_PWM,
	// 软件PWM
	IO_TYPE_PWN_SOFT,
}Io_Type_e;

typedef enum {
    /**
     * @brief 表示io失能状态
     */
    IO_DISABLE = 0,
    /**
     * @brief 表示io使能状态
     */
    IO_ENABLE
}Io_state_e;

typedef enum {
    /**
     * @brief 表示io低电平有效
     */
    IO_LOW_VALID = 0,
    /**
     * @brief 表示io高电平有效
     */
    IO_HIGH_VALID = 1,
}Io_valid_level_e;

/**
 * @brief 单个io控制配置
 */
typedef struct _Io_cfg_t{
    Io_Type_e io_type;
    void* port;
    u16 pin;
    Io_valid_level_e valid_level;
    void *pwm_tim;
    u8 pwm_channel;
    u16 pwm_dutycycle;//占空比
    u16 pwm_max_value;//周期
	u8 polarity;//极性
    Io_state_e (*covert_io_state)(struct _Io_cfg_t*, Io_state_e state);
}Io_cfg_t;

/**
 * @brief io控制时序
 */
typedef struct{
    //写io组的值
    u32 io_value;
    //写完后的延时时间
    u16 time_delay;
}Io_timing_t;

/**
 * @brief state [0:1]每两个为一组 0 无效 1 写低 2 写高
 */
typedef struct _Io_writable_t{
    Io_cfg_t* io_array;
    u8 io_array_len;
    /**
     * @brief 调用底层控制io或pwm操作接口
     */
    void (*mfunc_hd_write)(Io_cfg_t*, Io_state_e state);
    /**
     * @brief io整组写
     */
    void (*group_write)(struct _Io_writable_t*, u32);
    /**
     * @brief io单通道写
     */
    void (*io_write)(struct _Io_writable_t*, u8, Io_state_e);
    /**
     * @brief io时序控制
     */
    void (*timing_io_write)(struct _Io_writable_t*, void* ,u8);
}Io_writable_t;

/**
 * @brief io读取
 */
typedef struct _Io_readable_t {
    // 配置哪一组启用io监听, 对应位 1 启用 0 不启用
    u16 change_enable_cfg;
    // 保存io组读取的状态,用于判定是否发生翻转
    u16 current_io_state;
    Io_cfg_t* io_array;
    u8 io_array_len;
    u8 (*mfunc_hd_read)(Io_cfg_t*);
    u16 (*group_read)(struct _Io_readable_t*);
    Io_state_e (*io_read)(struct _Io_readable_t*, u8 id);
    /**
     * @brief 监听io是否翻转 两bit为一组如 [0-1] 0 不变 1 变为失能 2 变为使能
     */
    u32 (*listen)(struct _Io_readable_t*);
}Io_readable_t;

Io_readable_t* readable_io_new(u8 (*hd_read)(Io_cfg_t*), u16 change_enable_cfg, Io_cfg_t *io_array, u8 len);
Io_writable_t* writable_io_new(void (*hd_write)(Io_cfg_t*, Io_state_e), Io_cfg_t* io_array, u8 len);

#endif
