#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <stdarg.h>

#include <check.h>
#include <cglm/cglm.h>

#include "utils/vector.h"
#include "world.h"

bool model_matrix_eq(float* model_matrix1, float* model_matrix2)
{
    for (int i = 0; i < 8; i++) {
        if (!(fabs(model_matrix1[i] - model_matrix2[i]) <= 0.0001)) {
            return false;
        }
    }

    return true;
}

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

    ck_assert_msg(delta >= (0.9 * SIMULATION_SPEED) && delta <= (1.1 * SIMULATION_SPEED), "Expected delta to around 1 second got %f", delta);

    world_free(world);
}
END_TEST

START_TEST(test_world_object_update_model_matrix)
{
    mat4 model_matrix = {
        {2.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 2.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 2.0f, 0.0f},
        {5.0f, 5.0f, 5.0f, 1.0f}
    };

    vec3 scale = {2.0f, 2.0f, 2.0f};
    float rotate_angle = 360.0f;
    vec3 rotate_axis = {0.0f, 0.0f, 1.0f};
    vec3 translate = {5.0f, 5.0f, 5.0f};
    world_object_t* wobj = malloc(sizeof(world_object_t));

    glm_vec_copy(scale, wobj->scale);
    glm_vec_copy(rotate_axis, wobj->rotate_axis);
    wobj->rotate_angle = rotate_angle;
    glm_vec_copy(translate, wobj->translate);

    world_object_update_model_matrix(wobj);

    ck_assert_msg(model_matrix_eq(model_matrix[0], wobj->model_matrix[0]), "Model matrix is not equal");

    free(wobj);
}
END_TEST

void apply_forces_wrapper(void* force, ...)
{
    va_list ap;
    va_start(ap, force);
    world_object_apply_force(force, ap);
    va_end(ap);
}

START_TEST(test_world_object_apply_force)
{

    vec3 translate = {5.0f, 5.0f, 5.0f};
    double delta_time = 5.0;

    world_object_t* wobj = malloc(sizeof(world_object_t));
    glm_vec_copy(translate, wobj->translate);
    force_t* force = malloc(sizeof(force_t));
    force->translate[0] = 2.0f;
    force->translate[1] = 3.0f;
    force->translate[2] = 4.0f;

    apply_forces_wrapper(force, wobj, delta_time);

    ck_assert_msg(wobj->translate[0] == 15.0f, "Object X should have been 'pushed' 15 units got %f", wobj->translate[0]);
    ck_assert_msg(wobj->translate[1] == 20.0f, "Object Y should have been 'pushed' 15 units got %f", wobj->translate[1]);
    ck_assert_msg(wobj->translate[2] == 25.0f, "Object Z should have been 'pushed' 15 units got %f", wobj->translate[2]);

    // force is freed after applying it to the object, valgrind tests that
    free(wobj);
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

    TCase* tc3 = tcase_create("test_world_object");
    tcase_add_test(tc3, test_world_object_update_model_matrix);
    tcase_add_test(tc3, test_world_object_apply_force);

    suite_add_tcase(suite, tc1);
    suite_add_tcase(suite, tc2);
    suite_add_tcase(suite, tc3);
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
