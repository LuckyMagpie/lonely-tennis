#include <stdbool.h>
#include <stdio.h>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <cglm/cglm.h>

#include "graphics.h"

void set_sdl_attrs()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void set_opengl_attrs()
{
    SDL_GL_SetSwapInterval(1);
    glClearColor(0.9f, 0.9f, 0.9f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glewExperimental = GL_TRUE;
    glewInit();
}

graphics_t* graphics_init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
       fprintf(stderr, "%s:Unable to initialize SDL: %s\n", __func__, SDL_GetError());
       return NULL;
    }

    set_sdl_attrs();

    SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 1024, 768, SDL_WINDOW_OPENGL);

    if (!window) {
       fprintf(stderr, "%s:Unable to create window: %s\n", __func__, SDL_GetError());
       return NULL;
    }

    SDL_GLContext* context = SDL_GL_CreateContext(window);

    set_opengl_attrs();

    graphics_t* graphics = malloc(sizeof(graphics_t));
    graphics->window = window;
    graphics->context = context;

    vec3 eye = {0.0f, 0.0f, 5.0f};
    vec3 center = {0.0f, 0.0f, 0.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    glm_lookat(eye, center, up, graphics->view);

    glm_perspective(glm_rad(90.0f), 4.0f/3.0f, 0.1f, 100.0f, graphics->projection);

    glm_mat4_mul(graphics->projection, graphics->view, graphics->vp);

    return graphics;
}

void graphics_free(graphics_t* graphics)
{
    SDL_GL_DeleteContext(graphics->context);
    SDL_DestroyWindow(graphics->window);
    SDL_Quit();
    free(graphics);
}
