#include <unistd.h>

#include <check.h>

#include "world.h"

START_TEST(test_world_init)
{
    world_t* world = world_init();

    ck_assert_msg(world != NULL, "Failed to init world");
    ck_assert_msg(world->state == WORLD_IN_START_MENU, "Expected WORLD_IN_START_MENU(0) state but got %d", world->state);
    ck_assert_msg(world->score == 0, "Expected score of 0 but got %d", world->score);

    world_free(world);
}
END_TEST

START_TEST(test_world_start_timer)
{
    world_t* world = world_init();
    world_start_timer(world);

    ck_assert_msg(world->start_timer > 0, "Expected start_timer to be initiated got %d", world->start_timer);

    world_free(world);
}
END_TEST

START_TEST(test_world_current_delta_time)
{
    world_t* world = world_init();
    world_start_timer(world);
    sleep(1);

    double delta = world_current_delta_time(world);

    ck_assert_msg(delta >= 0.9 && delta <= 1.1 , "Expected delta to around 1 second got %f", delta);

    world_free(world);
}
END_TEST

Suite* world_suite()
{
    Suite* suite = suite_create("world");

    TCase* tc1 = tcase_create("test_world_init");
    tcase_add_test(tc1, test_world_init);

    TCase* tc2 = tcase_create("test_world_start_timer");
    tcase_add_test(tc2, test_world_start_timer);
    tcase_add_test(tc2, test_world_current_delta_time);

    suite_add_tcase(suite, tc1);
    suite_add_tcase(suite, tc2);
    return suite;
}

int main(void)
{
    Suite* suite = world_suite();
    SRunner* sr = srunner_create(suite);
    int nf;

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
