#include <stdio.h>

#include "io.h"
#include "mtrx_fill.h"

bool_t read_fem_data(char *filename, int *nys, int *nelem, double ***coords, int ***jt02) {
    bool_t error = FALSE;

    FILE *file = fopen(filename, "r");
    if (!file || fscanf(file, "%d", nys) != 1 || *nys <= 0 || make_double_mtrx(coords, 3, *nys)) {
        error = TRUE;
    } else {
        for (int i = 0; i < *nys && !error; i++) {
            if (fscanf(file, "%lf%lf%lf", (*coords)[0] + i, (*coords)[1] + i, (*coords)[2] + i) != 3) error = TRUE;
        }
        if (!error || fscanf(file, "%d", nelem) != 1 || make_int_mtrx(jt02, 2, *nelem)) {
            error = TRUE;
            destroy_mtrxs(1, coords);
        } else {
            for (int i = 0; i < *nelem && !error; i++) {
                if (fscanf(file, "%d%d", (*jt02)[0] + i, (*jt02)[1] + i) != 2) error = TRUE;
            }

            if (error) {
                destroy_mtrxs(2, coords, jt02);
            }
        }
    }
    fclose(file);
    return error;
}

bool_t output_result(char *filename, int ndof, double *u) {
    bool_t error = FALSE;

    FILE *file = fopen(filename, "w");
    if (file) {
        for (int i = 0; i < ndof; i++) {
            for (int j = 0; j < 10; j++) {
                if (i + j < ndof) fprintf(file, "%12.4f ", u[i + j]);
            }
            fputc('\n', file);
        }
    } else {
        error = TRUE;
    }

    return error;
}