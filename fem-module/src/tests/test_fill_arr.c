#include "test.h"
#include "mtrx_fill.h"

START_TEST(test_fill_arr1) {
    double *arr = NULL;
    make_double_arr(&arr, 3);
    ck_assert_ptr_nonnull(arr);
    destroy_arrs(1, arr);
} END_TEST;

START_TEST(test_fill_arr2) {
    double *arr1 = NULL;
    double *arr2 = NULL;
    make_work_arrs(2, 3, &arr1, &arr2);
    ck_assert_ptr_nonnull(arr1);
    ck_assert_ptr_nonnull(arr2);
    destroy_arrs(2, arr1, arr2);
} END_TEST;

Suite* test_fill_arr(void) {
    Suite *s = suite_create("test_fill_arr");
    TCase *tc = tcase_create("test_fill_arr");
    tcase_add_test(tc, test_fill_arr1);
    tcase_add_test(tc, test_fill_arr2);
    suite_add_tcase(s, tc);
    return s;
}