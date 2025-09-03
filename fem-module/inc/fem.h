#ifndef FEM_H
#define FEM_H

typedef struct node_coords {
    int iys; // номер узла
    double x;
    double y;
    double z;
} node_t;

typedef enum {
    FALSE = 0,
    TRUE
} bool_t;

extern const double e; // модуль упругости
extern const double puas; // коэффициент Пуассона
extern const double diam; // диаметр круглого поперечного сечения балки

#define PI 3.14159265358
#define AREA PI * diam * diam / 4

void beamElement(double len_elem, double area, double e, double puas, double aiy, double aiz, double aik, double **gest);

void assemblyGlobalMtrx(int ndofysla, int iys1, int iys2, double **gest, double **kglb);
bool_t assembleLocalStiffnessToGlobal(int ndofysla, double area, double aiy,\
                                    double aiz, double aik, double e, double puas, int nelem,\
                                    int **jt02, double **coords, double **kglb);

#endif