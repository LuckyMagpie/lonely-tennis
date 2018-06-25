#include <cglm/cglm.h>

#include "engine/executor.h"
#include "engine/graphics.h"
#include "engine/world.h"
#include "engine/graphics.h"
#include "game.h"
#include "utils/vector.h"

int main()
{
    graphics_t* graphics = game_graphics_init();
    world_t* world = game_world_init();
    game_callbacks_init();
    executor_set_current_state(GAME_START);

    executor_run(world, graphics);

    world_free(world);
    graphics_free(graphics);

    return 0;
}
