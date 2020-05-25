#include "methods.h"

dvector sweep_method(const sweep_arrays &params,
                     const border_conditions &left,
                     const border_conditions &right) {
    dvector etta(params.an.size() + 1);
    dvector tetta(params.an.size() + 1);
    dvector y(params.an.size());

    etta[1] = - left.m / left.k;
    tetta[1] = left.p / left.k;

    // direct stroke of the method
    for (auto i = 1ul; i < params.an.size(); i++) {
        auto determinator = params.bn[i] - params.an[i] * etta[i];
        etta[i + 1] = params.cn[i] / determinator;
        tetta[i + 1] = (params.dn[i] + params.an[i] * tetta[i]) / determinator;
    }

    // reverse stroke
    y[y.size() - 1] = (right.p - right.m * tetta[tetta.size() - 1])
            / (right.k + right.m * etta[etta.size() - 1]);

    for (auto i = static_cast<long long>(y.size() - 2); i >= 0; i--) {
        auto next_i = static_cast<size_t>(i + 1);
        y[static_cast<size_t>(i)] = etta[next_i] * y[next_i] + tetta[next_i];
    }

    return y;
}
