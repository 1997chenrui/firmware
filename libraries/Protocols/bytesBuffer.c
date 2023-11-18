#include "bytesBuffer.h"
#include <stdlib.h>

BytesBufferNode *New_BytesBufferNode(u8 *data , int len)
{
    BytesBufferNode * rt = malloc(sizeof(BytesBufferNode));
    rt->next = NULL;
    rt->data = malloc(len);
    memmove(rt->data,data,len); 
    rt->len = len;
    return rt;
}

void BytesBufferNode_Append(BytesBufferNode **head, BytesBufferNode *append)
{
	if ((*head) == NULL)
	{
		(*head) = append;
	}
	else
	{
		BytesBufferNode * temp = (*head);
		while (temp->next != NULL)
		{
			temp = temp->next;
		}

		temp->next = append;
	}

	//append->next = (*head);
	//(*head) = append;
}

int BytesBufferNode_Destroy(BytesBufferNode *head , u8 **buff)
{
	int len = 0;
	for (BytesBufferNode *i = head; i != NULL; i = i->next)
	{
		len += i->len;
	}
	
	u8*out = (*buff) = (u8*)malloc(len);
	int x = 0;
	for (BytesBufferNode *i = head; i != NULL;)
	{
		BytesBufferNode *temp = i;
		i = i->next;
		memmove(out + x, temp->data, temp->len);	//链表里面的数据往外拷贝s
		x += temp->len;
		free(temp->data);
		free(temp);
	}
	return len;
}




