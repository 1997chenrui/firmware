#include "tool.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


u16 ReadU16(u8* data) 
{
	u16 rt = 0;
	rt = data[0]|(data[1]<<8);
	return rt;
}

u32 ReadU32(u8* data)
{
	u32 rt = 0;
	rt =(u32) (data[0]|(data[1]<<8)|(data[2]<<16)|(data[3]<<24));//隐式转换
	//rt = u32 (data[0]|(data[1]<<8)|(data[2]<<16)|(data[3]<<24));显式转换
	return rt;
}

float ReadF32(u8 *data)
{
    float rt = 0;
    memcpy(&rt,data,4);
    return rt;
}
s32 ReadS32(u8* data)
{
    s32 rt = 0;
    memcpy(&rt,data,4);
    return rt;
}
void WriteF32(u8 *data, f32 code)
{
    union
    {
        u8    data[sizeof(code)];
        f32   code;
    }t;
    t.code = code;
    memmove(data,t.data,sizeof(code));
}

// void ExchangeU32(u32 *a,u32 *b)
// {

// }

void WriteU32(u8 *d,u32 code)
{
    union
    {
        u8    data[sizeof(code)];
        u32   code;
    }t;
    t.code = code;
    memmove(d,t.data,sizeof(code));

}
void WriteS32(u8 *d,s32 code)
{
    union
    {
        u8    data[sizeof(code)];
        s32   code;
    }t;
    t.code = code;
    memmove(d,t.data,sizeof(code));

}
void WriteU16(u8 *d,u16 code)
{
    union
    {
        u8    data[sizeof(code)];
        u16   code;
    }t;
    t.code = code;
    memmove(d,t.data,sizeof(code));
    
}

void reverse_bit(u32* value, u8 offset){
    u32 t = 1 << offset;
    u32 t1 = ~(*value & t);
    *value |= t;
    *value = *value & t1;
}


static const u32  POLYNOMIAL = 0xEDB88320;
bool have_table = false;
u32 table[256];


static void make_table()    //表是全局的
{
	int i, j;
	have_table = true;
	for (i = 0; i < 256; i++)
	for (j = 0, table[i] = i; j < 8; j++)
		table[i] = (table[i] >> 1) ^ ((table[i] & 1) ? POLYNOMIAL : 0);
    
}


u32 crc32(u32 crc, unsigned char *buff, int len)
{
	if (!have_table) make_table();
	crc = ~crc;
	for (int i = 0; i < len; i++)
		crc = (crc >> 8) ^ table[(crc ^ buff[i]) & 0xff];
	return ~crc;
}


int average(void * data,int data_size,int len)
{
    int sum = 0;
    switch(data_size)
    {
        case 1:
        {
            u8 * d = (u8*)data;
            for(int i = 0;i<len;i++)
            {
                sum += d[i];
            }
            return sum/len;
        }
        case 2:
        {
            short * d = (short*)data;
            for(int i = 0;i<len;i++)
            {
                sum += d[i];
            }
            return sum/len;
        }
        case 4:
        {
            int * d = (int*)data;
            for(int i = 0;i<len;i++)
            {
                sum += d[i];
            }
            return sum/len;
        }
    }
    return 0 ;
}


/**
 *  中位值平均滤波
 * @param pData:没有滤波的数据
 * @param nSize:数据大小
 * @return:滤波数值
 */
float median_filter_float(float *pData, int nSize) {
    float max, min;
    float sum;
    if (nSize > 2) {
        max = pData[0];
        min = max;
        sum = 0;
        for (int i = 0; i < nSize; i++) {
            sum += pData[i];
            if (pData[i] > max) {
                max = pData[i]; //一个循环之后max就是最大的值
            }

            if (pData[i] < min) {
                min = pData[i]; //一个循环之后min就是最小的值
            }
        }

        sum = sum - max - min;    //去掉最大的值和最小的值
        return sum / (nSize - 2); //对N-2个数求平均值
    }
    ASSERT(1, "data size <= 2");
}




// void NVIC_Close(int cnt,...)
// {
//     int * start = &cnt + 1;
//     for(int i =0;i<cnt;i++)
//     {
//         HAL_NVIC_DisableIRQ((IRQn_Type)start[i]);
//     }
//     
// }   

// void NVIC_Open(int cnt,...)
// {
//     int * start = &cnt + 1;
//     for(int i =0;i<cnt;i++)
//     {
//         HAL_NVIC_DisableIRQ((IRQn_Type)start[i]);
//     }
// }



// void NVIC_SystickEnable()
// {
//     SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
// }

// void NVIC_SystickDisable()
// {
//     SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
// }


static  const u16 ccitt_table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

u16 crc16(unsigned char *q, int len)
{
    unsigned short crc = 0;
    while (len-- > 0)
        crc = ccitt_table[(crc >> 8 ^ *q++) & 0xff] ^ (crc << 8);
    return ~crc;
}




