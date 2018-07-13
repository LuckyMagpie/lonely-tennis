#include <SDL2/SDL.h>
#include <cglm/cglm.h>

#include "ball.h"
#include "engine/context.h"
#include "engine/graphics.h"
#include "engine/render.h"
#include "engine/world.h"
#include "game.h"
#include "wall.h"

void game_callbacks_init(context_t* ctx)
{
    SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
    ctx->current_state = GAME_START;
    context_set_state_callback(ctx, GAME_START, &game_start_callback);
    context_set_state_callback(ctx, IN_GAME, &game_in_game_callback);
}

void game_start_callback(context_t* ctx)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                ctx->current_state = IN_GAME;
            }
            break;
        case SDL_QUIT:
            ctx->current_state = QUIT;
            break;
        }
    }

    render_world(ctx->world, ctx->graphics);
}

void game_in_game_callback(context_t* ctx)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
            }
            break;
        case SDL_QUIT:
            ctx->current_state = QUIT;
            break;
        }
    }

    world_simulate(ctx->world);
    render_world(ctx->world, ctx->graphics);
}

graphics_t* game_graphics_init()
{
    char* window_title = "Lonely Tennis";
    int window_width = 1024;
    int window_height = 768;
    vec3 camera_position = { 0.0f, 1.0f, 5.0f };
    vec3 camera_target = { 0.0f, 0.0f, 0.0f };
    char* vertex_filepath = "assets/vertex_shader.glsl";
    char* fragment_filepath = "assets/fragment_shader.glsl";

    return graphics_init(window_title, window_width, window_height, camera_position, camera_target, vertex_filepath, fragment_filepath);
}

world_t* game_world_init()
{
    world_t* world = world_init();
    world->simulation_speed = 500.0f;

    vec3 scale_ball = { 1.0f, 1.0f, 1.0f };
    float rotate_angle_ball = 0.0f;
    vec3 rotate_axis_ball = { 0.0f, 0.0f, 0.0f };
    vec3 translate_ball = { 0.0f, 1.0f, 0.0f };
    vec3 velocity = { 0.0f, 0.0f, 0.0f };

    vec3 scale_wall = { 3.0f, 1.0f, 50.0f };
    float rotate_angle_wall = 170.0f;
    vec3 rotate_axis_wall = { 1.0f, 0.0f, 0.0f };
    vec3 translate_wall = { 0.0f, 3.35f, 0.0f };

    vec3 scale_wall2 = { 3.0f, 1.0f, 50.0f };
    float rotate_angle_wall2 = -10.0f;
    vec3 rotate_axis_wall2 = { 1.0f, 0.0f, 0.0f };
    vec3 translate_wall2 = { 0.0f, -3.35f, 0.0f };

    vec3 scale_wall3 = { 50.0f, 50.0f, 50.0f };
    float rotate_angle_wall3 = 90.0f;
    vec3 rotate_axis_wall3 = { 1.0f, 0.0f, 0.0f };
    vec3 translate_wall3 = { 0.0f, 0.0f, -50.0f };

    world_object_t* wall = wall_init(scale_wall, rotate_angle_wall, rotate_axis_wall, translate_wall);
    world_object_t* wall2 = wall_init(scale_wall2, rotate_angle_wall2, rotate_axis_wall2, translate_wall2);
    world_object_t* wall3 = wall_init(scale_wall3, rotate_angle_wall3, rotate_axis_wall3, translate_wall3);
    world_object_t* ball = ball_init(scale_ball, rotate_angle_ball, rotate_axis_ball, translate_ball, velocity);

    vector_push_back(world->world_objects, ball);
    vector_push_back(world->world_objects, wall);
    vector_push_back(world->world_objects, wall2);
    vector_push_back(world->world_objects, wall3);

    return world;
}
