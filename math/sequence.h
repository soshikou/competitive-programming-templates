#pragma once
#include <bits/stdc++.h>

namespace Rem {
    /*
    * Fast Doubling Method
    */
    template <typename T>
    std::pair<T, T> fibonacci(T n) {
        if (n == 0) return { 0, 1 };
        auto p = fibonacci(n >> 1);
        T c = p.first * (2 * p.second - p.first);
        T d = p.first * p.first + p.second * p.second;
        if (n & 1)
            return { d, c + d };
        else
            return { c, d };
    }
}