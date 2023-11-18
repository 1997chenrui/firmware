#include "a2f.h"
#include "signal_det.h"



short test_count[24];
void AD_Detect_Thread(void *data)
{
	while(1)
	{
		bus_os_pend_sem(&AD_Detect_Thread_Cfg.sem,0);
        printf("一周完成\r\n");
        for(u8 i=0;i<24;i++)
        {
            printf("light =%d\r\n",ths.light_data[i].encoder_data);
           
        }
        for(u8 i=0;i<24;i++)
        {
            printf("tmp =%d\r\n",test_count[i]);
        }
    }
}


extern TIM_HandleTypeDef htim1;
void a2f_diskFSM(void)
{
    
}























