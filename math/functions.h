#pragma once
#include <bits/stdc++.h>

namespace Rem {
    int64_t euclid(int64_t a, int64_t b) {
        if (!b) return a;
        else return euclid(b, a % b);
    }
    /*
    * get the solution for equation ax + by = gcd(a, b) when x is minimum positive integer.
    * return gcd(a, b).
    */
    int64_t extended_euclid(int64_t a, int64_t b, int64_t &x, int64_t &y) {
        if (!b) {
            x = 1;
            y = 0;
            return a;
        }
        int64_t d = extended_euclid(b, a % b, y, x);
        y -= (a / b) * x;
        return d;
    }
    int64_t exponentiation(int64_t n, int64_t k) {
        int64_t res = 1;
        while (k) {
            if (k & 1) res *= n;
            n *= n;
            k >>= 1;
        }
        return res;
    }
    int64_t exponentiation(int64_t n, int64_t k, int64_t p) {
        int64_t res = 1;
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
    void factorize(int64_t n, std::vector<std::pair<int64_t, int64_t> > &f) {
        f.clear();
        for (int64_t i = 2; i <= n / i; i++) {
            std::pair<int64_t, int64_t> p = { i, 0 };
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
    int64_t get_divisor(int64_t n) {
        srand(time(0));
        int64_t s = 0, t = 0, c = 1LL * rand() % (n - 1) + 1;
        int64_t stp = 0, goal = 1;
        int64_t val = 1;
        for (goal = 1;; goal <<= 1, s = t, val = 1) {
            for (stp = 1; stp <= goal; ++stp) {
                t = (t * t + c) % n;
                val = val * (s < t ? (t - s) : (s - t)) % n;
                if ((stp % 127) == 0) {
                    int64_t d = euclid(val, n);
                    if (1 < d) return d;
                }
            }
            int64_t d = euclid(val, n);
            if (1 < d) return d;
        }
    }
    void get_divisors(int64_t n, std::vector<int64_t> &r) {
        r.clear();
        for (int64_t i = 1; i <= n / i; i++) {
            if (n % i == 0) {
                r.push_back(i);
                if (i != n / i) r.push_back(n / i);
            }
        }
    }
    /*
    * number of divisors is multiplicative function
    */
    int64_t number_of_divisors(int64_t n) {
        std::vector<std::pair<int64_t, int64_t> > f;
        factorize(n, f);
        int64_t ans = 1;
        for (auto iter : f) ans *= (iter.second + 1);
        return ans;
    }
    /*
    * sum of divisors is multiplicative function
    */
    int64_t sum_of_divisors(int64_t n) {
        std::vector<std::pair<int64_t, int64_t> > f;
        factorize(n, f);
        int64_t ans = 1;
        for (auto iter : f) ans *= (exponentiation(iter.first, iter.second + 1) - 1) / (iter.first - 1);
        return ans;
    }
    /*
    * Euler's totient function is multiplicative function
    */
    int64_t euler(int64_t n) {
        int64_t ans = n;
        for (int64_t i = 2; i * i <= ans; i++) {
            if (n % i == 0) {
                ans = ans / i * (i - 1);
                while (n % i == 0) n /= i;
            }
        }
        if (n > 1) ans = ans / n * (n - 1);
        return ans;
    }
    void euler(int64_t n, std::vector<int64_t> &e) {
        if (e.size() < n + 1) e.resize(n + 1);
        std::vector<int64_t> v(n + 1, 0), p;
        for (int64_t i = 2; i <= n; i++) {
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
    int64_t get_primitive_root(int64_t m) {
        std::vector<std::pair<int64_t, int64_t >> fm, fphi;
        factorize(m, fm);
        if (!(m == 2 || m == 4 || (fm.size() == 1 && fm[0].first != 2) || (fm.size() == 2 && fm[0].first == 2 && fm[0].second == 1))) return - 1;
        int64_t phi = euler(m);
        factorize(phi, fphi);
        auto ok = [&](int64_t g) {
            if (euclid(g, m) != 1) return false;
            for (auto iter : fphi) {
                if (exponentiation(g, phi / iter.first, m) == 1) return false;
            }
            return true;
        };
        for (int64_t g = 1; g < m; g++) {
            if (ok(g)) return g;
        }
        return - 1;
    }
    void get_primitive_root(int64_t m, std::vector<int64_t> &r) {
        r.clear();
        std::vector<std::pair<int64_t, int64_t> > fm, fphi;
        factorize(m, fm);
        if (!(m == 2 || m == 4 || (fm.size() == 1 && fm[0].first != 2) || (fm.size() == 2 && fm[0].first == 2 && fm[0].second == 1))) return;
        int64_t phi = euler(m);
        factorize(phi, fphi);
        auto ok = [&](int64_t g) {
            if (euclid(g, m) != 1) return false;
            for (auto iter : fphi) {
                if (exponentiation(g, phi / iter.first, m) == 1) return false;
            }
            return true;
        };
        int64_t g = 1;
        for (; g < m; g++) { if (ok(g)) break; }
        for (int64_t i = 1, p = g; i <= phi; i++) {
            if (euclid(i, phi) == 1) r.push_back(p);
            (p *= g) %= m;
        }
    }
}