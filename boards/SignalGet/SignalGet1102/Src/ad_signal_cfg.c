#include "ad_signal_cfg.h"
#include "delayus.h"
#include "cpu.h"
#include "AD7699.h"

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;

static void nss_write0(u8 value);
static void nss_write1(u8 value);
static void nss_write2(u8 value);
static u16 hd_signal_read(u8 idx);
static u32 hd_signal_read_two(u8 idx);
static u16* hd_signal_read_array(u8 idx, u16* rt_len);
static u16* hd_group_read_by(u16 group, u8* rt_len);
static u16* hd_group_read_all(void);

#define AD7699_CHANNEL_USED 4
#define AD7699_ALL_CHANNEL_USED 12

static AD7699CONFIG ad7699[3];
static u8 ad_channel_map[][AD7699_CHANNEL_USED] = {
	{1, 3, 5, 7},
	{1, 3, 5, 7},
	{1, 3, 5, 7}
};
static u16 data_buffer[AD7699_ALL_CHANNEL_USED];
Signal_read_t ad_signal;

void ad_signal_hd_init(void)
{
	//AD7699³õÊ¼»¯
	AD7699_Init(&ad7699[0], &hspi1, AD7699_CHANNEL_USED, nss_write0, ad_channel_map[0]);
	AD7699_Init(&ad7699[1], &hspi2, AD7699_CHANNEL_USED, nss_write1, ad_channel_map[1]);
	AD7699_Init(&ad7699[2], &hspi3, AD7699_CHANNEL_USED, nss_write2, ad_channel_map[2]);
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

static void nss_write1(u8 value)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,(GPIO_PinState)value);
}

static void nss_write2(u8 value)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,(GPIO_PinState)value);
}

static u16 hd_signal_read(u8 idx){
	ASSERT(idx >= AD7699_ALL_CHANNEL_USED, "idx over");
	AD7699CONFIG* ad = NULL;
	u8* map = NULL;
	if(idx >= (AD7699_CHANNEL_USED << 1))
	{
		ad = &ad7699[2];
		map = ad_channel_map[2];
		idx = idx - (AD7699_CHANNEL_USED << 1);
	}else if(idx >= AD7699_CHANNEL_USED)
	{
		ad = &ad7699[1];
		map = ad_channel_map[1];
		idx = idx - AD7699_CHANNEL_USED;
	}else{
		map = ad_channel_map[0];
		ad = &ad7699[0];
	}
	return AD7699_GetSingleData(ad, map[idx]);
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
	AD7699AllRead(&ad7699[1], &data[AD7699_CHANNEL_USED]);
	AD7699AllRead(&ad7699[2], &data[AD7699_CHANNEL_USED<<1]);
	return data;
}


