#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <cglm/cglm.h>

#include "utils/vector.h"
#include "graphics.h"

typedef enum {
    WORLD_IN_START_MENU,
    WORLD_GAME_START,
    WORLD_IN_FLIGHT,
    WORLD_HIT_CHANCE,
    WORLD_GAME_OVER,
    WORLD_QUIT
} game_states_t ;

typedef struct world_object_t {
    mat4 model;
    vector_t* uvs;
    vector_t* normals;
    vector_t* vertices;
    void (*do_render) (struct world_object_t*, graphics_t*);
    GLuint vao;
    GLuint vbos[3];
} world_object_t;

typedef struct {
    game_states_t state;
    unsigned int score;
    unsigned int world_objects_size;
    unsigned int world_objects_max_size;
    world_object_t* world_objects[];
} world_t;

world_t* world_init();
void world_free(world_t* world);
bool simulate(world_t* world);
#endif /* ifndef WORLD_H */
