#pragma once
#include <bits/stdc++.h>

namespace Rem {
    class DisjointSetUnion {
        struct Node {
            uint32_t m_father, m_rank;
            int32_t m_r10p;
        };
        std::vector<uint32_t> m_idx;
        std::vector<Node> m_nodes;
        uint32_t m_size;
    public:
        DisjointSetUnion(uint32_t size = 0) : m_size(0) { resize(size), reset(); }
        void resize(uint32_t size) {
            if (m_size < size) {
                m_size = size;
                m_idx.resize(m_size);
                m_nodes.resize(m_size);
            }
        }
        void reset() {
            for (uint32_t i = 0; i < m_size; i++) {
                m_idx[i] = i;
                m_nodes[i].m_father = i;
                m_nodes[i].m_r10p = 0;
                m_nodes[i].m_rank = 0;
            }
        }
        uint32_t get_father(uint32_t u) { return _get_father(m_idx[u]); }
        void merge(uint32_t u, uint32_t v, int32_t r = 0) { _merge(m_idx[u], m_idx[v], r); }
        void split(uint32_t u) {
            uint32_t n = m_nodes.size();
            m_nodes.push_back({n, 0, 0});
            m_idx[u] = n;
        }
    private:
        uint32_t _get_father(uint32_t u) {
            Node &n = m_nodes[u];
            if (n.m_father == u) return u;
            uint32_t fu = n.m_father;
            n.m_father = _get_father(fu);
            n.m_r10p += m_nodes[fu].m_r10p;
            return n.m_father;
        }
        void _merge(uint32_t u, uint32_t v, int32_t r = 0) {
            int fu = _get_father(u), fv = _get_father(v);
            if (fu == fv) return;
            Node &nfu = m_nodes[fu], &nfv = m_nodes[fv];
            if (nfu.m_rank < nfv.m_rank) {
                nfu.m_father = fv;
                nfu.m_r10p = m_nodes[v].m_r10p - m_nodes[u].m_r10p - r;
            }
            else {
                nfv.m_father = fu;
                nfv.m_r10p = m_nodes[u].m_r10p - m_nodes[v].m_r10p + r;
                if (nfu.m_rank == nfv.m_rank) nfu.m_rank++;
            }
        }
    };
}