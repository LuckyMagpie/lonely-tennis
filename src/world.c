#include <stdio.h>
#include <stdlib.h>

#include "world.h"

#define STARTING_NUMBER_OF_WALLS 5
#define STARTING_NUMBER_OF_BALLS 1

world_t* world_init()
{
    world_t* world = malloc(sizeof(world_t) + sizeof(world_object_t*) * (STARTING_NUMBER_OF_WALLS + STARTING_NUMBER_OF_BALLS));
    world->state = WORLD_IN_START_MENU;
    world->score = 0;
    world->world_objects_size = 0;
    world->world_objects_max_size = STARTING_NUMBER_OF_WALLS + STARTING_NUMBER_OF_BALLS;

    return world;
}

void world_free(world_t* world) {
    for (world_object_t** p = world->world_objects; p < world->world_objects + world->world_objects_size; p++) {
        free(*p);
    }

    free(world);
}

