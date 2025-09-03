#ifndef MTRX_FILL_H
#define MTRX_FILL_H

#include "fem.h"

bool_t make_double_mtrx(double ***mtrx, int row, int col);
bool_t make_int_mtrx(int ***mtrx, int row, int col);
bool_t make_null_double_mtrx(double ***mtrx, int row, int col);

bool_t make_double_arr(double **arr, int count_elem);
bool_t make_work_arrs(int size, int count_elem, ...);
bool_t make_arr(int **arr, int size);

void destroy_mtrxs(int count_elem, ...);
void destroy_arrs(int count_elem, ...);

#endif