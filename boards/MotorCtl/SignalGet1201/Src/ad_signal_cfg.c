#include "ad_signal_cfg.h"
#include "AD7699.h"

extern SPI_HandleTypeDef hspi3;

#define AD7699_ALL_CHANNEL_USED 4
static AD7699CONFIG ad7699;

static u8 ad_channel_map[] = {
	0, 1, 2, 3
};

static u16 buffer[32];

static void nss_write(u8 value);
static u16 hd_signal_read(u8 idx);
static u32 hd_signal_read_two(u8 idx);
static u16* hd_signal_read_array(u8 idx, u16* rt_len);
static u16* hd_group_read_by(u16 group, u8* rt_len);
static u16* hd_group_read_all(void);

Signal_read_t ad_signal;

void ad_signal_hd_init(){
	//AD7699³õÊ¼»¯
	AD7699_Init(&ad7699, &hspi3, AD7699_ALL_CHANNEL_USED, nss_write, ad_channel_map);
	ad_signal.hd_signal_read = hd_signal_read;
	ad_signal.hd_signal_read_two = hd_signal_read_two;
	ad_signal.hd_signal_read_array = hd_signal_read_array;
	ad_signal.hd_group_read_by = hd_group_read_by;
	ad_signal.hd_group_read_all = hd_group_read_all;
}

static u16 hd_signal_read(u8 idx){
	ASSERT(1, "signal read not support");
}

static u32 hd_signal_read_two(u8 idx){

	
	u32 data = 0;
	if(idx == 0)
	{
		data |= AD7699_GetData(&ad7699, ad_channel_map[1], buffer, 32);
		data <<= 16;
		data |= AD7699_GetData(&ad7699, ad_channel_map[0], buffer, 32);
	}else
	{
		data |= AD7699_GetData(&ad7699, ad_channel_map[3], buffer, 32);
		data <<= 16;
		data |= AD7699_GetData(&ad7699, ad_channel_map[2], buffer, 32);
	}
	return data;
}

static void nss_write(u8 value)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,(GPIO_PinState)value);
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
