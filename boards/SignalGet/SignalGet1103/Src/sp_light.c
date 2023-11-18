#include "sp_light.h"
#include "stm32f4xx_hal.h"
#include "delayus.h"
#include "Protocols.h"
#include "bus_can_f4_hal.h"
#include "ad_signal.h"
#include "async.h"
#include <stdbool.h>
#include <math.h>
#include "ad5245.h"
#include "gpio.h"
#include "ad_signal_cfg.h"

Wave wave[8];
Light_t Ths;
extern TIM_HandleTypeDef htim2;

#define STATE_IDLE				0
#define STATE_SAMPLE    		1

#define INVALID_CHANNEL			128

static void SPLight_Machine(bool sensor);


#define SENSOR()			((GPIOB->IDR & GPIO_PIN_5))

#define	 MAX_GAP_TIME		43970
#define	 MIN_GAP_TIME		9768

#define MAX_TIM_SPAN		10000

static u32 max_gap_time = 0;
static u32 min_gap_time = 0;
static volatile u32 mix_first_ad_time = 0;
static volatile bool isTheFirst = true;
//此值是采集12个数值使用的时间，可能是示波器量出，在未加延时前是268
#define LIGHT_READ_AD_NEED	 905//448  //268//


static void LightSampler_CloseFallEdgeIt(void)
{
	EXTI->FTSR &= ~(1<<5);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
}

static void LightSampler_OpenFallEdgeIt(void)
{
	EXTI->FTSR |= 1<<5;
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
}

// 混匀中断
void EXTI4_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

// 混匀中断
void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_4)
	{
		// 混匀中断
		Ths.ChannelStartFlag = 0;
		mix_first_ad_time = OSTimeGet();
	}
	else if(GPIO_Pin == GPIO_PIN_5)
	{
		// 光源中断
		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
		SPLight_Machine(SENSOR());
	}
}

void Light_ExitClose(void) {
	EXTI->IMR &= (~GPIO_PIN_5);
}
void Light_ExitOpen(void) {
	EXTI->IMR |= GPIO_PIN_5;
}

void Light_Init(void) {
	Light_ExitOpen();
	memset(&Ths, 0, sizeof(Ths));
	memset(wave, 0, sizeof(wave));
	for(int i=0;i<8;i++)
	{
		wave[i].need_updata = 1;
		wave[i].func_start = 1;
	}
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

static void SPLight_Machine(bool sensor) 
{
	switch (Ths.state) 
	{
		case STATE_IDLE:
		{
			if(isTheFirst)
			{
				max_gap_time = 0;
				min_gap_time = 0;
				TIM2->CNT = 0;
				isTheFirst = false;
//				printf("s\r\n");
				break;
			}
			if (sensor) 
			{
				TIM2->CNT = 0;
//                printf("s");
			} 
			else 
			{
//                printf("n");
				int t = TIM2->CNT;
//                 printf("n=%d\r\n",TIM2->CNT);
				
				if(t > max_gap_time)
				{
					max_gap_time = t;
//                    printf("max=%d\r\n",max_gap_time);
				}
				if(t < (max_gap_time>>1))
				{
					//要最后一个
					min_gap_time = t;
//                    printf("min=%d\r\n",min_gap_time);
					Ths.near_cnt++;
				}
				//逆时针旋转   16的时候最大缺口正对光源
				if(Ths.near_cnt > 16)
				{
					if(t > (max_gap_time>>1))
					{
						Ths.currentChannelIndex = 1;
						Ths.state = STATE_SAMPLE;
						
						
//					for(u8 i =0;i<12;i++)
//					{
//						ad5245_write(iic[i], 0, test_iic_buffer[Ths.currentChannelIndex][i]);
//					}
						//关闭下降沿中断
						LightSampler_CloseFallEdgeIt();
						Ths.near_cnt = 0;
						// 速度：4r/s
//						max_gap_time = 11375;
//						min_gap_time = 2555;
						
						// 速度：5r/s
//						max_gap_time = 9100;
//						min_gap_time = 2044;
						
						// 速度：10r/s
//						max_gap_time = 4455;
//						min_gap_time = 1003;
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
				|| (Ths.currentChannelIndex == 1) // 每秒只采两个值
				|| (Ths.currentChannelIndex == 2) // 全采
				|| (Ths.currentChannelIndex == 3) // 每秒只采两个值
				|| (Ths.currentChannelIndex == 4) // 全采
				|| (Ths.currentChannelIndex == 5 && (Ths.CircleCount == 0 || Ths.CircleCount == 5)) // 全采
				|| (Ths.currentChannelIndex == 6) // 全采
				|| (Ths.currentChannelIndex == 7) // 全采
				)
				{
					if(Ths.currentChannelIndex == 0)
					{
						//int delay = (max_gap_time - LIGHT_READ_AD_NEED)  / 2;
	//					delayUs(delay + 360);// 速度：4r/s
	//					delayUs(delay + 288);// 速度：5r/s
						//delayUs(delay + 144);// 速度：10r/s
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
					//	int delay = (min_gap_time - LIGHT_READ_AD_NEED) / 2;
	//					delayUs(delay - 200);// 速度：4r/s
	//					delayUs(delay - 160);// 速度：5r/s
						//delayUs(delay - 80); // 速度：10r/s
						delayUs((min_gap_time > LIGHT_READ_AD_NEED)? (min_gap_time - LIGHT_READ_AD_NEED) / 2 : 0); // 速度：10r/s
					}
					
					HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);
					u16 *buff = signal_reader_as_ref()->hd_group_read_all();
					
					HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);
					Ths.Ads[Ths.windex].channelIdx = Ths.currentChannelIndex;
					if(wave[Ths.Ads[Ths.windex].channelIdx].need_updata)
					{
						wave[Ths.Ads[Ths.windex].channelIdx].need_updata = 0;
						for(int i=0;i<AD7699_ALL_CHANNEL_USED;i++)
						{
							wave[Ths.Ads[Ths.windex].channelIdx].data[i] = buff[i];
						}
					}
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
						Ths.Ads[Ths.windex].data[i] = buff[i];
//                        printf("w=%d\r\n", buff[i]);
					}
				/*----------------------校准------------------------------------------------*/
				/*现在采取一路一路光校准*/
				
		/*-------------------------------------------------------------------------*/
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

				/*写入当前通道号的下一个通道的iic值*/

//					for(u8 i =0;i<12;i++)
//					{
//						ad5245_write(iic[i], 0, test_iic_buffer[Ths.currentChannelIndex][i]);
//					}
				
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
	
	if(channel == 0)
	{
		channel = 0xFFUL; // 全0也表示读所有通道
	}
	
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
		if((channel & (0x1UL << Ths.Ads[rindex].channelIdx)) != 0)
		{
			if(!(wave[Ths.Ads[Ths.windex].channelIdx].need_updata) && wave[Ths.Ads[Ths.windex].channelIdx].func_start)
			{
				for(unsigned char i = 0; i < AD7699_ALL_CHANNEL_USED-2; i++) 
				{
					float d = 1.0 * wave[Ths.Ads[rindex].channelIdx].data[10] / Ths.Ads[rindex].data[10];
					if(Ths.Ads[rindex].data[i] * d > 65535)
					{
						Ths.Ads[rindex].data[i] = 65535;
					}
					else
					{
						Ths.Ads[rindex].data[i] = Ths.Ads[rindex].data[i] * d;
					}
				}
			}
			data[index++] = Ths.Ads[rindex];
		}
		rindex++;
		if(rindex >= (LIGHT_BUFF_SIZE))
		{
			rindex = 0;
		}
	}
	Ths.rindex = rindex;
	return index;
}


void Light_ClearData(void)
{
	Ths.rindex = 0;
	Ths.windex = 0;
}

