#include <signal.h>
#include <stdbool.h>
#include <stdio.h>

#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <cglm/cglm.h>

#include "graphics.h"
#include "utils/files.h"

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

bool compile_shader(GLuint shader_id, const char* shader_code)
{
    GLint compile_result = GL_FALSE;

    glShaderSource(shader_id, 1, &shader_code, NULL);
    glCompileShader(shader_id);
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_result);

    if (compile_result == GL_FALSE) {
        int log_len;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_len);
        char error_log[log_len + 1];
        glGetShaderInfoLog(shader_id, log_len, NULL, error_log);
        fprintf(stderr, "%s:Unable to compile shader: %s\n", __func__, error_log);
    }

    return compile_result;
}

GLuint create_shader_program(const char* vertex_filepath, const char* fragment_filepath)
{
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    char* vertex_shader_code = file_to_str(vertex_filepath);
    char* fragment_shader_code = file_to_str(fragment_filepath);

    if (vertex_shader_code[0] == '\0' || fragment_shader_code[0] == '\0') {
        raise(SIGABRT);
    }

    bool vertex_shader_status = compile_shader(vertex_shader_id, vertex_shader_code);
    bool fragment_shader_status = compile_shader(fragment_shader_id, fragment_shader_code);

    if (!vertex_shader_status || !fragment_shader_status) {
        raise(SIGABRT);
    }

    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    GLint link_result = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &link_result);
    if (link_result == GL_FALSE) {
        int log_len;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_len);
        char error_log[log_len + 1];
        glGetProgramInfoLog(program_id, log_len, NULL, error_log);
        fprintf(stderr, "%s:Unable to link program: %s\n", __func__, error_log);

        raise(SIGABRT);
    }

    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
    free(vertex_shader_code);
    free(fragment_shader_code);

    return program_id;
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

    vec3 eye = { 0.0f, 0.0f, 5.0f };
    vec3 center = { 0.0f, 0.0f, 0.0f };
    vec3 up = { 0.0f, 1.0f, 0.0f };
    glm_lookat(eye, center, up, graphics->view);

    glm_perspective(glm_rad(90.0f), 4.0f / 3.0f, 0.1f, 100.0f, graphics->projection);

    glm_mat4_mul(graphics->projection, graphics->view, graphics->vp);

    graphics->program_id = create_shader_program("assets/vertex_shader.glsl", "assets/fragment_shader.glsl");
    graphics->program_mvp_id = glGetUniformLocation(graphics->program_id, "MVP");

    return graphics;
}

void graphics_free(graphics_t* graphics)
{
    SDL_GL_DeleteContext(graphics->context);
    SDL_DestroyWindow(graphics->window);
    glDeleteProgram(graphics->program_id);
    SDL_Quit();
    free(graphics);
}
