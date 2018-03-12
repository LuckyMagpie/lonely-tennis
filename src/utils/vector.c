#include <stdlib.h>
#include <stdarg.h>

#include "vector.h"

vector_t* vector_init()
{
    vector_t* vector = malloc(sizeof(vector_t));
    vector->items = malloc(sizeof(void*) * DEFAULT_MAX_SIZE);
    vector->max_size = DEFAULT_MAX_SIZE;
    vector->size = 0;

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
        vector_resize(vector, vector->max_size * 2);
    }

    unsigned int i = vector->size;
    (*vector->items)[i] = item;
    vector->size++;
}

void* vector_get(vector_t* vector, unsigned int index)
{
    return (*vector->items)[index];
}

void vector_foreach(vector_t* vector, void (*fp) (void*, ...), ...)
{
    va_list ap;
    va_start(ap, fp);

    for (void** p = *(vector->items); p < *(vector->items) + vector->size; p++) {
        fp(*p, ap);
    }

    va_end(ap);
}

void vector_trim(vector_t* vector)
{
    vector_resize(vector, vector->size);
}

void vector_generic_item_free(void* obj, ...)
{
    free(obj);
}

void vector_free(vector_t* vector) {
    free(vector->items);
    free(vector);
}