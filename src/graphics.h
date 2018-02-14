#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    SDL_Window* window;
    SDL_GLContext* context;
} graphics_t;

void set_opengl_attrs();
graphics_t* graphics_init();

#endif /* ifndef GRAPHICS_H */
