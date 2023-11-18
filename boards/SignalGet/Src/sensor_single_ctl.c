#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "sensor_single_ctl.h"
#include "sp_light.h"
#include "itf_00040040.h"
#include "itf_00040050.h"

#define ADVALUE_LENGTH 181

u16 adValue[ADVALUE_LENGTH];

void Light_Data_Get_Test(void);
	
extern unsigned char LightGetData(Light_Ad_t * const data, unsigned int readCnt);

extern void LightClearData(void);

extern unsigned short Light_RemianDataCountGet(void);

extern void LightDataClose(void);

extern void LightDataOpen(void);

extern u32 tim1;

/**
 * @brief 传感器信号指令接口
 * 
 * @param itf_code 指令码
 * @param packet 数据包
 */
void sensor_single_cmd_run(u32 itf_code, bus_packet *packet) {
    switch (itf_code) {
		
		case 0x00040040: {
//			printf("t0 = %d\r\n",OSTimeGet() - tim1);
			Itf_00040040_t *cmd = itf_00040040_new();
			cmd->packet_parse(cmd, packet);
			
			const unsigned int countconst = 6; // 每次上传数据的组数
			u8 ad_value_length = 0;
			
			Light_Ad_t dt[countconst] = {0};
			
			unsigned char readcount = LightGetData(dt, countconst);
			
			u16 remainCnt = Light_RemianDataCountGet();
				
//			printf("remainCnt = %d\r\n",remainCnt);
			
			adValue[ad_value_length++] = readcount;
			
			for(unsigned char i = 0; i < readcount; i++)
			{
//				printf("-----------%d-----------\r\n",i);
				
				
//				printf("channel_id = %d\r\n",dt[i].channelIdx);
				adValue[ad_value_length++] = (dt[i].channelIdx << 13) | ((remainCnt) & (~(0x07<<13)));
				
//				printf("remain_value = %d\r\n",(adValue[ad_value_length-1] & (~(0x07<<13))));
//				printf("remain = %d\r\n",remainCnt);
				
				
				adValue[ad_value_length++] = dt[i].mix1FirstAdTime;
				adValue[ad_value_length++] = dt[i].mix2FirstAdTime;
//				if((adValue[2+i*30] != 65535) || (adValue[3+i*30] != 65535))
//				{
//					printf("%d,%d\r\n",adValue[2+i*30],adValue[3+i*30]);
//				}	
//				printf("v%d=%d\r\n",dt[i].channelIdx,dt[i].data[13]);
				for(unsigned char k = 0; k < LIGHT_CHANNEL_SIZE; k++)
				{
					adValue[ad_value_length++] = dt[i].data[k];
					
//					printf("%d,\r\n",adValue[k*30 + ad_value_length-1]);
				}
				
			}
//			printf("len = %d\r\n",ad_value_length);
//			printf("t1 = %d\r\n",OSTimeGet() - tim1);
            cmd->rtn_and_free(cmd, packet, adValue, ad_value_length);
//			printf("t2 = %d\r\n",OSTimeGet() - tim1);
            break;
        }
		case 0x00040050: {
			Itf_00040050_t *cmd = itf_00040050_new();
			cmd->packet_parse(cmd, packet);
			cmd->content.m1_id_cmd >>= 4;
			printf("id = %d\r\n",cmd->content.m1_id_cmd);
			switch (cmd->content.m1_id_cmd)
			{
				case 0:
				{
					LightDataClose();
					break;
				}
				case 1:
				{
					LightDataOpen();
					break;
				}
				case 2:
				{
					LightClearData();
					break;
				}
				
			}
			cmd->rtn_and_free(cmd, packet, 0);
			
			break;
		}
}
}

/*brief:光源数据采集测试
**
**
**
*/
void Light_Data_Get_Test(void)
{
	const unsigned int countconst = 3; // 每次上传数据的组数
			u8 ad_value_length = 0;
			
			Light_Ad_t dt[countconst] = {0};
			
			unsigned char readcount = LightGetData(dt, countconst);
			
			u16 remainCnt = Light_RemianDataCountGet();
			
			
//			printf("remainCnt = %d\r\n",remainCnt);
			
			for(unsigned char i = 0; i < readcount; i++)
			{
//				printf("-----------%d-----------\r\n",i);
				adValue[ad_value_length++] = 30;
				adValue[ad_value_length++] = (dt[i].channelIdx << 13) | ((remainCnt--) & (~(0x07<<13)));
				adValue[ad_value_length++] = dt[i].mix1FirstAdTime;
				adValue[ad_value_length++] = dt[i].mix2FirstAdTime;
				if(dt[i].channelIdx == 3)
				{
					printf("%d,%d\r\n",dt[i].data[13],dt[i].data[25]);
				}
				for(unsigned char k = 0; k < LIGHT_CHANNEL_SIZE; k++)
				{
					adValue[ad_value_length++] = dt[i].data[k];
//					printf("%d,",dt[i].data[k]);
//					printf("%d,\r\n",adValue[k*30 + ad_value_length-1]);
				}
//				putchar('\n');
			}
			
//			for(int i = 0 ;i< 31; i++)
//			{
//				printf("%d,",adValue[i]);
//			}
//			putchar('\n');
//			for(int i = 31 ;i< 62; i++)
//			{
//				printf("%d,",adValue[i]);
//			}
//			putchar('\n');
//			for(int i = 62 ;i< 93; i++)
//			{
//				printf("%d,",adValue[i]);
//			}
//			putchar('\n');
//			printf("ad_value_length = %d\r\n",ad_value_length);
}



