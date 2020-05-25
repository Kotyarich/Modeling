#include "model.h"

Model::Model(double a0, double aN, double k0, double kN, double F0)
    : _a0(a0), _aN(aN), _k0(k0), _kN(kN), _F0(F0) {}

std::pair<dvector, dvector> Model::count_T() {
    count_a_params();
    count_k_params();

    sweep_params();
    left_border_cond();
    right_border_cond();

    dvector x;
    for (auto i = 0.; i <= l; i += h) {
        x.push_back(i);
    }

    auto result = sweep_method(arrays, left, right);

    return {x, result};
}

double Model::k(double x) const {
    return _a / (x - _b);
}

double Model::a(double x) const {
    return _c / (x - _d);
}

double Model::p(double x_n) const {
    return 2 * a(x_n) / R;
}

double Model::f(double x_n) const {
    return  2 * a(x_n) * T0 / R;
}

double Model::x_plus_half(double x_n) const {
    auto k_curr = k(x_n);
    auto k_next = k(x_n + h);
    return (k_curr + k_next) / 2;
    return 2. * k_curr * k_next / (k_curr + k_next);
}

double Model::x_minus_half(double x_n) const {
    auto k_curr = k(x_n);
    auto k_prev = k(x_n - h);
    return (k_curr + k_prev) / 2;
    return 2. * k_curr * k_prev / (k_curr + k_prev);
}

void Model::sweep_params() {
    dvector a(1);
    dvector b(1);
    dvector c(1);
    dvector d(1);

    for (auto x = 0.; x <= l; x += h) {
        auto x_prev = x_minus_half(x);
        auto x_next = x_plus_half(x);

        a.push_back(x_prev / h);
        c.push_back(x_next / h);
        b.push_back(a[a.size() - 1] + c[c.size() -1] + p(x) * h);
        d.push_back(f(x) * h);
    }

    arrays.an = a;
    arrays.bn = b;
    arrays.cn = c;
    arrays.dn = d;
}

void Model::left_border_cond() {
    auto x_in_half = x_plus_half(0);
    auto sqr_h = h * h;

    auto tmp = sqr_h * (p(0) + p(h)) / 16;

    auto k0 = x_in_half + tmp + sqr_h * p(0) / 4;
    auto m0 = tmp - x_in_half;
    auto p0 = h * _F0 + sqr_h * ((f(0) + f(h)) / 2 + f(0)) / 4;

    left.k = k0;
    left.m = m0;
    left.p = p0;
}

void Model::right_border_cond() {
    auto x_in_half = x_minus_half(l);

    auto tmp = h * (p(l) + p(l - h)) / 16;

    auto kN = - x_in_half / h - _aN - tmp - h * p(l) / 4;
    auto mN = x_in_half / h - tmp;
    auto pN = -(_aN * T0 + h * ((f(l) + f(l - h)) / 2 + f(l)) / 4);

    right.k = kN;
    right.m = mN;
    right.p = pN;
}

void Model::count_k_params() {
    _b = l * _kN / (_kN - _k0);
    _a = - _k0 * _b;
}

void Model::count_a_params() {
    _d = l * _aN / (_aN - _a0);
    _c = - _a0 * _d;
}
