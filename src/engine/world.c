#include <stdarg.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <cglm/cglm.h>

#include "colission.h"
#include "render.h"
#include "utils/vector.h"
#include "world.h"

world_t* world_init()
{
    world_t* world = malloc(sizeof(world_t));
    world->world_objects = vector_init();
    world->score = 0;
    world->simulation_speed = 1.0f;

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

    if (delta_time > MIN_DELTA_TIME) {
        delta_time = MIN_DELTA_TIME;
    }

    return delta_time * world->simulation_speed;
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
    if (wobj->forces) {
        float* f = malloc(sizeof(vec3));

        glm_vec_copy(force, f);

        vector_push_back(wobj->forces, f);
    }
}

void world_object_add_gravity(world_object_t* wobj)
{
    vec3 gravity = { 0.0f, -9.8f, 0.0f };
    world_object_add_force(wobj, gravity);
}

void world_object_sum_forces(void* force, va_list ap)
{
    vec3* net_force = va_arg(ap, vec3*);
    glm_vec_add(force, *net_force, *net_force);

    free(force);
}

void world_object_apply_forces(world_object_t* wobj, double delta_time)
{
    vec3 net_force = { 0.0f, 0.0f, 0.0f };
    vector_pop_loop(wobj->forces, &world_object_sum_forces, net_force);

    vec3 old_velocity;
    vec3 scaled_net_force;
    glm_vec_scale(net_force, (float)delta_time, scaled_net_force);
    glm_vec_copy(wobj->velocity, old_velocity);

    glm_vec_add(old_velocity, scaled_net_force, wobj->velocity);

    // position = position + (old_velocity + velocity) * 0.5 * dt
    glm_vec_add(old_velocity, wobj->velocity, old_velocity);
    glm_vec_scale(old_velocity, 0.5 * delta_time, old_velocity);
    glm_vec_add(wobj->translate, old_velocity, wobj->translate);
}

void world_object_check_colissions(void* victim_obj, va_list ap)
{
    world_object_t* perp = va_arg(ap, world_object_t*);
    world_object_t* victim = (world_object_t*)victim_obj;

    if (perp == victim) {
        return;
    }

    if (colission_test_intersection_bounding_volume(perp->bounding_volume, victim->bounding_volume)) {
        if (perp->fn_resolve_colission) {
            perp->fn_resolve_colission(perp, victim);
        }

        if (victim->fn_resolve_colission) {
            victim->fn_resolve_colission(victim, perp);
        }
    }
}

void world_object_fn_render_call(void* object, va_list ap)
{
    graphics_t* graphics = va_arg(ap, graphics_t*);
    ((world_object_t*)object)->fn_render(object, graphics);
}

void world_object_fn_simulate_call(void* object, va_list ap)
{
    double delta_time = va_arg(ap, double);
    vector_t* colission_victims = va_arg(ap, vector_t*);
    world_object_t* wobj = (world_object_t*)object;

    if (wobj->fn_simulate) {
        wobj->fn_simulate(wobj, delta_time, colission_victims);
    }
}

void world_simulate(world_t* world)
{
    vector_foreach(world->world_objects, &world_object_fn_simulate_call, world_current_delta_time(world), world->world_objects);
}

void world_object_free(void* object, va_list _)
{
    (void)_;
    world_object_t* wobj = object;

    free(wobj->vertices);
    free(wobj->uvs);
    free(wobj->normals);

    if (wobj->forces) {
        vector_foreach(wobj->forces, &vector_generic_item_free);
        vector_free(wobj->forces);
    }

    if (wobj->bounding_volume) {
        colission_free_bounding_volume(wobj->bounding_volume);
    }

    render_vertex_objects_free(wobj);
    render_texture_free(wobj);
    free(wobj);
}

void world_free(world_t* world)
{
    vector_foreach(world->world_objects, &world_object_free);
    vector_free(world->world_objects);
    free(world);
}
