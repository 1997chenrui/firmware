#include "bus.h"
#include "bus_can_f4_hal.h"
#include "write_data.h"



#define AD_DETECT_SEND_STACK_SIZE	512
#define AD_DETECT_SEND_PRIO			17
static unsigned int AD_DETECT_SEND_STACK[AD_DETECT_SEND_STACK_SIZE];


#define BUFF_LENGTH		64U
#define PACKET_COUNT	1024u

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


// 向总线主动发送数据，阻塞式发送
void bus_write_data(unsigned char *pbuf, unsigned short count, unsigned char adpId, unsigned char desId)
{
	unsigned short windex = sWriteList.windex;
	sWriteList.windex++;
	if(sWriteList.windex >= PACKET_COUNT)
	{
		sWriteList.windex = 0;
	}
	
	//printf("new windex = %d\r\n",sWriteList.windex);
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
	printf("write task \r\n");
	while(true)
	{
		
		//printf("new write send\r\n");
		
		OSSemPend(sWriteList.sem, 0, &err);
		if(sWriteList.rindex != sWriteList.windex)
		{
			//printf("1\r\n");
			sDataPacket = sWriteList.packet[sWriteList.rindex++];
			if(sWriteList.rindex >= PACKET_COUNT)
			{
				sWriteList.rindex = 0;
			}
			
			if(bus_ita_hb_send(sDataPacket.adpId, sDataPacket.desId, sDataPacket.count, sDataPacket.buf, (u8*)(&rt), &len) == 0)
			{
				printf("packet[%02d/%02d] send succ\r\n", sWriteList.rindex, sWriteList.windex);
				printf("cnt2:%d\r\n",rt);
                HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_4);
			}
			else
			{
				printf("packet[%02d/%02d] send error\r\n", sWriteList.rindex, sWriteList.windex);
                //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
			}
			
			
		}
		else
		{
			printf("packet[%02d/%02d]  error or empty\r\n", sWriteList.rindex, sWriteList.windex);
			OSTaskDel(100);
			
			
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
