#include <check.h>

#include "engine/context.h"
#include "utils/enum_hashtable.h"
#include "engine/world.h"
#include "engine/graphics.h"

static int counter = 0;

static graphics_t* graphics_test_init()
{
    char* window_title = "Test";
    int window_width = 1;
    int window_height = 1;
    vec3 camera_position = { 0.0f, 0.0f, 0.0f };
    vec3 camera_target = { 0.0f, 0.0f, 0.0f };
    char* vertex_filepath = "mock_assets/vertex_shader.glsl";
    char* fragment_filepath = "mock_assets/fragment_shader.glsl";

    return graphics_init(window_title, window_width, window_height, camera_position, camera_target, vertex_filepath, fragment_filepath);
}

void in_start_menu_cb(context_t* ctx)
{
    counter++;
    ctx->current_state = GAME_START;
}

void game_start_cb(context_t* ctx)
{
    counter++;
    ctx->current_state = IN_GAME;
}

void in_game_cb(context_t* ctx)
{
    counter++;
    ctx->current_state = PAUSE;
}

void pause_cb(context_t* ctx)
{
    counter++;
    ctx->current_state = GAME_OVER;
}

void game_over_cb(context_t* ctx)
{
    counter++;
    ctx->current_state = QUIT;
}

START_TEST(test_context_init)
{
    world_t* world = world_init();
    graphics_t* graphics =  graphics_test_init();

    context_t* ctx = context_init(world, graphics);

    ck_assert_msg(ctx->current_state == IN_START_MENU, "expected initial current state IN_START_MENU (0) got %d", ctx->current_state);
    ck_assert_msg(ctx->world == world, "expected context world to be the same as the one passed in params");
    ck_assert_msg(ctx->graphics == graphics, "expected context graphics to be the same as the one passed in params");
    ck_assert_msg(ctx->event_store != NULL, "expected event store to be initialized", counter);
    ck_assert_msg(ctx->state_callback_store != NULL, "expected state callback store to be initialized", counter);

    context_free(ctx);
}
END_TEST

static void callback(context_t* ctx)
{
    (void)ctx;

    return;
}

START_TEST(test_context_set_state_callback)
{
    world_t* world = world_init();
    graphics_t* graphics =  graphics_test_init();

    context_t* ctx = context_init(world, graphics);

    context_set_state_callback(ctx, IN_START_MENU, &callback);

    enum_hashtable_value_t* value = enum_hashtable_get(ctx->state_callback_store, IN_START_MENU);

    ck_assert_msg(((state_callback_t*)value->data)->callback_fn == &callback, "expected callback in hashtablte to be the same as the one being set");

    context_free(ctx);
}
END_TEST

START_TEST(test_context_run)
{
    world_t* world = world_init();
    graphics_t* graphics =  graphics_test_init();

    context_t* ctx = context_init(world, graphics);

    context_set_state_callback(ctx, IN_START_MENU, &in_start_menu_cb);
    context_set_state_callback(ctx, GAME_START, &game_start_cb);
    context_set_state_callback(ctx, IN_GAME, &in_game_cb);
    context_set_state_callback(ctx, PAUSE, &pause_cb);
    context_set_state_callback(ctx, GAME_OVER, &game_over_cb);

    context_run(ctx);

    ck_assert_msg(counter == 5, "expected 5 callbacks to be called got %d", counter);

    context_free(ctx);
}
END_TEST

Suite* context_suite()
{
    Suite* suite = suite_create("context");

    TCase* tc1 = tcase_create("test_context_init");
    tcase_add_test(tc1, test_context_init);

    TCase* tc2 = tcase_create("test_context_set_state_callback");
    tcase_add_test(tc2, test_context_set_state_callback);

    TCase* tc3 = tcase_create("test_context_run");
    tcase_add_test(tc3, test_context_run);

    suite_add_tcase(suite, tc1);
    suite_add_tcase(suite, tc2);
    suite_add_tcase(suite, tc3);
    return suite;
}

int main(void)
{
    Suite* suite = context_suite();
    SRunner* sr = srunner_create(suite);
    int nf;

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
