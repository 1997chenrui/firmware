#include "listenandsync.h"
#include "listenandsync_cfg.h"
#include "stm32f4xx.h"

ListenAndSync_cfg_t listenAndSync_config;

static void SetBetweenTime(u16 t)
{
	listenAndSync_config.betweenTime=t;
	listenAndSync_config.htim->Instance->ARR=t;
}

static void ListenGpio_Start(struct ListenGpio* _this)
{
	_this->state=ON;
}

static void ListenGpio_Stop(struct ListenGpio* _this)
{
	_this->state=OFF;
}

static void ListenAndSync_Start()
{
	listenAndSync_config.state =ON;
	HAL_TIM_Base_Start_IT(listenAndSync_config.htim);
}

static void ListenAndSync_Stop()
{
	listenAndSync_config.state =OFF;
	HAL_TIM_Base_Stop(listenAndSync_config.htim);
}


void ListenAndSync_Init(TIM_HandleTypeDef *htim,ListenGpio* listenList,u16 len)
{
	listenAndSync_config.htim=htim;//设定使用的定时器
	listenAndSync_config.listenList=listenList;//设定监听的列表
	listenAndSync_config.betweenTime=htim->Init.Period;//初始化 
	listenAndSync_config.state=OFF;
	
	listenAndSync_config.Start=ListenAndSync_Start;
	listenAndSync_config.Stop=ListenAndSync_Stop;
	listenAndSync_config.SetBetweenTime=SetBetweenTime;
	listenAndSync_config.listenListLen=len;
	for(int i=0;i<len;i++)
	{
		listenList[i].Start=ListenGpio_Start;
		listenList[i].Stop=ListenGpio_Stop;
		listenList[i].state=ON;
		listenList[i].isCaptuer=NO;
	}
	printf("listenandsync is start\r\n");
}

#if USED_TIM == 2
void TIM2_IRQHandler(void)
{
	static u16 cnt;
__HAL_TIM_CLEAR_IT(listenAndSync_config.htim, TIM_IT_UPDATE);
	if(listenAndSync_config.state==OFF) return;//如果 总开关关闭 则不执行
	
	for(int i=0;i<listenAndSync_config.listenListLen;i++)
	{
		if(listenAndSync_config.listenList[i].state==ON&&(cnt%listenAndSync_config.listenList[i].fd==0))//开启并达到分频时间
		{
			for(int j=0;j<listenAndSync_config.listenList[i].syncListLen;j++)
			{
				HAL_GPIO_WritePin(listenAndSync_config.listenList[i].syncList[j].port
							,listenAndSync_config.listenList[i].syncList[j].pin,
				HAL_GPIO_ReadPin(listenAndSync_config.listenList[i].port
								,listenAndSync_config.listenList[i].pin));//同步相应的引脚			
			}
		}
	}

	if(++cnt==60000)cnt=0;
}

//if(listenAndSync_config.listenList[i].listenMode==LOW)//捕捉低电平模式 
//			{
//				if(HAL_GPIO_ReadPin(listenAndSync_config.listenList[i].port
//										,listenAndSync_config.listenList[i].pin)==GPIO_PIN_RESET)//去捕获
//				{
//					listenAndSync_config.listenList[i].width=0;
//					listenAndSync_config.listenList[i].isCaptuer=OK;
//					for(int j=0;j<listenAndSync_config.listenList[i].syncListLen;j++)
//					{
//						HAL_GPIO_WritePin(listenAndSync_config.listenList[i].syncList[j].port
//									,listenAndSync_config.listenList[i].syncList[j].pin,GPIO_PIN_RESET);//同步相应的引脚
//					}
//					
//					listenAndSync_config.listenList[i].state=OFF;//捕获/同步 一次后 关闭
//				}
//			}
//			else if(listenAndSync_config.listenList[i].listenMode==HIGH)//捕捉高电平模式 
//			{
//				if(HAL_GPIO_ReadPin(listenAndSync_config.listenList[i].port
//										,listenAndSync_config.listenList[i].pin)==GPIO_PIN_SET)//去捕获
//				{
//					listenAndSync_config.listenList[i].width=0;
//					listenAndSync_config.listenList[i].isCaptuer=OK;
//					for(int j=0;j<listenAndSync_config.listenList[i].syncListLen;j++)
//					{
//						HAL_GPIO_WritePin(listenAndSync_config.listenList[i].syncList[j].port
//									,listenAndSync_config.listenList[i].syncList[j].pin,GPIO_PIN_SET);//同步相应的引脚
//					}
//					
//					listenAndSync_config.listenList[i].state=OFF;//捕获/同步 一次后 关闭
//				}				
//				
//			}
//			else if(listenAndSync_config.listenList[i].listenMode==FALLING)//下降沿捕获 GPIO应配置上拉电阻模式
//			{
//				if(listenAndSync_config.listenList[i].isCaptuer==NO)//如果还没有捕获
//				{
//					if(HAL_GPIO_ReadPin(listenAndSync_config.listenList[i].port
//										,listenAndSync_config.listenList[i].pin)==GPIO_PIN_RESET)//那就去捕获
//					{
//						listenAndSync_config.listenList[i].width=0;
//						listenAndSync_config.listenList[i].isCaptuer=OK;
//						for(int j=0;j<listenAndSync_config.listenList[i].syncListLen;j++)
//						{
//							HAL_GPIO_WritePin(listenAndSync_config.listenList[i].syncList[j].port
//										,listenAndSync_config.listenList[i].syncList[j].pin,GPIO_PIN_RESET);//同步相应的引脚
//						}
//					}	
//				}
//				else//如果已经捕获
//				{
//					if(HAL_GPIO_ReadPin(listenAndSync_config.listenList[i].port
//										,listenAndSync_config.listenList[i].pin)==GPIO_PIN_SET)//那就去等待释放
//					{
//						listenAndSync_config.listenList[i].isCaptuer=NO;
//						for(int j=0;j<listenAndSync_config.listenList[i].syncListLen;j++)
//						{
//							HAL_GPIO_WritePin(listenAndSync_config.listenList[i].syncList[j].port
//										,listenAndSync_config.listenList[i].syncList[j].pin,GPIO_PIN_SET);//同步相应的引脚
//							listenAndSync_config.listenList[i].state=OFF;//捕获/同步 一次后 关闭
//						}
//					}	
//					else
//					{
//						listenAndSync_config.listenList[i].width+=1;
//					}
//					
//				}

//			}
//			else if(listenAndSync_config.listenList[i].listenMode==RISING)
//			{
//				if(listenAndSync_config.listenList[i].isCaptuer==NO)//如果还没有捕获
//				{
//					if(HAL_GPIO_ReadPin(listenAndSync_config.listenList[i].port
//										,listenAndSync_config.listenList[i].pin)==GPIO_PIN_SET)//那就去捕获
//					{
//						listenAndSync_config.listenList[i].width=0;
//						listenAndSync_config.listenList[i].isCaptuer=OK;
//						for(int j=0;j<listenAndSync_config.listenList[i].syncListLen;j++)
//						{
//							HAL_GPIO_WritePin(listenAndSync_config.listenList[i].syncList[j].port
//										,listenAndSync_config.listenList[i].syncList[j].pin,GPIO_PIN_SET);//同步相应的引脚
//						}
//					}	
//				}
//				else//如果已经捕获
//				{
//					if(HAL_GPIO_ReadPin(listenAndSync_config.listenList[i].port
//										,listenAndSync_config.listenList[i].pin)==GPIO_PIN_RESET)//那就去等待释放
//					{
//						listenAndSync_config.listenList[i].isCaptuer=NO;
//						for(int j=0;j<listenAndSync_config.listenList[i].syncListLen;j++)
//						{
//							HAL_GPIO_WritePin(listenAndSync_config.listenList[i].syncList[j].port
//										,listenAndSync_config.listenList[i].syncList[j].pin,GPIO_PIN_RESET);//同步相应的引脚
//							listenAndSync_config.listenList[i].state=OFF;//捕获/同步 一次后 关闭
//						}
//					}	
//					else
//					{
//						listenAndSync_config.listenList[i].width+=1;
//					}
//					
//				}
//				
//			}
#elif (USED_TIM==3)
void TIM3_IRQHandler(void)
{

}
#elif (USED_TIM==4)
void TIM4_IRQHandler(void)
{

}

#endif


