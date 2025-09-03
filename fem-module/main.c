#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fem.h"
#include "mtrx_fill.h"
#include "LDLT.h"
#include "io.h"
#include "bc.h"

const double e = 2.1e5;
const double puas = 0.3;
const double diam = 10.;

int main(int argc, char* argv[]) {
    char error = EXIT_SUCCESS;

    if (argc == 2)
        return EXIT_FAILURE;
    int nys;                // число узлов конечно элементной модели
    int nelem;              // число балочных узлов
    const int ndofysla = 6; // число степеней свободы одного узла

    // необходимо изменить!!
    /*-----------------------------------------------------*/
    const int count_node_load = 1;  // число нагруженных узлов
    const int count_node_fixed = 1; // число закрепленных узлов
    /*-----------------------------------------------------*/

    double **kglb = NULL;    // глобальная матрица жесткости

    double **coords = NULL;  // двумерный массив координат
    int **jt02 = NULL;       // двумерный массив начальной и конечной точки для текущей линии

    double aiy, aiz, aik;
    aiy = aiz = PI * pow(diam, 4.) / 64; // моменты инерции относительно лок. осей Y, Z
    aik = aiy + aiz;

    if (read_fem_data(argv[1], &nys, &nelem, &coords, &jt02)) {
        perror("FAIL FROM READ FEM MODEL");
        return EXIT_FAILURE;
    }

    int ndof = nys * ndofysla; // общее число степеней свободы

    double *u = NULL;  // вектор перемещений
    double *r = NULL;  // вектор нагрузок
    double *x = NULL;  // рабочий массив
    double *y = NULL;  // второй рабочий массив

    int *nodesUnderLoad = NULL;  // массив нагруженых узлов
    int *nodesFixed = NULL;      // массив закрепленных узлов

    // выделение памяти для массивов и глобальной матрицы жесткости

    error += make_null_double_mtrx(&kglb, ndof, ndof);
    error += make_work_arrs(ndof, 4, &u, &r, &x, &y);
    error += make_arr(&nodesUnderLoad, count_node_load);
    error += make_arr(&nodesFixed, count_node_fixed);

    if (error) {
        perror("CAN'T MEMORY ALLOCATE");
        destroy_mtrxs(3, coords, jt02, kglb);
        destroy_arrs(6, u, r, x, y, nodesUnderLoad, nodesFixed);
        return error;
    }

    // формирование глобальной матрицы жесткости
    error += assembleLocalStiffnessToGlobal(ndofysla, AREA, aiy, aiz, aik, e, puas, nelem,\
                                jt02, coords, kglb);
    
    // задаем вектор закреплений
    set_fixed(ndofysla, count_node_fixed, nodesFixed, kglb);

    // задаем вектор нагрузок
    set_loads(ndofysla, count_node_load, nodesUnderLoad, r);

    // решение СЛАУ методом LDLT
    error += solveLinearSystemLDLT(kglb, u, r, x, ndof);
    
    output_result("../../build/result.txt", ndof, u);

    destroy_mtrxs(3, coords, jt02, kglb);
    destroy_arrs(6, u, r, x, y, nodesUnderLoad, nodesFixed);

    return error;
}