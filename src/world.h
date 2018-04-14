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
    mat4 model_matrix;
    vec3 scale;
    vec3 rotate_axis;
    vec3 translate;
    dumb_opengl_vector_t* vertices;
    dumb_opengl_vector_t* uvs;
    dumb_opengl_vector_t* normals;
    void (*do_render) (struct world_object_t*, graphics_t*);
    GLuint vbos[3];
    GLuint vao;
    float rotate_angle;
} world_object_t;

typedef struct world_t {
    game_states_t state;
    unsigned int score;
    vector_t* world_objects;
} world_t;

world_t* world_init(void);
void world_free(world_t* world);
void world_object_free(void* object, va_list _);
void world_object_update_model_matrix(world_object_t* world_object);
bool simulate(world_t* world);

#endif /* ifndef WORLD_H */
