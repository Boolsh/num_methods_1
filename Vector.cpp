#include "Vector.h"

Vector::Vector(std::istream& is)
{
    std::vector<double> temp;
    double value;
    while (is >> value) 
        temp.push_back(value);
    if (temp.empty()) throw std::runtime_error("Не удалось считать вектор!");

    n = temp.size();
    data = new double[n + 1]; // индекс 0 не используем
    for (size_t i = 0; i < n; i++) 
        data[i + 1] = temp[i];
}

Vector::Vector(const Vector& other)
{
    n = other.n;
    data = new double[n + 1];
    for (size_t i = 1; i <= n; i++) 
        data[i] = other.data[i];
}

Vector::~Vector()
{
    delete[] data;
}

Vector& Vector::operator=(const Vector& other)
{
    if (this == &other) return *this;
    delete[] data;
    n = other.n;
    data = new double[n + 1];
    for (size_t i = 1; i <= n; i++) 
        data[i] = other.data[i];
    return *this;
}

double& Vector::operator[](size_t i)
{
    if (i < 1 || i > n) throw std::out_of_range("Неверный индекс (индексация с 1).");
    return data[i];
}

const double& Vector::operator[](size_t i) const
{
    if (i < 1 || i > n) throw std::out_of_range("Неверный индекс (индексация с 1).");
    return data[i];
}

Vector Vector::operator+(const Vector& other) const
{
    if (n != other.n) throw std::runtime_error("Размеры векторов не совпадают.");
    Vector result = *this;
    for (size_t i = 1; i <= n; i++) 
        result[i] += other[i];
    return result;
}

Vector Vector::operator-(const Vector& other) const
{
    if (n != other.n) throw std::runtime_error("Размеры векторов не совпадают.");
    Vector result = *this;
    for (size_t i = 1; i <= n; i++) 
        result[i] -= other[i];
    return result;
}

//double Vector::dot(const Vector& other) const
//{
//    if (n != other.n) throw std::runtime_error("Размеры векторов не совпадают.");
//    double result = 0.0;
//    for (size_t i = 1; i <= n; i++) 
//        result += data[i] * other[i];
//    return result;
//}

double Vector::norm() const
{
    double max_val = std::numeric_limits<double>::min();
    for (size_t i = 1; i <= n; i++)
        max_val = std::max(max_val, abs(data[i]));
    return max_val;
}

void Vector::fill_random(double min, double max)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(min, max);

    for (size_t i = 1; i <= n; i++) 
        data[i] = dist(gen);
}

void Vector::print(std::ostream& out) const
{
    for (size_t i = 1; i <= n; i++) 
        out << data[i] << " ";
    out << std::endl;
}
