#pragma once
#include <bits/stdc++.h>
#include "../data_structure/balanced_binary_tree/size_balanced_tree.h"
// 6s86
namespace Rem {
    template <typename MapType = SizeBalancedTree<uint32_t, uint32_t>>
    class MexCalculator {
    private:
        MapType m_starts;
        vector<uint32_t> m_cnt;
        uint32_t m_size;
    public:
        explicit MexCalculator(uint32_t size) : m_size(size) { m_cnt.resize(size), MapType::resize(size), clear(); }
        void clear() { memset(m_cnt.data(), 0, sizeof(uint32_t) * m_size), m_starts.clear(); }
        void add(uint32_t x) {
            if (x >= m_size) return;
            if (!m_cnt[x]++) {
                uint32_t r = (x + 1 < m_size && m_cnt[x + 1]) ? m_starts.erase(x + 1) : x;
                if (x && m_cnt[x - 1]) m_starts.maximum_less_than(x)->set(r);
                else m_starts.update(x, r);
            }
        }
        void remove(uint32_t x) {
            if (x >= m_size) return;
            if (!(--m_cnt[x])) {
                uint32_t l = x, r = x;
                bool has_left = x && m_cnt[x - 1], has_right = x + 1 < m_size && m_cnt[x + 1];
                if (has_left) {
                    auto ptr = m_starts.maximum_less_than(x);
                    r = ptr->value();
                    ptr->set(x - 1);
                    if (has_right) m_starts.update(x + 1, r);
                } else {
                    r = m_starts.erase(x);
                    if (has_right) m_starts.update(x + 1, r);
                }
            }
        }
        uint32_t query() { return m_cnt[0] ? m_starts.find(0)->value() + 1 : 0; }
    };
}