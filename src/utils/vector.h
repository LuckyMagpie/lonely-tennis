#ifndef VECTOR_H
#define VECTOR_H

#define DEFAULT_MAX_SIZE 10

typedef struct vector_t {
    unsigned int size;
    unsigned int max_size;
    void* (*items)[];
} vector_t;

vector_t* vector_init(void);
static inline void vector_resize(vector_t* vector, unsigned int new_size);
void vector_push_back(vector_t* vector, void* item);
void* vector_get(vector_t* vector, unsigned int index);
void vector_foreach(vector_t* vector, void (*fp) (void*, va_list ap), ...);
void vector_trim(vector_t* vector);
void vector_generic_item_free(void* obj, va_list _);
void vector_free(vector_t* vector);

#endif /* ifndef VECTOR_H */
