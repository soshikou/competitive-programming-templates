#pragma once
#include <bits/stdc++.h>
#include "../util/buffer.h"
#include "modint.h"

namespace Rem {
    class Combinatoric {
        modint<> *m_fac, *m_inv, *m_f;
        uint32_t m_size;
    public:
        Combinatoric(uint32_t n) : m_size(n) {
            m_size = n;
            m_fac = Buffer::allocate<modint< >> (n);
            m_inv = Buffer::allocate<modint< >> (n);
            m_f = Buffer::allocate<modint< >> (n);
            m_fac[0] = m_fac[1] = m_inv[0] = m_inv[1] = m_f[0] = m_f[1] = 1;
            for (int i = 2; i < n; i++) {
                m_fac[i] = m_fac[i - 1] * i;
                m_f[i] = m_f[modint<>::get_modulo() % i] * (modint<>::get_modulo() - modint<>::get_modulo() / i);
                m_inv[i] = m_inv[i - 1] * m_f[i];
            }
        }
        modint<> P(long long a, long long b) {
            if (a < 0 || b < 0 || a < b) return 0;
            if (m_size <= a) {
                modint<> res = 1, t = a;
                for (long long i = 0; i < b; i++) {
                    res *= t;
                    t -= 1;
                }
                return res;
            }
            return m_fac[a] * m_inv[a - b];
        }
        Z C(long long a, long long b, bool lucas = false) {
            if (a < 0 || b < 0 || a < b) return 0;
            if (!lucas) {
                if (m_size <= a) {
                    Z res = 1, p = a, q = 1;
                    for (long long i = 0; i < b; i++) {
                        res *= p;
                        res /= q;
                        p.inc(-1);
                        q.inc(1);
                    }
                    return res;
                }
                return m_fac[a] * m_inv[b] * m_inv[a - b];
            } else return C(a % Z::get_modulo(), b % Z::get_modulo()) * C(a / Z::get_modulo(), b / Z::get_modulo(), true);
        }
        /*
        * C(n, 0) + C(n, 1) + ... + C(n, k)
        */
        Z S(long long n, long long k, bool lucas = false) {
            return C(n + k + 1, n + 1, lucas);
        }
        /*
        * the number of ways to place n 0 and n 1 in a row, so that in any prefix, the number of 0 is not less than the number of 1.
        * both formula are ok.
        */
        Z Catalan(long long n, bool lucas = false) {
            return C(2 * n, n, lucas) / (n + 1);
            return C(2 * n, n, lucas) - C(2 * n, n + 1, lucas);
        }
    };
}