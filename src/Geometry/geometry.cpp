#include <bits/stdc++.h>

template <class _derivedType, unsigned int _dimension, class _T = long double>
struct VectorBase
{
    std::array<_T, _dimension> coordinates;

    template <class __derivedType>
    _derivedType operator+(const VectorBase<__derivedType, _dimension, _T> &other)
    {
        _derivedType sumVector = _derivedType();
        for (unsigned int i = 0; i < _dimension; ++i)
            sumVector.coordinates[i] = this->coordinates[i] + other.coordinates[i];
        return sumVector;
    }

    template <class __derivedType>
    _derivedType &operator+=(const VectorBase<__derivedType, _dimension, _T> &other)
    {
        *this = *this + other;
        return *this;
    }

    template <class __derivedType>
    _derivedType operator-(const VectorBase<__derivedType, _dimension, _T> &other)
    {
        _derivedType diffVector = _derivedType();
        for (unsigned int i = 0; i < _dimension; ++i)
            diffVector.coordinates[i] = this->coordinates[i] - other.coordinates[i];
        return diffVector;
    }

    template <class __derivedType>
    _derivedType &operator-=(const VectorBase<__derivedType, _dimension, _T> &other)
    {
        *this = *this - other;
        return *this;
    }

    template <class __derivedType>
    _T dotProduct(const VectorBase<__derivedType, _dimension, _T> &other)
    {
        _T sum = _T(0);
        for (unsigned int i = 0; i < _dimension; ++i)
            sum += this->coordinates[i] * other.coordinates[i];
        return sum;
    }

    template <class __derivedType>
    _T operator*(const VectorBase<__derivedType, _dimension, _T> &other) { return dotProduct(other); }
};

template <class _derivedType, unsigned int _dimension, class _T = long double>
std::ostream &operator<<(std::ostream &stream, VectorBase<_derivedType, _dimension, _T> vec)
{
    stream << '(';
    for (unsigned int i = 0; i < _dimension - 1; ++i)
        stream << vec.coordinates[i] << ", ";
    if (_dimension > 0)
        stream << vec.coordinates[_dimension - 1];
    stream << ')';
    return stream;
}

template <class _T = long double>
struct Vector2 : VectorBase<Vector2<_T>, 2, _T>
{
    using VectorBase<Vector2<_T>, 2, _T>::coordinates;
    const _T &x = coordinates[0];
    const _T &y = coordinates[1];
};

template <class _T = long double>
struct Vector3 : VectorBase<Vector3<_T>, 3, _T>
{
    using VectorBase<Vector3<_T>, 3, _T>::coordinates;
    const _T &x = coordinates[0];
    const _T &y = coordinates[1];
    const _T &z = coordinates[2];

    Vector3 crossProduct(const Vector3 &other)
    {
        return Vector3({this->coordinates[1] * other.coordinates[2] - this->coordinates[2] * other.coordinates[1],
                        -(this->coordinates[0] * other.coordinates[2]) + this->coordinates[2] * other.coordinates[0],
                        this->coordinates[0] * other.coordinates[1] - this->coordinates[1] * other.coordinates[0]});
    }
};

template <class _T = long double>
struct Vector4 : VectorBase<Vector4<_T>, 4, _T>
{
    using VectorBase<Vector4<_T>, 4, _T>::coordinates;
    const _T &x = coordinates[0];
    const _T &y = coordinates[1];
    const _T &z = coordinates[2];
    const _T &w = coordinates[3];
};

int main()
{
    Vector3 v({1, 0, 0});
    Vector3 u({0, 1, 0});
    Vector3 t = v.crossProduct(u);

    std::cout << t << "\n";
}