#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include "Vector.h"   
#include <iomanip>
#include <cmath>
#include <limits>
#include <functional>

class Tridiag_matrix
{
private:
    size_t n;     // размер матрицы 
public:
    Vector a;     // поддиагональ (a2...an)
    Vector b;     // главная диагональ (b1...bn)
    Vector c;     // наддиагональ (c1...c{n-1})

    Tridiag_matrix(size_t n_) : n(n_), a(n_), b(n_), c(n_) {};
    size_t size() const { return n; }
    void read_from_file(std::ifstream& file);
    void read_from_console();
    void fill_random(double min, double max);

    Tridiag_matrix operator+(const Tridiag_matrix& other) const;
    Tridiag_matrix operator-(const Tridiag_matrix& other) const;
    Vector operator*(const Vector& x) const;

    void print(std::ostream& out) const;
    void print_full();
};

