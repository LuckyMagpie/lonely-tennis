#include <cglm/cglm.h>

#include "colission.h"
#include "utils/mesh.h"
#include "utils/vector.h"

static bool (*const sphere_jmp_table[])(void*, void*) = {
    (bool (*)(void*, void*)) & colission_test_intersection_sphere_obb,
    (bool (*)(void*, void*)) & colission_test_intersection_sphere_sphere
};

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

bool colission_test_intersection_sphere_obb(bounding_sphere_t* sphere, bounding_obb_t* obb)
{
    vec3 closest_point;
    vec3 distance;
    colission_query_closest_point_obb(obb, sphere->center, closest_point);
    glm_vec_sub(closest_point, sphere->center, distance);

    return glm_vec_dot(distance, distance) <= sphere->radius * sphere->radius;
}

bool colission_test_intersection_sphere_sphere(bounding_sphere_t* sphere, bounding_sphere_t* sphere2)
{
    vec3 distance;
    glm_vec_sub(sphere->center, sphere2->center, distance);

    float squared_distance = glm_vec_dot(distance, distance);
    float radius_sum = sphere->radius + sphere2->radius;

    return squared_distance <= radius_sum * radius_sum;
}

bool colission_test_intersection_ray_sphere(vec3 point, vec3 direction, bounding_sphere_t* sphere)
{
    vec3 distance;
    glm_vec_sub(point, sphere->center, distance);

    //Quadratic Formula
    float c = glm_vec_dot(distance, distance) - sphere->radius * sphere->radius;
    if (c <= 0.0f) {
        return true;
    }

    float b = glm_vec_dot(distance, direction);
    if (b > 0.0f) {
        return false;
    }

    float discriminant = b * b - c;
    if (discriminant < 0.0f) {
        return false;
    }

    return true;
}

bool colission_test_intersection_bounding_volume(bounding_volume_t* perp, bounding_volume_t* victim)
{
    switch (perp->kind) {
    case SPHERE:
        return sphere_jmp_table[victim->kind](perp->data, victim->data);
    case OBB:
        return false; //TODO
    default:
        return false;
    }
}

bool colission_test_raycasting(vec3 point, vec3 direction, bounding_volume_t* victim)
{
    switch (victim->kind) {
    case SPHERE:
        return colission_test_intersection_ray_sphere(point, direction, victim->data);
    case OBB:
        return false; //TODO
    default:
        return false;
    }
}

void colission_query_closest_point_obb(bounding_obb_t* obb, vec3 point, vec3 result)
{
    vec3 distance;
    glm_vec_sub(point, obb->center, distance);
    glm_vec_copy(obb->center, result);

    for (int i = 0; i < 3; i++) {
        float projected_distance = glm_vec_dot(distance, obb->rotation_matrix[i]);

        if (projected_distance > obb->extents[i]) {
            projected_distance = obb->extents[i];
        }

        if (projected_distance < -obb->extents[i]) {
            projected_distance = -obb->extents[i];
        }

        vec3 transformed_distance;
        glm_vec_scale(obb->rotation_matrix[i], projected_distance, transformed_distance);
        glm_vec_add(result, transformed_distance, result);
    }
}

void colission_free_bounding_volume(bounding_volume_t* bounding_volume)
{
    free(bounding_volume->data);
    free(bounding_volume);
}
