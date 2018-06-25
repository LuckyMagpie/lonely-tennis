#ifndef GAME_H
#define GAME_H

#include "engine/graphics.h"
#include "engine/world.h"

void game_callbacks_init(void);
void game_start_callback(world_t* world, graphics_t* graphics);
void game_in_game_callback(world_t* world, graphics_t* graphics);
graphics_t* game_graphics_init(void);
world_t* game_world_init(void);

#endif /* ifndef GAME_H */
