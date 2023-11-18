#include "bus.h"
#include "bus_can_f4_hal.h"
#include "write_data.h"
#include "auto_cali_client.h"


#define AD_DETECT_SEND_STACK_SIZE	512
#define AD_DETECT_SEND_PRIO			15
static unsigned int AD_DETECT_SEND_STACK[AD_DETECT_SEND_STACK_SIZE];


#define BUFF_LENGTH		512u
#define PACKET_COUNT	25u

#pragma pack(push, 1)
typedef struct _sDataPacket
{
	unsigned char adpId;
	unsigned char desId;
	unsigned short count;
	unsigned char buf[BUFF_LENGTH];
}sDataPacket_t;

typedef struct _sWriteList
{
	OS_EVENT *sem;
	unsigned short windex;
	unsigned short rindex;
	sDataPacket_t packet[PACKET_COUNT];
}sWriteList_t;
#pragma pack(pop)


static sWriteList_t sWriteList = {0};
static sDataPacket_t sDataPacket = {0};


// 启动发送
void bus_write_start(void)
{
	OSSemPost(sWriteList.sem);
}


static float q[BUFF_LENGTH/2]={[ 0 ... BUFF_LENGTH/2-1 ] = 0.0001f};
static float r[BUFF_LENGTH/2]={[ 0 ... BUFF_LENGTH/2-1 ] = 0.001f};
static float kg[BUFF_LENGTH/2]={0};
static float lastP[BUFF_LENGTH/2]={[ 0 ... BUFF_LENGTH/2-1 ] = 10000};
static float nowP[BUFF_LENGTH/2]={0};
static uint8_t out[BUFF_LENGTH]={0};






// 向总线主动发送数据，阻塞式发送
void bus_write_data(unsigned char *pbuf, unsigned short count, unsigned char adpId, unsigned char desId)
{
	unsigned short windex = sWriteList.windex;
	sWriteList.windex++;
	if(sWriteList.windex >= PACKET_COUNT)
	{
		sWriteList.windex = 0;
	}
	
	sWriteList.packet[windex].adpId = adpId;
	sWriteList.packet[windex].desId = desId;
	sWriteList.packet[windex].count = count;
	for(unsigned short i = 0; i < count; i++)
	{
		sWriteList.packet[windex].buf[i] = pbuf[i];
	}
	
	OSSemPost(sWriteList.sem);
}


static void bus_write_task(void *arg)
{
	u8 err;
	u32 rt = 0;
	u32 len = 0;
	(void)arg;
	
	u8 led=0;
	while(true)
	{
		OSSemPend(sWriteList.sem, 5000, &err);
		
		if(err==OS_ERR_TIMEOUT&&led==1)
		{
			led=0;
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
		}
		else if(err==OS_ERR_NONE)
		{
			if(sWriteList.rindex != sWriteList.windex)
			{
				sDataPacket = sWriteList.packet[sWriteList.rindex++];
				
				uint8_t rt=0;
				if(sWriteList.rindex >= PACKET_COUNT)
				{
					sWriteList.rindex = 0;
				}
				WriteU32(&out[0],ReadU32(&sDataPacket.buf[0]));
				WriteU32(&out[4],ReadU32(&sDataPacket.buf[4]));
				WriteU16(&out[8],ReadU16(&sDataPacket.buf[8]));
			
				//第一次不做卡尔曼
				if(ReadU16(&out[sDataPacket.count-2])==0)
				{
					for(uint16_t i=4 + 4 + 2,j=0 ;i<sDataPacket.count;i+=2,j++)
					{
						uint16_t pdata=ReadU16(&sDataPacket.buf[i]);
						WriteU16(&out[i],pdata);
					}	
					
				}
				else
				{
					for(uint16_t i=4 + 4 + 2,j=0 ;i<sDataPacket.count;i+=2,j++)
					{
						uint16_t pdata=ReadU16(&sDataPacket.buf[i]);
						nowP[j]=lastP[j]+q[j];
						kg[j]=nowP[j]/(nowP[j]+r[j]);
						uint16_t odata=ReadU16(&out[i])+kg[j]*(pdata-ReadU16(&out[i]));
						WriteU16(&out[i],odata);
						lastP[j]=(1-kg[j])*nowP[j];
					}
				}
				WriteU16(&out[sDataPacket.count-2],ReadU16(&sDataPacket.buf[sDataPacket.count-2]));
				rt=bus_ita_hb_send(sDataPacket.adpId, sDataPacket.desId, sDataPacket.count, (u8 *)out, (u8*)(&rt), &len);
				if(rt == 0)
				{
					printf("%d\r\n", ReadU16(&out[sDataPacket.count-2]));
					if(led==0)
					{
						led=1;
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
					}
					else
					{
						led=0;
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
					}
					
	 
				}
				else
				{
					//printf("packet[%02d/%02d] send error\r\n", sWriteList.rindex, sWriteList.windex);
					HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
				}
			}
			else
			{
				//printf("packet[%02d/%02d]  error\r\n", sWriteList.rindex, sWriteList.windex);
				OSTaskDel(200);
			}
		}


	}
}


void bus_write_init(void)
{
	sWriteList.rindex = 0;
	sWriteList.windex = 0;
	sWriteList.sem = OSSemCreate(0);
	bus_create_thread(NULL, bus_write_task, AD_DETECT_SEND_STACK, AD_DETECT_SEND_STACK_SIZE, AD_DETECT_SEND_PRIO, NULL);
}
