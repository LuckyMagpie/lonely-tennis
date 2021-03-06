#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <cglm/cglm.h>

#include "colission.h"
#include "graphics.h"
#include "utils/vector.h"

#define MIN_DELTA_TIME 0.00002

typedef struct world_object_t {
    mat4 model_matrix;
    vec3 scale;
    vec3 rotate_axis;
    vec3 translate;
    vec3 velocity;
    dumb_opengl_vector_t* vertices;
    dumb_opengl_vector_t* uvs;
    dumb_opengl_vector_t* normals;
    vector_t* forces;
    bounding_volume_t* bounding_volume;
    void (*fn_render)(struct world_object_t*, graphics_t*);
    void (*fn_simulate)(struct world_object_t*, double, vector_t*);
    void (*fn_resolve_colission)(struct world_object_t*, struct world_object_t*);
    GLuint texture_id;
    GLuint vbos[3];
    GLuint vao;
    float rotate_angle;
} world_object_t;

typedef struct world_t {
    vector_t* world_objects;
    Uint64 start_timer;
    float simulation_speed;
    unsigned int score;
} world_t;

world_t* world_init(void);
void world_start_timer(world_t* world);
double world_current_delta_time(world_t* world);
void world_free(world_t* world);
void world_object_free(void* object, va_list _);
void world_object_update_model_matrix(world_object_t* wobj);
void world_object_add_force(world_object_t* wobj, vec3 force);
void world_object_add_gravity(world_object_t* wobj);
void world_object_sum_forces(void* force, va_list ap);
void world_object_apply_forces(world_object_t* wobj, double delta_time);
void world_object_check_colissions(void* victim, va_list ap);
void world_simulate(world_t* world);
void world_object_fn_simulate_call(void* object, va_list ap);
void world_object_fn_render_call(void* object, va_list ap);

#endif /* ifndef WORLD_H */
