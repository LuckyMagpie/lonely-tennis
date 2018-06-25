#include <check.h>

#include "engine/executor.h"
#include "engine/world.h"
#include "engine/graphics.h"

static int counter = 0;

void in_start_menu_cb(world_t* _, graphics_t* __)
{
    (void)_;
    (void)__;
    counter++;
    executor_set_current_state(GAME_START);
}

void game_start_cb(world_t* _, graphics_t* __)
{
    (void)_;
    (void)__;
    counter++;
    executor_set_current_state(IN_GAME);
}

void in_game_cb(world_t* _, graphics_t* __)
{
    (void)_;
    (void)__;
    counter++;
    executor_set_current_state(PAUSE);
}

void pause_cb(world_t* _, graphics_t* __)
{
    (void)_;
    (void)__;
    counter++;
    executor_set_current_state(GAME_OVER);
}

void game_over_cb(world_t* _, graphics_t* __)
{
    (void)_;
    (void)__;
    counter++;
    executor_set_current_state(QUIT);
}

START_TEST(test_executor)
{
    world_t* world = world_init();

    executor_set_callback(IN_START_MENU, &in_start_menu_cb);
    executor_set_callback(GAME_START, &game_start_cb);
    executor_set_callback(IN_GAME, &in_game_cb);
    executor_set_callback(PAUSE, &pause_cb);
    executor_set_callback(GAME_OVER, &game_over_cb);

    executor_run(world, NULL);

    ck_assert_msg(counter == 5, "Expected to call 5 callbacks got %d", counter);

    world_free(world);
}
END_TEST


Suite* executor_suite()
{
    Suite* suite = suite_create("executor");

    TCase* tc1 = tcase_create("test_executor");
    tcase_add_test(tc1, test_executor);

    suite_add_tcase(suite, tc1);
    return suite;
}

int main(void)
{
    Suite* suite = executor_suite();
    SRunner* sr = srunner_create(suite);
    int nf;

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
