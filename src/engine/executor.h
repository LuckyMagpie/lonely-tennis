#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "graphics.h"
#include "world.h"

typedef enum {
    IN_START_MENU,
    GAME_START,
    IN_GAME,
    PAUSE,
    GAME_OVER,
    QUIT
} state_t;

void executor_set_current_state(state_t state);
void executor_set_callback(state_t state, void (*callback)(world_t*, graphics_t*));
void executor_run(world_t* world, graphics_t* graphics);

#endif /* ifndef EXECUTOR_H */
