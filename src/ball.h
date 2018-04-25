#ifndef BALL_H
#define BALL_H

#include <cglm/cglm.h>

#include "world.h"

world_object_t* ball_init(vec3 scale, float rotate_angle, vec3 rotate_axis, vec3 translate);
void ball_do_simulation(world_object_t* ball, double time_delta);
static inline void ball_apply_constant_forces(world_object_t* ball, double time_delta);

#endif /* ifndef BALL_H */
