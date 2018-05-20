#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <cglm/cglm.h>

#include "utils/vector.h"
#include "graphics.h"

#define SIMULATION_SPEED 100.0

typedef enum {
    WORLD_IN_START_MENU,
    WORLD_GAME_START,
    WORLD_IN_FLIGHT,
    WORLD_HIT_CHANCE,
    WORLD_GAME_OVER,
    WORLD_QUIT
} game_states_t ;

typedef struct force_t {
    vec3 translate;
    vec3 rotate_axis;
    float rotate_angle;
} force_t;

typedef struct world_object_t {
    mat4 model_matrix;
    vec3 scale;
    vec3 rotate_axis;
    vec3 translate;
    dumb_opengl_vector_t* vertices;
    dumb_opengl_vector_t* uvs;
    dumb_opengl_vector_t* normals;
    vector_t* forces;
    void (*fn_render) (struct world_object_t*, graphics_t*);
    void (*fn_simulate) (struct world_object_t*, double delta_time);
    void* bounding_volume;
    GLuint vbos[3];
    GLuint vao;
    float rotate_angle;
} world_object_t;

typedef struct world_t {
    vector_t* world_objects;
    Uint64 start_timer;
    unsigned int score;
    game_states_t state;
} world_t;

world_t* world_init(void);
void world_start_timer(world_t* world);
double world_current_delta_time(world_t* world);
void world_free(world_t* world);
void world_object_free(void* object, va_list _);
void world_object_update_model_matrix(world_object_t* wobj);
void world_object_add_force(world_object_t* wobj, vec3 translate, vec3 rotate_axis, float rotate_angle);
void world_object_apply_force(void* object, va_list ap);
void world_simulate(world_t* world);
void world_object_fn_simulate_call(void* object, va_list ap);
void world_object_fn_render_call(void* object, va_list ap);

#endif /* ifndef WORLD_H */
