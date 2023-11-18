#include "ad_signal_cfg.h"
#include "delayus.h"
#include "cpu.h"
#include "AD7699.h"

extern SPI_HandleTypeDef hspi3;


static void nss_write0(u8 value);


static u16 hd_signal_read(u8 idx);
static u32 hd_signal_read_two(u8 idx);
static u16* hd_signal_read_array(u8 idx, u16* rt_len);
static u16* hd_group_read_by(u16 group, u8* rt_len);
static u16* hd_group_read_all(void);

AD7699CONFIG ad7699[AD7899_NUM];
	
u8 ad_channel_map_0[4] =
{
	7,6,5,4
};

//片选使能
void AD7699_SetNSS_0(int v)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,(GPIO_PinState)v);
	
}


static u16 data_buffer[4];

Signal_read_t ad_signal;

void ad_signal_hd_init(void)
{
	//AD7699初始化
	AD7699_Init(&ad7699[0], &hspi3, 4, nss_write0, ad_channel_map_0);

	ad_signal.hd_signal_read = hd_signal_read;
	ad_signal.hd_signal_read_two = hd_signal_read_two;
	ad_signal.hd_signal_read_array = hd_signal_read_array;
	ad_signal.hd_group_read_by = hd_group_read_by;
	ad_signal.hd_group_read_all = hd_group_read_all;
}
static void nss_write0(u8 value)
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,(GPIO_PinState)value);
}



static u16 hd_signal_read(u8 idx)
{
	ASSERT(1, "hd_signal_read not support");
}

static u16* hd_signal_read_array(u8 idx, u16* rt_len)
{
	ASSERT(1, "hd_signal_read_array not support");
}

static u32 hd_signal_read_two(u8 idx)
{
	u32 data = 0;
	ASSERT(idx>=2, "idx Crossing");
	if(idx == 0)
	{
		data |= AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[1]);
		data <<= 16;
		data |= AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[0]);
	}else
	{
		data |= AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[3]);
		data <<= 16;
		data |= AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[2]);
	}
	return data;
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

void ad_singnal_test(void)
{
	hd_group_read_all();
	//printf("00	01	02	03\r\n");

	printf("%d,",data_buffer[0]);
	printf("%d,",data_buffer[1]);
	printf("%d,",data_buffer[2]);
	printf("%d",data_buffer[3]);
	printf("\n");
}