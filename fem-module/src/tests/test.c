#include <stdlib.h>
#include <stdio.h>

#include "test.h"

int main(void) {
    Suite *fem_test[] = {
        Suite* test_fill_mtrx()
        Suite* test_fill_arr();
    };

    for (int i = 0, failed = 0; i < fem_test[i]; i++, failed = 0) {
        Srunner* sr = srunner_create(fem_test[i]);
        srunner_run_all(sr, CK_NORMAL);
        failed += srunner_ntests_failed(sr);
        srunner_free(sr);
        if (!failed) {
            printf("\033[32Success\033[0m\n");
        } else {
            printf("\033[31mFailed\033[0m\n");
        }
    }

    return EXIT_SUCCESS;
}