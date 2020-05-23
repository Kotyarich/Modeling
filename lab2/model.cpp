#include "model.h"

Model::Model(double lk, double ck, double rk, double uc0, double i0, method m) :
    Lk(lk), Ck(ck), Rk(rk), Uc0(uc0), I0(i0) {
    _method = m;
}

void Model::start_modeling() {
    auto dI_lambda = [&](double t, double Uc, double I) {
      auto rp = this->count_rp(std::abs(I));
      // rp = 0.; thus row is needed if u want to run modelling wihout resistance
      auto dI = (Uc - (this->Rk + rp) * I) / this->Lk;
      return dI;
    };
    auto dU_lambda = [&](double t, double Uc, double I) {
        return -I / this->Ck;
    };

    auto Ii = this->I0;
    cur_vec.push_back(Ii);
    rp_vec.push_back(count_rp(Ii));
    t0_vec.push_back(count_t0(Ii));
    auto Ui = this->Uc0;
    uc_vec.push_back(Ui);
    for (auto time = time_start; time <= time_end; time += time_step) {
        auto result_i = _method(dI_lambda, dU_lambda, time, Ui, Ii, time_step);
        cur_vec.push_back(result_i.second);
        uc_vec.push_back(result_i.first);
        rp_vec.push_back(count_rp(result_i.second));
        t0_vec.push_back(count_t0(result_i.second));

        Ii = result_i.second;
        Ui = result_i.first;
        time_vec.push_back(time);
    }
}

double Model::count_rp(const double I) {
    auto sigma_table = get_sigma_table();
    auto t0 = count_t0(std::abs(I));

    auto denominator = 2. * M_PI * R * R * integrate(0, 1, [&](double z) {
        auto t = this->count_t(z, std::abs(I), t0);
        auto sigma = linear_interpolation(sigma_table[0], sigma_table[1], t);
        sigma = std::exp(sigma);
        return sigma * z;
    });

    auto rp = Le / denominator;

    return rp;
}

double Model::count_t(const double z, const double I, const double t0) {
    auto table = get_m_table();
    auto m = linear_interpolation(table[0], table[2], std::abs(I));
    auto t = t0 + (Tw - t0) * std::pow(z, m);
    return t;
}

double Model::count_t0(const double I) {
    auto m_table = get_m_table();
    auto t0 = linear_interpolation(m_table[0], m_table[1], std::abs(I));
    return t0;
}

d_vector Model::get_t0_vec() const { return t0_vec; }

d_vector Model::get_rp_vec() const { return rp_vec; }

d_vector Model::get_uc_vec() const { return uc_vec; }

d_vector Model::get_cur_vec() const { return cur_vec; }

d_vector Model::get_time_vec() const { return  time_vec; }

d_vector Model::get_lamp_vec() const {
    d_vector result(time_vec.size());

    for (auto i = 0ul; i < time_vec.size(); i++) {
        result[i] = cur_vec[i] * rp_vec[i];
    }

    return result;
}
