#include <cglm/cglm.h>

#include "utils/mesh.h"
#include "colission.h"

void colission_set_bounding_obb(world_object_t* wobj)
{
    bounding_obb_t* obb = malloc(sizeof(bounding_obb_t));
    vec3 center;
    vec3 extents;

    mesh_find_center(wobj->vertices, center);
    mesh_find_extents(wobj->vertices, extents);

    vec4 center_vec4;
    glm_vec4(center, 1, center_vec4);
    glm_mat4_mulv(wobj->model_matrix, center_vec4, center_vec4);
    glm_vec3(center_vec4, obb->center);

    glm_vec_mul(wobj->scale, extents, obb->extents);

    mat4 rotation_matrix = GLM_MAT4_IDENTITY_INIT;
    glm_rotate(rotation_matrix, glm_rad(wobj->rotate_angle), wobj->rotate_axis);
    glm_mat4_pick3(rotation_matrix, obb->rotation_matrix);

    wobj->bounding_volume = obb;
}

void colission_set_bounding_sphere(world_object_t* wobj)
{
    bounding_sphere_t* sphere = malloc(sizeof(bounding_sphere_t));
    vec3 center;
    vec3 extents;

    mesh_find_center(wobj->vertices, center);
    mesh_find_extents(wobj->vertices, extents);

    vec4 center_vec4;
    glm_vec_copy(center, sphere->local_center);
    glm_vec4(center, 1, center_vec4);
    glm_mat4_mulv(wobj->model_matrix, center_vec4, center_vec4);
    glm_vec3(center_vec4, sphere->center);

    glm_vec_mul(wobj->scale, extents, extents);
    float radius = 0.0f;
    for (int i = 0; i < 3; i++) {
        radius = (radius < extents[i]) ? extents[i] : radius;
    }
    sphere->radius = radius;

    wobj->bounding_volume = sphere;
}

void colission_update_bounding_sphere_center(world_object_t* wobj)
{
    bounding_sphere_t* sphere = (bounding_sphere_t*) wobj->bounding_volume;

    vec4 center_vec4;
    glm_vec4(sphere->local_center, 1, center_vec4);
    glm_mat4_mulv(wobj->model_matrix, center_vec4, center_vec4);
    glm_vec3(center_vec4, sphere->center);
}
