#ifndef MODEL_H
#define MODEL_H

#include <math.h>
#include "methods.h"

class Model {
public:
    Model();

    std::pair<std::vector<double>, std::vector<std::pair<double, dvector>>> count_T();
    bool is_power_balanced(double time, dvector) const;

    double h = 0.01;
    double tau = 1;
private:

    static const constexpr double l = 10.;
    static const constexpr double R = 0.5;
    static const constexpr double T0 = 300;

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

    static const constexpr double F_max = 5;
    static const constexpr double t_max = 5;

    static const constexpr double freq = 0.015;
    static const constexpr double period = 1. / freq;

    double F0(double time) const;

    border_conditions left;
    border_conditions right;

    sweep_arrays arrays;

    double k(double t) const;
    double c(double t) const;
    double alpha(double x) const;
    double p(double x_n) const;
    double f(double x_n) const;

    void sweep_params(dvector curr_T, dvector prev_T);
    void left_border_cond(dvector curr_T, dvector prev_T, double time);
    void right_border_cond(dvector curr_T, dvector prev_T);        
};

#endif // MODEL_H
