#include <stdarg.h>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <cglm/cglm.h>

#include "graphics.h"
#include "world.h"
#include "utils/vector.h"
#include "render.h"

void render_world(world_t* world, graphics_t* graphics)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(graphics->program_id);

    vector_foreach(world->world_objects, &do_render_call, graphics);

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

void render_generic_object_draw(world_object_t* object, graphics_t* graphics)
{
    glBindVertexArray(object->vao);

    mat4 MVP;
    glm_mat4_mul(graphics->vp, object->model, MVP);
    glUniformMatrix4fv(graphics->program_mvp_id, 1, GL_FALSE, MVP[0]);

    glDrawArrays(GL_TRIANGLES, 0, object->vertices->size/3);
}

void do_render_call(void* object, va_list ap)
{
    graphics_t* graphics = va_arg(ap, graphics_t*);
    ((world_object_t*)object)->do_render(object, graphics);
}
