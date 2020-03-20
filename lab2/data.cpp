#include "data.h"
#include <cmath>

vector<vector<double>> get_m_table_old() {
    return {
        {0.5, 1., 5., 10., 50., 200., 400., 800., 1200.},
        {6400., 6790., 7150., 7270., 8010., 9185., 10010., 11140., 12010.},
        {0.4, 0.55, 1.7, 3., 11., 32., 40., 41., 39.},
    };
}

vector<vector<double>> get_m_table() {
    return {
        {0.5, 1., 5., 10., 50., 200., 400., 800., 1200.},
        {6700., 6790., 7150., 7270., 8010., 9185., 10010., 11140., 12010.},
        {0.5, 0.55, 1.7, 3., 11., 32., 40., 41., 39.},
    };
}

vector<vector<double>> get_sigma_table() {
    vector<vector<double>> table{
        {4000., 5000., 6000., 7000., 8000., 9000., 10000., 11000., 12000., 13000., 14000.},
        {0.031, 0.27, 2.05, 6.06, 12., 19.9, 29.6, 41.1, 54.1, 67.7, 81.5},
    };


    for (auto &item: table[1]) {
        item = std::log(item);
    }

    return table;
}