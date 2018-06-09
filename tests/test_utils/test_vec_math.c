#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "vec_math.h"

START_TEST(test_vec_bounce)
{
    vec3 vector = { 0.0f, 0.0f, 100.0f };
    vec3 normal = { 0.0f, 0.0f, -1.0f };
    vec3 bounced_vector;

    vec3_bounce(vector, normal, 1.0f, bounced_vector);

    ck_assert_msg(bounced_vector[0] == 0.0f, "Expected X axis to be 0 got %f", bounced_vector[0]);
    ck_assert_msg(bounced_vector[1] == 0.0f, "Expected Y axis to be 0 got %f", bounced_vector[1]);
    ck_assert_msg(bounced_vector[2] == -100.0f, "Expected Z axis to be -100 got %f", bounced_vector[2]);
}
END_TEST

START_TEST(test_vec_bounce_friction)
{
    vec3 vector = { 0.0f, 0.0f, 100.0f };
    vec3 normal = { 0.0f, 0.0f, -1.0f };
    vec3 bounced_vector;

    vec3_bounce(vector, normal, 0.5f, bounced_vector);

    ck_assert_msg(bounced_vector[0] == 0.0f, "Expected X axis to be 0 got %f", bounced_vector[0]);
    ck_assert_msg(bounced_vector[1] == 0.0f, "Expected Y axis to be 0 got %f", bounced_vector[1]);
    ck_assert_msg(bounced_vector[2] == -50.0f, "Expected Z axis to be -100 got %f", bounced_vector[2]);
}
END_TEST

Suite* vec_math_suit()
{
    Suite* suite = suite_create("vec_math");

    TCase* tc1 = tcase_create("test_vec_bounce");
    tcase_add_test(tc1, test_vec_bounce);
    tcase_add_test(tc1, test_vec_bounce_friction);

    suite_add_tcase(suite, tc1);
    return suite;
}

int main(void)
{
    Suite* suite = vec_math_suit();
    SRunner* sr = srunner_create(suite);
    int nf;

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
