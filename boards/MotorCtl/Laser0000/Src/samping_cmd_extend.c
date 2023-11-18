#include "samping_cmd_extend.h"
#include "stm32f4xx.h"
#include "bus.h"
#include "itf_01000c31.h"
#include "itf_01000c32.h"
#include "itf_01000c33.h"
#include "bus_can_f4_hal.h"
#include "ad_signal.h"
#include <stdlib.h>
#include "AD7699.h"
#include "ad_signal_cfg.h"
#include "math.h"
#include "write_data.h"
u32 Samping_cmd_extend_ThreadSTk[256];


u8 signal_array[256];

//u16 signal_array_new[51][7];

static u8 laser_id;



static u8 sampingtaskBusyFlag=0;


//void Samping_cmd_extend_Thread(void *arg)
//{
//	printf("创建采值任务成功\r\n");
//	sampingtaskBusyFlag=1;

//	signal_len=(u16)(samping_ms)/time_spam_ms;
//	printf("检测次数为:%d 时间：%d 间隔时间：%d\r\n",signal_len,samping_ms,time_spam_ms);

//	for(;;)
//	{			
//		for(u16 i=0;i<signal_len;i++)
//		{
//		
//				WriteU32((u8*)&signal_array[0],0x1000c32);
//				WriteU32((u8*)&signal_array[0+2],(9));
//				WriteU16((u8*)&signal_array[2+2+0],signal_len);
//				WriteU16((u8*)&signal_array[2+2+1],i);
//				
//			
//			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
//			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
//			OSTimeDly(10);
//			WriteU16((u8*)&signal_array[2+2+2],AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[2]));
//			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
//			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
//			OSTimeDly(10);
//			WriteU16((u8*)&signal_array[2+2+3],AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[2]));
//			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
//			OSTimeDly(10);
//			WriteU16((u8*)&signal_array[2+2+4],AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[2]));
//			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
//			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
//			OSTimeDly(10);
//			WriteU16((u8*)&signal_array[2+2+5],AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[2]));
//			WriteU16((u8*)&signal_array[2+2+6],AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[3]));
//			
//			WriteU16((u8*)&signal_array[2+2+7],AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[1]));
//			WriteU16((u8*)&signal_array[2+2+8],AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[0]));
//			


//			bus_write_data((u8 *)&signal_array, 13*sizeof(u16), CAN_ADAPTER_ID, 0);
//			OSTimeDly(time_spam_ms-40);
//		}
//		
//		printf("检测完成\r\n");
//		sampingtaskBusyFlag=0;
//		OSTaskDel(6);
//	}
//}

void Samping_cmd_extend_Thread(void *arg)
{
	printf("创建采值任务成功\r\n");
	sampingtaskBusyFlag=1;


	printf("检测次数为:%d 时间：%d 间隔时间：%d\r\n",samping_count*time_span_ms,samping_count,time_span_ms);

	uint32_t buf_405[4];

	uint16_t buf_405_650[10][3];

	uint8_t count=0;

	uint8_t upcount=0;
	for(;;)
	{			
		for(u16 i=0;i<samping_count;i++)
		{
			if(laser_id == 0)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);//放大 1000
				OSTimeDly(10);
				buf_405[0]=AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[2]);//1000倍 405 透射

				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);//放大  10000
				OSTimeDly(10);
				buf_405[1]=AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[2]);//10000倍 405 透射

				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);//放大 100000
				OSTimeDly(10);
				buf_405[2]=AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[2]);//100000倍 405 透射

				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);//放大1000000
				OSTimeDly(10);
				buf_405[3]=AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[2]);//1000000倍 405 透射

				int8_t p;
				for( p=3;p>=0;p--)
				{
					if(buf_405[p]<=64000)
					{
						buf_405_650[count][0]=buf_405[p];
						buf_405_650[count][1]=p;
						break;
					}
				}

				if(p<0)
				{
					buf_405_650[count][0]=buf_405[0];
					buf_405_650[count][1]=0;
				}

	//			buf_405_650[count][2]=AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[1]);//650 透射



				count=count+1;

				if(count==10)//采集到10组数据
				{

					printf("new send 10\r\n");
					WriteU32((u8*)&signal_array[0],0x1000c32);
					WriteU16((u8*)&signal_array[4],(upcount));//current_count

					WriteU32((u8*)&signal_array[6],10);

					for(uint8_t k=0,d=10;k<10;k++)
					{
						WriteU16((u8*)&signal_array[d],buf_405_650[k][0]);

						d=d+2;
						signal_array[d]=buf_405_650[k][1];
						d=d+1;

						printf("id=%d,%d,%d\r\n",k,buf_405_650[k][0],buf_405_650[k][1]);

					}

					bus_write_data((u8 *)&signal_array, 10+(9*sizeof(u16)*2)+4+4+2, CAN_ADAPTER_ID, 0);

					upcount++;
					count=0;

				}
				else if(i==(samping_count)-1)//没采集到10组数据 但是已经是最后一次采集 也发送
				{

					printf("new send less 10\r\n");

					WriteU32((u8*)&signal_array[0],0x1000c32);
					WriteU16((u8*)&signal_array[4],(upcount));//current_count

					WriteU32((u8*)&signal_array[6],count);

					for(uint8_t k=0,d=10;k<count;k++)
					{
						WriteU16((u8*)&signal_array[d],buf_405_650[k][0]);
						d=d+2;
						signal_array[d]=buf_405_650[k][1];
						d=d+1;
						WriteU16((u8*)&signal_array[d],buf_405_650[k][2]);
						printf("id=%d,%d,%d\r\n",k,buf_405_650[k][0],buf_405_650[k][1]);
					}

					bus_write_data((u8 *)&signal_array, count+((count-1)*sizeof(u16)*2)+4+4+2, CAN_ADAPTER_ID, 0);

					upcount++;
					count=0;



				}	
			}
			else
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);//放大 1000
				OSTimeDly(10);
				buf_405[0]=AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[0]);//1000倍 650 透射

				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);//放大 10000
				OSTimeDly(10);
				buf_405[1]=AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[0]);//10000倍 650 透射

				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);//放大 100000
				OSTimeDly(10);
				buf_405[2]=AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[0]);//100000倍 650 透射

				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);//放大1000000
				OSTimeDly(10);
				buf_405[3]=AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[0]);//1000000倍 650 透射

				int8_t p;
				for( p=3;p>=0;p--)
				{
					if(buf_405[p]<=64000)
					{
						buf_405_650[count][0]=buf_405[p];
						buf_405_650[count][1]=p;
						break;
					}
				}

				if(p<0)
				{
					buf_405_650[count][0]=buf_405[0];
					buf_405_650[count][1]=0;
				}

	//			buf_405_650[count][2]=AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[1]);//650 透射



				count=count+1;

				if(count==10)//采集到10组数据
				{

					printf("new send 10\r\n");
					WriteU32((u8*)&signal_array[0],0x1000c32);
					WriteU16((u8*)&signal_array[4],(upcount));//current_count

					WriteU32((u8*)&signal_array[6],10);

					for(uint8_t k=0,d=10;k<10;k++)
					{
						WriteU16((u8*)&signal_array[d],buf_405_650[k][0]);

						d=d+2;
						signal_array[d]=buf_405_650[k][1];
						d=d+1;

						printf("id=%d,%d,%d\r\n",k,buf_405_650[k][0],buf_405_650[k][1]);

					}

					bus_write_data((u8 *)&signal_array, 10+(9*sizeof(u16)*2)+4+4+2, CAN_ADAPTER_ID, 0);

					upcount++;
					count=0;

				}
				else if(i==(samping_count)-1)//没采集到10组数据 但是已经是最后一次采集 也发送
				{

					printf("new send less 10\r\n");

					WriteU32((u8*)&signal_array[0],0x1000c32);
					WriteU16((u8*)&signal_array[4],(upcount));//current_count

					WriteU32((u8*)&signal_array[6],count);

					for(uint8_t k=0,d=10;k<count;k++)
					{
						WriteU16((u8*)&signal_array[d],buf_405_650[k][0]);
						d=d+2;
						signal_array[d]=buf_405_650[k][1];
						d=d+1;
						WriteU16((u8*)&signal_array[d],buf_405_650[k][2]);
						printf("id=%d,%d,%d\r\n",k,buf_405_650[k][0],buf_405_650[k][1]);
					}

					bus_write_data((u8 *)&signal_array, count+((count-1)*sizeof(u16)*2)+4+4+2, CAN_ADAPTER_ID, 0);

					upcount++;
					count=0;



				}
			}

			printf("samping count =%d",i);
			OSTimeDly(time_span_ms-40);
		}

		printf("检测完成\r\n");
		sampingtaskBusyFlag=0;
		OSTaskDel(6);
	}
}

u8 bus_ita_func_callback_extern(u32 func_code, bus_packet * packet){
	printf("扩展指令收到\r\n");
	switch(func_code){
		case 0x01000c31: {
			Itf_01000c31_t * cmd = Itf_01000c31_new();
			cmd->packet_parse(cmd,packet);
			printf("单次直接采集\r\n");
			WriteU32((u8*)&signal_array[0],(7));

			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
			OSTimeDly(10);
			WriteU16((u8*)&signal_array[4+0],AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[2]));
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
			OSTimeDly(10);
			WriteU16((u8*)&signal_array[4+2],AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[2]));
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_SET);
			OSTimeDly(10);
			WriteU16((u8*)&signal_array[4+4],AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[2]));
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_0, GPIO_PIN_RESET);
			OSTimeDly(10);
			WriteU16((u8*)&signal_array[4+6],AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[2]));
			WriteU16((u8*)&signal_array[4+8],AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[3]));

			WriteU16((u8*)&signal_array[4+10],AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[1]));
			WriteU16((u8*)&signal_array[4+12],AD7699_GetSingleData(&ad7699[0], ad_channel_map_0[0]));

			printf("采值结束数据长度：%d\r\n",(signal_array[0])|((u32)signal_array[1])<<16);

			//for(u16 i=0;)
			cmd->rtn_and_free(cmd,packet,signal_array, (7+2)*2);
		break;
		}

		case 0x01000c32: {

			printf("采值任务收到，准备开始\r\n");

			itf_01000c32_t * cmd = itf_01000c32_new();
			cmd->packet_parse(cmd,packet);
			laser_id = cmd->content.laser_id;
			if(sampingtaskBusyFlag==1)
			{
				    printf("上次采值未结束\r\n");
					cmd->rtn_and_free(cmd,packet,2);
					return 0;
			}

			time_span_ms=cmd->content.samping_span_ms;

			samping_count=cmd->content.samping_count;	


			printf("span_ms:%d\r\n",samping_count);
			printf("span_count:%d\r\n",samping_count);
			u8 rt= OSTaskCreateExt(Samping_cmd_extend_Thread, 
				0,
				&Samping_cmd_extend_ThreadSTk[256 - 1],
				6,
				6,
				&Samping_cmd_extend_ThreadSTk[0],
				256,
				0,
				(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

			if(rt!=0)//如果线程创建失败 就返回数组长度为0
			{
				printf("任务创建失败\r\n");
				cmd->rtn_and_free(cmd,packet,1);
			}
			else
			{
				printf("任务创建成功\r\n");
				cmd->rtn_and_free(cmd,packet,0);
			}

			break;
		}
		case 0x01000c33: {
			itf_01000c33_t * cmd = itf_01000c33_new();
			cmd->packet_parse(cmd,packet);

			u8 errorcode=0;
			if(cmd->content.m1_option==0)
			{
//				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
//				HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);			
			}
			else if(cmd->content.m1_option==1)//450
			{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET);
			}
			else if(cmd->content.m1_option==2)//650
			{
				HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);				
			}
			else if(cmd->content.m1_option==3)//全亮
			{
				HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
			}
			else	
			{
				errorcode=1;
			}
			cmd->rtn_and_free(cmd,packet,errorcode);
			break;
		}
		default:return 1;
	}
    return 0;
}