#include "methods.h"

std::pair<double, double> runge_kutt_2nd_iteration(
        const std::function<double(double, double, double)> &f,
        const std::function<double(double, double, double)> &g,
        double xn, double un, double in, double h) {
    auto i_in_half = in + h * f(xn, un, in) / 2.;
    auto u_in_half = un + h * g(xn, un, in) / 2.;

    auto i_der_in_half = f(xn + h / 2., u_in_half, i_in_half);
    auto u_der_in_half = g(xn + h / 2., u_in_half, i_in_half);

    auto i_res = in + h * i_der_in_half;
    auto u_res = un + h * u_der_in_half;

    return std::make_pair(u_res, i_res);
}

std::pair<double, double> runge_kutt_iteration(
        const std::function<double(double, double, double)> &f,
        const std::function<double(double, double, double)> &g,
        double x0, double u0, double i0, double h) {
    auto k1 = h * f(x0, u0, i0);
    auto q1 = h * g(x0, u0, i0);

    auto k2 = h * f(x0 + h/2, u0 + q1/2, i0 + k1/2);
    auto q2 = h * g(x0 + h/2, u0 + q1/2, i0 + k1/2);

    auto k3 = h * f(x0 + h/2, u0 + q2/2, i0 + k2/2);
    auto q3 = h * g(x0 + h/2, u0 + q2/2, i0 + k2/2);

    auto k4 = h * f(x0 + h, u0 + q3, i0 + k3);
    auto q4 = h * g(x0 + h, u0 + q3, i0 + k3);

    auto yn = u0 + (q1 + 2 * q2 + 2 * q3 + q4) / 6;
    auto zn = i0 + (k1 + 2 * k2 + 2 * k3 + k4) / 6;

    return std::make_pair(yn, zn);
}

double integrate(double lb, double ub, const std::function<double(double)> &function) {
  double step = 0.01;
  double result = 0;

  while (lb < ub) {
    double left = function(lb);
    lb += step;
    double right = function(lb);

    result += left + right;
  }

  return result * step / 2;
}

double linear_interpolation(const d_vector &first, const d_vector &second, const double value) {
    auto i = 0ul;
    for (; i < first.size() - 1; i++) {
        auto val_in_range = first[i] <= value && value <= first[i + 1];
        if (val_in_range || value < first[i]) {
            break;
        }
    }
    i = i == first.size() - 1? i-1 : i;

    auto result = second[i] + (second[i + 1] - second[i])
            * (value - first[i]) / (first[i + 1] - first[i]);

    return result;
}
