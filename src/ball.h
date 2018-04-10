#ifndef BALL_H
#define BALL_H

#include <cglm/cglm.h>

#include "world.h"

world_object_t* ball_init(vec3 scale, float rotate_angle, vec3 rotate_axis, vec3 translate);

#endif /* ifndef BALL_H */
