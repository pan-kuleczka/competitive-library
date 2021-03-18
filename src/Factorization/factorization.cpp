#include <bits/stdc++.h>
#define IOSTREAM_BOOST true
using namespace std;

template <class T = long long>
struct MillerRabin
{
    std::vector<T> deterministicWitnesses = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

    T modMult(const T &a, const T &b, const T &mod) const
    {
        T base = a;
        T result = 0;
        for (T bit = T(0); bit < T(63); bit++, base = (base + base) % mod)
            if (b & (T(1) << bit))
                result = (result + base) % mod;
        return result;
    }

    T modPow(const T &a, const T &b, const T &mod) const
    {
        T base = a;
        T result = T(1);
        for (T bit = T(0); bit < T(63); bit++, base = modMult(base, base, mod))
            if (b & (T(1) << bit))
                result = modMult(result, base, mod);
        return result;
    }

    bool checkComposite(const T &x, const T &witness, const T &remainder, const T &trailingZeroes)
    {
        T basePower = modPow(witness, remainder, x);
        if (basePower == T(1) || basePower == x - T(1))
            return false;
        for (unsigned int power = 1; power < trailingZeroes; power++)
        {
            basePower = modMult(basePower, basePower, x);
            if (basePower == x - T(1))
                return false;
        }
        return true;
    }

    bool isPrime(const T &x)
    {
        if (x < T(2))
            return false;
        for (const T &witness : deterministicWitnesses)
            if (x == witness)
                return true;

        unsigned int trailingZeroes = 0;
        T remainder = x - T(1);

        while ((remainder & T(1)) == T(0))
            remainder >>= T(1), trailingZeroes++;

        for (const T &witness : deterministicWitnesses)
            if (checkComposite(x, witness, remainder, trailingZeroes))
                return false;

        return true;
    }
};

MillerRabin<> millerrabin;

template <class T = long long>
struct RhoPollard
{
    T polynomial(T x, T c, T mod)
    {
        return (millerrabin.modMult(x, x, mod) + c) % mod;
    }

    T findFactor(T x, T x0, T c)
    {
        T a = x0;
        T b = x0;
        T gcd = 1;

        while (gcd == 1)
        {
            a = polynomial(a, c, x);
            b = polynomial(polynomial(b, c, x), c, x);
            gcd = __gcd(abs(a - b), x);
        }
        return gcd;
    }

    std::vector<T> factorise(T x, T x0 = T(2), T c0 = T(1)){
        if(x < T(2)) return {x};

        std::vector<T> result;
        std::stack<T> compositeFactors;
        compositeFactors.push(x);

        while(!compositeFactors.empty()){
            T factor = compositeFactors.top();
            compositeFactors.pop();

            if(millerrabin.isPrime(factor)){
                result.push_back(factor);
                continue;
            }

            T c = c0;
            T subFactor = T(0);
            
            if((factor & T(1)) == 0) subFactor = T(2);

            while(subFactor == T(0) || subFactor == T(1) || subFactor == factor) subFactor = findFactor(factor, x0, c), c++;

            compositeFactors.push(subFactor);
            compositeFactors.push(factor / subFactor);
        }

        return result;
    }

};

RhoPollard<> rhopollard;

map<long long, int> factors;

int main()
{

#if IOSTREAM_BOOST
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
#endif
    long long x;
    cin >> x;

    while(x != 0){
        factors.clear();
        std::vector<long long> v = rhopollard.factorise(x);
        for(long long f : v) factors[f]++;
        for(pair<long long, int> p : factors) cout << p.first << "^" << p.second << " ";
        cout << "\n";
        cin >> x;
    }

    return 0;
}