#include <stdbool.h>
#include <stdio.h>

#include <cglm/cglm.h>

#include "utils/mesh.h"
#include "render.h"
#include "world.h"
#include "wall.h"

world_object_t* wall_init(vec3 scale, float rotate_angle, vec3 rotate_axis, vec3 translate)
{
    world_object_t* wall = malloc(sizeof(world_object_t));

    bool status = load_obj("assets/wall.obj", &wall->vertices, &wall->uvs, &wall->normals);

    if (!status) {
        fprintf(stderr, "%s:Unable to load assets/wall.obj", __func__);
        free(wall);
    }

    render_vertex_objects_init(wall);

    glm_vec_copy(scale, wall->scale);
    glm_vec_copy(rotate_axis, wall->rotate_axis);
    wall->rotate_angle = rotate_angle;
    glm_vec_copy(translate, wall->translate);

    world_object_update_model_matrix(wall);

    wall->do_render = &render_generic_object_draw;
    wall->do_simulation = NULL;
    wall->forces = NULL;

    return wall;
}
