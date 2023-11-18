#include "lampCtl.h"
#include "ucos_ii.h"
#include "procfg.h"
#include "AD7699BySG0703.h"
#include "sp_light.h"

extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim2;

Light_t Ths;

unsigned char LightGetData(Light_Ad_t * const data, unsigned int readCnt);
unsigned short Light_RemianDataCountGet(void);
void Light_Start(void);
void LightDataOpen(void);
void LightDataOpen(void);
void SingleGetData(volatile u8 *ChannelStartFlag, Light_Ad_t *buffer, u16 *windx_len, u8 ChannelId, u8 LaserId);
	
static volatile u32 mix_first_ad_time_A = 0;

static volatile u32 mix_first_ad_time_B = 0;

volatile u16 sample_time = 0;

Wave wave[8];

/* 
 * @brief 判断光值采集写下标是否过大
 * @param windx_len 写长度的指针
 *
 *
 */

static void is_windx_over(u16 *windx_len)
{
	++ *windx_len;
	if((*windx_len) >= LIGHT_BUFF_SIZE)
	{
		*windx_len = 0;
	}
}

void HAL_Delay_us(uint16_t us)
{
	uint16_t differ=0xffff-us-5;
	__HAL_TIM_SET_COUNTER(&htim6,differ);
	HAL_TIM_Base_Start(&htim6);
	while(differ<0xffff-6)
	{
		differ=__HAL_TIM_GET_COUNTER(&htim6);
	}
	HAL_TIM_Base_Stop(&htim6);
}

/*
 * @brief 定时器中断，在中断中进行光源控制
 *
 *
 */
void TIM2_IRQHandler(void)
{
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	
	LampCtl();
	
}

// 混匀中断(来自抓手A)
void EXTI4_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);

	Ths.ChannelStartFlag &= 0xF0;
	mix_first_ad_time_A = OSTimeGet();
}

// 混匀中断(来自抓手B)
void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
	sample_time = 0;
	Ths.ChannelStartFlag &= 0x0F;
	mix_first_ad_time_B = OSTimeGet();
}

/*
 * @brief 光源参数初始化
 * 
 *
 */

void Light_Init(void) {
	memset(&Ths, 0, sizeof(Ths));
	for(int i=0;i<8;i++)
	{
		wave[i].need_updata = 1;
		wave[i].func_start = 1;
	}
	Ths.rindex = 0;
	Ths.windex = 0;
	Ths.CircleCount = 0;
	Ths.ChannelStartFlag = 0xFF;
}


/*
*****************************************************
*@brief 开启定时器进行光源控制
**
**
**
*****************************************************
*/
void Light_Start()
{
	HAL_TIM_Base_Start_IT(&htim2);
}

/*
 * @brief 光源的信号采集
 *
 *
 */
void SingleGetData(volatile u8 *ChannelStartFlag, Light_Ad_t *buffer, u16 *windx_len, u8 ChannelId, u8 LaserId)
{
	u16 FirstAdTime_A, FirstAdTime_B;
	
	if(((*ChannelStartFlag)&ChannelId) == 0)//混匀之后的第一次数据采集(来自抓手A)
	{
		(*ChannelStartFlag) |= ChannelId;
		FirstAdTime_A = OSTimeGet() - mix_first_ad_time_A ;
		
	}
	else
	{
		FirstAdTime_A = 0xFFFF;
	}
	if(((*ChannelStartFlag)&(ChannelId<<4)) == 0)//混匀之后的第一次数据采集(来自抓手B)
	{
		(*ChannelStartFlag) |= ChannelId<<4;
		FirstAdTime_B = OSTimeGet() - mix_first_ad_time_B ;
	}
	else
	{
		FirstAdTime_B = 0xFFFF;
	}
	AD7699_ReadAllChannel_GW6000(buffer, LaserId, FirstAdTime_A, FirstAdTime_B);

//光值校准

	
	if(wave[buffer->channelIdx].need_updata)
	{
		wave[buffer->channelIdx].need_updata = 0;
		
		for(int i=0;i<LIGHT_CHANNEL_SIZE;i++)
		{
			wave[buffer->channelIdx].data[i] = buffer->data[i];
		}
	}	
	
	is_windx_over(windx_len);
}




/*
 * @brief 光源的开关控制
 *
 *
 */

void LampCtl(void)
{
		u32 time1 = OSTimeGet();		
		//805nm
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_3, GPIO_PIN_RESET);//全灭
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);//红外光
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7|GPIO_PIN_5, GPIO_PIN_RESET);//全灭
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);//红外光
		OSTimeDly(8);
		
	
		//全灭 2
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3, GPIO_PIN_SET);//全灭
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5, GPIO_PIN_SET);//全灭
		OSTimeDly(12);

		
		//全灭 3
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3, GPIO_PIN_SET);//全灭
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5, GPIO_PIN_SET);//全灭
		OSTimeDly(12);
		
		//亮紫光 405nm 4
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);//
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4|GPIO_PIN_3, GPIO_PIN_SET);//紫光(module 1)
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);//
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6|GPIO_PIN_5, GPIO_PIN_SET);//紫光(module 2)
		
		OSTimeDly(12);
		
		if(Ths.CircleCount == 0 || Ths.CircleCount == 5)//每秒采2次
		{
			SingleGetData(&Ths.ChannelStartFlag, &Ths.Ads[Ths.windex], &Ths.windex, 0x02, 3);
		}
		
		
		//全灭 5
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3, GPIO_PIN_SET);//全灭
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5, GPIO_PIN_SET);//全灭

		OSTimeDly(12);
		
		//570nm 6
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3, GPIO_PIN_RESET);//黄光(module 1)
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5, GPIO_PIN_RESET);//黄光(module 2)
		OSTimeDly(12);
		if(Ths.CircleCount == 0 || Ths.CircleCount == 5)//每秒采2次
		{
			SingleGetData(&Ths.ChannelStartFlag, &Ths.Ads[Ths.windex], &Ths.windex, 0x04, 5);
		}
		else
		{
			OSTimeDly(4);
		}
	
		//680nm 7
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);//红光(module 1)
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7|GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET);//红光(module 2)
		OSTimeDly(12);
		
		//每秒采10次
		SingleGetData(&Ths.ChannelStartFlag, &Ths.Ads[Ths.windex], &Ths.windex, 0x08, 6);
					
		//全灭 8
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3, GPIO_PIN_SET);//全灭
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5, GPIO_PIN_SET);//全灭
		OSTimeDly(12);
	
		Ths.CircleCount++;
		if(Ths.CircleCount == 10) 
		{
			Ths.CircleCount = 0;
		}			
}

/*
 * @brief 返回剩余数据个数
 *
 *
 *
 */
unsigned short Light_RemianDataCountGet(void)
{
	if(Ths.windex >= Ths.rindex)
	{
		return Ths.windex - Ths.rindex;
	}
	else
	{
		return (LIGHT_BUFF_SIZE) - Ths.rindex + Ths.windex;
	}
}

/* 
 * @brief 光值读取
** @param data 光源控制的结构体指针，readCnt 一次读取的数据组数，一组28个数据 
** @return 读取到数据的组数
**
*/

unsigned char LightGetData(Light_Ad_t * const data, unsigned int readCnt)
{
	unsigned int index = 0;
	unsigned int rindex = Ths.rindex;
	
	
	while(index < readCnt && rindex != Ths.windex)
	{
		if((!(wave[Ths.Ads[Ths.windex].channelIdx].need_updata)) && wave[Ths.Ads[Ths.windex].channelIdx].func_start)	
		{
			float adjust_power_A = 1.0 * wave[Ths.Ads[rindex].channelIdx].data[12] / Ths.Ads[rindex].data[12]; //一区的校准系数
			float adjust_power_B = 1.0 * wave[Ths.Ads[rindex].channelIdx].data[25] / Ths.Ads[rindex].data[25]; //二区的校准系数
			
			for(unsigned char i = 0; i < 12; i++) 
			{
				if(Ths.Ads[rindex].data[i]  > 65535)
				{
					Ths.Ads[rindex].data[i] = 65535;
				}
				else
				{
					Ths.Ads[rindex].data[i] = Ths.Ads[rindex].data[i] * adjust_power_A; //光值校准
				}
			}
			for(unsigned char i = 13; i < 25; i++) 
			{
				
				if(Ths.Ads[rindex].data[i]  > 65535)
				{
					Ths.Ads[rindex].data[i] = 65535;
				}
				else
				{
					Ths.Ads[rindex].data[i] = Ths.Ads[rindex].data[i] * adjust_power_B; //光值校准
				}
			}
		}
		data[index++] = Ths.Ads[rindex];
		rindex ++;
		if(rindex >= (LIGHT_BUFF_SIZE))
		{
			rindex = 0;
		}
	}
	Ths.rindex = rindex;
	return index;
}

/*
 * @brief 数据读写下标清零
 *
 *
 */

void LightClearData(void)
{
	Ths.rindex = 0;
	Ths.windex = 0;
}

/*
 * @brief 开启数据读取
 * 
 *
 */

void LightDataOpen(void)
{
	
}

/*
 * @brief 关闭数据读取
 * 
 *
 */
void LightDataClose(void)
{
	
}

