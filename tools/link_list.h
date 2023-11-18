#ifndef __LINK_LIST_H__
#define __LINK_LIST_H__   
#include <stdbool.h>
#include "macro.h"

typedef struct _Link_node_t{
    int token;
    void*  elem;
    struct _Link_node_t *next;
}Link_node_t;

typedef struct _Link_list_t{
    int capacity;
    int size;
    Link_node_t* head;
    bool (*itf_empty)(struct _Link_list_t* list);
    int (*itf_size)(struct _Link_list_t* list);
    int (*itf_capacity)(struct _Link_list_t* list);
    void (*itf_delete_list)(struct _Link_list_t** list);
    Link_node_t* (*itf_append)(struct _Link_list_t* list, void* elem);
    bool (*itf_delete)(struct _Link_list_t* list, Link_node_t** node);
    void (*itf_clear)(struct _Link_list_t* list);
    void (*itf_delete_when)(struct _Link_list_t* list, bool (*reach)(void* elem));
    void (*itf_list_print)(struct _Link_list_t* list, void(*print)(void*));
}Link_list_t;

Link_list_t* new_list(int capacity);

#endif
