#ifndef METHODS_H
#define METHODS_H

#include <vector>
#include <cmath>
#include <functional>

using d_vector = std::vector<double>;
using method = std::pair<double, double>(
    const std::function<double(double, double, double)> &f,
    const std::function<double(double, double, double)> &g,
    double xn, double yn, double zn, double h);

std::pair<double, double> runge_kutt_2nd_iteration(
        const std::function<double(double, double, double)> &f,
        const std::function<double(double, double, double)> &g,
        double xn, double yn, double zn, double h);
std::pair<double, double> runge_kutt_iteration(
        const std::function<double(double, double, double)> &f,
        const std::function<double(double, double, double)> &g,
        double x0, double y0, double z0, double h);
double integrate(double lb, double ub, const std::function<double(double)> &function);
double linear_interpolation(const d_vector &first, const d_vector &second, const double value);


#endif // METHODS_H
