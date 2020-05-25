#include "model.h"

Model::Model() {}

std::pair<std::vector<double>, std::vector<std::pair<double, dvector>>> Model::count_T() {
    std::vector<std::pair<double, dvector>> results;
    dvector x;

    dvector prev_T;
    for (auto i = 0.; i <= l + h; i += h) {
        prev_T.push_back(300.);
        x.push_back(i);
    }
    results.push_back({0, prev_T});

    dvector curr_T(prev_T.size());
    std::copy(prev_T.begin(), prev_T.end(), curr_T.begin());

    for (auto time = tau; time <= 700; time += tau) {
        dvector previous(prev_T.size());
        std::copy(prev_T.begin(), prev_T.end(), previous.begin());

        while (true) {
            left_border_cond(curr_T, prev_T, time);
            right_border_cond(curr_T, prev_T);
            sweep_params(curr_T, prev_T);

            curr_T = sweep_method(arrays, left, right);

            auto max = 0.;
            for (auto i = 0ul; i < curr_T.size(); i++) {
                auto diff = std::abs(curr_T[i] - previous[i]) / curr_T[i];
                max = std::max(max, diff);
            }

            if (max < eps) {
                results.push_back({time, curr_T});
                break;
            }
            std::copy(curr_T.begin(), curr_T.end(), previous.begin());
        }

        auto max = 0.;
        for (auto i = 0ul; i < curr_T.size(); i++) {
            auto diff = std::abs(curr_T[i] - prev_T[i]) / curr_T[i];
            max = std::max(max, diff);
        }

        if (max < eps) {
            break;
        }
        std::copy(curr_T.begin(), curr_T.end(), prev_T.begin());
    }

    return {x, results};
}

double Model::F0(double time) {
    return 50;
}

double Model::k(double t) const {
//    Use this for getting results from previos lab
//    auto _k0 = 0.04;
//    auto _kN = 0.01;
//    auto _b = l * _kN / (_kN - _k0);
//    auto _a = - _k0 * _b;
//    return _a / (t - _b);

    return a1 * (b1 + c1 * std::pow(t, m1));
}

double Model::c(double t) const {
    return a2 + b2 * std::pow(t, m2) - c2 / (t * t);
}

double Model::alpha(double x) const {
    auto c = -(_alpha_N * _alpha_0 * l) / (_alpha_N - _alpha_0);
    auto d = _alpha_N * l / (_alpha_N - _alpha_0);
    // Use this for getting results from previous lab
    // c = - _alpha_0 * d;
    return c / (x - d);
}

double Model::p(double x_n) const {
    return 2 * alpha(x_n) / R;
}

double Model::f(double x_n) const {
    return  2 * alpha(x_n) * T0 / R;
}

void Model::sweep_params(dvector curr_T, dvector prev_T) {
    dvector a(1);
    dvector b(1);
    dvector c(1);
    dvector d(1);

    auto i = 1ul;
    for (auto x = 0.; x <= l; x += h, i++) {
        auto x_min_half = (k(curr_T[i]) + k(curr_T[i - 1])) / 2;
        auto x_plus_half = (k(curr_T[i]) + k(curr_T[i + 1])) / 2;
        // Use this for getting results from previos lab
        // x_min_half = 2 * k(x) * k(x-h) / (k(x) + k(x - h));
        // x_plus_half = 2 * k(x) * k(x+h) / (k(x) + k(x + h));

        a.push_back(x_min_half * tau / h);
        c.push_back(x_plus_half * tau / h);
        b.push_back(-a[a.size() - 1] - c[c.size() -1]
                - this->c(curr_T[i]) * h - p(x) * h * tau);
        d.push_back(-f(x) * h * tau - this->c(curr_T[i]) * prev_T[i] * h);
    }

    arrays.an = a;
    arrays.bn = b;
    arrays.cn = c;
    arrays.dn = d;
}

void Model::left_border_cond(dvector curr_T, dvector prev_T, double time) {
    auto c_in_half = c((curr_T[0] + curr_T[1]) / 2);
    auto c_0 = c(curr_T[0]);

    auto hi_in_half = (k(curr_T[0]) + k(curr_T[1])) / 2;
    // Use this for getting results from previos lab
    // hi_in_half = 2 * k(0) * k(h) / (k(0) + k(h));

    auto p_0 = p(0.);
    auto p_in_half = (p_0 + p(h)) / 2;

    auto f_0 = f(0.);
    auto f_in_half = (f_0 + f(h)) / 2;

    auto k0 = h * c_in_half / 8 + h * c_0 / 4 + tau * hi_in_half / h
            + tau * h * p_in_half / 8 + tau * h * p_0 / 4;
    auto m0 = h * c_in_half / 8 - tau * hi_in_half / h + tau * h * p_in_half / 8;
    auto p0 = h * c_in_half / 8 * (prev_T[0] + prev_T[1])
            + h * c_0 / 4 * prev_T[0] + F0(time) * tau + tau * h / 4 * (f_in_half + f_0);

    left.k = k0;
    left.m = m0;
    left.p = p0;
}

void Model::right_border_cond(dvector curr_T, dvector prev_T) {
    auto N = curr_T.size() - 1;

    auto c_N_min_half = c((curr_T[N] + curr_T[N - 1]) / 2);
    auto c_N = c(curr_T[N]);

    auto hi_N_min_half = (k(curr_T[N]) + k(curr_T[N - 1])) / 2;
    // Use this for getting results from previos lab
    // hi_N_min_half = 2 * k(l) * k(l-h) / (k(l) + k(l - h));

    auto p_N = p(l);
    auto p_N_min_half = (p_N + p(l - h)) / 2;

    auto f_N = f(l);
    auto f_N_min_half = (f_N + f(l - h)) / 2;

    auto kN = h * c_N / 4 + h * c_N_min_half / 8 + hi_N_min_half * tau / h
            + _alpha_N * tau + p_N * tau * h / 4 + p_N_min_half * tau * h / 8;
    auto mN = h * c_N_min_half / 8 - hi_N_min_half * tau / h
            + p_N_min_half * tau * h / 8;
    auto pN = h * c_N * prev_T[N] / 4 + h * c_N_min_half * (prev_T[N] + prev_T[N - 1]) / 8
            + _alpha_N * T0 * tau + (f_N + f_N_min_half) * tau * h / 4;

    right.k = kN;
    right.m = mN;
    right.p = pN;
}
