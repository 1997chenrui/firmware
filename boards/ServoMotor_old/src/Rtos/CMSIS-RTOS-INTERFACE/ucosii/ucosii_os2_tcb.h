#ifndef _UCOSII_OS2_TCB_H_
#define _UCOSII_OS2_TCB_H_


#include "ucos_ii.h"
#include <stdint.h>

typedef struct ucos_ii_extend_tcb
{
	OS_FLAG_GRP	*task_flag_grp;
	//扩展内容请追加到这里
	
}Ucos_ii_extend_tcb;

#endif 