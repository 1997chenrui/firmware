#ifndef __Z2F_H
#define __Z2F_H
#include "hole2lightPKG.h"
#include <stdlib.h>
typedef struct z2f  z2f;
struct z2f
{
    unsigned char type;//光源类型
    unsigned char z2f_num;//第一个采值的孔是几号
    unsigned char max_hole;//最大采值孔个数
    short hole1_angel;
    L4node_arr *encoder_data;
    L4node_arr *time_data;    
};

typedef struct _DriverInfo  //计算第一个采值孔与光源的位置  所需的参数
{
	double m_n2y;//两个光源孔的距离
	double m_z2s;//豁口到第一个孔的距离
	double m_l2l;//扇区之间的距离
	double m_s2s;//扇区内孔之间的距离
    double m_s2s_32;//32孔间距大的两个内孔距离
	double m_l2s;//豁口到最后一个孔的距离
    char m_s2s_32_num;//32孔间距大的内孔个数
	char  m_max_hole;//孔的数量
	char  m_sector_hole;//每个扇区多少个孔
}DriverInfo;

void z2f_Init(z2f *me,L4node_arr *encode,L4node_arr*tim,LIGHT_E type);

z2f cal_z2f_off(z2f *me ,const DriverInfo *info,const unsigned short z2f_t,double off_angel);
void get_hole_data(z2f *me);
unsigned short z2f_off_cal(const DriverInfo *info,const unsigned short z2f_t);
void cali_time(z2f *me);
extern z2f z2f_Info[4];//初始化对应的脉冲数组   记录最靠进光源孔的位置
extern L4node_arr z2f_Info_pulse0[36];
extern L4node_arr z2f_Info_pulse1[36];
extern L4node_arr z2f_Info_pulse2[36];
extern L4node_arr z2f_Info_pulse3[36];

extern L4node_arr z2f_Info_time[36];








#endif

