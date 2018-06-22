#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>

#include <cglm/cglm.h>
#include <check.h>

#include "engine/world.h"
#include "utils/vector.h"

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
    ck_assert_msg(world->simulation_speed == 1.0f, "Expected simulation speed of 1.0 but got %d", world->simulation_speed);

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

    double delta = world_current_delta_time(world);

    ck_assert_msg(delta >= (0.00000009) && delta <= (0.000009), "Expected delta to around 0.0000004 (a very small number that's not zero) seconds got %0.9f", delta);

    world_free(world);
}
END_TEST

START_TEST(test_world_current_delta_time_min)
{
    world_t* world = world_init();
    world_start_timer(world);
    sleep(1);

    double delta = world_current_delta_time(world);

    ck_assert_msg(delta == MIN_DELTA_TIME, "Expected delta to be MIN_DELTA_TIME when delta is too big got %f", delta);

    world_free(world);
}
END_TEST

START_TEST(test_world_object_update_model_matrix)
{
    mat4 model_matrix = {
        { 2.0f, 0.0f, 0.0f, 0.0f },
        { 0.0f, 2.0f, 0.0f, 0.0f },
        { 0.0f, 0.0f, 2.0f, 0.0f },
        { 5.0f, 5.0f, 5.0f, 1.0f }
    };

    vec3 scale = { 2.0f, 2.0f, 2.0f };
    float rotate_angle = 360.0f;
    vec3 rotate_axis = { 0.0f, 0.0f, 1.0f };
    vec3 translate = { 5.0f, 5.0f, 5.0f };
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

START_TEST(test_world_object_add_force)
{
    world_object_t* wobj = malloc(sizeof(world_object_t));
    wobj->forces = vector_init();
    vec3 force = { 1.0f, 2.0f, 3.0f };

    world_object_add_force(wobj, force);

    ck_assert_msg(wobj->forces->size == 1, "Expected force vector size to be 1 got %d", wobj->forces->size);
    ck_assert_msg(((float*)vector_get(wobj->forces, 0))[0] == 1.0f, "Expected X force in object to be 1.0 got %f", ((float*)vector_get(wobj->forces, 0))[0]);
    ck_assert_msg(((float*)vector_get(wobj->forces, 0))[1] == 2.0f, "Expected Y force in object to be 2.0 got %f", ((float*)vector_get(wobj->forces, 0))[1]);
    ck_assert_msg(((float*)vector_get(wobj->forces, 0))[2] == 3.0f, "Expected Z force in object to be 3.0 got %f", ((float*)vector_get(wobj->forces, 0))[2]);

    vector_foreach(wobj->forces, &vector_generic_item_free);
    vector_free(wobj->forces);
    free(wobj);
}
END_TEST

START_TEST(test_world_object_add_gravity)
{
    world_object_t* wobj = malloc(sizeof(world_object_t));
    wobj->forces = vector_init();

    world_object_add_gravity(wobj);

    ck_assert_msg(wobj->forces->size == 1, "Expected force vector size to be 1 got %d", wobj->forces->size);
    ck_assert_msg(((float*)vector_get(wobj->forces, 0))[0] == 0.0f, "Expected X force in object to be 0.0 got %f", ((float*)vector_get(wobj->forces, 0))[0]);
    ck_assert_msg(((float*)vector_get(wobj->forces, 0))[1] == -9.8f, "Expected Y force in object to be -9.8 got %f", ((float*)vector_get(wobj->forces, 0))[1]);
    ck_assert_msg(((float*)vector_get(wobj->forces, 0))[2] == 0.0f, "Expected Z force in object to be 0.0 got %f", ((float*)vector_get(wobj->forces, 0))[2]);

    vector_foreach(wobj->forces, &vector_generic_item_free);
    vector_free(wobj->forces);
    free(wobj);
}
END_TEST

START_TEST(test_world_object_apply_forces)
{
    vec3 translate = { 0.0f, 0.0f, 0.0f };
    vec3 force = { 5.0f, 0.0f, 0.0f };
    vec3 velocity = { 0.0f, 0.0f, 0.0f };
    double delta_time = 1.0f;
    world_object_t* wobj = malloc(sizeof(world_object_t));
    wobj->forces = vector_init();

    glm_vec_copy(velocity, wobj->velocity);
    glm_vec_copy(translate, wobj->translate);
    world_object_add_force(wobj, force);
    world_object_add_gravity(wobj);

    world_object_apply_forces(wobj, delta_time);

    ck_assert_msg(wobj->forces->size == 0, "Expected force vector to be empty got size %d", wobj->forces->size);
    ck_assert_msg(wobj->velocity[0] == 5.0f, "Expected X velocity in object to be 5.0 got %f", wobj->velocity[0]);
    ck_assert_msg(wobj->velocity[1] == -9.8f, "Expected Y velocity in object to be -9.8 got %f", wobj->velocity[1]);
    ck_assert_msg(wobj->velocity[2] == 0.0f, "Expected Z velocity in object to be 0.0 got %f", wobj->velocity[2]);

    ck_assert_msg(wobj->translate[0] == 2.5f, "Expected X position in object to be 2.5 got %f", wobj->translate[0]);
    ck_assert_msg(wobj->translate[1] == -4.9f, "Expected Y position in object to be -4.9 got %f", wobj->translate[1]);
    ck_assert_msg(wobj->translate[2] == 0.0f, "Expected Z position in object to be 0.0 got %f", wobj->translate[2]);

    vector_free(wobj->forces);
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
    tcase_add_test(tc2, test_world_current_delta_time_min);

    TCase* tc3 = tcase_create("test_world_object");
    tcase_add_test(tc3, test_world_object_update_model_matrix);
    tcase_add_test(tc3, test_world_object_add_force);
    tcase_add_test(tc3, test_world_object_add_gravity);
    tcase_add_test(tc3, test_world_object_apply_forces);

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

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
