#include "ad_signal_cfg.h"
#include "stm32f4xx_hal.h"

#define PRESS_CNT 10


extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

static u16 buffer[SIGNAL_CHANNEL_CNT];
static u16 hd_signal_read_ad(u8 idx);
static u32 hd_signal_read_ad_to_press(u8 idx);

static u16 hd_signal_read(u8 idx);
static u32 hd_signal_read_two(u8 idx);
static u16* hd_signal_read_array(u8 idx, u16* rt_len);
static u16* hd_group_read_by(u16 group, u8* rt_len);
static u16* hd_group_read_all(void);
static u16 hd_average_signal_read(u8 idx,u16 average_cnt);
static u16 hd_average_signal_read_old(u8 idx,u16 average_cnt);
static u32 hd_signal_read_ad_to_press_differ(u8 idx);
static u32 hd_signal_read_ad_to_press(u8 idx);
static u16 hd_signal_read_average(u8 idx, u16 average_cnt);
static int press_string[PRESS_CNT];
	
Signal_read_t ad_signal;

void ad_signal_hd_init(){
	ad_signal.hd_signal_read = hd_signal_read;
	ad_signal.hd_signal_read_ad = hd_signal_read_ad;
	ad_signal.hd_signal_read_ad_to_press = hd_signal_read_ad_to_press;
	ad_signal.hd_signal_read_two = hd_signal_read_two;
	ad_signal.hd_signal_read_ad_to_press_differ = hd_signal_read_ad_to_press_differ;
	ad_signal.hd_signal_read_array = hd_signal_read_array;
	ad_signal.hd_group_read_by = hd_group_read_by;
	ad_signal.hd_group_read_all = hd_group_read_all;
	ad_signal.hd_average_signal_read=hd_average_signal_read;
	ad_signal.hd_average_signal_read_old = hd_average_signal_read_old;
	ad_signal.hd_signal_read_average = hd_signal_read_average;
	HAL_ADC_Start_DMA(&hadc1, (u32*)(&buffer[0]), SIGNAL_CHANNEL_CNT);//将传感器接线映射到stm32RAM上（直接读传感器数据）
	
}
static void QuickSort(int *a,int left,int right)
{
    int i,j,temp,tp;
    temp=a[left];                 //暂存基准数
    i=left;                       //最左位置
    j=right;                      //最右位置

    if(left>right)                //递归结束条件
        return;

    while(i!=j)                  //当i和j不重合时
    {
        while(a[j]>=temp && i<j) //从右往左寻找小于基准数的值
            j--;
        while(a[i]<=temp && i<j) //从左往右寻找大于基准数的值
            i++;
        //找到了且i<j则交换数值
        if(i<j)
        {
            tp=a[i];
            a[i]=a[j];
            a[j]=tp;
        }
    }

    //将基准数和i、j的相遇数值进行交换
    a[left]=a[i];
    a[i]=temp;

    //应用递归对此时基准数的左边进行快速排序
    QuickSort(a,left,i-1);
    //应用递归对此时基准数的右边进行快速排序
    QuickSort(a,i+1,right);
}

static u16 average_press(int *press ,int left ,int right)
{
	int sum = 0;
	for(int i=left;i<right;i++)
	{
		sum+=press[i];
	}
	return sum/(right-left);
}

static u16 hd_average_signal_read(u8 idx,u16 average_cnt){
	ASSERT(idx > SIGNAL_CHANNEL_CNT-1, "hd_signal_read idx > 5");
	for(int i = 0 ; i< average_cnt ;i++)
	{
		press_string[i] = hd_signal_read(idx);
	}
	QuickSort(&press_string[0],0,9);
//	for(int i = 0;i< 10; i++)
//	{
//		printf("%d,",press_string[i]);
//	}
//	printf("\r\n");
	return average_press(&press_string[0],3,7);
}

static u16 hd_average_signal_read_old(u8 idx,u16 average_cnt){
	ASSERT(idx > SIGNAL_CHANNEL_CNT-1, "hd_signal_read idx > 5");
	for(int i = 0 ; i< average_cnt ;i++)
	{
		press_string[i] = hd_signal_read_ad_to_press_differ(idx);
	}
	QuickSort(&press_string[0],0,9);
//	for(int i = 0;i< 10; i++)
//	{
//		printf("%d,",press_string[i]);
//	}
//	printf("\r\n");
	return average_press(&press_string[0],3,7);
}

static u16 hd_signal_read(u8 idx){
	ASSERT(idx > SIGNAL_CHANNEL_CNT-1, "hd_signal_read idx > 2");
	return (buffer[idx]);
}

static u16 hd_signal_read_average(u8 idx, u16 average_cnt){
	int press_str[10];
	ASSERT(idx > SIGNAL_CHANNEL_CNT-1, "hd_signal_read idx > 5");
	for(int i = 0 ; i< average_cnt ;i++)
	{
		press_str[i] = hd_signal_read_ad(idx);
	}
	QuickSort(&press_str[0],0,9);
	return average_press(&press_str[0],3,7);
}

static u16 hd_signal_read_ad(u8 idx){
	ASSERT(idx > SIGNAL_CHANNEL_CNT-1, "hd_signal_read_ad idx > 5");
	return (buffer[idx]);
}
/*新气压传感器*/
static u32 hd_signal_read_ad_to_press(u8 idx){
	ASSERT(idx > SIGNAL_CHANNEL_CNT-1, "hd_signal_read_ad_to_press idx > 5");
	return (buffer[idx]*60.1049-24088);
}

///*老气压传感器*/
//static u32 hd_signal_read_ad_to_press_differ(u8 idx){//将AD值转换成压差（Vo/3.3=AD/4096 ,3.3*(0.09*p+0.08)=Vo,P=(100.0*AD/4096-8)/9*1000）
//	ASSERT(idx > SIGNAL_CHANNEL_CNT-1, "hd_signal_read_ad_to_press idx > 5");
//	return ((100.0*buffer[idx]/4096-8)/9*1000);
//}

/*老气压传感器*/
static u32 hd_signal_read_ad_to_press_differ(u8 idx){//将AD值转换成压差（Vo/3.3=AD/4096 ,3.3*(0.09*p+0.08)=Vo,P=(100.0*AD/4096-8)/9*1000）
	ASSERT(idx > SIGNAL_CHANNEL_CNT-1, "hd_signal_read_ad_to_press idx > 5");
	return (63.673*buffer[idx]-23806);
}

static u32 hd_signal_read_two(u8 idx){
	ASSERT(1, "hd_signal_read_two not support");
}

static u16* hd_signal_read_array(u8 idx, u16* rt_len)
{
	ASSERT(1, "hd_signal_read_array not support");
}

static u16* hd_group_read_by(u16 group, u8* rt_len)
{
	ASSERT(1, "hd_group_read_by not support");
}

static u16* hd_group_read_all(void)
{
	ASSERT(1, "hd_group_read_all not support");
}


