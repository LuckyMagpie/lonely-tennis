#ifndef ENUM_HASHTABLE_H
#define ENUM_HASHTABLE_H

#include "linked_list.h"

typedef struct enum_hashtable_value_t {
    void* data;
    int enum_value;
} enum_hashtable_value_t;

typedef struct enum_hashtable_t {
    void (*free_enum_value_fn)(enum_hashtable_value_t*);
    linked_list_t* (*buckets)[];
    unsigned int num_buckets;
} enum_hashtable_t;

enum_hashtable_t* enum_hashtable_init(unsigned int num_buckets, void (*free_enum_value_fn)(enum_hashtable_value_t*));
void enum_hashtable_put(enum_hashtable_t* hashtable, int enum_value, void* data);
enum_hashtable_value_t* enum_hashtable_get(enum_hashtable_t* hashtable, int enum_value);
void enum_hashtable_free(enum_hashtable_t* hashtable);

#endif /* ifndef ENUM_HASHTABLE_H */
