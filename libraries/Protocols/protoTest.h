#ifndef PROTO_TEST_H_
#define  PROTO_TEST_H_

#include "bus.h"


void ProtoTest_Init(void);
void ProtoTest_Start(void);
void ProtoTestExcute(u32 cmd  ,  bus_packet * packet );



#endif



