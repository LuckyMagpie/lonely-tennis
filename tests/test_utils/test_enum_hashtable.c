#include <check.h>
#include <stdlib.h>

#include "enum_hashtable.h"

typedef enum {
    TAG1,
    TAG2
} tags_t;

START_TEST(test_enum_hastable_init)
{
    unsigned int num_buckets = 2;
    enum_hashtable_t* hashtable = enum_hashtable_init(num_buckets, NULL);

    ck_assert_msg(hashtable != NULL, "expected hastable to be initialized");
    ck_assert_msg(hashtable->buckets != NULL, "expected hashtable buckets to be initialized");
    ck_assert_msg(hashtable->free_enum_value_fn != NULL, "expected hashtable value free function pointer to point to something");
    ck_assert_msg(hashtable->num_buckets == num_buckets, "expected number of buckets to be %d got %d", num_buckets, hashtable->num_buckets);

    enum_hashtable_free(hashtable);
}
END_TEST

START_TEST(test_enum_hastable_init_zero_buckets)
{
    unsigned int num_buckets = 0;
    enum_hashtable_t* hashtable = enum_hashtable_init(num_buckets, NULL);

    ck_assert_msg(hashtable == NULL, "expected hashtable to be NULL");
}
END_TEST

START_TEST(test_enum_hastable_put)
{
    unsigned int num_buckets = 2;
    int value1 = 5;
    int value2 = 10;
    enum_hashtable_t* hashtable = enum_hashtable_init(num_buckets, NULL);

    enum_hashtable_put(hashtable, TAG1, &value1);
    enum_hashtable_put(hashtable, TAG2, &value2);

    enum_hashtable_value_t* hashtable_value1 = (*hashtable->buckets)[TAG1 % num_buckets]->head->data;
    enum_hashtable_value_t* hashtable_value2 = (*hashtable->buckets)[TAG2 % num_buckets]->head->data;

    ck_assert_msg(hashtable_value1->enum_value == TAG1, "expected hashtable_value1 to be TAG1 (1) got %d", hashtable_value1->enum_value);
    ck_assert_msg(hashtable_value2->enum_value == TAG2, "expected hashtable_value2 to be TAG2 (2) got %d", hashtable_value2->enum_value);

    ck_assert_msg(*((int*)hashtable_value1->data) == value1, "expected hashtable_value1 data to be 5 got %d", *((int*)hashtable_value1->data));
    ck_assert_msg(*((int*)hashtable_value2->data) == value2, "expected hashtable_value2 data to be 10 got %d", *((int*)hashtable_value2->data));

    enum_hashtable_free(hashtable);
}
END_TEST

START_TEST(test_enum_hastable_put_replace)
{
    unsigned int num_buckets = 2;
    int value1 = 5;
    int value2 = 10;
    enum_hashtable_t* hashtable = enum_hashtable_init(num_buckets, NULL);

    enum_hashtable_put(hashtable, TAG1, &value1);
    enum_hashtable_value_t* hashtable_value1 = (*hashtable->buckets)[TAG1 % num_buckets]->head->data;
    ck_assert_msg(hashtable_value1->enum_value == TAG1, "expected hashtable_value1 to be TAG1 (1) got %d", hashtable_value1->enum_value);
    ck_assert_msg(*((int*)hashtable_value1->data) == value1, "expected hashtable_value1 data to be 5 got %d", *((int*)hashtable_value1->data));

    enum_hashtable_put(hashtable, TAG1, &value2);
    enum_hashtable_value_t* hashtable_value2 = (*hashtable->buckets)[TAG1 % num_buckets]->head->data;
    ck_assert_msg(hashtable_value2->enum_value == TAG1, "expected hashtable_value2 to be TAG1 (1) got %d", hashtable_value2->enum_value);
    ck_assert_msg(*((int*)hashtable_value2->data) == value2, "expected hashtable_value2 data to be 10 got %d", *((int*)hashtable_value2->data));

    enum_hashtable_free(hashtable);
}
END_TEST

START_TEST(test_enum_hastable_get)
{
    unsigned int num_buckets = 2;
    int value1 = 5;
    int value2 = 10;
    enum_hashtable_t* hashtable = enum_hashtable_init(num_buckets, NULL);

    enum_hashtable_put(hashtable, TAG1, &value1);
    enum_hashtable_put(hashtable, TAG2, &value2);

    enum_hashtable_value_t* hashtable_value1 = (*hashtable->buckets)[TAG1 % num_buckets]->head->data;
    enum_hashtable_value_t* hashtable_value2 = (*hashtable->buckets)[TAG2 % num_buckets]->head->data;

    enum_hashtable_value_t* hashtable_value_get1 = enum_hashtable_get(hashtable, TAG1);
    enum_hashtable_value_t* hashtable_value_get2 = enum_hashtable_get(hashtable, TAG2);

    ck_assert_msg(hashtable_value1 == hashtable_value_get1, "expected hashtable_value1 to the same as hashtable_value_get1");
    ck_assert_msg(hashtable_value2 == hashtable_value_get2, "expected hashtable_value2 to the same as hashtable_value_get2");

    enum_hashtable_free(hashtable);
}
END_TEST

static void special_enum_value_free(enum_hashtable_value_t* value)
{
    free(value->data);
    free(value);
}

START_TEST(test_enum_hastable_free_specialfree)
{
    unsigned int num_buckets = 1;
    int* value1 = malloc(sizeof(int));
    enum_hashtable_t* hashtable = enum_hashtable_init(num_buckets, &special_enum_value_free);

    enum_hashtable_put(hashtable, TAG1, value1);

    enum_hashtable_free(hashtable);
}
END_TEST

Suite* enum_hashtable_suit()
{
    Suite* suite = suite_create("enum_hashtable");

    TCase* tc1 = tcase_create("test_enum_hastable_init");
    tcase_add_test(tc1, test_enum_hastable_init);
    tcase_add_test(tc1, test_enum_hastable_init_zero_buckets);

    TCase* tc2 = tcase_create("test_enum_hastable_put");
    tcase_add_test(tc2, test_enum_hastable_put);
    tcase_add_test(tc2, test_enum_hastable_put_replace);

    TCase* tc3 = tcase_create("test_enum_hastable_get");
    tcase_add_test(tc3, test_enum_hastable_get);

    TCase* tc4 = tcase_create("test_enum_hastable_free");
    tcase_add_test(tc4, test_enum_hastable_free_specialfree);

    suite_add_tcase(suite, tc1);
    suite_add_tcase(suite, tc2);
    suite_add_tcase(suite, tc3);
    suite_add_tcase(suite, tc4);
    return suite;
}

int main(void)
{
    Suite* suite = enum_hashtable_suit();
    SRunner* sr = srunner_create(suite);
    int nf;

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
