#include <iostream>
#include <Windows.h>
#include "Vector.h"
#include "Tridiag_matrix.h"
#include "SLAE_solver.h"
#include "Experiment.h"

using SolverFunction = Vector(*)(const Tridiag_matrix&, const Vector&, bool cons);

int read_and_check(const char* message, int a, int b);
int main_menu();
int variants();
int variants2();
void solve_system(const std::string& method_name, SolverFunction solver);
void one_exp();

int main()
{
	SetConsoleOutputCP(1251);

	int menu{}; 
	do
	{
		menu = main_menu();
		switch (menu)
		{
		case 1:
		{
			solve_system("методом прогонки", SLAE_solver::run_through_method);
			break;
		}
		case 2:
		{
			solve_system("неустойчивым методом", SLAE_solver::unstable_method);
			break;
		}
		case 3:
		{
			one_exp();
			break;
		}
		case 4:
		{
			Experiment::run_size_experiment();
			break;
		}
		case 5:
		{
			Experiment::run_condition_experiments(); 
			break;
		}
		}
	} while (menu != 6);

	return 0;
}

int read_and_check(const char* message, int a, int b)
{
	int x;
	std::cout << message;
	while (!(std::cin >> x && (x >= a && x <= b)))
	{
		std::cout << "Ошибка ввода!\n";
		std::cin.clear();
		std::cin.ignore(std::cin.rdbuf()->in_avail());
		std::cout << message;
	}
	return x;
}

int main_menu()
{
	std::cout << "\n=== Главное меню ===\n";
	std::cout << "1. Решение системы уравнений методом прогонки\n";
	std::cout << "2. Решение системы уравнений неустойчивым методом\n";
	std::cout << "3. Одиночный вычислительный эксперимент\n";
	std::cout << "4. Вычислительный эксперимент для разных размеров\n";
	std::cout << "5. Сравнение хорошо и плохо обусловленных матриц\n";
	std::cout << "6. Выход\n";

	return read_and_check("\nВыберите действие: ", 1, 6);
}

int variants()
{
	std::cout << "\n=== Выбор ввода ===\n";
	std::cout << "1. Загрузить матрицу и вектор из файлов\n";
	std::cout << "2. Ввести матрицу и вектор вручную\n";
	std::cout << "3. Сгенерировать матрицу и вектор рандомным образом\n";
	std::cout << "4. Главное меню\n";

	return read_and_check("\nВыберите действие: ", 1, 4);
}

int variants2()
{
	std::cout << "\n=== Выбор ввода ===\n";
	std::cout << "1. Загрузить матрицу и вектор из файлов\n";
	std::cout << "2. Сгенерировать матрицу и вектор рандомным образом\n";
	std::cout << "3. Главное меню\n";

	return read_and_check("\nВыберите действие: ", 1, 3);
}

void solve_system(const std::string& method_name, SolverFunction solver)
{
	int var{};
	do
	{
		std::cout << "\n=== Решение системы уравнений " << method_name << " == = ";
		var = variants();
		switch (var)
		{
		case 1:
		{
			std::ifstream file("matrix.txt");
			std::ifstream file2("vector.txt");
			Tridiag_matrix A(5);
			A.read_from_file(file);
			std::cout << "Исходная матрица: \n";
			A.print_full();
			Vector d(file2);
			std::cout << "Вектор правой части: \n";
			d.print(std::cout);
			Vector res = solver(A, d, true);
			std::cout << "Решение: \n";
			res.print(std::cout);
			break;
		}
		case 2:
		{
			std::cout << "Введите размер матрицы: \n";
			size_t n{};
			std::cin >> n;
			Tridiag_matrix A(n);
			A.read_from_console();
			std::cout << "Введите вектор правой части: " << n << " значений (через пробел), в конце CTRL+Z\n";
			Vector d(std::cin);
			Vector res = solver(A, d, true);
			std::cout << "Решение: \n";
			res.print(std::cout);
			break;
		}
		case 3:
		{
			std::cout << "Введите размер матрицы: \n";
			size_t n{};
			std::cin >> n;
			Tridiag_matrix A(n);
			double mi{}, ma{};
			std::cout << "Введите нижнюю границу диапазона: \n";
			std::cin >> mi;
			std::cout << "Введите верхнюю границу диапазона: \n";
			std::cin >> ma;
			A.fill_random(mi, ma);
			Vector d(n);
			d.fill_random(mi, ma);
			std::cout << "Исходная матрица: \n";
			A.print_full();
			std::cout << "Вектор правой части: \n";
			d.print(std::cout);
			Vector res = solver(A, d, true);
			std::cout << "Решение: \n";
			res.print(std::cout);
			break;
		}
		}
	} while (var != 4);

}

void one_exp()
{
	int var{};
	do
	{
		std::cout << "\n=== Одиночный вычислительный эксперимент ===\n";
		var = variants2();
		switch (var)
		{
		case 1:
		{
			std::ifstream file("matrix_exp.txt");
			std::ifstream file2("vector_exp.txt");
			Tridiag_matrix A(10);
			A.read_from_file(file);
			std::cout << "Исходная матрица: \n";
			A.print_full();
			Vector x(file2);
			std::cout << "Известное точное решение: \n";
			x.print(std::cout);
			double abs_run, rel_run, abs_unst, rel_unst;
			Experiment::run(A, x, A.size(), abs_run, rel_run, abs_unst, rel_unst, true);
			break;
		}
		case 2:
		{
			std::cout << "Введите размер матрицы: \n";
			size_t n{};
			std::cin >> n;
			Tridiag_matrix A(n);
			double mi{}, ma{};
			std::cout << "Введите нижнюю границу диапазона: \n";
			std::cin >> mi;
			std::cout << "Введите верхнюю границу диапазона: \n";
			std::cin >> ma;
			A.fill_random(mi, ma);
			Vector x(n);
			x.fill_random(mi, ma);
			std::cout << "Исходная матрица: \n";
			A.print_full();
			std::cout << "Вектор точного решения: \n";
			x.print(std::cout);
			double abs_run, rel_run, abs_unst, rel_unst;
			Experiment::run(A, x, n, abs_run, rel_run, abs_unst, rel_unst, true);
		}
		}
	} while (var != 3);
}