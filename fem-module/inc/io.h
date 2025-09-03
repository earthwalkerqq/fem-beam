#ifndef IO_H
#define IO_H

#include "fem.h"

bool_t read_fem_data(char *filename, int *nys, int *nelem, double ***coords, int ***jt02);
bool_t output_result(char *filename, int ndof, double *u);

#endif