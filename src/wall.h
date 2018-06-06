#ifndef WALL_H
#define WALL_H

#include <cglm/cglm.h>

#include "engine/world.h"

world_object_t* wall_init(vec3 scale, float rotate_angle, vec3 rotate_axis, vec3 translate);

#endif /* ifndef WALL_H */
