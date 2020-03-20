#ifndef MODELING_H
#define MODELING_H

#include "methods.h"
#include "data.h"

class Model {
public:
    Model(double lk, double ck, double rk, double uc0, double i0, method m);

    void start_modeling();

    d_vector get_t0_vec() const;
    d_vector get_rp_vec() const;
    d_vector get_uc_vec() const;
    d_vector get_cur_vec() const;
    d_vector get_time_vec() const;
    d_vector get_lamp_vec() const;

private:
    static const constexpr auto Tw = 2000.;
    static const constexpr auto time_start = 0.;
    static const constexpr auto time_end = 10e-6;
    static const constexpr auto time_step = 0.5e-6;

    double Lk;
    double Ck;
    double Rk;
    double Uc0;
    double I0;

    d_vector t0_vec;
    d_vector rp_vec;
    d_vector uc_vec;
    d_vector cur_vec;
    d_vector time_vec;

    method *_method;

    double count_rp(const double I);
    double count_t(const double z, const double I, const double t0);
    double count_t0(const double I);
};

#endif // MODELING_H
