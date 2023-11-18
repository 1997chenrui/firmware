#include "data_struct.h"
#include <stdio.h>
#include "os_implement.h"

#define  ERROR(x)  { while(1){printf(  (x)  ); printf("ERR in line %d \r\n",__LINE__);}



volatile int DsMallocTest = 0;
void * ds_malloc(int size)
{
	void * p = malloc(size);
    if(p==NULL)
    {
        int cnt = 0;
        while(1)
        {
            if(malloc(8)!=NULL)
            {
                cnt++;
            }
            printf("ds_malloc failed test=%d, size = %d,cnt=%d\r\n",DsMallocTest, size,cnt);
        }
    }
	memset(p, 0, size);
	DsMallocTest++;
	return p;
}

void ds_free(void *p)
{
	DsMallocTest--;
	free(p);
}

int ds_mem_get()
{
	return DsMallocTest;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 左边的全部小于  右边的全部大于


static void BST_Node_construct(BST_struct *ths, BST_Node *node,void *key,void *value)
{
	//BST_Node的构造函数
	node->left = node->right = NULL;
	node->data.key = ds_malloc(ths->key_size);
	node->data.value = ds_malloc(ths->value_size);
	//key 和value 拷贝到节点里面去
	memmove(node->data.key, key, ths->key_size);
	memmove(node->data.value, value, ths->value_size);
}

static void BST_Node_destruct(BST_struct *ths, BST_Node *node)
{
	//BST_Node的析构函数
	ds_free(node->data.key);
	ds_free(node->data.value);
}


void BST_Init(BST_struct *ths, int key_size, int value_size, int(*key_compare)(void *, void *), void(*print_key)(void*), void(*print_value)(void*))
{
	ths->key_size = key_size;
	ths->value_size = value_size;
	ths->key_compare = key_compare;
	ths->print_key = print_key;
	ths->print_value = print_value;
	ths->root = NULL;
}

void BST_Insert(BST_struct *ths, BST_Tree *bst, void *key, void *value)
{
	BST_Tree s;
	if (*bst == NULL)
	{
		s = (BST_Tree)ds_malloc(sizeof(BST_Node));
		BST_Node_construct(ths, s, key, value);
		*bst = s;	
	}
	else
	{
		int res = ths->key_compare(key, (*bst)->data.key);
		if (res < 0)
		{
			BST_Insert(ths, &((*bst)->left), key, value);
		}
		else if ( res >0 )
		{
			BST_Insert(ths, &((*bst)->right), key, value);
		}
		else
		{

		}

	}


}

void BST_Del(BST_struct *ths,  void *key)
{
	BST_Node *p, *f, *s, *q;
	///
	BST_Tree t = ths->root;

	p = t; f = NULL;
	while (p)
	{
		int res = ths->key_compare( p->data.key,key);
		if (res == 0)
		{
			break;
		}
		f = p;
		if (res > 0)
		{
			p = p->left;
		}
		else
		{
			p = p->right;
		}
	}
	if (p == NULL)
	{
		ths->root = t;	//root 就变了
		return ;
	}

	if (p->left == NULL)
	{
		if (f == NULL)
		{
			t = p->right;
		}
		else if (f->left == p)	//p is f's left son
		{
			f->left = p->right;
		}
		else
		{
			f->right = p->right;
		}
		//释放被删除的节点 p
		BST_Node_destruct(ths, p);
		ds_free(p);
	}
	else
	{
		// p have left tree
		q = p; s = p->left;
		while (s->right)
		{
			q = s;
			s = s->right;
		}
		if (q == p)
		{
			q->left = s->left;
		}
		else
		{
			q->right = s->left;
		}
		// s的值赋给 p
		BST_Node_destruct(ths, p);
		p->data = s->data;
		ds_free(s);
	}
	ths->root = t;	//root 就变了
	return;
}

void BST_PrintAll(BST_struct *ths, BST_Tree bst)
{
	if (bst == NULL)
	{
		return;
	}
	else
	{
		BST_PrintAll(ths, bst->left);
		ths->print_key(bst->data.key);
		ths->print_value(bst->data.value);
		BST_PrintAll(ths, bst->right);
	}
}

void BST_Traverse(BST_struct *ths, BST_Tree bst, void(*view_key)(void *ths, void*k), void(*view_value)(void *ths,void*v) , void *obj )
{
	if (bst == NULL)
	{
		return;
	}
	else
	{
		BST_Traverse(ths, bst->left,view_key,view_value,obj);
		view_key(obj,bst->data.key);
		view_value(obj,bst->data.value);
		BST_Traverse(ths, bst->right, view_key, view_value,obj);
	}
}



BST_Tree BST_Search(BST_struct *ths, BST_Tree bst, void *key)
{
	if (!bst)
		return NULL;
	else {
		int res = ths->key_compare(key, bst->data.key);
		if (res < 0)
		{
			return BST_Search(ths, bst->left, key);
		}
		else if (res > 0)
		{
			return BST_Search(ths, bst->right, key);
		}
		else
		{
			return bst;
		}
	}

}

void BST_Destroy(BST_struct *ths, BST_Tree tree)
{
	if (tree == NULL)
		return;
	BST_Destroy(ths, tree->left);
	BST_Destroy(ths, tree->right);
	BST_Node_destruct(ths, tree);
	ds_free(tree);

}


//------------------------------------------------------ 循环链表 --------------------------------------------------------------------------------

//void DLink_Add(DLink_Struct *ths, void *data)
//{
//
//}


//------------------------------------------------------ 循环队列 --------------------------------------------------------------------------------


void SeqQueue_Init(SeqQueue_Struct *ths, int data_size, bool(*pend)(void), bool(*post)(void))
{
	memset(ths, 0, sizeof(SeqQueue_Struct));
	ths->data_size = data_size;
	ths->pend = pend;
	ths->post = post;
	ths->r_idx = 0;
	ths->w_idx = 0;
	ths->ele_size = 0;
}

bool SeqQueue_Enter(SeqQueue_Struct *ths, void *data)
{
	if(ths == NULL || data == NULL)
	{
		return false;
	}
	
	if (ths->ele_size >= SEQ_QUEUE_SIZE)
	{
		printf("SeqQueue_Enter full\r\n");
		return false;
	}
	else
	{
		if(ths->data_size > 0)
		{
			CPU_SR_ALLOC();
			OS_ENTER_CRITICAL();
			void *temp = ds_malloc(ths->data_size);
			if(temp != NULL)
			{
				memmove(temp, data, ths->data_size);
				ths->element[ths->w_idx++] = temp;
				if(ths->w_idx >= SEQ_QUEUE_SIZE)
				{
					ths->w_idx = 0;
				}
				
				ths->ele_size++;
				ths->post();
			}
			
			OS_EXIT_CRITICAL();
			return true;
		}
	}
	
	return false;
}

bool SeqQueue_Delete(SeqQueue_Struct *ths, void *data)
{
	if(ths == NULL || data == NULL)
	{
		return false;
	}
	
	if (ths->ele_size <= 0)
	{
		return false;
	}
	else
	{
        CPU_SR_ALLOC();
        OS_ENTER_CRITICAL();
		memmove(data, ths->element[ths->r_idx], ths->data_size);
		ds_free(ths->element[ths->r_idx++]);
		if(ths->r_idx >= SEQ_QUEUE_SIZE)
		{
			ths->r_idx = 0;
		}
		
		ths->ele_size--;
        OS_EXIT_CRITICAL();
		return true;
	}
}

bool SeqQueue_Waite(SeqQueue_Struct *ths)
{
	return ths->pend();
}

int SeqQueue_GetSize(SeqQueue_Struct *ths)
{
	return ths->ele_size;
}


