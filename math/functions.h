#pragma once
#include <bits/stdc++.h>

namespace Rem {
    template <typename T>
    T euclid(T a, T b) {
        if (!b) return a;
        else return euclid(b, a % b);
    }
    /*
    * get the solution for equation ax + by = gcd(a, b) when x is minimum positive integer.
    * return gcd(a, b).
    */
    template <typename T>
    T extended_euclid(T a, T b, T &x, T &y) {
        if (!b) {
            x = 1;
            y = 0;
            return a;
        }
        T d = extended_euclid(b, a % b, y, x);
        y -= (a / b) * x;
        return d;
    }
    template <typename T>
    T exponentiation(T n, T k) {
        T res = 1;
        while (k) {
            if (k & 1) res *= n;
            n *= n;
            k >>= 1;
        }
        return res;
    }
    template <typename T>
    T exponentiation(T n, T k, T p) {
        T res = 1;
        while (k) {
            if (k & 1) (res *= n) %= p;
            (n *= n) %= p;
            k >>= 1;
        }
        return res;
    }
    /*
    * <prime, exponent>
    */
    template <typename T>
    void factorize(T n, std::vector<std::pair<T, T> > &f) {
        f.clear();
        for (T i = 2; i <= n / i; i++) {
            pair<T, T> p = { i, 0 };
            while (n % i == 0) {
                n /= i;
                p.second++;
            }
            if (p.second) f.push_back(p);
        }
        if (n > 1) f.push_back({ n, 1 });
    }
    /*
    * Pollard Rho Algorithm.
    */
    template <typename T>
    T get_divisor(T n) {
        srand(time(0));
        T s = 0, t = 0, c = (T)1 * rand() % (n - 1) + 1;
        int stp = 0, goal = 1;
        T val = 1;
        for (goal = 1;; goal <<= 1, s = t, val = 1) {
            for (stp = 1; stp <= goal; ++stp) {
                t = (t * t + c) % n;
                val = val * (s < t ? (t - s) : (s - t)) % n;
                if ((stp % 127) == 0) {
                    T d = euclid(val, n);
                    if (1 < d) return d;
                }
            }
            T d = euclid(val, n);
            if (1 < d) return d;
        }
    }
    template <typename T>
    void get_divisors(T n, std::vector<T> &r) {
        r.clear();
        for (int i = 1; i <= n / i; i++) {
            if (n % i == 0) {
                r.push_back(i);
                if (i != n / i) r.push_back(n / i);
            }
        }
    }
    /*
    * number of divisors is multiplicative function
    */
    template <typename T>
    T number_of_divisors(T n) {
        std::vector<std::pair<T, T> > f;
        factorize(n, f);
        T ans = 1;
        for (auto iter : f) ans *= (iter.second + 1);
        return ans;
    }
    /*
    * sum of divisors is multiplicative function
    */
    template <typename T>
    T sum_of_divisors(T n) {
        std::vector<std::pair<T, T> > f;
        factorize(n, f);
        T ans = 1;
        for (auto iter : f) ans *= (exponentiation(iter.first, iter.second + 1) - 1) / (iter.first - 1);
        return ans;
    }
    /*
    * Euler's totient function is multiplicative function
    */
    template <typename T>
    T euler(T n) {
        T ans = n;
        for (T i = 2; i * i <= ans; i++) {
            if (n % i == 0) {
                ans = ans / i * (i - 1);
                while (n % i == 0) n /= i;
            }
        }
        if (n > 1) ans = ans / n * (n - 1);
        return ans;
    }
    template <typename T>
    void euler(T n, std::vector<T> &e) {
        if (e.size() < n + 1) e.resize(n + 1);
        std::vector<T> v(n + 1, 0), p;
        for (T i = 2; i <= n; i++) {
            if (v[i] == 0) {
                v[i] = i;
                p.push_back(i);
                e[i] = i - 1;
            }
            for (auto x : p) {
                if (x > v[i] || x > n / i) break;
                v[i * x] = x;
                e[i * x] = e[i] * (i % x ? x - 1 : x);
                if (i % x == 0) break;
            }
        }
    }
    /*
    * a number  g is called a primitive root modulo n if the length of cycle of g's power equals  phi(n)
    */
    template <typename T>
    T primitive_root(T m) {
        vector<pair<T, T> > fm, fphi;
        factorize(m, fm);
        if (!(m == 2 || m == 4 || (fm.size() == 1 && fm[0].first != 2) || (fm.size() == 2 && fm[0].first == 2 && fm[0].second == 1))) return -1;
        T phi = euler(m);
        factorize(phi, fphi);
        auto ok = [&](T g) {
            if (euclid(g, m) != 1) return false;
            for (auto iter : fphi) {
                if (exponentiation(g, phi / iter.first, m) == 1) return false;
            }
            return true;
        };
        for (T g = 1; g < m; g++) {
            if (ok(g)) return g;
        }
        return -1;
    }
    template <typename T>
    void primitive_root(T m, std::vector<T> &r) {
        r.clear();
        std::vector<std::pair<T, T> > fm, fphi;
        factorize(m, fm);
        if (!(m == 2 || m == 4 || (fm.size() == 1 && fm[0].first != 2) || (fm.size() == 2 && fm[0].first == 2 && fm[0].second == 1))) return;
        T phi = euler(m);
        factorize(phi, fphi);
        auto ok = [&](T g) {
            if (euclid(g, m) != 1) return false;
            for (auto iter : fphi) {
                if (exponentiation(g, phi / iter.first, m) == 1) return false;
            }
            return true;
        };
        T g = 1;
        for (; g < m; g++) { if (ok(g)) break; }
        for (T i = 1, p = g; i <= phi; i++) {
            if (euclid(i, phi) == 1) r.push_back(p);
            (p *= g) %= m;
        }
    }
}