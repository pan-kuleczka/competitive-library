#include <bits/stdc++.h>

template <class T = long double>
struct Complex
{
    constexpr const static long double PI = acos(-1);

    T real, imag;

    Complex(T _real = T(0), T _imag = T(0)) : real(_real), imag(_imag) {}

    Complex<T> operator+(const Complex<T> &other) { return Complex<T>(this->real + other.real, this->imag + other.imag); }
    Complex<T> operator-(const Complex<T> &other) { return Complex<T>(this->real - other.real, this->imag - other.imag); }
    Complex<T> operator*(const Complex<T> &other)
    {
        return Complex<T>(
            this->real * other.real - this->imag * other.imag,
            this->real * other.imag + this->imag * other.real);
    }

    Complex<T> operator/(const Complex<T> &other)
    {
        T denominator = other.real * other.real + other.imag * other.imag;
        if (denominator == T(0))
            throw std::runtime_error("Complex number math error: Division by 0\n");
        return Complex<T>(
            (this->real * other.real + this->imag * other.imag) / denominator,
            (this->imag * other.real - this->real * other.imag) / denominator);
    }

    Complex<T> &operator+=(const Complex<T> &other)
    {
        *this = *this + other;
        return *this;
    }

    Complex<T> &operator-=(const Complex<T> &other)
    {
        *this = *this - other;
        return *this;
    }

    Complex<T> &operator*=(const Complex<T> &other)
    {
        *this = *this * other;
        return *this;
    }

    Complex<T> &operator/=(const Complex<T> &other)
    {
        *this = *this / other;
        return *this;
    }

    T magnitude()
    {
        return sqrtl(real * real + imag * imag);
    }

    T argument()
    {
        if (magnitude() == T(0))
            return 0;
        if (real == T(0))
            return (imag > T(0) ? T(PI / 2) : T(PI * 3 / 2));

        T baseAngle = T(atanl(imag / real));

        if (real < T(0))
            baseAngle += T(PI);
        if (baseAngle < T(0))
            baseAngle += T(PI * 2);

        return baseAngle;
    }

    ~Complex() {}
};

template <class T>
std::ostream &operator<<(std::ostream &out, const Complex<T> &z)
{
    out << z.real << " + " << z.imag << "i";
    return out;
}
template <class T>
std::istream &operator>>(std::istream &in, Complex<T> &z)
{
    T real, imag;
    in >> real >> imag;
    z = Complex<T>(real, imag);
    return in;
}

template <class _complexClass = Complex<long double>, class _floatingPointClass = long double>
class FFT
{
    constexpr static _floatingPointClass PI = _floatingPointClass(3.141592653589793238L);

    void subblockSort(std::vector<_complexClass> &vec)
    {
        // Sort elements by their original position's reversed bit order
        // e.g. for vec.size() equal to 8 sorted vector is a_0, a_4, a_2, a_6, a_1, a_5, a_3, a_7

        unsigned long int sortIndex = 0;
        unsigned long int reversedSortIndex = 0;

        while (sortIndex < vec.size())
        {
            if (sortIndex < reversedSortIndex)
                std::swap(vec[sortIndex], vec[reversedSortIndex]);

            sortIndex++;

            // Increment reversedSortIndex by 1 to avoid recomputation
            unsigned long int bit = vec.size() >> 1;
            while (bit && (reversedSortIndex & bit))
                reversedSortIndex ^= bit, bit >>= 1;
            reversedSortIndex ^= bit;
        }
    }

public:
    template <class T>
    std::vector<_complexClass> toComplexVector(const std::vector<T> &vec)
    {
        std::vector<_complexClass> complexVec(vec.size());
        for (unsigned int i = 0; i < vec.size(); ++i)
            complexVec[i] = _complexClass(vec[i]);
        return complexVec;
    }

    void transform(std::vector<_complexClass> &vec, bool inverseTransform = false)
    {
        unsigned long int n = vec.size();

        if (n & (n - 1)) // Check if vec.size() is not a power of two
            throw std::runtime_error("FFT error: Vector size is not a power of two");

        subblockSort(vec);

        for (unsigned int blockSize = 2; blockSize <= n; blockSize <<= 1)
        {
            _floatingPointClass firstRootAngle = _floatingPointClass(PI * 2 / _floatingPointClass(blockSize));
            if (inverseTransform)
                firstRootAngle = -firstRootAngle;
            _complexClass firstRoot = _complexClass(cosl(firstRootAngle), sinl(firstRootAngle));
            unsigned int combinationOffset = blockSize >> 1;

            for (unsigned int blockBegin = 0; blockBegin + blockSize <= n; blockBegin += blockSize)
            {
                _complexClass currentRoot(1, 0);
                for (unsigned int root = 0; root < combinationOffset; ++root)
                {
                    _complexClass a(vec[blockBegin + root]);
                    _complexClass b(vec[blockBegin + root + combinationOffset] * currentRoot);

                    vec[blockBegin + root] = a + b;
                    vec[blockBegin + root + combinationOffset] = a - b;

                    currentRoot *= firstRoot;
                }
            }
        }

        if (inverseTransform)
            for (_complexClass &x : vec)
                x /= _complexClass(n);
    }
};

FFT<> fft;

std::vector<int> multiplyPolynomials(const std::vector<int> &a, const std::vector<int> &b)
{   
    unsigned int size = 1;
    while(size < a.size() + b.size() - 1) size <<= 1;

    std::vector<Complex<long double>> complexA(size), complexB(size), complexC(size);

    for(unsigned int i = 0; i < a.size(); ++i) complexA[i] = Complex<long double>(a[i]);
    for(unsigned int i = 0; i < b.size(); ++i) complexB[i] = Complex<long double>(b[i]);

    fft.transform(complexA);
    fft.transform(complexB);
    for(unsigned int i = 0; i < size; ++i) complexC[i] = complexA[i] * complexB[i];
    fft.transform(complexC, true);

    std::vector<int> c(a.size() + b.size() - 1);

    for(unsigned int i = 0; i < c.size(); ++i) c[i] = int(roundl(complexC[i].real));

    return c;
}

std::vector<int> v = {1, 2, 3, -1};
std::vector<int> v2 = {1, 4, 3};

int main()
{
    auto r = multiplyPolynomials(v, v2);

    for(int x : r) std::cout << x << " ";
    std::cout << "\n";
}
