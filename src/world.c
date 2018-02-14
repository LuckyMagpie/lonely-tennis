#include <stdio.h>
#include <stdlib.h>

#include "world.h"

#define STARTING_NUMBER_OF_WALLS 5
#define NUMBER_OF_BALLS 1

world_t* world_init()
{
    world_t* world = malloc(sizeof(world_t) + sizeof(void*) * (STARTING_NUMBER_OF_WALLS + NUMBER_OF_BALLS));
    world->state = WORLD_IN_START_MENU;

    return world;
}

