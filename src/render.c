#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "render.h"

void render_world(world_t* world, graphics_t* graphics)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(graphics->window);
}
