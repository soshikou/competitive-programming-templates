#pragma once
#include <bits/stdc++.h>

namespace Rem {
    template<typename Tv = int32_t, typename Te = int32_t>
    class Graph {
        uint32_t m_v = 0;
        std::vector<Tv> m_wv;
        struct raw_edge {
            uint32_t m_from, m_to;
            Te m_weight;
        };
        struct edge {
            uint32_t m_to;
            Te m_weight;
        };
        std::vector<raw_edge> m_raw_edges;
        std::vector<edge> m_edges;
        std::vector<uint32_t> m_starts;
    public:
        const uint32_t& size() { return m_v; }
        template <typename Callback>
        void operator()(uint32_t from, Callback&& call) const {
            auto* first = m_edges.data() + m_starts[from], * last = m_edges.data() + m_starts[from + 1];
            for (auto it = first; it != last; ++it) call(it->m_to, it->m_weight);
        }
        Tv& operator[](uint32_t i) { return m_wv[i]; }
        void resize(uint32_t v, uint32_t e) {
            if (!(m_v = v)) return;
            m_wv.resize(m_v);
            m_raw_edges.clear(), m_raw_edges.reserve(e), m_starts.assign(m_v + 1, 0);
        }
        void add_edge(uint32_t s, uint32_t t, Te w) {
            m_starts[s + 1]++, m_raw_edges.push_back({ s, t, w });
        }
        void freeze() {
            for (uint32_t i = 1; i != m_v + 1; i++) m_starts[i] += m_starts[i - 1];
            m_edges.resize(m_starts.back());
            auto cursor = m_starts;
            for (auto& e : m_raw_edges) m_edges[cursor[e.m_from]++] = { e.m_to, e.m_weight };
        }
    };
}