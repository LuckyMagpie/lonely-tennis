#include <stdio.h>
#include <stdlib.h>

#include "world.h"
#include "utils/vector.h"

world_t* world_init()
{
    world_t* world = malloc(sizeof(world_t));
    world->world_objects = vector_init();
    world->state = WORLD_IN_START_MENU;
    world->score = 0;

    return world;
}

void world_free(world_t* world)
{
    vector_foreach(world->world_objects, &vector_generic_item_free);
    vector_free(world->world_objects);
    free(world);
}

