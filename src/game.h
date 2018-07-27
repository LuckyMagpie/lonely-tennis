#ifndef GAME_H
#define GAME_H

#include "engine/context.h"

void game_callbacks_init(context_t* ctx);
void game_start_callback(context_t* ctx);
void game_in_game_callback(context_t* ctx);
void game_world_objects_init(context_t* ctx);
graphics_t* game_graphics_init(void);
world_t* game_world_init(void);

#endif /* ifndef GAME_H */
