#include <functional>
#include <vector>
#include <iostream>
#include <cmath>
#include <chrono>
#include "polynomial.h"

std::vector<Polynomial> pikars_polynomials(const int order) {
    double square_c[] = {0., 0., 1.};
    Polynomial square_pol(2, square_c);

    double start_c[] = {0};
    Polynomial start_pol(0, start_c);
    std::vector<Polynomial> polynomials{start_pol};

    for (auto i = 0ul; i < order; i++) {
        auto pol = square_pol + (polynomials.at(i) * polynomials.at(i));
        auto integrated_pol = pol.integrate();
        polynomials.push_back(integrated_pol);
    }

    return polynomials;
}

std::vector<double> pikar_method(const int order,
                                 const std::vector<Polynomial> &polynomials,
                                 const std::vector<double> &values) {
    std::vector<double> result(values.size());

    for (auto i = 0ul; i < values.size(); i++) {
        auto val = polynomials[order].value(values.at(i));
        result[i] = val;
    }

    return result;
}

std::vector<double> explicit_method(const std::vector<double> &values, const double h) {
    auto y = 0.;
    std::vector<double> result{y};

    auto x_num = 1;
    for (auto x = h; x_num < values.size(); x += h) {
        auto disc = 1 - 4 * h * (y + h * x * x);
        y = (1 - std::sqrt(disc)) / (2 * h);
        if (x >= values.at(x_num)) {
            x_num++;
            result.push_back(y);
        }
    }

    return result;
}

std::vector<double> implicit_method(const std::vector<double> &values, const double h) {
    auto y = 0.;
    std::vector<double> result{y};

    auto x_num = 1;
    for (auto x = 0.; x_num < values.size(); x += h) {
        y += h * (x * x + y * y);
        if (x + h >= values.at(x_num)) {
            x_num++;
            result.push_back(y);
        }
    }

    return result;
}

void print_table(std::vector<std::vector<double>> values, const int order) {
    std::cout.precision(4);
    std::cout << "    x   |           Picard's method            | Implicit method | Explicit method"
              << std::endl;
    std::cout.width(4);
    std::cout << "        | " << order - 2 << " approx.  | " << order - 1 << " approx.  | " << order << " approx.  |"
              << "                 |" << std::endl;
    std::cout << "────────┼────────────┼────────────┼────────────┼─────────────────┼────────────────" << std::endl;
    std::cout.setf(std::ios::scientific);
    for (auto i = 0ul; i < values[0].size(); i++) {
        std::cout.width(7);

        std::cout << std::fixed << values[0][i] << std::scientific;
        std::cout << " | " << values[1][i] << " | " << values[2][i] << " | " << values[3][i] << " |   ";
        std::cout << values[4][i] << "    |  " << values[5][i] << std::endl;
    }
}

int main() {
    auto order = 0;
    auto h = 0.;
    auto x_min = 0.;
    auto x_max = 0.;
    auto print_step = 0.;

    std::cout << "Enter Picard's method order: ";
    std::cin >> order;
    std::cout << "Enter h for numeric methods: ";
    std::cin >> h;
    std::cout << "Enter min x: ";
    std::cin >> x_min;
    std::cout << "Enter max x: ";
    std::cin >> x_max;
    std::cout << "Enter print step: ";
    std::cin >> print_step;
    std::cout << std::endl;

    std::vector<double> values;
    for (auto i = x_min; i < x_max + print_step; i += print_step) values.push_back(i);

    auto implicit_result = implicit_method(values, h);
    auto explicit_result = explicit_method(values, h);

    auto begin = std::chrono::steady_clock::now();

    auto pols = pikars_polynomials(order);
    auto pikars_result_3 = pikar_method(order, pols, values);
    auto pikars_result_2 = pikar_method(order - 1, pols, values);
    auto pikars_result_1 = pikar_method(order - 2, pols, values);

    auto end = std::chrono::steady_clock::now();

    print_table({values, pikars_result_1, pikars_result_2, pikars_result_3, implicit_result, explicit_result}, order);

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << std::endl << std::endl << "Time: " << elapsed_ms.count() << std::endl;

    return 0;
}