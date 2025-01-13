#pragma once
#include <bits/stdc++.h>

namespace Rem {
    template <typename T>
    class IndexedHeap {
        std::vector<uint32_t> m_heap;
        std::vector<uint32_t> m_pos;
        T* m_data;
        uint32_t m_size;
    public:
        IndexedHeap(T* data, uint32_t size) : m_data(data) { resize(size); }
        void resize(uint32_t size) {
            m_heap.resize(size);
            m_pos.assign(size, -1);
            m_size = 0;
        }
        template <bool direct = true>
        void push(uint32_t i) {
            if (!~m_pos[i]) m_heap[m_pos[i] = m_size++] = i;
            if constexpr (direct) adjust_up(i);
            else adjust_down(i);
        }
        void pop() {
            m_pos[m_heap[0]] = -1;
            if (--m_size) {
                m_pos[m_heap[m_size]] = 0;
                adjust_down(m_heap[0] = m_heap[m_size]);
            }
        }
        uint32_t top() const { return m_heap[0]; }
        void clear() {
            std::fill_n(m_pos.data(), m_pos.size(), -1);
            m_size = 0;
        }
        bool empty() const { return !m_size; }
        uint32_t size() const { return m_size; }
    private:
        void adjust_up(uint32_t i) {
            uint32_t* pos = m_pos.data();
            uint32_t* heap = m_heap.data();
            uint32_t curpos = pos[i], p;
            T curvalue = m_data[i];
            for (; curpos; curpos = (curpos - 1) >> 1) {
                if (m_data[p = heap[(curpos - 1) >> 1]] < curvalue) break;
                heap[pos[p] = curpos] = p;
            }
            heap[pos[i] = curpos] = i;
        }
        void adjust_down(uint32_t i) {
            uint32_t* pos = m_pos.data();
            uint32_t* heap = m_heap.data();
            uint32_t curpos = pos[i], c;
            T curvalue = m_data[i];
            for (; (c = curpos * 2 + 1) < m_size; curpos = c) {
                if (c + 1 < m_size && m_data[heap[c + 1]] < m_data[heap[c]]) c++;
                if (curvalue < m_data[heap[c]]) break;
                pos[heap[curpos] = heap[c]] = curpos;
            }
            heap[pos[i] = curpos] = i;
        }
    };
}