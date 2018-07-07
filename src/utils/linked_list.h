#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>

typedef struct linked_list_node_t {
    void* data;
    struct linked_list_node_t* next;
} linked_list_node_t;

typedef struct linked_list_t {
    linked_list_node_t* head;
    unsigned int size;
} linked_list_t;

linked_list_t* linked_list_init(void);
void linked_list_add_tail(linked_list_t* list, void* data);
void linked_list_add_head(linked_list_t* list, void* data);
linked_list_node_t* linked_list_node_find(linked_list_t* list, void* data_to_find, bool (*cmp)(void*, void*));
void linked_list_node_delete(linked_list_t* list, void* data_to_find, bool (*cmp)(void*, void*), void (*free_data_fn)(void*));
void linked_list_free(linked_list_t* list, void (*free_data_fn)(void*));

#endif /* ifndef LINKED_LIST_H */
