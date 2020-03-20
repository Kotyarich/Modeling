#ifndef DATA_H
#define DATA_H

#include <vector>

using std::vector;

static const constexpr double R = 0.35;
static const constexpr double Le = 12;

vector<vector<double>> get_m_table();
vector<vector<double>> get_sigma_table();

#endif // DATA_H
