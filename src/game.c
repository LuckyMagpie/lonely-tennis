#include <SDL2/SDL.h>

#include "engine/graphics.h"
#include "engine/render.h"
#include "engine/world.h"
#include "game.h"

void run_loop(world_t* world, graphics_t* graphics)
{
    world_start_timer(world);
    world->simulation_speed = 600.0f;
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    SDL_Event event;

    while (world->state != WORLD_QUIT) {
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
