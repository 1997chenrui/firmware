#include <string.h>
#include "cal.h"
#include <stdio.h>
#include "light_type_e.h"
#include "macro.h"


typedef struct
{
	unsigned char type;
    unsigned char hole;
    double angel;
}hole_angel_list;

typedef struct {
    unsigned char light_type;
    unsigned char first_hole;
    double first_dx;
}hole_info;

light_time light_Table[LNODE_BUFF_MAX];
Collection_t collection_table;

static void collection_light_add(Collection_t* self, light_time* data)
{
	ASSERT(self->cnt == 0, "collection_light_add error empty");
	collection_tick_ele_t* tick = &self->tick_ele[self->cnt-1];
	tick->light_ele[tick->cnt].hole = data->hole;
	tick->light_ele[tick->cnt].type = data->type;
	tick->cnt++;
}

static void collection_tick_add(Collection_t* self, light_time* data)
{
	self->tick_ele[self->cnt].ticks = data->ticks;
	self->cnt++;
	collection_light_add(self, data);
}

static collection_tick_ele_t* collection_tick_get(Collection_t* self, int idx)
{
	ASSERT(idx >= self->cnt || self->cnt == 0, "collection_light_add get error");
	return &self->tick_ele[idx];
}

void FreeLinkList(void) 
{
    memset(light_Table,0,sizeof(light_Table));
	memset(&collection_table,0,sizeof(collection_table));
	collection_table.light_add = collection_light_add;
	collection_table.tick_add = collection_tick_add;
	collection_table.tick_get = collection_tick_get;
}

// 整个盘片根据0号孔的所有偏移 wangzc 重写
static void hole_angle_from_zero(hole_angel_list *list, const DriverInfo *info)
{
	list[0].hole = 0;
	list[0].angel = info->m_z2s;
	for(unsigned char i=1; i<info->m_max_hole; i++)
	{
		list[i].hole = i;
		if(info->m_sector_hole > 0)
		{
			if((i % info->m_sector_hole) == 0 || (info->m_max_hole == 32 && ((i + 1) % info->m_sector_hole) == 0))
			{
				list[i].angel = list[i-1].angel + info->m_l2l;
			}
			else
			{
				if(info->type==28)
				{
					float val = 0.0f;
					switch(i % info->m_sector_hole)
					{
						case 1:val = 14.5;break;
						case 2:
						case 3:
						case 4: val = 13;break;
						case 5: 
						case 6: val = info->m_s2s;break;
						default:while(1){printf("disk param 24\r\n");}
					}
					list[i].angel = list[i-1].angel + val;
				}
				else if(info->type==281)
				{
					float val = 0.0f;
					switch(i % info->m_sector_hole)
					{
						case 1:val = 7.571;break;
						case 2:val = 12.895;break;
						case 3:val = 12.889;break;
						case 4: val = 12.831;break;
						case 5: val = 12.857;break;
						case 6: val = 12.857;break;
						default:while(1){printf("disk param 24\r\n");}
					}
					list[i].angel = list[i-1].angel + val;	
					
				}
				else
					list[i].angel = list[i-1].angel + info->m_s2s;
				
			}
		}
		else
		{
			list[i].angel = list[i-1].angel + info->m_s2s;
		}
	}
	
	
//	printf("hole_angle_from_zero:\r\n");
//	for(unsigned char i=0; i<info->m_max_hole; i++)
//	{
//		printf("    hole=%02d, angle=%lf\r\n", list[i].hole, list[i].angel);
//	}
}

static void get_first_hole(hole_info *phinfo, double angle, hole_angel_list *list, const DriverInfo *info)
{
	double value;
	double zeroangle = angle*0.010986f;
	for(phinfo->first_hole=0; phinfo->first_hole<info->m_max_hole; phinfo->first_hole++)
	{
		if(list[phinfo->first_hole].angel < zeroangle)
		{
			if((zeroangle - list[phinfo->first_hole].angel) < 0.0f)
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
	value = 0.0f;
	if(phinfo->first_hole >= info->m_max_hole)
	{
		value = 360.0f;
		phinfo->first_hole -= info->m_max_hole;
		printf("Turns the overflow.\r\n");
	}
	phinfo->first_dx = value + list[phinfo->first_hole].angel - zeroangle;
//	printf("get_first_hole: zeroangle=%lf, hole=%02d, firstangle=%lf, first_dx=%lf\r\n",\
//	zeroangle, phinfo->first_hole, list[phinfo->first_hole].angel, phinfo->first_dx);
}

// wangzc ��д
static void hole_angle_from_light(hole_info *new_info, light_time *list, const DriverInfo *info)
{
	list[0].hole = new_info->first_hole;
	list[0].angle = new_info->first_dx;
	list[0].type = new_info->light_type;
	for(unsigned char i=1; i<info->m_max_hole; i++)
	{
		list[i].ticks = 0;
		list[i].type = new_info->light_type;
		list[i].hole = (list[0].hole + i) % info->m_max_hole;
		if(info->m_sector_hole > 0)
		{
			if((list[i].hole % info->m_sector_hole) == 0 || (info->m_max_hole == 32 && ((list[i].hole + 1) % info->m_sector_hole) == 0))
			{
				list[i].angle = list[i-1].angle + info->m_l2l;
			}
			else
			{
				if(info->type == 28)
                {
                    float val = 0.0f;
                    switch(list[i].hole % info->m_sector_hole)
                    {
                        case 1:val = 14.5;break;
                        case 2:
                        case 3:
                        case 4: val = 13;break;
                        case 5: 
                        case 6: val = info->m_s2s;break;
                        default:while(1){printf("disk param 24\r\n");}
                    }
                    list[i].angle = list[i-1].angle + val;
                }
				else if(info->type == 281)
                {
                    float val = 0.0f;
                    switch(list[i].hole % info->m_sector_hole)
                    {
                        case 1:val = 7.571;break;
                        case 2:val = 12.895;break;
                        case 3:val = 12.889;break;
                        case 4: val = 12.831;break;
                        case 5: val = 12.857;break;
                        case 6: val = 12.857;break;
                        default:while(1){printf("disk param 24\r\n");}
                    }
                    
					list[i].angle = list[i-1].angle + val;
                }
				else
				{
					list[i].angle = list[i-1].angle + info->m_s2s;
				}
			}
		}
		else
		{
			list[i].angle = list[i-1].angle + info->m_s2s;
		}
		if(i == info->m_max_hole-1)
		{
			//0.15对应220us，而采值时间需要200us，防止最后一次采值后导致错过零位中断
			list[i].angle -= 0.15;
		}
		
	}
	
//	printf("hole_angle_from_light:\r\n");
//	for(unsigned char i=0; i<info->m_max_hole; i++)
//	{
//		printf("    type=%d, hole=%02d, angle=%lf\r\n", new_info->light_type, list[i].hole, list[i].angle);
//	}
}

void listx_sort(light_time *list, char count)
{
	light_time temp;
	for(char i = 0; i < count-1; ++i)
	{
		for(char k = 0; k < count-1-i; ++k)
		{
			if(list[k].angle > list[k+1].angle)
			{
				temp.type = list[k].type;
				temp.hole = list[k].hole;
				temp.ticks = list[k].ticks;
				temp.angle = list[k].angle;
				list[k].type = list[k+1].type;
				list[k].hole = list[k+1].hole;
				list[k].ticks = list[k+1].ticks;
				list[k].angle = list[k+1].angle;
				list[k+1].type = temp.type;
				list[k+1].hole = temp.hole;
				list[k+1].ticks = temp.ticks;
				list[k+1].angle = temp.angle;
			}
		}
	}
}


unsigned int off_time(double angle, f32 time_per_degree,const DriverInfo *info)
{
	unsigned int totaltime;
	hole_angel_list list_off_tmp[info->m_max_hole];
	//hole_info slight[LIGHT_COUNT] = {{MPURPLE, 0, 0.0f}, {MYELLOW, 0, 0.0f}, {third, 0, 0.0f}, {four, 0, 0.0f}};
	hole_info slight[LIGHT_COUNT] = {{n_800, 0, 0.0f}, {n_660, 0, 0.0f}, {n_405, 0, 0.0f}, {n_570, 0, 0.0f}};
	
    hole_angle_from_zero(list_off_tmp, info);//������ſ׽���ƫ��
	
	get_first_hole(&slight[0], angle, list_off_tmp, info);
	get_first_hole(&slight[1], angle - 91.022222f * info->m_n2y, list_off_tmp, info);
	get_first_hole(&slight[2], angle - 91.022222f * info->m_n3y, list_off_tmp, info);
	get_first_hole(&slight[3], angle - 91.022222f * info->m_n4y, list_off_tmp, info);
	
	hole_angle_from_light(&slight[0], &light_Table[0*info->m_max_hole], info);
	hole_angle_from_light(&slight[1], &light_Table[1*info->m_max_hole], info);
	hole_angle_from_light(&slight[2], &light_Table[2*info->m_max_hole], info);
	hole_angle_from_light(&slight[3], &light_Table[3*info->m_max_hole], info);
	
	listx_sort(light_Table, info->m_max_hole*LIGHT_COUNT);
	light_Table[0].ticks = light_Table[0].ticks < 50 ? 50 : light_Table[0].ticks;

	angle = 0.0f;
	totaltime = 0u;
	for(unsigned char i=0; i<info->m_max_hole*LIGHT_COUNT; ++i)
	{
		light_Table[i].ticks = 0u;
		if(light_Table[i].angle > angle)
		{
			light_Table[i].ticks = (unsigned long)((light_Table[i].angle - angle) * time_per_degree);
			//light_Table[i].ticks = (unsigned long)((light_Table[i].angle - angle) * 1388.888889f);
			totaltime += light_Table[i].ticks;
			angle = light_Table[i].angle;
		}
		
		//printf("idx=%03d, type=%d, hole=%02d, angle=%lf, ticks=%ld\r\n", i,\
		light_Table[i].type, light_Table[i].hole, light_Table[i].angle, light_Table[i].ticks);
	}
	//unsigned char light_cnt = 0;
	unsigned int time_cnt = 0;
	collection_table.tick_add(&collection_table, &light_Table[0]);
	for(unsigned char i=1; i<info->m_max_hole*LIGHT_COUNT; i++)
	{
		time_cnt += light_Table[i].ticks;
		if(time_cnt < 200)
		{
			collection_table.light_add(&collection_table, &light_Table[i]);
		}else
		{
			time_cnt = 0;
			collection_table.tick_add(&collection_table, &light_Table[i]);
		}
	}
//	printf("----------------------\r\n");
//	u8 idx = 0;
//	for(unsigned char i=0; i<collection_table.cnt; i++)
//	{
//		for(u8 j=0;j<collection_table.tick_ele[i].cnt;j++)
//		{
//			printf("idx=%03d, type=%d, hole=%02d, ticks=%ld\r\n", idx,\
//			collection_table.tick_ele[i].light_ele[j].type, collection_table.tick_ele[i].light_ele[j].hole, collection_table.tick_ele[i].ticks);
//			idx++;
//		}
//	}
//	
//	printf("time map sum: %dus\r\n", totaltime);
	return totaltime;
}


