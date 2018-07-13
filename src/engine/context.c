#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils/enum_hashtable.h"
#include "context.h"
#include "graphics.h"
#include "world.h"

static void context_store_value_free(enum_hashtable_value_t* value)
{
    free(value->data);
    free(value);
}

context_t* context_init(world_t* world, graphics_t* graphics)
{
    context_t* ctx = malloc(sizeof(context_t));
    ctx->current_state = IN_START_MENU;
    ctx->world = world;
    ctx->graphics = graphics;
    ctx->event_store = enum_hashtable_init(26, &context_store_value_free);
    ctx->state_callback_store = enum_hashtable_init(5, &context_store_value_free);

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
