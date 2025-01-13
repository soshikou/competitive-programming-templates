#pragma once
#include <bits/stdc++.h>
#include <../util/buffer.h>

namespace Rem {
    template<typename T, template <typename> typename BufferType = BufferWithoutCollect>
    class PairingHeap {
        struct Node {
            T m_val;
            Node *m_child, *m_sibling;
        };
        Node* m_root;
        static Node* _merge(Node* p, Node* q) {
            if (q->m_val < p->m_val) std::swap(p, q);
            q->m_sibling = p->m_child;
            p->m_child = q;
            return p;
        }
    public:
        PairingHeap() : m_root(NULL) {}
        static void resize(uint32_t length) { BufferType<Node>::resize(length); }
        T top() const {
            if (m_root == NULL) [[unlikely]] return {};
            else return m_root->m_val; 
        }
        Node* push(const T& val) {
            Node* p = BufferType<Node>::allocate();
            p->m_val = val;
            return m_root = m_root == NULL ? p : _merge(m_root, p);
        }
        Node* pop() {
            if (m_root == NULL) [[unlikely]] return m_root;
            if (m_root->m_child == NULL) return m_root = NULL;
            Node *end = m_root->m_child, *p = end, *q, *n;
            while (end->m_sibling != NULL) end = end->m_sibling;
            for (;;) {
                if (p == end) return m_root = p;
                q = p->m_sibling;
                p->m_sibling = NULL;
                if (q == end) return m_root = _merge(p, q);
                else {
                    n = q->m_sibling;
                    q->m_sibling = NULL;
                    end->m_sibling = _merge(p, q);
                    end = end->m_sibling;
                    p = n;
                }
            }
        }
        Node* join(PairingHeap& h) {
            if (h.m_root == NULL) [[unlikely]] return m_root;
            return m_root = m_root == NULL ? h.m_root, h.m_root = NULL : _merge(m_root, h.m_root);
        }
        bool empty() { return m_root == NULL; }
    };
}