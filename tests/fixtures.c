#include <stdbool.h>

#include <cglm/cglm.h>

#include "engine/colission.h"
#include "engine/graphics.h"
#include "engine/render.h"
#include "engine/world.h"
#include "fixtures.h"
#include "utils/mesh.h"
#include "utils/vector.h"

void make_flat_square(dumb_opengl_vector_t* vertices)
{
    vertices->items[0] = 1.0f;
    vertices->items[1] = -1.0f;
    vertices->items[2] = 0.0f;
    vertices->items[3] = -1.0f;
    vertices->items[4] = -1.0f;
    vertices->items[5] = 0.0f;
    vertices->items[6] = -1.0f;
    vertices->items[7] = 1.0f;
    vertices->items[8] = 0.0f;
    vertices->items[9] = 1.0f;
    vertices->items[10] = 1.0f;
    vertices->items[11] = 0.0f;
}

bounding_volume_t* make_bounding_sphere(vec3 scale, vec3 translate, vec3 rotate_axis, float rotate_angle)
{
    dumb_opengl_vector_t* vertices = dumb_opengl_vector_init(12);
    mat4 model_matrix = GLM_MAT4_IDENTITY_INIT;

    make_flat_square(vertices);
    glm_translate(model_matrix, translate);
    glm_rotate(model_matrix, glm_rad(rotate_angle), rotate_axis);
    glm_scale(model_matrix, scale);

    bounding_volume_t* bounding_volume = colission_init_bounding_sphere(vertices, model_matrix, scale);

    free(vertices);
    return bounding_volume;
}

bounding_volume_t* make_bounding_obb(vec3 scale, vec3 translate, vec3 rotate_axis, float rotate_angle)
{
    dumb_opengl_vector_t* vertices = dumb_opengl_vector_init(12);
    mat4 model_matrix = GLM_MAT4_IDENTITY_INIT;

    make_flat_square(vertices);
    glm_translate(model_matrix, translate);
    glm_rotate(model_matrix, glm_rad(rotate_angle), rotate_axis);
    glm_scale(model_matrix, scale);

    bounding_volume_t* bounding_volume = colission_init_bounding_obb(vertices, model_matrix, scale, rotate_axis, rotate_angle);

    free(vertices);
    return bounding_volume;
}

world_object_t* world_object_test_init(vec3 scale, float rotate_angle, vec3 rotate_axis, vec3 translate, vec3 velocity)
{
    world_object_t* wobj = malloc(sizeof(world_object_t));

    bool status = load_obj("mock_assets/wall.obj", &wobj->vertices, &wobj->uvs, &wobj->normals);

    if (!status) {
        fprintf(stderr, "%s:Unable to load mock_assets/wall.obj", __func__);
        free(wobj);
    }

    render_vertex_objects_init(wobj);
    render_texture_init(wobj, "mock_assets/wall_texture.png");

    glm_vec_copy(scale, wobj->scale);
    glm_vec_copy(rotate_axis, wobj->rotate_axis);
    wobj->rotate_angle = rotate_angle;
    glm_vec_copy(translate, wobj->translate);
    glm_vec_copy(velocity, wobj->velocity);

    world_object_update_model_matrix(wobj);

    wobj->bounding_volume = colission_init_bounding_sphere(wobj->vertices, wobj->model_matrix, wobj->scale);

    wobj->fn_render = &render_generic_object_draw;
    wobj->forces = vector_init();
    wobj->fn_simulate = NULL;
    wobj->fn_resolve_colission = NULL;

    return wobj;
}

graphics_t* graphics_test_init()
{
    char* window_title = "Test";
    int window_width = 10;
    int window_height = 10;
    vec3 camera_position = { 0.0f, 0.0f, 2.0f };
    vec3 camera_target = { 0.0f, 0.0f, 0.0f };
    char* vertex_filepath = "mock_assets/vertex_shader.glsl";
    char* fragment_filepath = "mock_assets/fragment_shader.glsl";

    return graphics_init(window_title, window_width, window_height, camera_position, camera_target, vertex_filepath, fragment_filepath);
}
