#pragma once
#include "Tridiag_matrix.h"
#include "SLAE_solver.h"
#include "Vector.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <functional>

class Experiment
{
public:
	static void run(const Tridiag_matrix& A, const Vector& x_exact, size_t system_size, double& abs_err_run, double& rel_err_run, double& abs_err_unst, double& rel_err_unst, bool from_one);
    static void run_single_experiment(size_t system_size, double& abs_err_run, double& rel_err_run,  double& abs_err_unst, double& rel_err_unst);
    static void run_size_experiment();
	static void run_condition_experiments();

private:
	static std::pair<double, double> compute_errors(const Vector& x_approx, const Vector& x_exact);
};

