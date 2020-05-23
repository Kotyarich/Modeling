#ifndef MODEL_H
#define MODEL_H

#include <math.h>
#include "methods.h"

class Model {
public:
    Model(double F0);

    std::pair<std::vector<double>, std::vector<std::pair<double, dvector>>> count_T();

    static const constexpr double h = 1e-2;
private:
    static const constexpr double l = 10.;
    static const constexpr double R = 0.5;
    static const constexpr double T0 = 300;
    static const constexpr double tau = 2;

    static const constexpr double a1 = 0.0134;
    static const constexpr double b1 = 1;
    static const constexpr double c1 = 4.35e-4;
    static const constexpr double m1 = 1;

    static const constexpr double a2 = 2.049;
    static const constexpr double b2 = 0.563e-3;
    static const constexpr double c2 = 0.528e+5;
    static const constexpr double m2 = 1;

    static const constexpr double _alpha_0 = 0.05;
    static const constexpr double _alpha_N = 0.01;

    static const constexpr double eps = 1e-5;

    double _F0;

    border_conditions left;
    border_conditions right;

    sweep_arrays arrays;

    double k(double t) const;
    double c(double t) const;
    double alpha(double x) const;
    double p(double x_n) const;
    double f(double x_n) const;

    double x_plus_half(double x_n) const;
    double x_minus_half(double x_n) const;

    void sweep_params(dvector curr_T, dvector prev_T);
    void left_border_cond(dvector curr_T, dvector prev_T);
    void right_border_cond(dvector curr_T, dvector prev_T);
};

#endif // MODEL_H
