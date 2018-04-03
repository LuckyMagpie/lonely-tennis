#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <cglm/cglm.h>

#include "world.h"
#include "render.h"

void render_world(world_t* world, graphics_t* graphics)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(graphics->window);
}

void render_vertex_objects_init(world_object_t* object)
{
    glGenVertexArrays(1, &object->vao);
    glBindVertexArray(object->vao);

    glGenBuffers(3, object->vbos);

    glBindBuffer(GL_ARRAY_BUFFER, object->vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, object->vertices->size * sizeof(float), object->vertices->items, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, object->vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, object->uvs->size * sizeof(float), object->vertices->items, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, object->vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, object->normals->size * sizeof(float), object->vertices->items, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void render_vertex_objects_free(world_object_t* object)
{
    glDeleteBuffers(3, object->vbos);
    glDeleteVertexArrays(1, &object->vao);
}
