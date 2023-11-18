#ifndef DATA_STRUCT_H_
#define DATA_STRUCT_H_

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


void * ds_malloc(int size);
void ds_free(void *p);
int ds_mem_get(void);



typedef struct _BST_Node{
	struct _BST_Node  *left;
	struct _BST_Node  *right;
	struct
	{
		void              *key;
		void              *value;
	}data;

}BST_Node,  *BST_Tree;

typedef struct
{
	BST_Node *root;
	int       key_size;
	int       value_size;
	int(*key_compare)(void *, void *);
	void(*print_key)(void*);
	void(*print_value)(void *);
}BST_struct;


void BST_Init(BST_struct *ths, int key_size, int value_size, int(*key_compare)(void *, void *), void(*print_key)(void*), void(*print_value)(void*));
void BST_Insert(BST_struct *ths, BST_Tree *bst, void *key, void *value);	//递归函数 所以必须带个 *bst
void BST_Del(BST_struct *ths,  void *key);
BST_Tree BST_Search(BST_struct *ths, BST_Tree bst, void *key);
void BST_PrintAll(BST_struct *ths, BST_Tree bst);
void BST_Traverse(BST_struct *ths, BST_Tree bst, void(*view_key)(void *ths, void*k), void(*view_value)(void *ths, void*v), void *obj);
BST_Tree BST_Search(BST_struct *ths, BST_Tree bst, void *key);
void BST_Destroy(BST_struct *ths, BST_Tree tree);

//------------------------------------------ 双向链表 -----------------------------------------------------------------------------------------------------------


//typedef struct DLinkNode
//{
//	struct DLinkNode *next;
//	struct DLinkNode *prior;
//	void             *data;
//
//}DLinkNode,*DLinkList;
//
//
//
//
//typedef struct
//{
//	DLinkList  list;
//
//
//}DLink_Struct;
//
//
//void DLink_Init(DLink_Struct *ths, int data_size  );
//void DLink_Add(DLink_Struct *ths, void *data);


//------------------------------------------ 队列 ----------------------------------------------------------------------------------------------------------------

#if defined(STM32F103xB)
#define SEQ_QUEUE_SIZE   (2 << 5)
#elif defined(STM32F407xx)
#define SEQ_QUEUE_SIZE   100
#endif

typedef struct 
{
	void *element[SEQ_QUEUE_SIZE];
	
	volatile unsigned short r_idx;
	volatile unsigned short w_idx;

	volatile int data_size;

	bool (*pend)(void);	//专门给阻塞队列用的
	bool (*post)(void);

	volatile int ele_size;
}SeqQueue_Struct;



void SeqQueue_Init(SeqQueue_Struct *ths, int data_size, bool(*pend)(void), bool(*post)(void));
bool SeqQueue_Enter(SeqQueue_Struct *ths, void *data);
bool SeqQueue_Delete(SeqQueue_Struct *ths, void *data);
bool SeqQueue_Waite(SeqQueue_Struct *ths);
int  SeqQueue_GetSize(SeqQueue_Struct *ths);



#endif







