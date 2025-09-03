#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "mtrx_fill.h"

char make_double_mtrx(double ***mtrx, int row, int col) {
    char error = FALSE;

    double *data = (double*)malloc(row * col * sizeof(double));
    if (!data) {
        error = TRUE;
    } else {
        *mtrx = (double**)malloc(row * sizeof(double*));
        if (!*mtrx) {
            error = TRUE;
            free(data);
        } else {
            for (int i = 0; i < row; i++) (*mtrx)[i] = data + i * col;
        }
    }
    
    return error;
}

char make_null_double_mtrx(double ***mtrx, int row, int col) {
    char error = FALSE;

    double *data = (double*)calloc(row * col * sizeof(double));
    if (!data) {
        error = TRUE;
    } else {
        *mtrx = (double**)calloc(row * sizeof(double*));
        if (!*mtrx) {
            error = TRUE;
            free(data);
        } else {
            for (int i = 0; i < row; i++) (*mtrx)[i] = data + i * col;
        }
    }
    
    return error;
}

void destroy_mtrxs(int count_elem, ...) {
    va_list list;
    va_start(list, count_elem);
    for (int i = 0; i < count_elem; i++) {
        void **mtrx = va_arg(list, void**);
        if (*mtrx) free(*mtrx);
        if (mtrx) free(mtrx);
    }
    va_end(list);
}

char read_fem_data(char *filename, int *nys, int *nelem, double ***coords, int ***jt02) {
    char error = FALSE;

    FILE *file = fopen(filename, "r");
    if (!file || fscanf(file, "%d", nys) != 1 || *nys <= 0 || make_double_mtrx(coords, 3, *nys)) {
        error = TRUE;
    } else {
        for (int i = 0; i < *nys && !error; i++) {
            if (fscanf(file, "%lf%lf%lf", (*coords)[0] + i, (*coords)[1] + i, (*coords)[2] + i) != 3) error = TRUE;
        }
        if (!error || fscanf(file, "%d", nelem) != 1 || make_double_mtrx(jt02, 2, *nelem)) {
            error = TRUE;
            destroy_mtrx(coords);
        } else {
            for (int i = 0; i < *nelem && !error; i++) {
                if (fscanf(file, "%d%d", (*jt02)[0] + i, (*jt02)[1] + i) != 2) error = TRUE;
            }

            if (error) {
                destroy_mtrx(coords);
                destroy_mtrx(jt02);
            }
        }
    }
    fclose(file);
    return error;
}

char make_double_arr(double **arr, int size) {
    char error = FALSE;

    *arr = (double*)malloc(size * sizeof(double));
    if (!*arr)
        error = TRUE;
    
    return error;
}

char make_work_arrs(int count_elem, int size, ...) {
    char error = FALSE;

    double **arr_pull = (double**)malloc(count_elem * sizeof(double*));

    va_list list;
    va_start(list, count_elem);
    int i;
    for (i = 0; i < count_elem && !error; i++) {
        double **arr = va_arg(list, double**);
        *arr = (double*)calloc(size, sizeof(double));
        arr_pull[i] = *arr;
        if (*arr) {
            error = TRUE;
        }
    }

    if (error) {
        for (; i >= 0; i--) {
            if (!arr_pull[i]) free(arr_pull);
        }
    }

    va_end(list);

    return error;
}

char make_arr(int **arr, int size) {
    char error = FALSE;

    *arr = (int*)malloc(size * sizeof(int));
    if (!*arr) {
        error = TRUE;
    }

    return error;
}

void destroy_arrs(int count_elem, ...) {
    va_list list;
    va_start(list, count_elem);
    for (int i = 0; i < count_elem; i++) {
        void *arr = va_arg(list, void*);
        if (arr) free(arr);
    }
    va_end(list);
}