#ifndef _ad_signal_cfg_
#define _ad_signal_cfg_


#include "ad_signal.h"

#include "AD7699.h"

#define AD7899_NUM 1
#define SIGNAL_CHANNEL_CNT 10
extern void ad_signal_hd_init(void);
extern void ad_singnal_test(void);
extern AD7699CONFIG ad7966_cfg[AD7899_NUM];

extern AD7699CONFIG ad7699[AD7899_NUM];
	
extern u8 ad_channel_map_0[4];
#endif
