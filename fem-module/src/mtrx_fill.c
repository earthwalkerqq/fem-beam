#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "mtrx_fill.h"

bool_t make_double_mtrx(double ***mtrx, int row, int col) {
    bool_t error = FALSE;

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

bool_t make_int_mtrx(int ***mtrx, int row, int col) {
    bool_t error = FALSE;

    int *data = (int*)malloc(row * col * sizeof(int));
    if (!data) {
        error = TRUE;
    } else {
        *mtrx = (int**)malloc(row * sizeof(int*));
        if (!*mtrx) {
            error = TRUE;
            free(data);
        } else {
            for (int i = 0; i < row; i++) (*mtrx)[i] = data + i * col;
        }
    }
    
    return error;
}

bool_t make_null_double_mtrx(double ***mtrx, int row, int col) {
    bool_t error = FALSE;

    double *data = (double*)calloc(row * col, sizeof(double));
    if (!data) {
        error = TRUE;
    } else {
        *mtrx = (double**)calloc(row, sizeof(double*));
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

bool_t make_double_arr(double **arr, int size) {
    bool_t error = FALSE;

    *arr = (double*)malloc(size * sizeof(double));
    if (!*arr)
        error = TRUE;
    
    return error;
}

bool_t make_work_arrs(int size, int count_elem, ...) {
    bool_t error = FALSE;

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

bool_t make_arr(int **arr, int size) {
    bool_t error = FALSE;

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