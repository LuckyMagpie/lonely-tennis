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
void vector_foreach(vector_t* vector, void (*fp) (void*, ...), ...);
void vector_generic_item_free(void* obj, ...);
void vector_free(vector_t* vector);

#endif /* ifndef VECTOR_H */
