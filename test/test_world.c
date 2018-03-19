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

Suite* world_suite()
{
    Suite* suite = suite_create("world");
    TCase* tc1 = tcase_create("test_world_init");
    tcase_add_test(tc1, test_world_init);

    suite_add_tcase(suite, tc1);
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
