#pragma once
#include "Tridiag_matrix.h"
#include "Vector.h"
#include <stdexcept>

class SLAE_solver
{
public:
	static Vector run_through_method(const Tridiag_matrix& A, const Vector& d, bool cons);
	static Vector unstable_method(const Tridiag_matrix& A, const Vector& d, bool cons);
};
