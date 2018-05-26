#ifndef COLISSION_H
#define COLISSION_H

#include <stdbool.h>

#include <cglm/cglm.h>

typedef enum {
    OBB,
    SPHERE
} bounding_volume_kind_t;

typedef struct bounding_volume_t {
    void* data;
    bounding_volume_kind_t kind;
} bounding_volume_t;

typedef struct bounding_obb_t {
    mat3 rotation_matrix;
    vec3 center;
    vec3 extents;
} bounding_obb_t;

typedef struct bounding_sphere_t {
    vec3 center;
    vec3 local_center;
    float radius;
} bounding_sphere_t;

bounding_volume_t* colission_init_bounding_obb(dumb_opengl_vector_t* vertices, mat4 model_matrix, vec3 scale, vec3 rotate_axis, float rotate_angle);
bounding_volume_t* colission_init_bounding_sphere(dumb_opengl_vector_t* vertices, mat4 model_matrix, vec3 scale);
void colission_update_bounding_sphere_center(bounding_sphere_t* sphere, mat4 model_matrix);
void colission_query_closest_point_obb(bounding_obb_t* obb, vec3 point, vec3 result);
bool colission_test_intersection_sphere_sphere(bounding_sphere_t* sphere, bounding_sphere_t* sphere2);
bool colission_test_intersection_sphere_obb(bounding_sphere_t* sphere, bounding_obb_t* obb);
bool colission_test_intersection_bounding_volume(bounding_volume_t* perp, bounding_volume_t* victim);
void colission_free_bounding_volume(bounding_volume_t* bounding_volume);

static bool (*sphere_jmp_table[])(void*, void*) = {
    (bool (*) (void*, void*))&colission_test_intersection_sphere_obb,
    (bool (*) (void*, void*))&colission_test_intersection_sphere_sphere
};

#endif /* ifndef COLISSION_H */
