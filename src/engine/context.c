#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <cglm/cglm.h>

#include "context.h"
#include "engine/colission.h"
#include "graphics.h"
#include "utils/enum_hashtable.h"
#include "utils/vector.h"
#include "world.h"

static void context_state_callback_store_value_free(enum_hashtable_value_t* value)
{
    free(value->data);
    free(value);
}

static void context_event_store_value_free(enum_hashtable_value_t* value)
{
    vector_foreach(value->data, &vector_generic_item_free);
    vector_free(value->data);
    free(value);
}

static void context_event_store_fire_event(void* obj, va_list ap)
{
    event_listener_t* listener = obj;
    context_t* ctx = va_arg(ap, context_t*);
    SDL_Event event = va_arg(ap, SDL_Event);
    void* target = va_arg(ap, void*);

    if (target == listener->target) {
        listener->handler_fn(target, event, ctx);
    }
}

static void context_event_store_dispatch(context_t* ctx, SDL_Event event, void* target)
{
    enum_hashtable_value_t* listener_store = enum_hashtable_get(ctx->event_store, event.type);
    if (listener_store) {
        vector_foreach(listener_store->data, &context_event_store_fire_event, ctx, event, target);
    }
}

static void context_find_mouse_target(void* obj, va_list ap)
{
    world_object_t* wobj = obj;
    void** target = va_arg(ap, void**);
    float* camera_position = va_arg(ap, float*);
    float* click_normal = va_arg(ap, float*);

    if (colission_test_raycasting(camera_position, click_normal, wobj->bounding_volume)) {
        if (*target) {
            float current_target_distance = colission_query_distance2_bounding_volume(camera_position, ((world_object_t*)*target)->bounding_volume);
            float new_target_distance = colission_query_distance2_bounding_volume(camera_position, wobj->bounding_volume);
            *target = new_target_distance < current_target_distance? wobj : *target;
        } else {
            *target = wobj;
        }
    }
}

void* context_get_mouse_target(context_t* ctx, int mouse_pos_x, int mouse_pos_y)
{
    int window_width;
    int window_height;
    SDL_GetWindowSize(ctx->graphics->window, &window_width, &window_height);

    vec3 click_vector = { mouse_pos_x, mouse_pos_y, 1.0f };
    vec4 viewport = { 0, window_height, window_width, -window_height };
    vec3 unprojected_click_vector;

    glm_unproject(click_vector, ctx->graphics->vp, viewport, unprojected_click_vector);

    vec3 click_normal;
    glm_vec_sub(unprojected_click_vector, ctx->graphics->camera_position, click_normal);
    glm_vec_normalize(click_normal);

    void* target = NULL;

    vector_foreach(ctx->world->world_objects, &context_find_mouse_target, &target, ctx->graphics->camera_position, click_normal);

    return target;
}

context_t* context_init(world_t* world, graphics_t* graphics)
{
    context_t* ctx = malloc(sizeof(context_t));
    ctx->current_state = IN_START_MENU;
    ctx->world = world;
    ctx->graphics = graphics;
    ctx->event_store = enum_hashtable_init(26, &context_event_store_value_free);
    ctx->state_callback_store = enum_hashtable_init(5, &context_state_callback_store_value_free);

    return ctx;
}

void context_free(context_t* ctx)
{
    world_free(ctx->world);
    graphics_free(ctx->graphics);
    enum_hashtable_free(ctx->event_store);
    enum_hashtable_free(ctx->state_callback_store);

    free(ctx);
}

void context_set_state_callback(context_t* ctx, state_t state, void (*callback)(context_t*))
{
    if (state != QUIT) {
        state_callback_t* state_callback = malloc(sizeof(state_callback_t));
        state_callback->callback_fn = callback;

        enum_hashtable_put(ctx->state_callback_store, state, state_callback);
    }
}

void context_event_store_add_listener(context_t* ctx, SDL_EventType type, void* target, void (*callback_fn)(void* target, SDL_Event event, context_t* ctx))
{
    enum_hashtable_value_t* listener_store = enum_hashtable_get(ctx->event_store, type);
    if (!listener_store) {
        vector_t* store = vector_init();
        enum_hashtable_put(ctx->event_store, type, store);
        listener_store = enum_hashtable_get(ctx->event_store, type);
    }

    event_listener_t* event_listener = malloc(sizeof(event_listener_t));
    event_listener->handler_fn = callback_fn;
    event_listener->target = target;

    vector_push_back(listener_store->data, event_listener);
}

void context_events_listen(context_t* ctx)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:;
            void* mouse_button_target = context_get_mouse_target(ctx, event.button.x, event.button.y);
            context_event_store_dispatch(ctx, event, mouse_button_target);
            break;
        case SDL_MOUSEMOTION:;
            void* mouse_motion_target = context_get_mouse_target(ctx, event.motion.x, event.motion.y);
            context_event_store_dispatch(ctx, event, mouse_motion_target);
            break;
        default:
            context_event_store_dispatch(ctx, event, NULL);
            break;
        }
    }
}

void context_run(context_t* ctx)
{
    state_callback_t* state_callback;
    world_start_timer(ctx->world);
    bool running = true;

    while (running) {
        if (ctx->current_state == QUIT) {
            running = false;
        } else if ((state_callback = enum_hashtable_get(ctx->state_callback_store, ctx->current_state)->data)) {
            state_callback->callback_fn(ctx);
        } else {
            const char* state_name_table[5] = { "IN_START_MENU", "GAME_START", "IN_GAME", "PAUSE", "GAME_OVER" };
            fprintf(stderr, "%s:Undefined state %s\n", __func__, state_name_table[ctx->current_state]);
            running = false;
        }

        world_start_timer(ctx->world);
    }
}
