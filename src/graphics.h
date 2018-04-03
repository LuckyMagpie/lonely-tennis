#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdbool.h>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <cglm/cglm.h>

typedef struct graphics_t {
    mat4 projection;
    mat4 view;
    mat4 vp;
    SDL_Window* window;
    SDL_GLContext* context;
} graphics_t;

void set_opengl_attrs(void);
void set_sdl_attrs(void);
graphics_t* graphics_init(void);
void graphics_free(graphics_t* graphics);

#endif /* ifndef GRAPHICS_H */
