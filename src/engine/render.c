#include <stdarg.h>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <cglm/cglm.h>

#include "graphics.h"
#include "render.h"
#include "utils/vector.h"
#include "world.h"

void render_world(world_t* world, graphics_t* graphics)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(graphics->program_id);

    vector_foreach(world->world_objects, &world_object_fn_render_call, graphics);

    glBindVertexArray(0);
    glUseProgram(0);

    SDL_GL_SwapWindow(graphics->window);
}

void render_vertex_objects_init(world_object_t* wobj)
{
    glGenVertexArrays(1, &wobj->vao);
    glBindVertexArray(wobj->vao);

    glGenBuffers(3, wobj->vbos);

    glBindBuffer(GL_ARRAY_BUFFER, wobj->vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, wobj->vertices->size * sizeof(float), wobj->vertices->items, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, wobj->vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, wobj->uvs->size * sizeof(float), wobj->uvs->items, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, wobj->vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, wobj->normals->size * sizeof(float), wobj->normals->items, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void render_vertex_objects_free(world_object_t* wobj)
{
    glDeleteBuffers(3, wobj->vbos);
    glDeleteVertexArrays(1, &wobj->vao);
}

void render_generic_object_draw(world_object_t* wobj, graphics_t* graphics)
{
    glBindVertexArray(wobj->vao);

    mat4 MVP;
    glm_mat4_mul(graphics->vp, wobj->model_matrix, MVP);
    glUniformMatrix4fv(graphics->program_mvp_id, 1, GL_FALSE, MVP[0]);

    glDrawArrays(GL_TRIANGLES, 0, wobj->vertices->size / 3);
}