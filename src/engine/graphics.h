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
    vec3 camera_position;
    SDL_Window* window;
    SDL_GLContext* context;
    GLuint program_id;
    GLuint program_texture_sampler_id;
    GLuint program_mvp_id;
} graphics_t;

void set_opengl_attrs(void);
void set_sdl_attrs(void);
bool compile_shader(GLuint shader_id, const char* shader_code);
GLuint create_shader_program(const char* vertex_filepath, const char* fragment_filepath);
graphics_t* graphics_init(char* window_title, int window_width, int window_height, vec3 camera_position, vec3 camera_target, char* vertex_filepath, char* fragment_filepath);
void graphics_free(graphics_t* graphics);

#endif /* ifndef GRAPHICS_H */
