#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "enum_hashtable.h"
#include "linked_list.h"

static inline bool enum_hashtable_value_cmp(enum_hashtable_value_t* value, int* to_find)
{
    return value->enum_value == *to_find;
}

enum_hashtable_t* enum_hashtable_init(unsigned int num_buckets, void (*free_enum_value_fn)(enum_hashtable_value_t*))
{
    if (num_buckets > 0) {
        enum_hashtable_t* hashtable = malloc(sizeof(enum_hashtable_t));
        hashtable->num_buckets = num_buckets;
        hashtable->buckets = calloc(num_buckets, sizeof(linked_list_t*));

        if (!free_enum_value_fn) {
            free_enum_value_fn = (void (*)(enum_hashtable_value_t*)) &free;
        }

        hashtable->free_enum_value_fn = free_enum_value_fn;

        return hashtable;
    }

    fprintf(stderr, "%s:Number of buckets need to be >0\n", __func__);
    return NULL;
}

enum_hashtable_value_t* enum_hashtable_get(enum_hashtable_t* hashtable, int enum_value)
{
    unsigned int pos = (unsigned int)enum_value % hashtable->num_buckets;
    linked_list_t* list = (*hashtable->buckets)[pos];

    if (!list) {
        return NULL;
    }

    linked_list_node_t* node = linked_list_node_find(list, &enum_value, (bool (*)(void*, void*))&enum_hashtable_value_cmp);

    return node ? node->data : NULL;
}

void enum_hashtable_put(enum_hashtable_t* hashtable, int enum_value, void* data)
{
    enum_hashtable_value_t* value = malloc(sizeof(enum_hashtable_value_t));
    value->enum_value = enum_value;
    value->data = data;

    unsigned int pos = (unsigned int)enum_value % hashtable->num_buckets;
    linked_list_t** list = &(*hashtable->buckets)[pos];

    if (!*list) {
        *list = linked_list_init();
        linked_list_add_head(*list, value);
        return;
    }

    linked_list_node_delete(*list, &enum_value, (bool (*)(void*, void*))&enum_hashtable_value_cmp, (void (*)(void*))hashtable->free_enum_value_fn);

    linked_list_add_head(*list, value);
}

void enum_hashtable_free(enum_hashtable_t* hashtable)
{
    for (unsigned int i = 0; i < hashtable->num_buckets; i++) {
        if ((*hashtable->buckets)[i]) {
            linked_list_free((*hashtable->buckets)[i], (void (*)(void*))hashtable->free_enum_value_fn);
        }
    }

    free(hashtable->buckets);
    free(hashtable);
}
