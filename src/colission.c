#include <cglm/cglm.h>

#include "utils/mesh.h"
#include "utils/vector.h"
#include "colission.h"

bounding_volume_t* colission_init_bounding_obb(dumb_opengl_vector_t* vertices, mat4 model_matrix, vec3 scale, vec3 rotate_axis, float rotate_angle)
{
    bounding_volume_t* bounding_volume = malloc(sizeof(bounding_volume_t));
    bounding_obb_t* obb = malloc(sizeof(bounding_obb_t));
    vec3 center;
    vec3 extents;

    mesh_find_center(vertices, center);
    mesh_find_extents(vertices, extents);

    vec4 center_vec4;
    glm_vec4(center, 1, center_vec4);
    glm_mat4_mulv(model_matrix, center_vec4, center_vec4);
    glm_vec3(center_vec4, obb->center);

    glm_vec_mul(scale, extents, obb->extents);

    mat4 rotation_matrix = GLM_MAT4_IDENTITY_INIT;
    glm_rotate(rotation_matrix, glm_rad(rotate_angle), rotate_axis);
    glm_mat4_pick3(rotation_matrix, obb->rotation_matrix);

    bounding_volume->kind = OBB;
    bounding_volume->data = obb;

    return bounding_volume;
}

bounding_volume_t* colission_init_bounding_sphere(dumb_opengl_vector_t* vertices, mat4 model_matrix, vec3 scale)
{
    bounding_volume_t* bounding_volume = malloc(sizeof(bounding_volume_t));
    bounding_sphere_t* sphere = malloc(sizeof(bounding_sphere_t));
    vec3 center;
    vec3 extents;

    mesh_find_center(vertices, center);
    mesh_find_extents(vertices, extents);

    vec4 center_vec4;
    glm_vec_copy(center, sphere->local_center);
    glm_vec4(center, 1, center_vec4);
    glm_mat4_mulv(model_matrix, center_vec4, center_vec4);
    glm_vec3(center_vec4, sphere->center);

    glm_vec_mul(scale, extents, extents);
    float radius = 0.0f;
    for (int i = 0; i < 3; i++) {
        radius = (radius < extents[i]) ? extents[i] : radius;
    }
    sphere->radius = radius;

    bounding_volume->kind = SPHERE;
    bounding_volume->data = sphere;

    return bounding_volume;
}

void colission_update_bounding_sphere_center(bounding_sphere_t* sphere, mat4 model_matrix)
{
    vec4 center_vec4;
    glm_vec4(sphere->local_center, 1, center_vec4);
    glm_mat4_mulv(model_matrix, center_vec4, center_vec4);
    glm_vec3(center_vec4, sphere->center);
}

void colission_free_bounding_volume(bounding_volume_t* bounding_volume)
{
    free(bounding_volume->data);
    free(bounding_volume);
}
