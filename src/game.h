#ifndef GAME_H
#define GAME_H

#include "engine/graphics.h"
#include "engine/world.h"

void run_loop(world_t* world, graphics_t* graphics);
graphics_t* game_graphics_init(void);
world_t* game_world_init(void);

#endif /* ifndef GAME_H */
