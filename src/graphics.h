#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    SDL_Window* window;
    SDL_GLContext* context;
} graphics_t;

graphics_t* graphics_init();
void graphics_free(graphics_t*);
void set_opengl_attrs(void);
void set_sdl_attrs(void);

#endif /* ifndef GRAPHICS_H */
