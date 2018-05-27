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

world_object_t* ball_init(vec3 scale, float rotate_angle, vec3 rotate_axis, vec3 translate, vec3 velocity)
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
    glm_vec_copy(velocity, ball->velocity);

    world_object_update_model_matrix(ball);

    ball->bounding_volume = colission_init_bounding_sphere(ball->vertices, ball->model_matrix, ball->scale);

    ball->fn_render = &render_generic_object_draw;
    ball->forces = vector_init();
    ball->fn_simulate = &ball_simulate;

    return ball;
}

void ball_simulate(world_object_t* ball, double delta_time, vector_t* colission_victims)
{
    vector_foreach(colission_victims, &world_object_check_colissions, ball);
    world_object_add_gravity(ball);
    world_object_apply_forces(ball, delta_time);
    world_object_update_model_matrix(ball);
    colission_update_bounding_sphere_center(ball->bounding_volume->data, ball->model_matrix);
}

