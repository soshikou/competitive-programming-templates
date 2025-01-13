#pragma once
#include <bits/stdc++.h>
#include "graph.h"

namespace Rem {
    template<typename Tv = int32_t, typename Te = int32_t>
    class Tree {
        std::vector<uint32_t> m_father, m_depth;
        Graph<Tv, Te> m_graph;
        uint32_t m_root;
    public:
        void bind(Graph<Tv, Te> *graph) { m_graph = graph, m_father.resize(m_graph->size()), m_depth.resize(m_graph->size()); }
        void root(uint32_t u) { m_root = u, _root(u); }
    private:
        void _root(uint32_t u, uint32_t f = 0xFFFFFFFF, uint32_t d = 0) {
            m_father[u] = f;
            m_depth[u] = d;
            (*m_graph)(u, [&](uint32_t to, Te w) { 
                if (to == f) return;
                _root(to, u, d + 1); 
            });
        }
    };
}