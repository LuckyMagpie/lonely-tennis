#ifndef BALL_H
#define BALL_H

#include <cglm/cglm.h>

#include "world.h"

world_object_t* ball_init(vec3 scale, float rotate_angle, vec3 rotate_axis, vec3 translate);
void ball_simulate(world_object_t* ball, double delta_time);

#endif /* ifndef BALL_H */
