#include "ADCDetectHandle.h"
#include "cmd.h"
#include "protocols.h"
#include "ErrCode.h"
#include "io_operation.h"
#include "cmd.h"
#include "auto_cali_client.h"
#include "itf_137.h"

#define DECLARE_SET_QUEUE_ELE(obj)              MC_AD_Ele_t ele;                    \
                                                ele.adapterId = packet->adapterId;        \
                                                ele.cmd = func_code;                    \
                                                ele.srcId = packet->srcId;                \
                                                ele.send = obj;                        \
                                                ele.sessionId = packet->sessionId;

extern u16 singelReadByChannel(int channel);
extern void Mc500Light_ReadContinuous(u32 detect_circle_cnt,u32 z2f,f32 time_per_degree,MC_AD_Ele_t* ele);

extern u32 check_flg;


void ADCDetectCmdExcute(u32 func_code  ,  bus_packet * packet )//由上位机发送 传感器相关命令由这个函数执行。
{
	switch(func_code)
	{
		case CMD_ADAutoCaliStart: // 自动校准(定标)命令（0x1a）
		{
			printf("CMD_ADAutoCaliStart\r\n");
			ADAutoCaliStart_Send* cali = New_ADAutoCaliStart_Send();
			ADAutoCaliStart_Return* rt = New_ADAutoCaliStart_Return();

			cali->GetFromBytes(cali, packet->data, 4);
            
            auto_client_t.errcode = 0;
            
			rt->m2_errCode = appcali_start();//这里开始定标
			//!!此处阻塞到 定标完成
//			printf("rt->m2_errCode = %d\r\n",rt->m2_errCode);
			rt->m1_z2f = light2hole();//记录传感器定标信息
			BytesBufferNode * node =  rt->ToBytes(rt);
			u8 *data = NULL;
			int len = BytesBufferNode_Destroy(node, &data);
			bus_ita_return(packet->adapterId, packet->srcId, packet->sessionId, len, data);
			free(data);

			Delete_ADAutoCaliStart_Send(cali);
			Delete_ADAutoCaliStart_Return(rt);
			break;
		}
		case CMD_ADSampleRead://样本读取命令
		{
			printf("CMD_ADSampleRead\r\n");
			ADSampleRead_Send* s_read = New_ADSampleRead_Send();
			ADSampleRead_Return* rt = New_ADSampleRead_Return();
			
			s_read->GetFromBytes(s_read,packet->data,4);
            
			rt->m1_val = singelReadByChannel(s_read->m1_channel);//直接读取一次采样数据

			BytesBufferNode * node =  rt->ToBytes(rt);
			u8 *data = NULL;
			int len = BytesBufferNode_Destroy(node,&data);
			bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);
			free(data);
			
			Delete_ADSampleRead_Send(s_read);
			Delete_ADSampleRead_Return(rt);
			break;
		}
		
		case CMD_ADDetectConfigSet://传感器配置命令
		{
			printf("CMD_ADDetectConfigSet\r\n");
			ADDetectConfigSet_Send* cfg = New_ADDetectConfigSet_Send();
			ADDetectConfigSet_Return* rt = New_ADDetectConfigSet_Return();
			cfg->GetFromBytes(cfg,packet->data,4);
		
			extern double offangle;
			check_flg=cfg->m3_light_offset;
			int32_t bcangle = (int32_t)cfg->m5_light_motor_speed;
			offangle = (float)bcangle / 1000000.0f;
			printf("check_flg=%d, bcangle=%d, offangle=%lf\r\n", check_flg, bcangle, offangle);
			
			rt->m1_errCode = 0;
			BytesBufferNode * node =  rt->ToBytes(rt);
			u8 *data = NULL;
			int len = BytesBufferNode_Destroy(node,&data);
			bus_ita_return(packet->adapterId,packet->srcId,packet->sessionId,len,data);
			free(data);
			Delete_ADDetectConfigSet_Send(cfg);
			Delete_ADDetectConfigSet_Return(rt);
			printf("return OK...\r\n");
				break;
		}
		case CMD_ADDataRead://传感器数据读取
		{
			printf("CMD_ADDataRead\r\n");
			ADDataRead_Send* send = New_ADDataRead_Send();
			send->GetFromBytes(send,packet->data,4);
			DECLARE_SET_QUEUE_ELE(send);//将ele数据封装到一起  赋值ele.cmd为CMD_ADDataRead
			
			printf("m5_detect_circle_cnt = %d\r\n", send->m5_detect_circle_cnt); // 检测圈数
			printf("m6_z2f = %d\r\n", send->m6_z2f); // 定标结果值
			
			
			printf("m7_time_per_degree = %d\r\n", send->m7_time_per_degree); // 定标结果值
			
			f32 timePer_degree=0;
			timePer_degree=(double)send->m7_time_per_degree/10000+0.5;
			
			//timePer_degree=1388.888889f;
			Mc500Light_ReadContinuous(send->m5_detect_circle_cnt, send->m6_z2f,timePer_degree, &ele);//根据参数内的定标结果和设定的检测圈数进行连续的读取
			break;
			//！ send 在传感器工作线程中 释放
		} 

    case CMD_DISKInstalledRed:
		{
			printf("CMD_DISKInstalledRed\r\n");
			DiskInstalledRed_Send* send = New_DiskInstalledRed_Send();
			
			send->GetFromBytes(send,packet->data,4);//对上位机传来的数据 进行解包
			
			DECLARE_SET_QUEUE_ELE(send);//将ele数据封装到一起  赋值ele.cmd为CMD_ADDataRead
			printf("m1_cnt=%d, m2_time_span_ms=%d, m3_channel=%d\r\n", send->m1_cnt, send->m2_time_span_ms, send->m3_channel);
			DiskInstalledReadWork(send->m1_cnt, send->m2_time_span_ms, send->m3_channel,&ele);
		
			
			break;
		}	
		/*
		case 0x137:
		{
			short timeout = 0;
			Itf_137_t* cmd = Itf_137_new();
            cmd->packet_parse(cmd, packet);
            
			printf("CMD_CommonCtrl_1: m1=%d, m2=%d, m3=%d\r\n", cmd->content.cnt, cmd->content.time_span_ms, cmd->content.channel);
			uint16_t curval;
			uint16_t minval = 0xFFFF; // 次小值
			uint16_t maxval = 0; // 最大值
			u16 min  = 0xFFFF; // 最小值
			for(uint16_t i = 0; i < cmd->content.cnt; i++)
			{
				timeout = 0;
				do
				{
					curval = averageRead((LIGHT_E)cmd->content.channel);
					if(timeout++ > 3)
					{
						curval = averageRead((LIGHT_E)cmd->content.channel);
						printf("data error [%d]\r\n", curval);
						break;
					}
				} while (curval > 65500);
				if(maxval < curval)
				{
					maxval = curval;
				}
				if(curval < min)
				{
					min = curval;
				}
				else if(curval < minval)
				{
					minval = curval;
				}
				OSTimeDly(cmd->content.time_span_ms);
			}
            cmd->rtn_and_free(cmd, packet, 0, maxval - minval, min);
			printf("max(%d) - min(%d) = %d, ignore:%d\r\n", maxval, minval, maxval - minval, min);
			break;
		}
		*/
        default:break;
    
	}
}	
