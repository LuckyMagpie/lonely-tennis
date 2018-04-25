#include <stdlib.h>
#include <stdarg.h>

#include <cglm/cglm.h>
#include <SDL2/SDL.h>

#include "render.h"
#include "utils/vector.h"
#include "world.h"

world_t* world_init()
{
    world_t* world = malloc(sizeof(world_t));
    world->world_objects = vector_init();
    world->state = WORLD_IN_START_MENU;
    world->score = 0;

    return world;
}

void world_start_timer(world_t* world)
{
    world->start_timer = SDL_GetPerformanceCounter();
}

double world_current_delta_time(world_t* world)
{
    double delta_time;
    Uint64 now = SDL_GetPerformanceCounter();

    delta_time = (double)(now - world->start_timer) / SDL_GetPerformanceFrequency();

    return delta_time * SIMULATION_SPEED;
}

void world_object_update_model_matrix(world_object_t* world_object)
{
    mat4 model_matrix = GLM_MAT4_IDENTITY_INIT;

    glm_translate(model_matrix, world_object->translate);
    glm_rotate(model_matrix, glm_rad(world_object->rotate_angle), world_object->rotate_axis);
    glm_scale(model_matrix, world_object->scale);

    glm_mat4_copy(model_matrix, world_object->model_matrix);
}

void world_object_apply_force(void* object, va_list ap)
{
    force_t* force = (force_t*)object;
    world_object_t* ball = va_arg(ap, world_object_t*);
    double time_delta = va_arg(ap, double);

    vec3 scaled_translate;
    glm_vec_scale(force->translate, (float)time_delta, scaled_translate);

    glm_vec_add(ball->translate, scaled_translate, ball->translate);

    free(force);
}

void do_simulate_call(void* object, va_list ap)
{
    double delta_time = va_arg(ap, double);
    world_object_t* wobj = (world_object_t*)object;

    if (wobj->do_simulation != NULL) {
        wobj->do_simulation(wobj, delta_time);
    }
}

void world_simulate(world_t* world)
{
    vector_foreach(world->world_objects, &do_simulate_call, world_current_delta_time(world));
}

void world_object_free(void* object, va_list _)
{
    (void)_;
    world_object_t* wobj = object;

    free(wobj->vertices);
    free(wobj->uvs);
    free(wobj->normals);

    if (wobj->forces != NULL) {
        vector_foreach(wobj->forces, &vector_generic_item_free);
        vector_free(wobj->forces);
    }

    render_vertex_objects_free(wobj);
}

void world_free(world_t* world)
{
    vector_foreach(world->world_objects, &world_object_free);
    vector_free(world->world_objects);
    free(world);
}

