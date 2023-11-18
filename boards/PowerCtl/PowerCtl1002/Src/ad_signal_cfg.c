#include "ad_signal_cfg.h"
#include "delayus.h"
#include "cpu.h"
#include "MS5194T.h"

extern SPI_HandleTypeDef hspi1;


static void nss_write0(u8 value);

static u32 hd_singal_read_32(u8 idx);
static u16 hd_signal_read(u8 idx);
static u32 hd_signal_read_two(u8 idx);
static u16* hd_signal_read_array(u8 idx, u16* rt_len);
static u16* hd_group_read_by(u16 group, u8* rt_len);

#define MS5194T_CHANNEL_USED 6

 MS5194TCONFIG MS5194T[1];

u8 MS5194T_map[MS5194T_CHANNEL_USED] = 
{
	0, 1, 2, 3, 4, 5
};


Signal_read_t ad_signal;



void ad_signal_hd_init(void)
{
	//AD7699初始化
	MS5194T_Init(&MS5194T[0], &hspi1, MS5194T_CHANNEL_USED, nss_write0);
	ad_signal.hd_signal_read = hd_signal_read;
	ad_signal.hd_singal_read_32 = hd_singal_read_32 ;
	ad_signal.hd_signal_read_two = hd_signal_read_two;
	ad_signal.hd_signal_read_array = hd_signal_read_array;
	ad_signal.hd_group_read_by = hd_group_read_by;
}


static void nss_write0(u8 value)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,(GPIO_PinState)value);
}


static u16 hd_signal_read(u8 idx){
	ASSERT(idx >= MS5194T_CHANNEL_USED, "idx over");
	u32 data =0;
	
	data = MS5194T_GetData(&MS5194T[0], MS5194T_map[idx]);
//	printf("idx=%d data=%d\r\n",idx,data);
	return data;

}

u32 hd_singal_read_32(u8 idx)
{
	ASSERT(idx >= MS5194T_CHANNEL_USED, "idx over");
	u32 data =0;
	
	data = MS5194T_GetData(&MS5194T[0], MS5194T_map[idx]);
//	printf("idx=%d data=%d\r\n",idx,data);
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




