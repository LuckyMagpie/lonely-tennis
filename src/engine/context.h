#ifndef CONTEXT_H
#define CONTEXT_H

#include "utils/enum_hashtable.h"
#include "graphics.h"
#include "world.h"

typedef enum {
    IN_START_MENU,
    GAME_START,
    IN_GAME,
    PAUSE,
    GAME_OVER,
    QUIT
} state_t;

typedef struct context_t {
    world_t* world;
    graphics_t* graphics;
    enum_hashtable_t* event_store;
    enum_hashtable_t* state_callback_store;
    state_t current_state;
} context_t;

typedef struct state_callback_t {
    void (*callback_fn)(context_t*);
} state_callback_t;

context_t* context_init(world_t* world, graphics_t* graphics);
void context_set_state_callback(context_t* ctx, state_t state, void (*callback)(context_t*));
void context_run(context_t* ctx);
void context_free(context_t* cxt);

#endif /* ifndef CONTEXT_H */
