#include <stdbool.h>

#include <cglm/cglm.h>

#include "utils/mesh.h"
#include "utils/vector.h"
#include "render.h"
#include "world.h"
#include "render.h"
#include "wall.h"

world_object_t* wall_init()
{
    world_object_t* wall = malloc(sizeof(world_object_t));

    bool status = load_obj("assets/wall.obj", &wall->vertices, &wall->uvs, &wall->normals);

    if (!status) {
        fprintf(stderr, "%s:Unable to load assets/wall.obj", __func__);
        free(wall);
    }

    render_vertex_objects_init(wall);
    glm_mat4_copy(GLM_MAT4_IDENTITY, wall->model);
    wall->do_render = &render_generic_object_draw;

    return wall;
}
