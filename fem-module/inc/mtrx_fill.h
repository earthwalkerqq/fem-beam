#ifndef MTRX_FILL_H
#define MTRX_FILL_H

#define TRUE 1
#define FALSE 0

char make_double_mtrx(double ***mtrx, int row, int col);
char make_null_double_mtrx(double ***mtrx, int row, int col);

char read_fem_data(char *filename, int *nys, int *nelem, double ***coords, int ***jt02);

char make_double_arr(double **arr, int count_elem);
char make_work_arrs(int count_elem, int size, ...);
char make_arr(int **arr, int size);

void destroy_mtrxs(int count_elem, ...);
void destroy_arrs(int count_elem, ...);

#endif