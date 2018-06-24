#ifndef RENDER_H
#define RENDER_H

#include "graphics.h"
#include "world.h"

#define DEFAULT_TEXTURE "assets/default_texture.png"

void render_world(world_t* world, graphics_t* graphics);
void render_vertex_objects_init(world_object_t* wobj);
void render_texture_init(world_object_t* wobj, char* texture_path);
void render_vertex_objects_free(world_object_t* wobj);
void render_texture_free(world_object_t* wobj);
void render_generic_object_draw(world_object_t* wobj, graphics_t* graphics);

#endif /* ifndef RENDER_H */
