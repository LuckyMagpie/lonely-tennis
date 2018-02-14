#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "graphics.h"

void set_opengl_attrs()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    SDL_GL_SetSwapInterval(1);
    glewExperimental = GL_TRUE;
    glewInit();
}

graphics_t* graphics_init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
       fprintf(stderr, "Unable to initialize SDL: %s", SDL_GetError()) ;
       return NULL;
    }

    SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_OPENGL);

    if (!window) {
       fprintf(stderr, "Unable to create window: %s", SDL_GetError()) ;
       return NULL;
    }

    SDL_GLContext* context = SDL_GL_CreateContext(window);

    graphics_t* graphics = malloc(sizeof(graphics_t));
    graphics->window = window;
    graphics->context = context;

    return graphics;
}
