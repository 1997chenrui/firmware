#include "bus.h"
#include "bus_can_f4_hal.h"
#include "write_data.h"



#define WRITE_DATA_STACK_SIZE	512
#define WRITE_DATA_PRIO			15

static unsigned int WRITE_DATA_STACK[WRITE_DATA_STACK_SIZE];


#define BUFF_LENGTH		64u
#define PACKET_COUNT	512u

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
	
	while(true)
	{
		OSSemPend(sWriteList.sem, 0, &err);

		if(sWriteList.rindex != sWriteList.windex)
		{
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
			printf("packet[%02d/%02d]  error\r\n", sWriteList.rindex, sWriteList.windex);
			OSTaskDel(200);
		}
	}
}


void bus_write_init(void)
{
	sWriteList.rindex = 0;
	sWriteList.windex = 0;
	sWriteList.sem = OSSemCreate(0);
	bus_create_thread(NULL, bus_write_task, WRITE_DATA_STACK, WRITE_DATA_STACK_SIZE, WRITE_DATA_PRIO, NULL);
}
