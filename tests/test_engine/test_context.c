#include <check.h>

#include "engine/context.h"
#include "engine/graphics.h"
#include "engine/world.h"
#include "fixtures.h"
#include "utils/enum_hashtable.h"
#include "utils/vector.h"

static int counter = 0;

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
    graphics_t* graphics = graphics_test_init();

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
    graphics_t* graphics = graphics_test_init();

    context_t* ctx = context_init(world, graphics);

    context_set_state_callback(ctx, IN_START_MENU, &callback);

    enum_hashtable_value_t* value = enum_hashtable_get(ctx->state_callback_store, IN_START_MENU);

    ck_assert_msg(((state_callback_t*)value->data)->callback_fn == &callback, "expected callback in hashtablte to be the same as the one being set");

    context_free(ctx);
}
END_TEST

START_TEST(test_context_get_mouse_target)
{
    vec3 sphere_scale = { 1.0f, 1.0f, 1.0f };
    float sphere_rotate_angle = 0.0f;
    vec3 sphere_rotate_axis = { 0.0f, 0.0f, 0.0f };
    vec3 sphere_translate = { 0.0f, 0.0f, 0.0f };
    vec3 sphere_velocity = { 0.0f, 0.0f, 0.0f };

    world_object_t* wobj = world_object_test_init(sphere_scale, sphere_rotate_angle, sphere_rotate_axis, sphere_translate, sphere_velocity);

    world_t* world = world_init();
    graphics_t* graphics = graphics_test_init();
    vector_push_back(world->world_objects, wobj);
    context_t* ctx = context_init(world, graphics);

    void* target = context_get_mouse_target(ctx, 5, 5);

    ck_assert_msg(target == wobj, "expected mouse target to be sphere in the middle of the screen");

    context_free(ctx);
}
END_TEST

START_TEST(test_context_get_mouse_target_overlap)
{
    vec3 sphere_scale = { 1.0f, 1.0f, 1.0f };
    vec3 big_sphere_scale = { 10.0f, 10.0f, 10.0f };
    float sphere_rotate_angle = 0.0f;
    vec3 sphere_rotate_axis = { 0.0f, 0.0f, 0.0f };
    vec3 sphere_translate = { 0.0f, 0.0f, 0.0f };
    vec3 sphere_velocity = { 0.0f, 0.0f, 0.0f };

    world_object_t* wobj = world_object_test_init(sphere_scale, sphere_rotate_angle, sphere_rotate_axis, sphere_translate, sphere_velocity);
    world_object_t* wobj2 = world_object_test_init(big_sphere_scale, sphere_rotate_angle, sphere_rotate_axis, sphere_translate, sphere_velocity);

    world_t* world = world_init();
    graphics_t* graphics = graphics_test_init();
    vector_push_back(world->world_objects, wobj);
    vector_push_back(world->world_objects, wobj2);
    context_t* ctx = context_init(world, graphics);

    void* target = context_get_mouse_target(ctx, 5, 5);

    ck_assert_msg(target == wobj2, "expected mouse target to be bigger sphere");

    context_free(ctx);
}
END_TEST

START_TEST(test_context_run)
{
    world_t* world = world_init();
    graphics_t* graphics = graphics_test_init();

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

    TCase* tc4 = tcase_create("test_context_get_mouse_target");
    tcase_add_test(tc4, test_context_get_mouse_target);
    tcase_add_test(tc4, test_context_get_mouse_target_overlap);

    suite_add_tcase(suite, tc1);
    suite_add_tcase(suite, tc2);
    suite_add_tcase(suite, tc3);
    suite_add_tcase(suite, tc4);
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
