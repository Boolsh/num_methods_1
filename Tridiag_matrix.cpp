#include "Tridiag_matrix.h"

void Tridiag_matrix::read_from_file(std::ifstream& file)
{
    std::string line;

    {
        std::getline(file, line);
        std::stringstream ss(line);
        Vector a_in(ss);
        a = a_in;
    }
    {
        std::getline(file, line);
        std::stringstream ss(line);
        Vector b_in(ss);
        b = b_in;
    }
    {
        std::getline(file, line);
        std::stringstream ss(line);
        Vector c_in(ss);
        c = c_in;
    }
}

void Tridiag_matrix::read_from_console()
{
    std::cout << "Введите поддиагональ a (a2..an):\n";
    a[1] = 0;
    for (size_t i = 2; i <= n; i++) std::cin >> a[i];

    std::cout << "Введите главную диагональ b (b1..bn):\n";
    for (size_t i = 1; i <= n; i++) std::cin >> b[i];

    std::cout << "Введите наддиагональ c (c1..c{n-1}):\n";
    for (size_t i = 1; i <= n-1; i++) std::cin >> c[i];
    c[n] = 0;
}

void Tridiag_matrix::fill_random(double min, double max)
{
    b.fill_random(min, max);
    a.fill_random(min, max);
    a[1] = 0.0;
    c.fill_random(min, max);
    c[c.size()] = 0.0;
}

Tridiag_matrix Tridiag_matrix::operator+(const Tridiag_matrix& other) const
{
    if (n != other.n) throw std::runtime_error("Размеры матриц не совпадают");

    Tridiag_matrix result(n);
    for (size_t i = 1; i <= n; i++) 
    {
        result.a[i] = a[i] + other.a[i];
        result.b[i] = b[i] + other.b[i];
        result.c[i] = c[i] + other.c[i];
    }
    return result;
}

Tridiag_matrix Tridiag_matrix::operator-(const Tridiag_matrix& other) const
{
    if (n != other.n) throw std::runtime_error("Размеры матриц не совпадают");

    Tridiag_matrix result(n);
    for (size_t i = 1; i <= n; i++) {
        result.a[i] = a[i] - other.a[i];
        result.b[i] = b[i] - other.b[i];
        result.c[i] = c[i] - other.c[i];
    }
    return result;
}

Vector Tridiag_matrix::operator*(const Vector& x) const
{
    if (x.size() != n) throw std::runtime_error("Размер вектора не совпадает");
    Vector result(n);

    for (size_t i = 1; i <= n; i++) 
    {
        double val = b[i] * x[i];
        if (i > 1) val += a[i] * x[i - 1];
        if (i < n) val += c[i] * x[i + 1];
        result[i] = val;
    }
    return result;
}

void Tridiag_matrix::print(std::ostream& out) const
{
    a.print(out);
    b.print(out);
    c.print(out);
}

void Tridiag_matrix::print_full()
{
    const int width = 10; 
    for (size_t i = 1; i <= n; i++)
    {
        for (size_t j = 1; j <= n; j++)
        {
            double val = 0.0;
            if (j == i) val = b[i];
            else if (j == i - 1) val = a[i];
            else if (j == i + 1) val = c[i];

            if (val == 0.0) 
                std::cout << std::setw(width) << " ";
            else 
                std::cout << std::setw(width) << val;
        }
        std::cout << "\n";
    }
}

