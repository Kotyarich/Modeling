#ifndef MODEL_H
#define MODEL_H

#include "methods.h"

class Model {
public:
    Model(double a0, double aN, double k0, double kN, double F0);

    std::pair<dvector, dvector> count_T();

private:
    static const constexpr double l = 10.;
    static const constexpr double R = 0.5;
    static const constexpr double T0 = 300;
    static const constexpr double h = 1e-2;

    double _a0;
    double _aN;
    double _k0;
    double _kN;
    double _F0;

    double _a;
    double _b;
    double _c;
    double _d;

    border_conditions left;
    border_conditions right;

    sweep_arrays arrays;

    void count_k_params();
    void count_a_params();

    double k(double x) const;
    double a(double x) const;

    double p(double x_n) const;
    double f(double x_n) const;

    double x_plus_half(double x_n) const;
    double x_minus_half(double x_n) const;

    void sweep_params();
    void left_border_cond();
    void right_border_cond();
};

#endif // MODEL_H
