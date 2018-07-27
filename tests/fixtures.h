#ifndef TESTING_UTILS_H
#define TESTING_UTILS_H

#include <cglm/cglm.h>

#include "engine/colission.h"
#include "engine/graphics.h"
#include "engine/world.h"
#include "utils/vector.h"

void make_flat_square(dumb_opengl_vector_t* vertices);
world_object_t* world_object_test_init(vec3 scale, float rotate_angle, vec3 rotate_axis, vec3 translate, vec3 velocity);
bounding_volume_t* make_bounding_sphere(vec3 scale, vec3 translate, vec3 rotate_axis, float rotate_angle);
bounding_volume_t* make_bounding_obb(vec3 scale, vec3 translate, vec3 rotate_axis, float rotate_angle);
graphics_t* graphics_test_init();

#endif /* ifndef TESTING_UTILS_H */
