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

    glm_vec_copy(scale, ball->scale);
    glm_vec_copy(rotate_axis, ball->rotate_axis);
    ball->rotate_angle = rotate_angle;
    glm_vec_copy(translate, ball->translate);

    world_object_update_model_matrix(ball);

    ball->do_render = &render_generic_object_draw;

    return ball;
}
