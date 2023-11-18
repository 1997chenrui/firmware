#ifndef __H2L_H
#define __H2L_H

#include "auto_caliPKG.h"
#include "auto_cali_it.h"
#include <math.h>
#include <string.h> 
#include "light_type_e.h"
struct z2f;
struct a2f;
struct L4node_arr;

#define HOLE_NUM            36
#define GIRTH           32768

/*保存孔位脉冲  时间信息*/
typedef struct L4node_arr  L4node_arr;
struct L4node_arr
{
    char state;//判断采集是否就绪
    char light_type;
    unsigned short encoder_data;
};
#endif

