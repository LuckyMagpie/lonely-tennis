#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

vector_t* vector_init()
{
    vector_t* vector = malloc(sizeof(vector_t));
    vector->items = malloc(sizeof(void*) * DEFAULT_MAX_SIZE);
    vector->max_size = DEFAULT_MAX_SIZE;
    vector->size = 0;

    return vector;
}

dumb_opengl_vector_t* dumb_opengl_vector_init(unsigned int size)
{
    dumb_opengl_vector_t* vector = malloc(sizeof(dumb_opengl_vector_t) + sizeof(float) * size);
    vector->size = size;

    return vector;
}

static inline void vector_resize(vector_t* vector, unsigned int new_size)
{
    vector->items = realloc(vector->items, sizeof(void*) * new_size);
    vector->max_size = new_size;
}

void vector_push_back(vector_t* vector, void* item)
{
    if (vector->size >= vector->max_size) {
        vector_resize(vector, vector->max_size * GROWTH_FACTOR);
    }

    unsigned int i = vector->size;
    (*vector->items)[i] = item;
    vector->size++;
}

void* vector_get(vector_t* vector, unsigned int index)
{
    return (*vector->items)[index];
}

void* vector_pop(vector_t* vector)
{
    if (vector->size > 0) {
        unsigned int i = --vector->size;
        return (*vector->items)[i];
    }

    return NULL;
}

void vector_pop_loop(vector_t* vector, void (*fp)(void*, va_list ap), ...)
{
    va_list ap;
    void* obj;
    while ((obj = vector_pop(vector))) {
        va_start(ap, fp);
        fp(obj, ap);
        va_end(ap);
    }
}

void vector_foreach(vector_t* vector, void (*fp)(void*, va_list ap), ...)
{
    va_list ap;

    for (void** p = *(vector->items); p < *(vector->items) + vector->size; p++) {
        va_start(ap, fp);
        fp(*p, ap);
        va_end(ap);
    }
}

void vector_trim(vector_t* vector)
{
    vector_resize(vector, vector->size);
}

void vector_generic_item_free(void* obj, va_list _)
{
    (void)_;
    free(obj);
}

void vector_free(vector_t* vector)
{
    free(vector->items);
    free(vector);
}
