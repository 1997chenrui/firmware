#ifndef BYTES_BUFFER_H_
#define BYTES_BUFFER_H_
#include "bus.h"

typedef struct BytesBufferNode
{
	struct BytesBufferNode *next;
	u8  *data;
	int len ;
    
}BytesBufferNode;



BytesBufferNode *New_BytesBufferNode(u8 *data , int len);
void BytesBufferNode_Append(BytesBufferNode **head, BytesBufferNode *append);
int BytesBufferNode_Destroy(BytesBufferNode *head , u8 **buff);



#endif

