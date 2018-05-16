#ifndef COLISSION_H
#define COLISSION_H

#include <cglm/cglm.h>

#include "world.h"

typedef struct bounding_obb_t {
    mat3 rotation_matrix;
    vec3 center;
    vec3 extents;
} bounding_obb_t;

typedef struct bounding_sphere_t {
    vec3 center;
    float radius;
} bounding_sphere_t;

void colission_set_bounding_obb(world_object_t* wob);
void colission_set_bounding_sphere(world_object_t* wobj);

#endif /* ifndef COLISSION_H */
