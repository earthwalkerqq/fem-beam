#include "bc.h"

// задаем вектор нагрузок
void set_loads(int ndofysla, int count_node_load, int *nodesUnderLoad, double *r) {
    // определение вектора нагрузок и закрепление узлов следует изменить
    for (int i = 0; i < count_node_load; i++) {
        int node_load = nodesUnderLoad[i] - 1; // номер нагружаемого узла без единицы
        int kdof = node_load * ndofysla;
        r[kdof] = 0.;     // отсутствие нагружения по оси X для узла nodesUnderLoad[i]
        r[kdof + 1] = 10; // нагрузка в 10Н по Y для узла nodesUnderLoad[i]
    }
}

void set_fixed(int ndofysla, int count_node_fixed, int *nodesFixed, double **kglb) {
    for (int i = 0; i < count_node_fixed; i++) {
        int node_fixed = nodesFixed[i] - 1; // номер закрепляемого узла без единицы
        int kdof = node_fixed * ndofysla;   // номер степени свободы для закрепленного узла
        for (int j = 0; j < ndofysla; j++) {
            kglb[kdof + j][kdof + j] += 1.e10;
        }
    }
}