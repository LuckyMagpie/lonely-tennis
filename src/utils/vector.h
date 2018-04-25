#ifndef VECTOR_H
#define VECTOR_H

#include <stdarg.h>
#include <string.h>

#define DEFAULT_MAX_SIZE 4
#define GROWTH_FACTOR 1.5

typedef struct dumb_opengl_vector_t {
    unsigned int size;
    float items[];
} dumb_opengl_vector_t;

typedef struct vector_t {
    unsigned int size;
    unsigned int max_size;
    void* (*items)[];
} vector_t;

vector_t* vector_init(void);
dumb_opengl_vector_t* dumb_opengl_vector_init(unsigned int size);
static inline void vector_resize(vector_t* vector, unsigned int new_size);
void vector_push_back(vector_t* vector, void* item);
void* vector_get(vector_t* vector, unsigned int index);
void* vector_pop(vector_t* vector);
void vector_pop_loop(vector_t* vector,void (*fp) (void*, va_list ap), ...);
void vector_foreach(vector_t* vector, void (*fp) (void*, va_list ap), ...);
void vector_trim(vector_t* vector);
void vector_generic_item_free(void* obj, va_list _);
void vector_free(vector_t* vector);

#endif /* ifndef VECTOR_H */
