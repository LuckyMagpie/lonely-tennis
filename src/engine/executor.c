#include <stdbool.h>
#include <stdio.h>

#include "executor.h"
#include "graphics.h"
#include "world.h"

static state_t current_state = IN_START_MENU;
static void (*state_callbacks[5])(world_t*, graphics_t*) = { NULL };

void executor_set_current_state(state_t state)
{
    current_state = state;
}

void executor_set_callback(state_t state, void (*callback)(world_t*, graphics_t*))
{
    if (state != QUIT) {
        state_callbacks[state] = callback;
    }
}

void executor_run(world_t* world, graphics_t* graphics)
{
    world_start_timer(world);
    bool running = true;

    while (running) {
        if (current_state == QUIT) {
            running = false;
        } else if (state_callbacks[current_state] != NULL) {
            state_callbacks[current_state](world, graphics);
        } else {
            const char* state_name_table[5] = { "IN_START_MENU", "GAME_START", "IN_GAME", "PAUSE", "GAME_OVER" };
            fprintf(stderr, "%s:Undefined state %s\n", __func__, state_name_table[current_state]);
            running = false;
        }

        world_start_timer(world);
    }
}
