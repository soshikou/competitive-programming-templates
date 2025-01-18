#pragma once
#include <bits/stdc++.h>
#include <../graph.h>
#include "../../data_structure/heap/indexed_heap.h"

namespace Rem {
    template<typename Tv = int32_t, typename Te = int32_t>
    class Dijkstra {
        std::vector<Te> m_distance;
        Graph<Tv, Te>* m_graph = nullptr;
    public:
        void bind(Graph<Tv, Te>* graph) { m_graph = graph; }
        void run(uint32_t s, Te infinite) {
            m_distance.assign(m_graph->size(), infinite);
            m_distance[s] = {};
            IndexedHeap<Te> heap(m_distance.data(), m_graph->size());
            heap.push(s);
            uint32_t from;
            Te to_dis, d;
            while (heap.size()) {
                from = heap.top();
                heap.pop();
                d = m_distance[from];
                m_graph->for_edges(from, [&](uint32_t to, Te w) { if ((to_dis = d + w) < m_distance[to] ? m_distance[to] = to_dis, true : false) heap.push(to); });
            }
        }
        const Te& query(uint32_t t) const { return m_distance[t]; }
    };
}