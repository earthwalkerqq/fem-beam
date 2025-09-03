#include <stdlib.h>

#include "fem.h"

// Функция формирования локальной матрицы жесткости
void beamElement(double len_elem, double area, double e, double puas,\
                double aiy, double aiz, double aik, double **gest) {
    // area - площадь поперечного сечения
    // aiy - момент инерции относительно локальной оси y
    // aiz - момент инерции относительно локальной оси z
    // aik - полярный момент инерции отнотельно локальной оси z

    double g = e / 2. * (1. + puas); // модуль сдвига

    // растяжение-сжатие (по оси X)
    gest[0][0] = e * area / len_elem;
    gest[0][6] = -e * area / len_elem;
    gest[6][0] = -e * area / len_elem;
    gest[6][6] = e * area / len_elem;

    // кручение (относительно оси X)
    gest[3][3] = g * aik / len_elem;
    gest[3][9] = -g * aik / len_elem;
    gest[9][3] = -g * aik / len_elem;
    gest[9][9] = g * aik / len_elem;

    // изгиб в плоскости XOY
    gest[1][1] = 12. * e * aiz / pow(len_elem, 3.);
    gest[5][1] = 6. * e * aiz / pow(len_elem, 2.);
    gest[7][1] = -12. * e * aiz / pow(len_elem, 3.);
    gest[11][1] = 6. * e * aiz / pow(len_elem, 2.);

    gest[1][5] = 6. * e * aiz / pow(len_elem, 2.);
    gest[5][5] = 4. * e * aiz / len_elem;
    gest[7][5] = -6. * e * aiz / pow(len_elem, 2.);
    gest[11][5] = 2. * e * aiz / len_elem;

    gest[1][7] = -12. * e * aiz / pow(len_elem, 3.);
    gest[5][7] = -6. * e * aiz / pow(len_elem, 2.);
    gest[7][7] = 12. * e * aiz / pow(len_elem, 3.);
    gest[11][7] = -6. * e * aiz / pow(len_elem, 2.);

    gest[1][11] = 6. * e * aiz / pow(len_elem, 2.);
    gest[5][11] = 2. * e * aiz / len_elem;
    gest[7][11] = -6. * e * aiz / pow(len_elem, 2.);
    gest[11][11] = 4. * e * aiz / len_elem;

    // Изгиб в плоскости XOZ
    gest[2][2] = 12. * e * aiy / pow(len_elem, 3.);
    gest[4][2] = -6. * e * aiy / pow(len_elem, 2.);
    gest[8][2] = -12. * e * aiy / pow(len_elem, 3.);
    gest[10][2] = -6. * e * aiy / pow(len_elem, 2.);

    gest[2][4] = -6. * e * aiy / pow(len_elem, 2.);
    gest[4][4] = 4. * e * aiy / len_elem;
    gest[8][4] = 6. * e * aiy / pow(len_elem, 2.);
    gest[10][4] = 2. * e * aiy / len_elem;

    gest[2][8] = -12. * e * aiy / pow(len_elem, 3.);
    gest[4][8] = 6. * e * aiy / pow(len_elem, 2.);
    gest[8][8] = 12. * e * aiy / pow(len_elem, 3.);
    gest[10][8] = 6. * e * aiy / pow(len_elem, 2.);

    gest[2][10] = -6. * e * aiy / pow(len_elem, 2.);
    gest[4][10] = 2. * e * aiy / len_elem;
    gest[8][10] = 6. * e * aiy / pow(len_elem, 2.);
    gest[10][10] = 4. * e * aiy / len_elem;
}

// сборка глобальной матрицы жесткости
void assemblyGlobalMtrx(int ndof, int ndofysla, int iys1, \
                        int iys2, double **gest, double **kglb) {
    int ig, jg, il, jl;  // начальные позиции в лок. и глоб. матрицах
    int iblock, jblock;    // добавочные коэф. к позициям матриц

    int *nys = (int*)malloc(2 * sizeof(int));
    nys[0] = iys1 - 1;
    nys[1] = iys2 - 1;

    for (int iy = 0; iy < 2; iy++) {
        for (int jy = 0; jy < 2; jy++) {
            il = iy * ndofysla;
            jl = jy * ndofysla; // начальные блоки в лок. матрице жесткости

            ig = nys[iy] * ndofysla;
            jg = nys[jy] * ndofysla; // начальные блоки в глоб. матрице жесткости

            for (iblock = 0; iblock < ndofysla; iblock++) {
                for (jblock = 0; jblock < ndofysla; jblock++) {
                    kglb[ig + iblock][jg + jblock] += gest[il + iblock][jl + jblock];
                }
            }
        }
    }

    free(nys);
}

bool_t assembleLocalStiffnessToGlobal(int ndof, int ndofysla, double area, double aiy,\
                                    double aiz, double aik, double e, double puas, int nelem,\
                                    int **jt02, double **coords, double **kglb) {
    bool_t error = FALSE;

    double **gest = NULL; // локальная матрица жесткости

    error += make_null_double_mtrx(&gest, ndofysla, ndofysla);

    if (!error) {
        for (int ielem = 0; ielem < nelem; ielem++) {
            int iys1 = jt02[0][ielem];
            int iys2 = jt02[1][ielem];

            node_t node1 = {iys1, coords[0][iys1 - 1],\
                        coords[1][iys1 - 1], coords[2][iys1 - 1]};
            node_t node2 = {iys2, coords[0][iys2 - 1],\
                        coords[1][iys2 - 1], coords[2][iys2 - 1]};

            double beam_len = sqrt(pow(node1.x - node2.x, 2.) +\
                            pow(node1.y - node2.y, 2.) + pow(node1.z - node2.z));

            beamElement(beam_len, area, e, puas, aiy, aiz, aik, gest);
            assemblyGlobalMtrx(ndof, ndofysla, node1.iys, node2.iys, gest, kglb);
        }
    }

    return error;
}