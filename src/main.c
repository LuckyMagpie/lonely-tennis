#include <cglm/cglm.h>

#include "ball.h"
#include "engine/context.h"
#include "engine/graphics.h"
#include "engine/world.h"
#include "game.h"
#include "utils/vector.h"

int main()
{
    graphics_t* graphics = game_graphics_init();
    world_t* world = game_world_init();
    context_t* ctx = context_init(world, graphics);

    game_callbacks_init(ctx);
    game_world_objects_init(ctx);

    context_run(ctx);

    context_free(ctx);
    return 0;
}
