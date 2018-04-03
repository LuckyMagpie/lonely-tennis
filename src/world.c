#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "render.h"
#include "utils/vector.h"
#include "world.h"

world_t* world_init()
{
    world_t* world = malloc(sizeof(world_t));
    world->world_objects = vector_init();
    world->state = WORLD_IN_START_MENU;
    world->score = 0;

    return world;
}

void world_object_free(void* object, va_list _)
{
    (void)_;
    world_object_t* wobj = object;

    free(wobj->vertices);
    free(wobj->uvs);
    free(wobj->normals);

    render_vertex_objects_free(wobj);
}

void world_free(world_t* world)
{
    vector_foreach(world->world_objects, &world_object_free);
    vector_free(world->world_objects);
    free(world);
}

