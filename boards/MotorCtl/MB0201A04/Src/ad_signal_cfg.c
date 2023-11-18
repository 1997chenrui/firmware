#include "ad_signal_cfg.h"
#include "stm32f4xx_hal.h"

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

static u16 buffer[SIGNAL_CHANNEL_CNT];
static u16 hd_signal_read(u8 idx);
static u32 hd_signal_read_two(u8 idx);
static u16* hd_signal_read_array(u8 idx, u16* rt_len);
static u16* hd_group_read_by(u16 group, u8* rt_len);
static u16* hd_group_read_all(void);

Signal_read_t ad_signal;

void ad_signal_hd_init(){
	ad_signal.hd_signal_read = hd_signal_read;
	ad_signal.hd_signal_read_two = hd_signal_read_two;
	ad_signal.hd_signal_read_array = hd_signal_read_array;
	ad_signal.hd_group_read_by = hd_group_read_by;
	ad_signal.hd_group_read_all = hd_group_read_all;
	HAL_ADC_Start_DMA(&hadc1, (u32*)(&buffer[0]), SIGNAL_CHANNEL_CNT);//将传感器接线映射到stm32RAM上（直接读取质量传感器数据）
	
}

static u16 hd_signal_read(u8 idx){
	ASSERT(idx > SIGNAL_CHANNEL_CNT-1, "hd_signal_read idx > 5");
	return buffer[idx];
}

static u32 hd_signal_read_two(u8 idx){
	ASSERT(1, "hd_signal_read_two not support");
}

static u16* hd_signal_read_array(u8 idx, u16* rt_len)
{
	ASSERT(1, "hd_signal_read_array not support");
}

static u16* hd_group_read_by(u16 group, u8* rt_len)
{
	ASSERT(1, "hd_group_read_by not support");
}

static u16* hd_group_read_all(void)
{
	ASSERT(1, "hd_group_read_all not support");
}
