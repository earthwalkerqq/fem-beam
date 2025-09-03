#include "test.h"
#include "mtrx_fill.h"

START_TEST(test_fill_mtrx1) {
    double **mtrx = NULL;
    int row = 2, col = 3;
    make_double_mtrx(&mtrx, row, col);
    ck_assert_ptr_nonnull(mtrx);
    ck_assert_ptr_nonnull(mtrx[0]);
    destroy_mtrx(mtrx);
} END_TEST

START_TEST(test_fill_mtrx2) {
    double **mtrx = NULL;
    int row = 2; col = 3;
    make_null_double_mtrx(&mtrx, row, col);
    ck_assert_ptr_nonnull(mtrx);
    ck_assert_ptr_nonnull(*mtrx);
    ck_assert_double_eq(*mtrx, 0.);
    destroy_mtrx(mtrx);
}

Suite* test_fill_mtrx(void) {
    Suite *s = suite_create("test_fill_mtrx");
    TCase *tc = tcase_create("test_fill_mtrx");
    tcase_add_test(tc, test_fill_mtrx1);
    tcase_add_test(tc, test_fill_mtrx2);
    suite_add_tcase(s, tc);
    return s;
}