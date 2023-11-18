#include "ad_signal_cfg.h"
#include "delayus.h"
#include "cpu.h"
#include "AD7699.h"
#include "stm32f4xx_hal_spi.h"
extern SPI_HandleTypeDef hspi2;


static void nss_write0(u8 value);


static u16 hd_signal_read(u8 idx);
static u32 hd_signal_read_two(u8 idx);
static u16* hd_signal_read_array(u8 idx, u16* rt_len);
static u16* hd_group_read_by(u16 group, u8* rt_len);
static u16* hd_group_read_all(void);

AD7699CONFIG ad7699[AD7899_NUM];
	
static u8 ad_channel_map_0[1] =
{
	1
};


static void nss_write0(u8 value)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,(GPIO_PinState)value);
}



Signal_read_t ad_signal;

void ad_signal_hd_init(void)
{
	//AD7699初始化
	AD7699_Init(&ad7699[0], &hspi2, 1, nss_write0, ad_channel_map_0);

	ad_signal.hd_signal_read = hd_signal_read;
	ad_signal.hd_signal_read_two = hd_signal_read_two;
	ad_signal.hd_signal_read_array = hd_signal_read_array;
	ad_signal.hd_group_read_by = hd_group_read_by;
	ad_signal.hd_group_read_all = hd_group_read_all;
	
	//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
}


static u16 hd_signal_read(u8 idx)
{
	u16 data;
	
	data = AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[idx]);

	return data;
}

static u16* hd_signal_read_array(u8 idx, u16* rt_len)
{
	ASSERT(1, "hd_signal_read_array not support");
}

static u32 hd_signal_read_two(u8 idx)
{
	ASSERT(1, "hd_signal_read not support");
}

static u16* hd_group_read_by(u16 group, u8* rt_len)
{
	ASSERT(1, "hd_group_read_by not support");
}

static u16* hd_group_read_all(void)
{
	ASSERT(1, "hd_group_read_by not support");
}

void ad_singnal_test(void)
{
	
	printf("%d,",AD7699_GetSingleData(&ad7699[0], 0));
	printf("%d,",AD7699_GetSingleData(&ad7699[0], 1));
	printf("%d,",AD7699_GetSingleData(&ad7699[0], 2));
	printf("%d,",AD7699_GetSingleData(&ad7699[0], 3));
	printf("%d,",AD7699_GetSingleData(&ad7699[0], 4));
	printf("%d,",AD7699_GetSingleData(&ad7699[0], 5));
	printf("%d,",AD7699_GetSingleData(&ad7699[0], 6));
	printf("%d\r\n",AD7699_GetSingleData(&ad7699[0], 7));

}