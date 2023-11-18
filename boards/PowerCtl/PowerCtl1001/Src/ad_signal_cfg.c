#include "ad_signal_cfg.h"
#include "delayus.h"
#include "cpu.h"
#include "AD7699.h"

extern SPI_HandleTypeDef hspi1;


static void nss_write0(u8 value);

static u16 hd_signal_read(u8 idx);
static u32 hd_signal_read_two(u8 idx);
static u16* hd_signal_read_array(u8 idx, u16* rt_len);
static u16* hd_group_read_by(u16 group, u8* rt_len);
static u16* hd_group_read_all(void);

#define AD7699_CHANNEL_USED 6
#define AD7699_ALL_CHANNEL_USED 6

 AD7699CONFIG ad7699[1];
static u8 ad_channel_map[6] = {
		5,//adc1 对应通道5
		6,//adc2 对应通道6
		7,//adc3 对应通道7
		0,//adc4 对应通道0
		1,//adc5 对应通道1
		2 //adc6 对应通道2
};
static u16 data_buffer[AD7699_ALL_CHANNEL_USED];
Signal_read_t ad_signal;



void ad_signal_hd_init(void)
{
	//AD7699初始化
	AD7699_Init(&ad7699[0], &hspi1, AD7699_CHANNEL_USED, nss_write0, ad_channel_map);
	ad_signal.hd_signal_read = hd_signal_read;
	ad_signal.hd_signal_read_two = hd_signal_read_two;
	ad_signal.hd_signal_read_array = hd_signal_read_array;
	ad_signal.hd_group_read_by = hd_group_read_by;
	ad_signal.hd_group_read_all = hd_group_read_all;
}


static void nss_write0(u8 value)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,(GPIO_PinState)value);
}


static u16 hd_signal_read(u8 idx){
	ASSERT(idx >= AD7699_ALL_CHANNEL_USED, "idx over");
	u32 data =0;
	
	data=AD7699_GetData(&ad7699[0], ad_channel_map[idx],16);

	printf("idx=%d data=%d\r\n",idx,data);
	return data;

}

static u16* hd_signal_read_array(u8 idx, u16* rt_len)
{
	ASSERT(1, "hd_signal_read_array not support");
}

static u32 hd_signal_read_two(u8 idx)
{
	ASSERT(1, "hd_signal_read_two not support");
}

static u16* hd_group_read_by(u16 group, u8* rt_len)
{
	ASSERT(1, "hd_group_read_by not support");
}

static u16* hd_group_read_all(void)
{
	u16* data = data_buffer;
	AD7699AllRead(&ad7699[0], data);
	return data;
}



