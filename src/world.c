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

void world_object_update_model_matrix(world_object_t* wobj)
{
    mat4 model_matrix = GLM_MAT4_IDENTITY_INIT;

    glm_translate(model_matrix, wobj->translate);
    glm_rotate(model_matrix, glm_rad(wobj->rotate_angle), wobj->rotate_axis);
    glm_scale(model_matrix, wobj->scale);

    glm_mat4_copy(model_matrix, wobj->model_matrix);
}

void world_object_add_force(world_object_t* wobj, vec3 force)
{
    float* f = malloc(sizeof(vec3));

    glm_vec_copy(force, f);

    vector_push_back(wobj->forces, f);
}

void world_object_add_gravity(world_object_t* wobj)
{
    vec3 gravity = {0.0f, -9.8f, 0.0f};
    world_object_add_force(wobj, gravity);
}

void world_object_apply_force(void* object, va_list ap)
{
    force_t* force = (force_t*)object;
    world_object_t* ball = va_arg(ap, world_object_t*);
    double delta_time = va_arg(ap, double);

    vec3 scaled_translate;
    glm_vec_scale(force->translate, (float)delta_time, scaled_translate);

    glm_vec_add(ball->translate, scaled_translate, ball->translate);

    free(force);
}

void world_object_fn_render_call(void* object, va_list ap)
{
    graphics_t* graphics = va_arg(ap, graphics_t*);
    ((world_object_t*)object)->fn_render(object, graphics);
}

void world_object_fn_simulate_call(void* object, va_list ap)
{
    double delta_time = va_arg(ap, double);
    world_object_t* wobj = (world_object_t*)object;

    if (wobj->fn_simulate != NULL) {
        wobj->fn_simulate(wobj, delta_time);
    }
}

void world_simulate(world_t* world)
{
    vector_foreach(world->world_objects, &world_object_fn_simulate_call, world_current_delta_time(world));
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

    if (wobj->bounding_volume != NULL) {
        free(wobj->bounding_volume);
    }

    render_vertex_objects_free(wobj);
}

void world_free(world_t* world)
{
    vector_foreach(world->world_objects, &world_object_free);
    vector_free(world->world_objects);
    free(world);
}

