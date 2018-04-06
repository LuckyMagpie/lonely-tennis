#include <cglm/cglm.h>

#include "wall.h"
#include "world.h"
#include "graphics.h"
#include "game.h"
#include "utils/vector.h"

int main()
{
    world_t* world = world_init();
    graphics_t* graphics = graphics_init();


    vec3 scale = {2.0f, 5.0f, 2.0f};
    float rotate_angle = 0.0f;
    vec3 rotate_axis = {1.0f, 0.0f, 0.0f};
    vec3 translate = {0.0f, 0.0f, 0.0f};

    world_object_t* wall = wall_init(scale, rotate_angle, rotate_axis, translate);
    vector_push_back(world->world_objects, wall);

    run_loop(world, graphics);
    return 0;
}
