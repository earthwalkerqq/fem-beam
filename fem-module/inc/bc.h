#ifndef BC_H
#define BC_H

void set_loads(int ndofysla, int count_node_load, int *nodesUnderLoad, double *r);
void set_fixed(int ndofysla, int count_node_fixed, int *nodesFixed, double **kglb);

#endif