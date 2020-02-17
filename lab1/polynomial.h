#ifndef LAB1_POLYNOM_H
#define LAB1_POLYNOM_H


#include <thread>
#include <vector>
#include <iostream>

class Polynomial {
public:
    explicit Polynomial(size_t a);
    Polynomial(size_t a, double *c);

    Polynomial operator+(const Polynomial &obj);
    Polynomial operator*(const Polynomial &obj);
    Polynomial integrate();
    double value(double x) const;

private:
    std::vector<double> _c;
    size_t _a;
};


#endif //LAB1_POLYNOM_H
