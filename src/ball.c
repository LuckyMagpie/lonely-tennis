#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <cglm/cglm.h>

#include "ball.h"
#include "engine/colission.h"
#include "engine/context.h"
#include "engine/render.h"
#include "engine/world.h"
#include "utils/mesh.h"
#include "utils/vector.h"

world_object_t* ball_init(vec3 scale, float rotate_angle, vec3 rotate_axis, vec3 translate, vec3 velocity)
{
    world_object_t* ball = malloc(sizeof(world_object_t));

    bool status = load_obj("assets/ball.obj", &ball->vertices, &ball->uvs, &ball->normals);

    if (!status) {
        fprintf(stderr, "%s:Unable to load assets/ball.obj", __func__);
        free(ball);
    }

    render_vertex_objects_init(ball);
    render_texture_init(ball, "assets/ball_texture.png");

    glm_vec_copy(scale, ball->scale);
    glm_vec_copy(rotate_axis, ball->rotate_axis);
    ball->rotate_angle = rotate_angle;
    glm_vec_copy(translate, ball->translate);
    glm_vec_copy(velocity, ball->velocity);

    world_object_update_model_matrix(ball);

    ball->bounding_volume = colission_init_bounding_sphere(ball->vertices, ball->model_matrix, ball->scale);

    ball->fn_render = &render_generic_object_draw;
    ball->forces = vector_init();
    ball->fn_simulate = &ball_simulate;
    ball->fn_resolve_colission = NULL;

    return ball;
}

void ball_click_listener(void* target, SDL_Event event, context_t* ctx)
{
    (void)event;
    ctx->current_state = IN_GAME;
    ((world_object_t*)target)->velocity[1] = 0;
    ((world_object_t*)target)->velocity[2] = 0;
    vec3 push = { 0.0f, 100.0f, -1200.0f };
    world_object_add_force(target, push);
}

void ball_simulate(world_object_t* ball, double delta_time, vector_t* colission_victims)
{
    vector_foreach(colission_victims, &world_object_check_colissions, ball);
    world_object_add_gravity(ball);
    world_object_apply_forces(ball, delta_time);
    world_object_update_model_matrix(ball);
    colission_update_bounding_sphere_center(ball->bounding_volume->data, ball->model_matrix);
}
