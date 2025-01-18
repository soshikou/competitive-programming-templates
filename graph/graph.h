#pragma once
#include <bits/stdc++.h>
#include "../util/buffer.h"

namespace Rem {
    template<typename Tv = int32_t, typename Te = int32_t>
    class Graph {
        struct edge {
            uint32_t m_t;
            Te m_w;
        };
        struct raw_edge {
            uint32_t m_s, m_t;
            Te m_w;
        };
        uint32_t m_v;
        Tv *m_wv;
        vector<raw_edge> m_raw_edges;
        edge *m_edges;
        uint32_t *m_starts;
        edge **m_cursor;
    public:
        Graph(uint32_t v, uint32_t e) : m_v(v) {
            m_wv = Buffer::allocate<Tv>(m_v);
            m_raw_edges.m_data = Buffer::allocate<raw_edge>(e);
            m_edges = Buffer::allocate<edge>(e);
            m_starts = Buffer::allocate<uint32_t>(m_v + 1);
            edge **placeholder = Buffer::allocate<edge *>();
            m_cursor = Buffer::allocate<edge *>(m_v + 1);
            reset();
        }
        void reset() {
            m_raw_edges.m_size = 0;
            memset(m_starts, 0, sizeof(uint32_t) * (m_v + 1));
        }
        Tv &operator[](uint32_t i) { return m_wv[i]; }
        const uint32_t &size() { return m_v; }
        void add_edge(uint32_t s, uint32_t t, Te w) { m_starts[s + 1]++; m_raw_edges.m_data[m_raw_edges.m_size++] = {s, t, w}; }
        void freeze() {
            m_cursor[0] = m_edges;
            for (uint32_t i = 1; i != m_v + 1; i++) m_cursor[i] = m_edges + (m_starts[i] += m_starts[i - 1]);
            raw_edge *rb = m_raw_edges.m_data + m_raw_edges.m_size;
            for (raw_edge *e = m_raw_edges.m_data; e != rb; e++) new (m_cursor[e->m_s]++)edge({e->m_t, e->m_w});
            m_cursor--;
            m_cursor[0] = m_edges;
        }
        const edge *left_boundary(uint32_t u) const {return m_cursor[u];}
        const edge *right_boundary(uint32_t u) const {return m_cursor[u + 1];}
    };
}