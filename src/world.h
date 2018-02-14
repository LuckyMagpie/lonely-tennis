#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>

typedef enum {
    WORLD_IN_START_MENU,
    WORLD_GAME_START,
    WORLD_IN_FLIGHT,
    WORLD_HIT_CHANCE,
    WORLD_GAME_OVER,
    WORLD_QUIT
} game_states_t ;

typedef struct {
    game_states_t state;
    unsigned int score;
    unsigned int objects_size;
    void* objects[];
} world_t;

world_t* world_init();
bool simulate(world_t* world);
#endif /* ifndef WORLD_H */
