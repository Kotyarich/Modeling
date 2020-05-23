#ifndef METHODS_H
#define METHODS_H

#include <vector>

using dvector = std::vector<double>;

struct sweep_arrays {
    dvector an;
    dvector bn;
    dvector cn;
    dvector dn;
};

struct border_conditions {
    double k;
    double m;
    double p;
};

dvector sweep_method(const sweep_arrays &params,
                     const border_conditions &left,
                     const border_conditions &right);

#endif // METHODS_H
