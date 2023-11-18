#ifndef __AD_SIGNAL_H__
#define __AD_SIGNAL_H__
#include "macro.h"
#include "bus.h"

typedef struct _Signal_read_t {
    u16 (*hd_signal_read)(u8 idx);
	u32 (*hd_singal_read_32)(u8 idx);
	/**
	 * @brief 低16位为数据0 高16位位数据1 
	 * 
	 */
	u16 (*hd_signal_read_ad)(u8 idx);
	u32 (*hd_signal_read_ad_to_press)(u8 idx);
	u32 (*hd_signal_read_ad_to_press_differ)(u8 idx);
	u32 (*hd_signal_read_two)(u8 idx);
	u16* (*hd_signal_read_array)(u8 idx, u16* rt_len);
	u16* (*hd_group_read_by)(u16 group, u8* rt_len);
	u16* (*hd_group_read_all)(void);
	u16* (*hd_dma_signal_read)(u8 idx);
	u16 (*hd_average_signal_read)(u8 idx,u16 average_cnt);
	u16 (*hd_average_signal_read_old)(u8 idx,u16 average_cnt);
	 u16 (*hd_signal_read_average)(u8 idx, u16 average_cnt);
} Signal_read_t;

void ad_signal_init(void);
Signal_read_t* signal_reader_as_ref(void);
void signal_cmd_run(u32 func_code, bus_packet* packet);

#endif
