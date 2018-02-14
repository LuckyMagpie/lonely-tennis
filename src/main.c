#include "world.h"
#include "graphics.h"
#include "game.h"

int main()
{
    world_t* world = world_init();
    graphics_t* graphics = graphics_init();

    run_loop(world, graphics);
    return 0;
}
