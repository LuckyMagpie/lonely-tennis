#include <stdbool.h>
#include <stdlib.h>

#include "linked_list.h"

static inline linked_list_node_t* linked_list_node_init(void* data)
{
    linked_list_node_t* node = malloc(sizeof(linked_list_node_t));
    node->next = NULL;
    node->data = data;

    return node;
}

static bool linked_list_node_data_address_cmp(void* cur_data, void* data_to_find)
{
    return cur_data == data_to_find;
}

linked_list_t* linked_list_init()
{
    linked_list_t* list = malloc(sizeof(linked_list_t));
    list->size = 0;
    list->head = NULL;

    return list;
}

static inline void linked_list_node_free(linked_list_node_t* node, void (*free_data_fn)(void*))
{
    if (free_data_fn) {
        free_data_fn(node->data);
    }

    free(node);
}

void linked_list_add_tail(linked_list_t* list, void* data)
{
    linked_list_node_t* new_node = linked_list_node_init(data);

    if (!list->head) {
        list->head = new_node;
        list->size++;
        return;
    }

    linked_list_node_t* node = list->head;
    for (; node->next; node = node->next) { }
    node->next = new_node;
    list->size++;
}

void linked_list_add_head(linked_list_t* list, void* data)
{
    linked_list_node_t* new_node = linked_list_node_init(data);

    new_node->next = list->head;
    list->head = new_node;
    list->size++;
}

linked_list_node_t* linked_list_node_find(linked_list_t* list, void* data_to_find, bool (*cmp)(void*, void*))
{
    if (!cmp) {
        cmp = &linked_list_node_data_address_cmp;
    }

    for (linked_list_node_t* node = list->head; node; node = node->next) {
        if (cmp(node->data, data_to_find)) {
            return node;
        }
    }

    return NULL;
}

void linked_list_node_delete(linked_list_t* list, void* data_to_find, bool (*cmp)(void*, void*), void (*free_data_fn)(void*))
{
    if (!cmp) {
        cmp = &linked_list_node_data_address_cmp;
    }

    linked_list_node_t* node;
    linked_list_node_t* prev_node;
    for (node = list->head, prev_node = NULL; node; prev_node = node, node = node->next) {
        if (cmp(node->data, data_to_find)) {
            if (node == list->head) {
                list->head = node->next;
                list->size--;
                linked_list_node_free(node, free_data_fn);
                break;
            } else {
                prev_node->next = node->next;
                list->size--;
                linked_list_node_free(node, free_data_fn);
                break;
            }
        }
    }
}

void linked_list_free(linked_list_t* list, void (*free_data_fn)(void*))
{
    linked_list_node_t* buffer;
    for (linked_list_node_t* node = list->head; node;) {
        buffer = node;
        node = node->next;
        linked_list_node_free(buffer, free_data_fn);
    }

    free(list);
}
