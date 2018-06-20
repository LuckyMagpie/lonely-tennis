#include <stdarg.h>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

void render_texture_init(world_object_t* wobj, char* texture_path)
{
    SDL_Surface* texture = IMG_Load(texture_path);

    if (!texture) {
        fprintf(stderr, "%s:Unable to load texture, using default: %s\n", __func__, SDL_GetError());
        texture = IMG_Load(DEFAULT_TEXTURE);
    }

    glGenTextures(1, &wobj->texture_id);
    glBindTexture(GL_TEXTURE_2D, wobj->texture_id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->w, texture->h, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    SDL_FreeSurface(texture);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void render_vertex_objects_free(world_object_t* wobj)
{
    glDeleteBuffers(3, wobj->vbos);
    glDeleteVertexArrays(1, &wobj->vao);
}

void render_texture_free(world_object_t* wobj)
{
    glDeleteTextures(1, &wobj->texture_id);
}

void render_generic_object_draw(world_object_t* wobj, graphics_t* graphics)
{
    glBindVertexArray(wobj->vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wobj->texture_id);
    glUniform1i(graphics->program_texture_sampler_id, GL_TEXTURE0);

    mat4 MVP;
    glm_mat4_mul(graphics->vp, wobj->model_matrix, MVP);
    glUniformMatrix4fv(graphics->program_mvp_id, 1, GL_FALSE, MVP[0]);

    glDrawArrays(GL_TRIANGLES, 0, wobj->vertices->size / 3);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
}
