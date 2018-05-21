#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"

START_TEST(test_files_to_str)
{
    const char* string = file_to_str("mock_assets/test_file.txt");

    ck_assert_msg(strcmp(string, "This is a file in a string\n") == 0, "got unexpected string: %s", string);

    free((void*)string);
}
END_TEST


Suite* files_suit()
{
    Suite* suite = suite_create("files");

    TCase* tc1 = tcase_create("test_file_to_str");
    tcase_add_test(tc1, test_files_to_str);

    suite_add_tcase(suite, tc1);
    return suite;
}

int main(void)
{
    Suite* suite = files_suit();
    SRunner* sr = srunner_create(suite);
    int nf;

    srunner_run_all(sr, CK_VERBOSE);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);

    return nf == 0 ? 0 : 1;
}
