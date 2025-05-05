#pragma once
#include <bits/stdc++.h>
#include "functions.h"

namespace Rem {
    class modint {
        int64_t m_val;
        static int64_t P;
        static int64_t norm(int64_t x) {
            if (x < 0) x += P;
            if (x >= P) x -= P;
            return x;
        }
    public:
        modint(int64_t x = 0) : m_val(norm(x % P)) {}
        static void set_modulo(int64_t np) { P = np; }
        static int64_t get_modulo() { return P; }
        int64_t val() const { return m_val; }
        /*
        * return the inverse element of current, i.e. a/b=a*b.inv()
        * I * x = 1 mod P <=> I * x + P * y = 1
        */
        modint reciprocal() const {
            int64_t x, y;
            extended_euclid(m_val, modint::get_modulo(), x, y);
            return x;
        }
        modint exponentiation(int64_t k) { return Rem::exponentiation(m_val, k, modint::get_modulo()); }
        /*
        * return the smallest x that satisfies a ^ x = I (mod P)
        * return -1 if there's no solution
        */
        modint logarithm(int64_t a, modint n) {
            int64_t b = n.val(), P = modint::get_modulo();
            int64_t k = 1, add = 0, g;
            while ((g = euclid(a, P)) > 1) {
                if (b == k) return add;
                if (b % g) return -1;
                b /= g, P /= g, ++add;
                k = (k * a / g) % P;
            }
            int64_t m = sqrt(P) + 1;
            int64_t an = 1;
            for (int64_t i = 0; i < m; i++)
                an = (an * a) % P;
            std::unordered_map<int64_t, int64_t> vals;
            for (int64_t q = 0, cur = b; q <= m; q++) {
                vals[cur] = q;
                cur = (cur * a) % P;
            }
            for (int64_t p = 1, cur = k; p <= m; ++p) {
                cur = (cur * an) % P;
                if (vals.count(cur)) {
                    int64_t ans = m * p - vals[cur] + add;
                    return ans;
                }
            }
            return -1;
        }
        /*
        * return all numbers x that satisfies x ^ k = I (mod P)
        */
        void root(int64_t k, modint n, std::vector<int64_t> &r) {
            r.clear();
            int64_t m = n.val();
            if (m == 0) {
                r.push_back(0);
                return;
            }
            int64_t g = get_primitive_root(P);
            int64_t sq = sqrt(P) + 1;
            std::vector<std::pair<int64_t, int64_t> > dec(sq);
            for (int64_t i = 1; i <= sq; i++) dec[i - 1] = { Rem::exponentiation(g, i * sq * k % (P - 1), P), i };
            sort(dec.begin(), dec.end());
            int64_t any_ans = -1;
            for (int64_t i = 0; i < sq; i++) {
                int64_t my = Rem::exponentiation(g, i * k % (P - 1), P) * m % P;
                auto it = lower_bound(dec.begin(), dec.end(), std::make_pair(my, 0));
                if (it != dec.end() && it->first == my) {
                    any_ans = it->second * sq - i;
                    break;
                }
            }
            if (any_ans == -1) return;
            int64_t delta = (P - 1) / euclid(k, P - 1);
            for (int64_t cur = any_ans % delta; cur < P - 1; cur += delta)
                r.push_back(Rem::exponentiation(g, cur, P));
        }
        bool operator < (const modint& rhs) const { return m_val < rhs.m_val; }
        modint operator - () const { return modint(norm(P - m_val)); }
        modint &operator *= (const modint& rhs) { m_val = m_val * rhs.m_val % P; return *this; }
        modint &operator += (const modint& rhs) { m_val = norm(m_val + rhs.m_val); return *this; }
        modint &operator -= (const modint& rhs) { m_val = norm(m_val - rhs.m_val); return *this; }
        modint &operator /= (const modint& rhs) { return *this *= rhs.reciprocal(); }
        modint &operator >>= (int64_t p) {
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
            res *= rhs.reciprocal();
            return res;
        }
        modint operator >> (int64_t p) {
            modint res = *this;
            res >>= p;
            return res;
        }
        friend std::istream &operator >> (std::istream& is, modint& a) {
            int64_t v;
            is >> v;
            a = modint(v);
            return is;
        }
        friend std::ostream &operator << (std::ostream& os, const modint& a) { return os << a.val(); }
    };
    int64_t modint::P = 1e9 + 7;
}