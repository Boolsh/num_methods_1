#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <cmath>
#include <stdexcept>

class Vector 
{
private:
	size_t n;
	double* data;
public:
	Vector(size_t n_) : n(n_) 
	{
		data = new double[n + 1];  
		for (size_t i = 1; i <= n; i++) 
			data[i] = 0.0;
	};
	Vector(std::istream& is);
	Vector(const Vector& other);
	~Vector();
	
	Vector& operator=(const Vector& other);
	size_t size() const { return n; };
	double& operator[](size_t i);
	const double& operator[](size_t i) const;

	Vector operator+(const Vector& other) const;
	Vector operator-(const Vector& other) const;
	//double dot(const Vector& other) const;
	double norm() const;

	void fill_random(double min, double max);
	void print(std::ostream& out) const;
};
