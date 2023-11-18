#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "link_list.h"

#define ASSERT_FULL(list) ASSERT(list->size == list->capacity, "list full\r\n")

static struct
{
    volatile int token;
}token_creator;

static bool itf_empty(Link_list_t* list);
static int itf_size(Link_list_t* list);
static int itf_capacity(Link_list_t* list);
static void itf_delete_list(Link_list_t** list);
static Link_node_t* itf_append(Link_list_t* list, void* elem);
static bool itf_delete(Link_list_t* list, Link_node_t** node);
static void itf_clear(Link_list_t* list);
static void itf_delete_when(Link_list_t* list, bool (*reach)(void* elem));
static void itf_list_print(Link_list_t* list, void(*print)(void*));
static int token_create(void);
static bool is_equal(Link_node_t* dest, Link_node_t* src);
static Link_node_t* new_node(void* elem);


Link_list_t* new_list(int capacity){
    Link_list_t* list = (Link_list_t*)malloc(sizeof(Link_list_t));
    list->capacity = capacity;
    list->size = 0;
    list->head = NULL;
    list->itf_empty = itf_empty;
    list->itf_size = itf_size;
    list->itf_capacity = itf_capacity;
    list->itf_delete_list = itf_delete_list;
    list->itf_append = itf_append;
    list->itf_delete = itf_delete;
    list->itf_clear = itf_clear;
    list->itf_delete_when = itf_delete_when;
    list->itf_list_print = itf_list_print;
    return list;
}

/** Link_list_t成员函数 **/
static bool itf_empty(Link_list_t* list){
    return list->size == 0;
}

static int itf_size(Link_list_t* list){
    return list->size;
}

static int itf_capacity(Link_list_t* list){
    return list->capacity;
}

static void itf_delete_list(Link_list_t** list){
    itf_clear(*list);
    free(*list);
    *list = NULL;
}

static Link_node_t* itf_append(Link_list_t* list, void* elem){
    // ASSERT_FULL(list);
    Link_node_t* node=new_node(elem);
    if(itf_empty(list)){
        list->head = node;
    }else{
        Link_node_t* temp = list->head;
        while(temp->next != NULL){
            temp = temp->next;
        }
        temp->next = node;
    }
    list->size++;
    return node;
}

static void itf_head_delete(Link_list_t* list){
    if(list->head == NULL){
        return;
    }
    Link_node_t* temp = list->head;
    if(temp->next == NULL){
        list->head = NULL;
    }else{
        list->head = list->head->next;
    }
    free(temp);
    list->size--;
}

static bool itf_delete(Link_list_t* list, Link_node_t** node){
    if(itf_empty(list)){
        return false;
    }
    Link_node_t* temp = list->head;
    if(is_equal(temp, *node)){
        itf_head_delete(list);
        *node = NULL;
        return true;
    }
    Link_node_t* pre = list->head;
    while (temp->next != NULL){
        temp = temp->next;
        if(is_equal(temp, *node)){
            if(temp->next != NULL){
                pre->next = temp->next;
            }else{
                pre->next = NULL;
            }
            free(temp);
            list->size--;
            *node = NULL;
            return true;
        }else{
            pre = temp;
        }
    }
    return false;
}

static void itf_clear(Link_list_t* list){
    if(itf_empty(list)){
        return;
    }
    Link_node_t* temp = NULL;
    while (list->head->next != NULL){
        temp = list->head;
        free(temp);
        list->head = list->head->next;
    }
    itf_head_delete(list);
    list->size = 0;
}

static void itf_delete_when(Link_list_t* list, bool (*reach)(void* elem)){
    if(itf_empty(list)){
        return;
    }
    Link_node_t* pre = list->head;
    Link_node_t* temp = list->head;
    while (temp->next != NULL){
        temp = temp->next;
        if(reach(temp->elem)){
            free(temp);
            list->size--;
        }else{
            pre->next = temp;
            pre = temp;
        }
    }
    if(pre != NULL){
        pre->next = NULL;
    }
    if(list->head->elem != NULL && reach(list->head->elem)){
        itf_head_delete(list);
    }
}

static void itf_list_print(Link_list_t* list, void(*print)(void*)){
    Link_node_t* temp = list->head;
    while(temp != NULL){
        print(temp->elem);
        temp = temp->next;
    }
    printf("size = %d\r\n---------------\r\n", itf_size(list));
}

/**Link_node_t 成员函数**/
static bool is_equal(Link_node_t* dest, Link_node_t* src){
    return dest != NULL && src != NULL && dest->token == src->token;
}

static Link_node_t* new_node(void* elem){
    Link_node_t* node=(Link_node_t*)malloc(sizeof(Link_node_t));
    node->elem = elem;
    node->token = token_create();
    node->next = NULL;
    return node;
}


/**局部函数**/
static int token_create(){
    return token_creator.token++;
}

#if 0
/**test function**/
static void my_print(void* data){
    int val = *(int *)data;
    printf("%d\r\n", val);
}

static bool when0(void* data){
    int val = *(int *)data;
    return val > 0;
}

static bool when1(void* data){
    int val = *(int *)data;
    return val > 1;
}

static bool when7(void* data){
    int val = *(int *)data;
    return val > 7;
}

int main(int argc, char const *argv[]){
    Link_list_t* list = new_list(20);
    int val[7] = {1,2,3,4,5,6,7};
    Link_node_t* node1 = itf_append(list, &val[0]);
    Link_node_t* node2 = itf_append(list, &val[1]);
    Link_node_t* node3 = itf_append(list, &val[2]);
    Link_node_t* node4 = itf_append(list, &val[3]);
    Link_node_t* node5 = itf_append(list, &val[4]);
    itf_list_print(list, my_print);

    // itf_head_delete(list);
    // itf_head_delete(list);
    // itf_list_print(list, my_print);

    // itf_delete(list, &node1);
    // itf_delete(list, &node1);
    // itf_delete(list, node7);
    // itf_delete(list, node7);
    // itf_list_print(list, my_print);
    // Link_node_t* node6 = append(list, &val[5]);
    // Link_node_t* node7 = append(list, &val[6]);
    // itf_list_print(list, my_print);

    // itf_delete_when(list, when1);
    // itf_delete_when(list, when1);
    // itf_list_print(list, my_print);

    // itf_delete_when(list, when7);
    // itf_delete_when(list, when7);
    // itf_list_print(list, my_print);

    // itf_clear(list);
    // itf_clear(list);
    // itf_list_print(list, my_print);

    // itf_delete_list(&list);
    // printf("list address = %p\r\n", list);

    return 0;
}
#endif
