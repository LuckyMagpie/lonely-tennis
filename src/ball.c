#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include <cglm/cglm.h>

#include "utils/mesh.h"
#include "utils/vector.h"
#include "colission.h"
#include "render.h"
#include "world.h"
#include "ball.h"

static inline void ball_apply_constant_forces(world_object_t* ball, double delta_time)
{
    vec3 gravity = {0.0f, -9.8f, 0.0f};
    glm_vec_scale(gravity, (float)delta_time, gravity);
    glm_vec_add(ball->translate, gravity, ball->translate);
}

world_object_t* ball_init(vec3 scale, float rotate_angle, vec3 rotate_axis, vec3 translate)
{
    world_object_t* ball = malloc(sizeof(world_object_t));

    bool status = load_obj("assets/ball.obj", &ball->vertices, &ball->uvs, &ball->normals);

    if (!status) {
        fprintf(stderr, "%s:Unable to load assets/ball.obj", __func__);
        free(ball);
    }

    render_vertex_objects_init(ball);

    glm_vec_copy(scale, ball->scale);
    glm_vec_copy(rotate_axis, ball->rotate_axis);
    ball->rotate_angle = rotate_angle;
    glm_vec_copy(translate, ball->translate);

    world_object_update_model_matrix(ball);

    colission_set_bounding_sphere(ball);
    ball->fn_render = &render_generic_object_draw;
    ball->forces = vector_init();
    ball->fn_simulate = &ball_simulate;

    return ball;
}

void ball_simulate(world_object_t* ball, double delta_time)
{
    ball_apply_constant_forces(ball, delta_time);
    vector_pop_loop(ball->forces, &world_object_apply_force, ball, delta_time);
    world_object_update_model_matrix(ball);
    colission_update_bounding_sphere_center(ball);
}

