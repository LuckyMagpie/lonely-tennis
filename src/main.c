#include <cglm/cglm.h>

#include "ball.h"
#include "engine/graphics.h"
#include "engine/world.h"
#include "game.h"
#include "utils/vector.h"
#include "wall.h"

int main()
{
    world_t* world = world_init();
    graphics_t* graphics = graphics_init();

    vec3 scale_ball = { 1.0f, 1.0f, 1.0f };
    float rotate_angle_ball = 0.0f;
    vec3 rotate_axis_ball = { 0.0f, 0.0f, 0.0f };
    vec3 translate_ball = { 0.0f, 5.0f, 3.0f };
    vec3 velocity = { 0.0f, 0.0f, 0.0f };

    vec3 scale_wall = { 1.0f, 1.0f, 80.0f };
    float rotate_angle_wall = -20.0f;
    vec3 rotate_axis_wall = { 1.0f, 0.0f, 0.0f };
    vec3 translate_wall = { 0.0f, -1.0f, -1.0f };

    world_object_t* wall = wall_init(scale_wall, rotate_angle_wall, rotate_axis_wall, translate_wall);
    world_object_t* ball = ball_init(scale_ball, rotate_angle_ball, rotate_axis_ball, translate_ball, velocity);

    vector_push_back(world->world_objects, ball);
    vector_push_back(world->world_objects, wall);

    run_loop(world, graphics);
    return 0;
}
