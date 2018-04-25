#include <SDL2/SDL.h>

#include "world.h"
#include "graphics.h"
#include "render.h"
#include "game.h"

void run_loop(world_t* world, graphics_t* graphics)
{
    world_start_timer(world);

    while (world->state != WORLD_QUIT) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                world->state = WORLD_QUIT;
            }
        }

        world_simulate(world);
        render_world(world, graphics);
        world_start_timer(world);
    }

    world_free(world);
    graphics_free(graphics);
}
