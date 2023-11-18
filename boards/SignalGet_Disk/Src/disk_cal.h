#ifndef __DISK_CAL_H
#define __DISK_CAL_H

typedef struct _LNodeData{
	char yellow_idx;
	char purple_idx;
	int off;
	int yellow_offset;//黄光相对孔的偏移
	int puplue_offset;//紫光相对孔的偏移
}DATA_LNode;

typedef struct _Pos
{
	int hole;//孔与豁口的相对位置
	float offset;//孔在扇区内的相对偏移
}PosInfo;

typedef struct _PROCESS
{
	DATA_LNode *Phead;
	char    less;
	char     lenght;
	char    start_idx;
}PROCESS;

typedef struct _DriverInfo  //计算第一个采值孔与光源的位置  所需的参数
{
	double m_n2y;//两个光源孔的距离
	double m_z2s;//豁口到第一个孔的距离
	double m_l2l;//扇区之间的距离
	double m_s2s;//扇区内孔之间的距离
	double m_l2s;//豁口到最后一个孔的距离
	char  m_max_hole;//孔的数量
	char  m_sector_hole;//每个扇区多少个孔
}DriverInfo_t;


PROCESS CreateProceeList(unsigned int angle,DriverInfo_t Info,unsigned char );

#endif

