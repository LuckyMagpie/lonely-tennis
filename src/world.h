#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>

#include "ball.h"
#include "wall.h"

typedef enum {
    WORLD_IN_START_MENU,
    WORLD_GAME_START,
    WORLD_IN_FLIGHT,
    WORLD_HIT_CHANCE,
    WORLD_GAME_OVER,
    WORLD_QUIT
} game_states_t ;

typedef enum {
    BALL,
    WALL
} world_object_kind_t;

typedef struct {
    world_object_kind_t kind;
    union {
        ball_t* ball;
        wall_t* wall;
    };
} world_object_t;

typedef struct {
    game_states_t state;
    unsigned int score;
    unsigned int world_objects_size;
    unsigned int world_objects_max_size;
    world_object_t* world_objects[];
} world_t;

world_t* world_init();
void world_free(world_t* world);
bool simulate(world_t* world);
#endif /* ifndef WORLD_H */
