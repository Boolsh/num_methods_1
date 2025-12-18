#include "SLAE_solver.h"

Vector SLAE_solver::run_through_method(const Tridiag_matrix& A, const Vector& d, bool cons)
{
    double eps = 1e-15;

    size_t n = A.size();
    if (d.size() != n) 
        throw std::runtime_error("Размер вектора d не совпадает с размером матрицы");

    // Прямой ход метода прогонки
    Vector L(n + 1);
    Vector M(n + 1);

    if (abs(A.b[1]) < eps) throw std::runtime_error("Деление на ноль: b1 = 0");

    L[2] = A.c[1] / A.b[1];
    M[2] = d[1] / A.b[1];

    // Вычисление прогоночных коэффициентов
    for (size_t i = 2; i <= n - 1; i++)
    {
        double denominator = A.b[i] - A.a[i] * L[i];
        if (abs(denominator)  < eps)
            throw std::runtime_error("Деление на ноль при вычислении прогоночных коэффициентов");
        L[i + 1] = A.c[i] / denominator;
        M[i + 1] = (d[i] - A.a[i] * M[i]) / denominator;
    }

    // Последнее уравнение
    double denominator = A.b[n] - A.a[n] * L[n];
    if (abs(denominator) < eps)
    {
        throw std::runtime_error("Деление на ноль при вычислении M[n+1]");
    }
    M[n + 1] = (d[n] - A.a[n] * M[n]) / denominator;

    // Обратный ход
    Vector x(n);
    x[n] = M[n + 1];

    for (int i = n - 1; i >= 1; i--) 
        x[i] = M[i + 1] - L[i + 1] * x[i + 1];

    if (cons)
    {
        std::cout << "\nL: ";
        for (int i = 2; i < n + 1; i++)
            std::cout << L[i] << ' ';
        std::cout << '\n';

        std::cout << "M: ";
        for (int i = 2; i < n + 2; i++)
            std::cout << M[i] << ' ';
        std::cout << '\n';
    }

    return x;
}

 Vector SLAE_solver::unstable_method(const Tridiag_matrix& A, const Vector& d, bool cons)
{
    size_t n = A.size();
    if (d.size() != n) throw std::runtime_error("Размер вектора d не совпадает");

    double eps = 1e-15;

    Vector y(n), z(n);

    // Условия для y
    if (A.c[1] == 0.0)
        throw std::runtime_error("Деление на ноль: c[1] = 0");
    y[1] = 0;
    y[2] = d[1] / A.c[1];
    for (size_t i = 2; i <= n - 1; i++) 
    {
        if (abs(A.c[i]) < eps)
            throw std::runtime_error("Деление на ноль: c[i] = 0 при вычислении y");
        y[i + 1] = (d[i] - A.a[i] * y[i - 1] - A.b[i] * y[i]) / A.c[i];
    }

    // Условия для z
    z[1] = 1;
    z[2] = -A.b[1] / A.c[1];
    for (size_t i = 2; i <= n - 1; i++) 
        z[i + 1] = -(A.a[i] * z[i - 1] + A.b[i] * z[i]) / A.c[i];

    // Константа K
    double denom = A.a[n] * z[n - 1] + A.b[n] * z[n];
    if (abs(denom) < eps) throw std::runtime_error("Деление на ноль при вычислении K");
    double K = (d[n] - A.a[n] * y[n - 1] - A.b[n] * y[n]) / denom;

    if (cons)
    {
        std::cout << "\ny: ";
        y.print(std::cout);
        std::cout << "z: ";
        z.print(std::cout);
        std::cout << "K: ";
        std::cout << K << '\n';
    }

    // Итоговое решение
    Vector x(n);
    for (size_t i = 1; i <= n; i++) 
        x[i] = y[i] + K * z[i];
    
    return x;
}
