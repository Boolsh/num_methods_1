#include "Experiment.h"

void Experiment::run(const Tridiag_matrix& A, const Vector& x_exact, size_t system_size, double& abs_err_run, double& rel_err_run, double& abs_err_unst, double& rel_err_unst, bool from_one)
{
    SLAE_solver solver;

    // Вычисляем правую часть
    Vector d = A * x_exact;
    if (from_one)
    {
        std::cout << "Полученный вектор правой части: \n";
        d.print(std::cout);
    }

    // Решения двумя методами
    Vector x_run = solver.run_through_method(A, d, from_one);
    Vector x_unst = solver.unstable_method(A, d, from_one);

    if (from_one)
    {
        std::cout << "Решение методом прогонки: \n";
        x_run.print(std::cout);
        std::cout << "Решение неустойчивым методом: \n";
        x_unst.print(std::cout);
    }

    // Вычисляем ошибки
    std::pair <double, double> run = compute_errors(x_run, x_exact);
    std::pair <double, double> unst = compute_errors(x_unst, x_exact);

    abs_err_run = run.first;
    rel_err_run = run.second;
    abs_err_unst = unst.first;
    rel_err_unst = unst.second;

    if (from_one)
    {
        // Печать таблицы
        std::cout << std::setw(20) << "Абс. погр. (A)"
            << std::setw(20) << "Отн. погр. (A)"
            << std::setw(20) << "Абс. погр. (B)"
            << std::setw(20) << "Отн. погр. (B)" << "\n";

        std::cout << std::setw(20) << run.first
            << std::setw(20) << run.second
            << std::setw(20) << unst.first
            << std::setw(20) << unst.second << "\n";
    };
}

void Experiment::run_single_experiment(size_t system_size, double& abs_err_run, double& rel_err_run, double& abs_err_unst, double& rel_err_unst)
{
    Tridiag_matrix A(system_size);
    Vector x_exact(system_size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> solution_dist(-10.0, 10.0);

    A.fill_random(1.0, 10.0);
    x_exact.fill_random(-10.0, 10.0);

    bool f{ false };
    run(A, x_exact, system_size, abs_err_run, rel_err_run, abs_err_unst, rel_err_unst, f);
}

void Experiment::run_size_experiment()
{
    std::cout << "\n" << std::string(95, '=') << "\n";
    std::cout << "ЭКСПЕРИМЕНТ: Зависимость погрешности от размера системы\n";
    std::cout << std::string(95, '=') << "\n\n";

    // Логарифмическая шкала размеров (10 значений)
    std::vector<size_t> sizes = { 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };

    // Заголовок таблицы
    std::cout << std::setw(10) << "Размер"
        << std::setw(20) << "Абс. погр. (A)"
        << std::setw(20) << "Отн. погр. (A)"
        << std::setw(20) << "Абс. погр. (B)"
        << std::setw(20) << "Отн. погр. (B)"<< "\n";

    std::cout << std::string(95, '-') << "\n";

    // Проводим эксперименты для каждого размера
    for (size_t size : sizes) 
    {
        double abs_run, rel_run, abs_unst, rel_unst;
        run_single_experiment(size, abs_run, rel_run, abs_unst, rel_unst);

        // Выводим строку таблицы
        std::cout << std::setw(10) << size
            << std::setw(20) << std::setprecision(5) << abs_run
            << std::setw(20) <<  std::setprecision(5) << rel_run
            << std::setw(20) <<  std::setprecision(5) << abs_unst
            << std::setw(20) <<  std::setprecision(5) << rel_unst<<"\n";

        std::cout << "\n";
    }

    std::cout << std::string(95, '=') << "\n";
}

void Experiment::run_condition_experiments()
{
    std::cout << "\n" << std::string(95, '=') << "\n";
    std::cout << "ЭКСПЕРИМЕНТ: Сравнение хорошо и плохо обусловленных матриц\n";
    std::cout << std::string(95, '=') << "\n\n";

    std::vector<size_t> sizes = { 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };

    std::cout << std::setw(10) << "Тип/размер"
        << std::setw(20) << "Абс. погр. (A)"
        << std::setw(20) << "Отн. погр. (A)"
        << std::setw(20) << "Абс. погр. (B)"
        << std::setw(20) << "Отн. погр. (B)" << "\n";
    std::cout << std::string(95, '-') << "\n";

    std::random_device rd;
    std::mt19937 gen(rd());

    for (size_t n : sizes)
    {
        Vector x_exact(n);
        x_exact.fill_random(-10.0, 10.0);

        // Хорошо обусловленная матрица 
        Tridiag_matrix A_good(n);
        std::uniform_real_distribution<> dist_offdiag(-1.0, 1.0);
        std::uniform_real_distribution<> dist_diag(10.0, 20.0); 

        for (size_t i = 1; i <= n; i++)
        {
            // Главная диагональ большие значения
            A_good.b[i] = dist_diag(gen);

            // Внедиагональные элементы маленькие 
            if (i > 1) A_good.a[i] = dist_offdiag(gen);
            if (i < n) A_good.c[i] = dist_offdiag(gen);
        }

        double abs_run_g, rel_run_g, abs_unst_g, rel_unst_g;
        run(A_good, x_exact, n, abs_run_g, rel_run_g, abs_unst_g, rel_unst_g, false);

        std::cout << std::setw(10) << ("Good " + std::to_string(n))
            << std::setw(20) << std::setprecision(5) << abs_run_g
            << std::setw(20) << std::setprecision(5) << rel_run_g
            << std::setw(20) << std::setprecision(5) << abs_unst_g
            << std::setw(20) << std::setprecision(5) << rel_unst_g << "\n";

        // Плохо обусловленная матрица 
        Tridiag_matrix A_bad(n);
        std::uniform_real_distribution<> dist_small(-1, 1);  


        for (size_t i = 1; i <= n; i++)
        {
            A_bad.b[i] = dist_small(gen);  
            if (i > 1) A_bad.a[i] = dist_offdiag(gen) * 50.0; 
            if (i < n) A_bad.c[i] = dist_offdiag(gen) * 50.0;
        }

        A_good.a[1] = A_bad.a[1] = 0.0;
        A_good.c[n] = A_bad.c[n] = 0.0;

        double abs_run_b, rel_run_b, abs_unst_b, rel_unst_b;
        run(A_bad, x_exact, n, abs_run_b, rel_run_b, abs_unst_b, rel_unst_b, false);

        std::cout << std::setw(10) << ("Bad " + std::to_string(n))
            << std::setw(20) << std::setprecision(5) << abs_run_b
            << std::setw(20) << std::setprecision(5) << rel_run_b
            << std::setw(20) << std::setprecision(5) << abs_unst_b
            << std::setw(20) << std::setprecision(5) << rel_unst_b << "\n";

        std::cout << std::string(95, '-') << "\n";
    }

    std::cout << std::string(95, '=') << "\n";
}

std::pair<double, double> Experiment::compute_errors(const Vector& x_approx, const Vector& x_exact)
{
    Vector difference = x_approx - x_exact;
    double abs_err = difference.norm(); 
    double rel_err = 0.0;
    double eps = std::sqrt(std::numeric_limits<double>::epsilon());

    for (size_t i = 1; i <= x_exact.size(); i++)
    {
        double diff = abs(difference[i]);  
        double denom = abs(x_exact[i]);  

        if (denom < eps)
            rel_err = std::max(rel_err, diff);
        else
            rel_err = std::max(rel_err, diff / denom);
    }

    return { abs_err, rel_err };
}
