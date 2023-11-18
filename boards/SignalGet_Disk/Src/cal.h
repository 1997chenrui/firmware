#ifndef __CAL__
#define __CAL__

#define HOLE_MAX  36
#define LIGHT_COUNT	4
#define LNODE_BUFF_MAX  HOLE_MAX*LIGHT_COUNT
#define  EPSINON  0.15
#define  Compare_float(x)  (((x) >= - 0.005f) && ((x) <= 0.005f))

typedef float				f32;
typedef struct _DriverInfo
{
	unsigned short type;
	double m_n2y;//1-2光源孔的距离
    double m_n3y;//1-3光源孔的距离
    double m_n4y;//1-4光源孔的距离
	double m_z2s;//豁口到第一个孔的距离
	double m_l2l;//扇区之间的距离
	double m_s2s;//扇区内孔之间的距离
	double m_l2s;//豁口到最后一个孔的距离
	unsigned char m_max_hole;//孔的数量
	unsigned char m_sector_hole;//每个扇区多少个孔
}DriverInfo;

typedef struct
{
	double angle;
	unsigned char type;
	unsigned char hole;
	unsigned long ticks;
}light_time;

typedef struct
{
	unsigned char type;
	unsigned char hole;
}collection_light_ele_t;

typedef struct
{
	// 定时器中断到达后，需要采值的孔信息
	collection_light_ele_t light_ele[LIGHT_COUNT];
	// 需要采值的孔数量
	unsigned char cnt;
	// 到达本次采值的定时器时间
	unsigned long ticks;
}collection_tick_ele_t;

typedef struct _Collection_t
{
	// 定时器采值表
	collection_tick_ele_t tick_ele[LNODE_BUFF_MAX];
	// 定时器采值表长度
	unsigned char cnt;
	// 增加光源 即 到达定时器后需要采值的光源
	void (*light_add)(struct _Collection_t* self, light_time* data);
	// 扩展定时器表
	void (*tick_add)(struct _Collection_t* self, light_time* data);
	collection_tick_ele_t* (*tick_get)(struct _Collection_t* self, int idx);
}Collection_t;

unsigned int off_time(double ztime, f32 time_per_degree,const DriverInfo *info);
void FreeLinkList(void);

#endif


