#include "signal_det.h"
#include "AD7699.h"
#include "stdlib.h"

#define SIGNEL_READ_CNT  2
#define AVERAGE_READ_CNT 4

#define YELLOW_CHANNEL   1 // 黄光采值通道
#define PURPLE_CHANNEL   2 //5 // 紫光采值通道

static u16 singelArray[SIGNEL_READ_CNT];
static u16 averageArray[AVERAGE_READ_CNT];

//单次采集
u16 singelRead(LIGHT_E type)
{
    int data = 65535;
    switch(type)
    {
        case n_570:
        {
            data = get_ad_data(n_570,singelArray,SIGNEL_READ_CNT);
            break;
        }
        case n_405:
        {
            data = get_ad_data(n_405,singelArray,SIGNEL_READ_CNT);
            break;
        }
		case n_660:
        {
            data = get_ad_data(n_660,singelArray,SIGNEL_READ_CNT);
            break;
        }
        case n_800:
        {
            data = get_ad_data(n_800,singelArray,SIGNEL_READ_CNT);
            break;
        }
        default:break;
    }
    return data;
    
}
//单次采集  参数 通道号
u16 singelReadByChannel(int channel)
{
	return get_ad_data(channel,singelArray,SIGNEL_READ_CNT);
}

int Ad_cmp_tt(const void* p1, const void* p2) 
{
	return(*(uint16_t*)p1 - *(uint16_t*)p2);
}
uint16_t Ad_samping(LIGHT_E type)
{

	
	uint16_t buff[5]={0};
	uint32_t temp=0;

	get_ad_data(type,buff,5);
	qsort(buff,5, sizeof(buff[0]),Ad_cmp_tt);
	temp=buff[1]+buff[2]+buff[3];
	//temp=buff[3];
	temp=(float)temp/3+0.5;
	
	
//	static uint16_t lastN450=0;
//	static uint16_t lastN570=0;
//	static uint16_t lastN660=0;
//	static uint16_t lastN800=0;
//	
//	static float lastk;
//	static float lastP;
//	static float currentP;
//	
//	static float kg=0;
//	
//	
//	float q=0.001;
//	float r=1;
//	
//	
//	if(type==n_405)
//	{
//		lastk=lastN450;
//	}
//	else if(type==n_570)
//	{
//		lastk=lastN570;
//	}
//	else if(type==n_660)
//	{
//		lastk=lastN660;
//	}
//	else if(type==n_800)
//	{
//		lastk=lastN800;
//	}
//	
//			
//	currentP=lastP+q;
//	kg=currentP/(currentP+r);
//	
//	temp=(float)lastk+kg*((float)temp-lastk)+0.5;
//	
//	lastP=(1-kg)*currentP;
//	
//	
//	if(type==n_405)
//	{
//		lastN450=temp;
//	}
//	else if(type==n_570)
//	{
//		lastN570=temp;
//	}
//	else if(type==n_660)
//	{
//		lastN660=temp;
//	}
//	else if(type==n_800)
//	{
//		lastN800=temp;
//	}
//	

	return temp;
	
}

//多次平均
u16 averageRead(LIGHT_E type)
{
	return Ad_samping(type);
    int data = 0;
    switch(type)
    {
        case n_570:
        {		
			#ifdef GET_AD_BY_MID_DATA 
				data = get_ad_by_mid_data(YELLOW_CHANNEL,averageArray,AVERAGE_READ_CNT);
			#else
				data = get_ad_data(YELLOW_CHANNEL,averageArray,AVERAGE_READ_CNT);
			#endif
            break;
        }
        case n_405:
        {
			#ifdef GET_AD_BY_MID_DATA 
				data = get_ad_by_mid_data(PURPLE_CHANNEL,averageArray,AVERAGE_READ_CNT);
			#else
				data = get_ad_data(PURPLE_CHANNEL,averageArray,AVERAGE_READ_CNT);
			#endif
            break;
        }
        case n_660:
        {		
			#ifdef GET_AD_BY_MID_DATA 
				data = get_ad_by_mid_data(YELLOW_CHANNEL,averageArray,AVERAGE_READ_CNT);
			#else
				data = get_ad_data(n_660,averageArray,AVERAGE_READ_CNT);
			#endif
            break;
        }
        case n_800:
        {		
			#ifdef GET_AD_BY_MID_DATA 
				data = get_ad_by_mid_data(YELLOW_CHANNEL,averageArray,AVERAGE_READ_CNT);
			#else
				data = get_ad_data(n_800,averageArray,AVERAGE_READ_CNT);
			#endif
            break;
        }
        default:
			break;
    }	
    return data;    
}
