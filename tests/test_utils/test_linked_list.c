#include <check.h>
#include <stdbool.h>
#include <stdlib.h>

#include "linked_list.h"

START_TEST(test_linked_list_init)
{
    linked_list_t* list = linked_list_init();

    ck_assert_msg(list->size == 0, "expected list size to be 0 got %d", list->size);
    ck_assert_msg(list->head == NULL, "expected list head to be NULL got %d", list->head);

    linked_list_free(list, NULL);
}
END_TEST

START_TEST(test_linked_list_add_tail)
{
    int test_data1 = 1;
    int test_data2 = 2;
    linked_list_t* list = linked_list_init();

    linked_list_add_tail(list, &test_data1);
    linked_list_add_tail(list, &test_data2);

    ck_assert_msg(list->size == 2, "expected list size to be 2 got %d", list->size);
    ck_assert_msg(*((int*)list->head->data) == test_data1, "expected list head to be test_data1 (1) got %d", *((int*)list->head->data));
    ck_assert_msg(*((int*)list->head->next->data) == test_data2, "expected second node to be test_data2 (2) got %d", *((int*)list->head->next->data));

    linked_list_free(list, NULL);
}
END_TEST

START_TEST(test_linked_list_add_head)
{
    int test_data1 = 1;
    int test_data2 = 2;
    linked_list_t* list = linked_list_init();

    linked_list_add_head(list, &test_data1);
    linked_list_add_head(list, &test_data2);

    ck_assert_msg(list->size == 2, "expected list size to be 2 got %d", list->size);
    ck_assert_msg(*((int*)list->head->data) == test_data2, "expected list head to be test_data2 (2) got %d", *((int*)list->head->data));
    ck_assert_msg(*((int*)list->head->next->data) == test_data1, "expected second node to be test_data1 (1) got %d", *((int*)list->head->next->data));

    linked_list_free(list, NULL);
}
END_TEST

START_TEST(test_linked_list_node_find_defaultcmp)
{
    int test_data1 = 1;
    int test_data2 = 2;
    int test_data3 = 3;
    int test_data4 = 4;
    linked_list_t* list = linked_list_init();

    linked_list_add_head(list, &test_data1);
    linked_list_add_head(list, &test_data2);
    linked_list_add_head(list, &test_data3);
    linked_list_add_head(list, &test_data4);

    linked_list_node_t* found_node = linked_list_node_find(list, &test_data3, NULL);

    ck_assert_msg(*((int*)found_node->data) == test_data3, "expected found node data to be test_data3 (3) got %d", *((int*)found_node->data));

    linked_list_free(list, NULL);
}
END_TEST

static bool int_cmp(int* num, int* to_find)
{
    return *num == *to_find;
}

START_TEST(test_linked_list_node_find_specialcmp)
{
    int test_data1 = 1;
    int test_data2 = 2;
    int test_data3 = 3;
    int test_data4 = 4;
    linked_list_t* list = linked_list_init();

    linked_list_add_head(list, &test_data1);
    linked_list_add_head(list, &test_data2);
    linked_list_add_head(list, &test_data3);
    linked_list_add_head(list, &test_data4);

    linked_list_node_t* found_node = linked_list_node_find(list, &test_data4, (bool (*)(void*, void*))&int_cmp);

    ck_assert_msg(*((int*)found_node->data) == test_data4, "expected found node data to be test_data4 (4) got %d", *((int*)found_node->data));

    linked_list_free(list, NULL);
}
END_TEST

START_TEST(test_linked_list_node_find_notfound)
{
    int test_data1 = 1;
    int test_data2 = 2;
    int test_data3 = 3;
    int test_data4 = 4;
    int test_data5 = 5;
    linked_list_t* list = linked_list_init();

    linked_list_add_head(list, &test_data1);
    linked_list_add_head(list, &test_data2);
    linked_list_add_head(list, &test_data3);
    linked_list_add_head(list, &test_data4);

    linked_list_node_t* found_node = linked_list_node_find(list, &test_data5, (bool (*)(void*, void*))&int_cmp);

    ck_assert_msg(found_node == NULL, "expected return value to be null (not found)");

    linked_list_free(list, NULL);
}
END_TEST
START_TEST(test_linked_list_node_delete_defaultcmp)
{
    int test_data1 = 1;
    int test_data2 = 2;
    linked_list_t* list = linked_list_init();

    linked_list_add_tail(list, &test_data1);
    linked_list_add_tail(list, &test_data2);

    linked_list_node_delete(list, &test_data2, NULL, NULL);

    ck_assert_msg(list->size == 1, "expected list size to be 1 got %d", list->size);
    ck_assert_msg(*((int*)list->head->data) == test_data1, "expected list head to be test_data1 (1) got %d", *((int*)list->head->data));
    ck_assert_msg(list->head->next == NULL, "expected second node to be NULL (deleted)");

    linked_list_free(list, NULL);
}
END_TEST

START_TEST(test_linked_list_node_delete_head)
{
    int test_data1 = 1;
    int test_data2 = 2;
    linked_list_t* list = linked_list_init();

    linked_list_add_tail(list, &test_data1);
    linked_list_add_tail(list, &test_data2);

    linked_list_node_delete(list, &test_data1, NULL, NULL);

    ck_assert_msg(list->size == 1, "expected list size to be 1 got %d", list->size);
    ck_assert_msg(*((int*)list->head->data) == test_data2, "expected list head to be test_data2 (2) got %d", *((int*)list->head->data));
    ck_assert_msg(list->head->next == NULL, "expected second node to be NULL since test_data2 is the new head");

    linked_list_free(list, NULL);
}
END_TEST

START_TEST(test_linked_list_node_delete_specialcmp)
{
    int test_data1 = 1;
    int test_data2 = 2;
    linked_list_t* list = linked_list_init();

    linked_list_add_tail(list, &test_data1);
    linked_list_add_tail(list, &test_data2);

    linked_list_node_delete(list, &test_data2, (bool (*)(void*, void*))&int_cmp, NULL);

    ck_assert_msg(list->size == 1, "expected list size to be 1 got %d", list->size);
    ck_assert_msg(*((int*)list->head->data) == test_data1, "expected list head to be test_data1 (1) got %d", *((int*)list->head->data));
    ck_assert_msg(list->head->next == NULL, "expected second node to be NULL (deleted)");

    linked_list_free(list, NULL);
}
END_TEST

START_TEST(test_linked_list_node_delete_notfound)
{
    int test_data1 = 1;
    int test_data2 = 2;
    int test_data3 = 3;
    linked_list_t* list = linked_list_init();

    linked_list_add_tail(list, &test_data1);
    linked_list_add_tail(list, &test_data2);

    linked_list_node_delete(list, &test_data3, (bool (*)(void*, void*))&int_cmp, NULL);

    ck_assert_msg(list->size == 2, "expected list size to be 1 got %d", list->size);
    ck_assert_msg(*((int*)list->head->data) == test_data1, "expected list head to be test_data1 (1) got %d", *((int*)list->head->data));
    ck_assert_msg(*((int*)list->head->next->data) == test_data2, "expected next node to be test_data2 (2) got %d", *((int*)list->head->next->data));

    linked_list_free(list, NULL);
}
END_TEST

START_TEST(test_linked_list_free_specialfree)
{
    int* test_data1 = malloc(sizeof(int));
    int* test_data2 = malloc(sizeof(int));
    linked_list_t* list = linked_list_init();

    linked_list_add_tail(list, test_data1);
    linked_list_add_tail(list, test_data2);

    linked_list_free(list, &free);

    // Valgrind to test for proper list deletion
}
END_TEST

Suite* linked_list_suit()
{
    Suite* suite = suite_create("linked_list");

    TCase* tc1 = tcase_create("test_linked_list_init");
    tcase_add_test(tc1, test_linked_list_init);

    TCase* tc2 = tcase_create("test_linked_list_add");
    tcase_add_test(tc2, test_linked_list_add_tail);
    tcase_add_test(tc2, test_linked_list_add_head);

    TCase* tc3 = tcase_create("test_linked_list_node_find");
    tcase_add_test(tc3, test_linked_list_node_find_defaultcmp);
    tcase_add_test(tc3, test_linked_list_node_find_specialcmp);
    tcase_add_test(tc3, test_linked_list_node_find_notfound);

    TCase* tc4 = tcase_create("test_linked_list_node_delete");
    tcase_add_test(tc4, test_linked_list_node_delete_defaultcmp);
    tcase_add_test(tc4, test_linked_list_node_delete_head);
    tcase_add_test(tc4, test_linked_list_node_delete_specialcmp);
    tcase_add_test(tc4, test_linked_list_node_delete_notfound);

    TCase* tc5 = tcase_create("test_linked_list_free");
    tcase_add_test(tc5, test_linked_list_free_specialfree);

    suite_add_tcase(suite, tc1);
    suite_add_tcase(suite, tc2);
    suite_add_tcase(suite, tc3);
    suite_add_tcase(suite, tc4);
    suite_add_tcase(suite, tc5);
    return suite;
}

int main(void)
{
    Suite* suite = linked_list_suit();
    SRunner* sr = srunner_create(suite);
    int nf;

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
