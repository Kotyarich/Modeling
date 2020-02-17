#include <cstring>
#include <algorithm>
#include "polynomial.h"

Polynomial::Polynomial(size_t a)
    : _a(a), _c(a + 1) {
    for (auto i = 0; i <= a; i++) {
        _c[i] = 0;
    }
}

Polynomial::Polynomial(size_t a, double *c)
    : _a(a), _c(a + 1) {
    for (auto i = 0ul; i <= a; i++) {
        _c[i] = c[i];
    }
}

// In program we using it only for adding some polynomial to t^2/3,
// so it's optimised for this operation
Polynomial Polynomial::operator+(const Polynomial &obj) {
    Polynomial tmp = obj;
    tmp._c[2] += _c[2];

    return tmp;
}

Polynomial Polynomial::operator*(const Polynomial &obj) {
    // indexes optimised for function u' = x^2 + u^2
    // as we need to multiply only polynomials with degrees like 3 + 4*i,
    // where i is natural number
    Polynomial tmp(_a + obj._a);

    if (_a > 100) {
        std::vector<std::thread> threads(4);
        auto lines_for_thr = static_cast<double>(_a) / 4;
        for (auto i = 0ul; i < 4; i++) {
            auto start = static_cast<size_t>(i * lines_for_thr);
            start += 3 - (start % 4);
            auto end = static_cast<size_t>(i == 3 ? _a + 1 : (i + 1) * lines_for_thr);
            auto thread = std::thread(
                [&](size_t start, size_t end) {
                    for (auto k = start; k <= end; k += 4) {
                        for (auto j = 3; j <= obj._a; j += 4) {
                            tmp._c[k + j] += _c[k] * _c[j];
                        }
                    }
                }, start, end);
            threads[i] = std::move(thread);
        }
        for (auto &thr: threads) {
            if (thr.joinable()) {
                thr.join();
            }
        }
    }
    else {
        for (auto i = 3; i <= _a; i += 4) {
            for (auto j = 3; j <= obj._a; j += 4) {
                tmp._c[i + j] += _c[i] * _c[j];
            }
        }
    }

    return tmp;
}

double Polynomial::value(double x) const {
    auto result = _c[0];
    // indexes optimised for function u' = x^2 + u^2
    // as we need to get value of polynomials with degrees like 3 + 4*i,
    // where i is natural number
    for (auto i = 3; i <= _a; i += 4) {
        auto val = _c[i];
        if (val != 0) {
            for (auto j = 0; j < i; j++) {
                val *= x;
            }
        }
        result += val;
    }

    return result;
}

Polynomial Polynomial::integrate() {
    Polynomial tmp(_a + 1);
    // indexes optimised for function u' = x^2 + u^2
    // as we need to integrate only polynomials with degrees like 3 + 4*i,
    // where i is natural number
    if (_a > 100) {
        std::vector<std::thread> threads(4);
        auto lines_for_thr = static_cast<double>(_a) / 4;
        for (auto i = 0ul; i < 4; i++) {
            auto start = static_cast<size_t>(i * lines_for_thr);
            start += 3 - (start % 4);
            auto end = static_cast<size_t>(i == 3 ? _a + 1 : (i + 1) * lines_for_thr);
            auto thread = std::thread(
                [&](size_t start, size_t end) {
                    for (auto k = start; k <= end; k += 4) {
                        tmp._c[k] = _c[k - 1] / k;
                    }
                }, start, end);
            threads[i] = std::move(thread);
        }
        for (auto &thr: threads) {
            if (thr.joinable()) {
                thr.join();
            }
        }
    }
    else {
        for (int i = 3; i <= _a + 1; i += 4) {
            tmp._c[i] = _c[i - 1] / i;
        }
    }

    return tmp;
}

