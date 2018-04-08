#include <check.h>
#include <stdarg.h>

#include "vector.h"

START_TEST(test_vector_init)
{
    vector_t* vector = vector_init();

    ck_assert_msg(vector != NULL, "Failed to init vector");
    ck_assert_msg(vector->size == 0, "Vector size should be zero got %d", vector->size);
    ck_assert_msg(vector->items != NULL, "vector\'s items should not be null");

    vector_free(vector);
}
END_TEST

START_TEST(test_vector_push_back)
{
    vector_t* vector = vector_init();
    int test_value = 5;

    vector_push_back(vector, &test_value);
    ck_assert_msg(*(int*)(*vector->items)[0] == test_value, "Value in first item should be test_value got %d", *(int*)(*vector->items)[0]);

    vector_free(vector);
}
END_TEST

START_TEST(test_vector_push_back_resize)
{
    vector_t* vector = vector_init();
    unsigned int old_max_size = vector->max_size;
    int test_value = 5;


    for(int i = 0; i < 15; i++) {
        vector_push_back(vector, &test_value);
    }

    ck_assert_msg(vector->max_size > old_max_size, "old_max_size(%d) is bigger than new max_size(%d)", old_max_size, vector->max_size);
    vector_free(vector);
}
END_TEST

START_TEST(test_vector_get)
{
    vector_t* vector = vector_init();
    int test_value = 5;
    int test_value2 = 6;

    vector_push_back(vector, &test_value);
    vector_push_back(vector, &test_value2);

    ck_assert_msg(*(int*)(*vector->items)[0] == *(int*)vector_get(vector, 0), "vector_get should return same value as arr index");
    ck_assert_msg(*(int*)(*vector->items)[1] == *(int*)vector_get(vector, 1), "vector_get should return same value as arr index");

    vector_free(vector);
}
END_TEST

void mul2(void* obj, va_list _)
{
    (void)_;
    *(int*)obj = *(int*)obj * 2;
}

START_TEST(test_vector_foreach)
{
    vector_t* vector = vector_init();
    int test_value = 1;
    int test_value2 = 2;
    int test_value3 = 3;

    vector_push_back(vector, &test_value);
    vector_push_back(vector, &test_value2);
    vector_push_back(vector, &test_value3);
    vector_foreach(vector, &mul2);

    ck_assert_msg(*(int*)vector_get(vector, 0) == 2, "First value should be double of 1 got %d", *(int*)vector_get(vector, 0));
    ck_assert_msg(*(int*)vector_get(vector, 1) == 4, "Second value should be double of 2 got %d", *(int*)vector_get(vector, 1));
    ck_assert_msg(*(int*)vector_get(vector, 2) == 6, "Third value should be double of 3 got %d", *(int*)vector_get(vector, 2));

    vector_free(vector);
}
END_TEST

void mul_by_x(void* obj, va_list ap)
{
    int x = va_arg(ap, int);
    *(int*)obj = *(int*)obj * x;
}

START_TEST(test_vector_foreach_vargs)
{
    vector_t* vector = vector_init();
    int test_value = 1;
    int test_value2 = 2;
    int test_value3 = 3;

    vector_push_back(vector, &test_value);
    vector_push_back(vector, &test_value2);
    vector_push_back(vector, &test_value3);
    vector_foreach(vector, &mul_by_x, 2);

    ck_assert_msg(*(int*)vector_get(vector, 0) == 2, "First value should be double of 1 got %d", *(int*)vector_get(vector, 0));
    ck_assert_msg(*(int*)vector_get(vector, 1) == 4, "Second value should be double of 2 got %d", *(int*)vector_get(vector, 1));
    ck_assert_msg(*(int*)vector_get(vector, 2) == 6, "Third value should be double of 3 got %d", *(int*)vector_get(vector, 2));

    vector_free(vector);
}
END_TEST

START_TEST(test_vector_trim)
{
    vector_t* vector = vector_init();
    int test_value = 1;
    int test_value2 = 2;
    int test_value3 = 3;

    vector_push_back(vector, &test_value);
    vector_push_back(vector, &test_value2);
    vector_push_back(vector, &test_value3);
    vector_trim(vector);

    ck_assert_msg(vector->max_size == vector->size, "Size should be the same as max size");
    ck_assert_msg(*(int*)vector_get(vector, 0) == 1, "First value is lost, got %d", *(int*)vector_get(vector, 0));
    ck_assert_msg(*(int*)vector_get(vector, 1) == 2, "Second value is lost, got %d", *(int*)vector_get(vector, 1));
    ck_assert_msg(*(int*)vector_get(vector, 2) == 3, "Third value is lost, got %d", *(int*)vector_get(vector, 2));
    vector_free(vector);
}
END_TEST

Suite* vector_suit()
{
    Suite* suite = suite_create("vector");

    TCase* tc1 = tcase_create("test_vector_init");
    tcase_add_test(tc1, test_vector_init);

    TCase* tc2 = tcase_create("test_vector_push_back");
    tcase_add_test(tc2, test_vector_push_back);
    tcase_add_test(tc2, test_vector_push_back_resize);

    TCase* tc3 = tcase_create("test_vector_get");
    tcase_add_test(tc3, test_vector_get);

    TCase* tc4 = tcase_create("test_vector_foreach");
    tcase_add_test(tc4, test_vector_foreach);
    tcase_add_test(tc4, test_vector_foreach_vargs);

    TCase* tc5 = tcase_create("test_vector_trim");
    tcase_add_test(tc5, test_vector_trim);

    suite_add_tcase(suite, tc1);
    suite_add_tcase(suite, tc2);
    suite_add_tcase(suite, tc3);
    suite_add_tcase(suite, tc4);
    suite_add_tcase(suite, tc5);
    return suite;
}

int main(void)
{
    Suite* suite = vector_suit();
    SRunner* sr = srunner_create(suite);
    int nf;

    srunner_run_all(sr, CK_ENV);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
