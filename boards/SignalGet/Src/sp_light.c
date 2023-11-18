#include "sp_light.h"
#include "stm32f4xx_hal.h"
#include "delayus.h"
#include "Protocols.h"
#include "bus_can_f4_hal.h"
#include "AD7699BySG0703.h"
#include "async.h"
#include <stdbool.h>
#include <math.h>


Light_t Ths;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;

#define STATE_IDLE				0
#define STATE_SAMPLE    		1

#define INVALID_CHANNEL			128

static void SPLight_Machine(bool sensor);

#define LED_OPEN()	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
#define LED_CLOSE()	 		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
#define SENSOR()			((GPIOB->IDR & GPIO_PIN_3))

#define	 MAX_GAP_TIME		43970
#define	 MIN_GAP_TIME		9768

#define MAX_TIM_SPAN		10000

static u32 max_gap_time = 0;
static u32 min_gap_time = 0;
static volatile u32 mix_first_ad_time = 0;
static volatile bool isTheFirst = true;
//此值是采集12个数值使用的时间，可能是示波器量出，在未加延时前是268
#define LIGHT_READ_AD_NEED	 905  //268//



static void LightSampler_CloseFallEdgeIt(void)
{
	EXTI->FTSR &= ~(1<<3);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
}

static void LightSampler_OpenFallEdgeIt(void)
{
	EXTI->FTSR |= 1<<3;
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
}

/*
配置为双边沿触发

走完时间确定改为上升沿触发,


*/
void EXTI3_IRQHandler() {
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
//	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_5);
	SPLight_Machine(SENSOR());
}

// 混匀中断
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_5);
	Ths.ChannelStartFlag = 0;
	mix_first_ad_time = OSTimeGet();
}

void Light_ExitClose(void) {
	EXTI->IMR &= (~GPIO_PIN_3);
}
void Light_ExitOpen(void) {
	EXTI->IMR |= GPIO_PIN_3;
}

void Light_Init(void) {
	Light_ExitOpen();
	memset(&Ths, 0, sizeof(Ths));
	Ths.preChannelIndex = INVALID_CHANNEL;
	Ths.rindex = 0;
	Ths.windex = 0;
	Ths.CircleCount = 0;
	Ths.ChannelStartFlag = 0;
}

void Light_Start(void) {
	Light_ExitOpen();
	TIM2->CNT = 0;
	TIM2->ARR = 0xffffffff;
	HAL_TIM_Base_Start(&htim2);
	Ths.state = STATE_IDLE;
}


/*
10r/s 0号孔对应的间隙走过时间需要4.56ms
其他孔对应的间隙走过时间 980us
采值时间 905us   中断执行时间是955us
当前时间采值在光轮的最大速度下

正常情况下上升沿开始采值

采值流程分两个阶段：1.确定大小孔对应的间隙走过需要的时间
				    2.中断上升沿触发，经过一定延时后开始采值

*/
static void SPLight_Machine(bool sensor) 
{

	switch (Ths.state) 
	{
		case STATE_IDLE:
		{
			if(isTheFirst)
			{
				TIM2->CNT = 0;
				isTheFirst = false;
				max_gap_time = 0;
				min_gap_time = 0;
				break;
			}
			if (sensor) 
			{
				TIM2->CNT = 0;
			} 
			else 
			{
				int t = TIM2->CNT;
				if(Ths.near_cnt < 128){
					//加速阶段
					Ths.near_cnt++;
					break;
				}else if(Ths.near_cnt < 144){
					//留出2圈找参数
					if(t > max_gap_time)
					{
						max_gap_time = t;
					}
					if(t < (max_gap_time>>1))
					{
						//要最后一个
						min_gap_time = t;
						Ths.near_cnt++;
					}
				}else{
					Ths.near_cnt++;
					if(Ths.near_cnt > 160){
						//两圈内未跳出,则重新计算
						Ths.near_cnt = 0;
						isTheFirst = true;
						break;
					}
					if(t > (max_gap_time>>1))
					{
						Ths.currentChannelIndex = 1;
						Ths.state = STATE_SAMPLE;
						//关闭下降沿中断
						LightSampler_CloseFallEdgeIt();
						Ths.near_cnt = 0;
					}
				}
				
			}
			break;
		}
			
		case STATE_SAMPLE:
		{
			if(sensor)
			{
				// 以下逻辑几乎全部修改(wzc)
				// 以光耦为基准, 逆时针 宽度最大的缝隙对应340
				// 340
				// 405
				// 546
				// 570
				// 670
				// 700
				// 800
				// DARK
				
				//以光源为基准,因为光源与光耦错两个格
				// 2:340波长所在孔
				// 3:405波长所在孔,405每秒只采两个值，这两个值分别来自第五圈和第十圈
				// 4:546波长所在孔
				// 5：570波长所在孔,570每秒只采两个值，这两个值分别来自第五圈和第十圈
				// 6：670波长所在孔,670每秒采10个值
				// 7:700波长所在孔
				// 0:800波长所在孔
				// 1:DARK
				TIM2->CNT = 0;
				if((Ths.currentChannelIndex == 0) // 全采
				|| (Ths.currentChannelIndex == 1) // 全采
				|| (Ths.currentChannelIndex == 2) // 全采
				|| (Ths.currentChannelIndex == 3 && (Ths.CircleCount == 0 || Ths.CircleCount == 5)) // 每秒只采两个值
				|| (Ths.currentChannelIndex == 4) // 全采
				|| (Ths.currentChannelIndex == 5 && (Ths.CircleCount == 0 || Ths.CircleCount == 5)) // 每秒只采两个值
				|| (Ths.currentChannelIndex == 6) // 全采
				|| (Ths.currentChannelIndex == 7) // 全采
				)
				{
					if(Ths.currentChannelIndex == 0)
					{
						delayUs((max_gap_time > LIGHT_READ_AD_NEED)? (max_gap_time - LIGHT_READ_AD_NEED) / 2 : 0);
						if(!SENSOR())
						{
							printf("error\r\n");
							// 报错
							isTheFirst = true;
							Ths.CircleCount = 0; // 出错，则计数归零
							Ths.state = STATE_IDLE;
							LightSampler_OpenFallEdgeIt(); // 打开下降沿中断
							break;
						}
					}
					else
					{
						delayUs((min_gap_time > LIGHT_READ_AD_NEED)? (min_gap_time - LIGHT_READ_AD_NEED) / 2 : 0); // 速度：10r/s
					}
					
					HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);
//					u16 *buff = AD7699_ReadAllChannel_GW6000();
					HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);
					
					Ths.Ads[Ths.windex].channelIdx = (Ths.currentChannelIndex+6)%8;
					if((Ths.ChannelStartFlag & (0x01 << Ths.currentChannelIndex)) != 0)
					{
						Ths.Ads[Ths.windex].mix2FirstAdTime = MAX_TIM_SPAN;
					}
					else
					{
						// 混匀之后的第一次采集标志
						Ths.Ads[Ths.windex].mix2FirstAdTime = OSTimeGet() - mix_first_ad_time;
						Ths.ChannelStartFlag |= 0x01 << Ths.currentChannelIndex;
					}
					
					// 保存数据
					for(unsigned int i = 0; i < LIGHT_CHANNEL_SIZE; i++)
					{
//						Ths.Ads[Ths.windex].data[i] = buff[i];
					}

					Ths.windex++;
					if(Ths.windex >= (LIGHT_BUFF_SIZE))
					{
						Ths.windex = 0;
					}
				}

				// 光源数统计
				Ths.currentChannelIndex++;
				if(Ths.currentChannelIndex >= 8)
				{
					Ths.currentChannelIndex = 0;
					
					// 光源盘圈数统计
					Ths.CircleCount++;
					if(Ths.CircleCount >= 10)
					{
						Ths.CircleCount = 0; // 每秒10圈
					}
				}
				//TODO::动态调整放大倍数，暂时不需要				
//				amp_as_ref()->change_wavelength((WaveLength)Ths.currentChannelIndex);
			}
//			else
//			{
//				// 报错
//				isTheFirst = true;
//				Ths.CircleCount = 0; // 出错，则计数归零
//				Ths.state = STATE_IDLE;
//				LightSampler_OpenFallEdgeIt(); // 打开下降沿中断
//			}
			
			break;
		}
		
		default: break;
	}
}

// 返回剩余数据个数
unsigned short Ligrh_RemianDataCountGet(void)
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

// 返回读取到的数据个数
unsigned char Light_GetData(Light_Ad_t * const data, unsigned int readCnt, unsigned int channel)
{
	// 读指定波长的值，每个bit代表一种波长
	unsigned int index = 0;
	unsigned int rindex = Ths.rindex;
	channel &= 0x000000ffUL;
//	if(channel == 0)
//	{
//		channel = 0xFFUL; // 全0也表示读所有通道
//	}
	
	// 调试的时候筛选光波长
//	channel = channel | (0x1UL << 0);
//	channel = channel | (0x1UL << 1);
//	channel = channel | (0x1UL << 2);
//	channel = channel | (0x1UL << 3);
//	channel = channel | (0x1UL << 4);
//	channel = channel | (0x1UL << 5);
//	channel = channel | (0x1UL << 6);
//	channel = channel | (0x1UL << 7);
	
	while(index < readCnt && rindex != Ths.windex)
	{
//		if((channel & (0x1UL << Ths.Ads[rindex].channelIdx)) != 0)
//		{
//			data[index++] = Ths.Ads[rindex];
//        }
		
		if(Ths.Ads[rindex].channelIdx == 3 || Ths.Ads[rindex].channelIdx == 5 || Ths.Ads[rindex].channelIdx == 6)
		{
			data[index++] = Ths.Ads[rindex];
		}
		rindex++;
		if(rindex >= (LIGHT_BUFF_SIZE))
		{
			rindex = 0;
		}
	}
//     printf("\r\n");
	
	Ths.rindex = rindex;
	return index;
}


void Light_ClearData(void)
{
	Ths.rindex = 0;
	Ths.windex = 0;
}

