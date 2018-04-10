#include <stdbool.h>

#include <cglm/cglm.h>

#include "utils/mesh.h"
#include "render.h"
#include "world.h"
#include "ball.h"

world_object_t* ball_init(vec3 scale, float rotate_angle, vec3 rotate_axis, vec3 translate)
{
    world_object_t* ball = malloc(sizeof(world_object_t));

    bool status = load_obj("assets/ball.obj", &ball->vertices, &ball->uvs, &ball->normals);

    if (!status) {
        fprintf(stderr, "%s:Unable to load assets/ball.obj", __func__);
        free(ball);
    }

    render_vertex_objects_init(ball);

    mat4 model_matrix = GLM_MAT4_IDENTITY_INIT;
    glm_scale(model_matrix, scale);
    glm_rotate(model_matrix, glm_rad(rotate_angle), rotate_axis);
    glm_translate(model_matrix, translate);

    glm_mat4_copy(model_matrix, ball->model);

    ball->do_render = &render_generic_object_draw;

    return ball;
}
