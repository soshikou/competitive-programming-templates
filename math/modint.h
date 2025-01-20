#pragma once
#include <bits/stdc++.h>
#include "euclid.h"

namespace Rem {
    template <typename T = int64_t>
    class modint {
        T m_val;
        static T P;
        static T norm(T x) {
            if (x < 0) x += P;
            if (x >= P) x -= P;
            return x;
        }
    public:
        modint(T x = 0) : m_val(norm(x % P)) {}
        static void set_modulo(T np) { P = np; }
        static T get_modulo() { return P; }
        T val() const { return m_val; }
        /*
        * return the inverse element of current, i.e. a/b=a*b.inv()
        * I * x = 1 mod P <=> I * x + P * y = 1
        */
        modint inverse() const {
            T x, y;
            extended_euclid(m_val, P, x, y);
            return x;
        }
        /*
        * return the b-th exponentiation of current number
        */
        modint exponentiation(T b) const {
            if (b < 0) return (this->exponentiation(-b)).inverse();
            modint res = 1, a = *this;
            for (; b; b >>= 1, a *= a) {
                if (b & 1) res *= a;
            }
            return res;
        }
        /*
        * return the smallest x that satisfies a ^ x = I (mod P)
        * return -1 if there's no solution
        */
        modint logarithm(T a) {
            T b = m_val;
            T k = 1, add = 0, g;
            while ((g = euclid(a, P)) > 1) {
                if (b == k) return add;
                if (b % g) return -1;
                b /= g, P /= g, ++add;
                k = (k * a / g) % P;
            }
            T n = sqrt(P) + 1;
            T an = 1;
            for (T i = 0; i < n; i++)
                an = (an * a) % P;
            unordered_map<T, T> vals;
            for (T q = 0, cur = b; q <= n; q++) {
                vals[cur] = q;
                cur = (cur * a) % P;
            }
            for (T p = 1, cur = k; p <= n; ++p) {
                cur = (cur * an) % P;
                if (vals.count(cur)) {
                    T ans = n * p - vals[cur] + add;
                    return ans;
                }
            }
            return -1;
        }
        /*
        * return all numbers x that satisfies x ^ k = I (mod P)
        */
        void root(T k, std::vector<T> &r) {
            r.clear();
            if (m_val == 0) {
                r.push_back(0);
                return;
            }
            T g = primitive_root(P);
            T sq = sqrt(P) + 1;
            vector<pair<T, T> > dec(sq);
            for (T i = 1; i <= sq; i++) dec[i - 1] = { binary_exponentiation(g, i * sq * k % (P - 1), P), i };
            sort(dec.begin(), dec.end());
            T any_ans = -1;
            for (T i = 0; i < sq; i++) {
                T my = binary_exponentiation(g, i * k % (P - 1), P) * m_val % P;
                auto it = lower_bound(dec.begin(), dec.end(), make_pair(my, 0));
                if (it != dec.end() && it->first == my) {
                    any_ans = it->second * sq - i;
                    break;
                }
            }
            if (any_ans == -1) return;
            T delta = (P - 1) / euclid(k, P - 1);
            for (T cur = any_ans % delta; cur < P - 1; cur += delta)
                r.push_back(binary_exponentiation(g, cur, P));
        }
        bool operator < (const modint& rhs) const { return m_val < rhs.m_val; }
        modint operator - () const { return modint(norm(P - m_val)); }
        modint &operator *= (const modint& rhs) { m_val = m_val * rhs.m_val % P; return *this; }
        modint &operator += (const modint& rhs) { m_val = norm(m_val + rhs.m_val); return *this; }
        modint &operator -= (const modint& rhs) { m_val = norm(m_val - rhs.m_val); return *this; }
        modint &operator /= (const modint& rhs) { return *this *= rhs.inverse(); }
        modint &operator >>= (T p) {
            modint inv2 = (modint::get_modulo() + 1) >> 1;
            inv2 = inv2.exponentiation(p);
            *this *= inv2;
            return *this;
        }
        modint operator * (const modint& rhs) {
            modint res = *this;
            res *= rhs;
            return res;
        }
        modint operator + (const modint& rhs) {
            modint res = *this;
            res += rhs;
            return res;
        }
        modint operator - (const modint& rhs) {
            modint res = *this;
            res -= rhs;
            return res;
        }
        modint operator / (const modint& rhs) {
            modint res = *this;
            res *= rhs.inverse();
            return res;
        }
        modint operator >> (T p) {
            modint res = *this;
            res >>= p;
            return res;
        }
        friend std::istream &operator >> (std::istream& is, modint& a) {
            T v;
            is >> v;
            a = modT(v);
            return is;
        }
        friend std::ostream &operator << (std::ostream& os, const modint& a) { return os << a.val(); }
    };
    template <typename T> T modint<T>::P = 1e9 + 7;
}