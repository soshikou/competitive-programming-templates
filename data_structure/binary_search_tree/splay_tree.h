#pragma once
#include <bits/stdc++.h>
#include "../../util/buffer.h"

namespace Rem {
    template <typename KeyType = int32_t, typename ValueType = int32_t>
    class SplayTree {
        class Node {
        private:
            KeyType m_key;
            ValueType m_value;
            uint32_t m_count;
            uint32_t m_size;
            Node *m_parent;
            Node *m_child[2];
            friend class SplayTree;
        public:
            Node() : m_count(0), m_size(0) { m_parent = m_child[0] = m_child[1] = this; }
            const KeyType &key() const { return m_key; }
            const ValueType &value() const { return m_value; }
            void set(ValueType v) { m_value = v; }
            const uint32_t &count() const { return m_count; }
            const uint32_t &size() const { return m_size; }
        };
        Node *m_root;
        bool m_succeed;
        static Node *s_empty;
    public:
        void clear() { m_root = s_empty; }
        uint32_t size() { return m_root->m_size; }
        const Node *empty() const { return s_empty; }
        Node *insert(KeyType k) {
            if (m_root == s_empty) [[unlikely]] {
                m_root = Buffer::allocate<Node>();
                m_root->m_parent = m_root->m_child[0] = m_root->m_child[1] = s_empty;
                m_root->m_key = k;
                m_root->m_count = m_root->m_size = 1;
                return m_root;
            }
            Node* n = m_root, *q;
            uint32_t s;
            for (;;) {
                n->m_size++;
                if (n->m_key == k) [[unlikely]] {
                    n->m_count++;
                    splay(n);
                    return n;
                }
                s = k < n->m_key ? 0 : 1;
                if (n->m_child[s] == s_empty) [[unlikely]] {
                    n->m_child[s] = q = Buffer::allocate<Node>();
                    q->m_parent = n;
                    q->m_child[0] = q->m_child[1] = s_empty;
                    q->m_key = k;
                    q->m_count = q->m_size = 1;
                    splay(q);
                    return q;
                } else n = n->m_child[s];
            }
        }
        ValueType erase(KeyType k) {
            Node* n = m_root, *q, *z;
            m_succeed = false;
            for (;;) {
                if (n == s_empty) [[unlikely]] return n->m_value;
                if (n->m_key == k) [[unlikely]] {
                    m_succeed = true;
                    splay(n);
                    if (n->m_count > 1) {
                        n->m_size--;
                        n->m_count--;
                        return n->m_value;
                    }
                    if (n->m_child[0] == s_empty) {
                        m_root = n->m_child[1];
                        m_root->m_parent = s_empty;
                        return n->m_value;
                    }
                    if (n->m_child[1] == s_empty) {
                        m_root = n->m_child[0];
                        m_root->m_parent = s_empty;
                        return n->m_value;
                    }
                    q = n->m_child[0];
                    while (q->m_child[1] != s_empty) q = q->m_child[1];
                    n->m_child[0]->m_parent = s_empty;
                    splay(q);
                    z = n->m_child[1];
                    z->m_parent = q;
                    q->m_child[1] = z;
                    q->m_size += z->m_size;
                    return n->m_value;
                } else n = k < n->m_key ? n->m_child[0] : n->m_child[1];
            }
        }
        Node *update(KeyType k, ValueType v) {
            Node* n = m_root, *p = s_empty;
            for (;;) {
                if (n == s_empty) {
                    n = Buffer::allocate<Node>();
                    n->m_parent = p;
                    n->m_child[0] = n->m_child[1] = s_empty;
                    n->m_size = 1;
                    n->m_key = k;
                    n->m_value = v;
                    splay(n);
                    return n;
                } else if (n->m_key == k) {
                    n->m_value = v;
                    splay(n);
                    return n;
                } else {
                    p = n;
                    n = k < n->m_key ? n->m_child[0] : n->m_child[1];
                }
            }
        }
        Node *find(KeyType k) {
            Node *n = m_root;
            for (;;) {
                if (n == s_empty) return n;
                if (n->m_key == k) {
                    splay(n);
                    return n;
                }
                n = k < n->m_key ? n->m_child[0] : n->m_child[1];
            }
        }
        Node *rank(uint32_t k) {
            Node *n = m_root;
            for (;;) {
                if (k < n->m_child[0]->m_size) n = n->m_child[0];
                else if (k < n->m_child[0]->m_size + n->m_count) {
                    splay(n);
                    return n;
                } else {
                    k -= n->m_child[0]->m_size + n->m_count;
                    n = n->m_child[1];
                }
            }
        }
        Node *last(Node *n) {
            Node *q = n->m_child[0];
            while (q->m_child[1] != s_empty) q = q->m_child[1];
            return q;
        }
        Node *next(Node *n) {
            Node *q = n->m_child[1];
            while (q->m_child[0] != s_empty) q = q->m_child[0];
            return q;
        }
        uint32_t count_less_than(KeyType k) {
            Node *n = maximum_less_than(k);
            return n == s_empty ? 0 : n->m_child[0]->m_size + n->m_count;
        }
        uint32_t count_greater_than(KeyType k) {
            Node *n = minimum_greater_than(k);
            return n == s_empty ? 0 : n->m_child[1]->m_size + n->m_count;
        }
        Node *maximum_less_than(KeyType k) {
            if (m_root == s_empty) [[unlikely]] return m_root;
            Node *n = m_root, *q = s_empty;
            for (;;) {
                if (n->m_key < k) {
                    q = n;
                    if (n->m_child[1] == s_empty) [[unlikely]] {
                        splay(q);
                        return q;
                    }
                    n = n->m_child[1];
                } else {
                    if (n->m_child[0] == s_empty) [[unlikely]] {
                        if (q != s_empty) [[likely]] splay(q);
                        else splay(n);
                        return q;
                    }
                    n = n->m_child[0];
                }
            }
        }
        Node *minimum_greater_than(KeyType k) {
            if (m_root == s_empty) [[unlikely]] return m_root;
            Node *n = m_root, *q = s_empty;
            for (;;) {
                if (k < n->m_key) {
                    q = n;
                    if (n->m_child[0] == s_empty) [[unlikely]] {
                        splay(q);
                        return q;
                    } else n = n->m_child[0];
                } else {
                    if (n->m_child[1] == s_empty) [[unlikely]] {
                        if (q != s_empty) [[likely]] splay(q);
                        else splay(n);
                        return q;
                    } else n = n->m_child[1];
                }
            }
        }
        void splay(Node* n) {
            Node* p, *g, *gg, *a, *b, *c, *d;
            uint32_t sn, sp, sg;
            for (;;) {
                p = n->m_parent;
                g = p->m_parent;
                sn = n == p->m_child[0] ? 0 : 1;
                sp = p == g->m_child[0] ? 0 : 1;
                a = n->m_child[sn], b = n->m_child[sn ^ 1], c = p->m_child[sn ^ 1], d = g->m_child[sp ^ 1];
                if (p == s_empty) break;
                if (g == s_empty) {
                    n->m_parent = s_empty;
                    p->m_parent = n;
                    b->m_parent = p;
                    n->m_child[sn ^ 1] = p;
                    p->m_child[sn] = b;
                    n->m_size = p->m_size;
                    p->m_size = b->m_size + c->m_size + p->m_count;
                    break;
                }
                gg = g->m_parent;
                sg = g == gg->m_child[0] ? 0 : 1;
                if (sn == sp) {
                    //_rotate(p, sp);
                    //_rotate(n, sn);
                    n->m_parent = gg;
                    p->m_parent = n;
                    g->m_parent = p;
                    b->m_parent = p;
                    c->m_parent = g;
                    n->m_child[sn ^ 1] = p;
                    p->m_child[sn] = b;
                    p->m_child[sn ^ 1] = g;
                    g->m_child[sn] = c;
                    if (gg != s_empty) gg->m_child[sg] = n;
                    n->m_size = g->m_size;
                    g->m_size = c->m_size + d->m_size + g->m_count;
                    p->m_size = b->m_size + g->m_size + p->m_count;
                } else {
                    //_rotate(n, sn);
                    //_rotate(p, sp);
                    n->m_parent = gg;
                    p->m_parent = n;
                    g->m_parent = n;
                    a->m_parent = g;
                    b->m_parent = p;
                    n->m_child[sn] = g;
                    n->m_child[sn ^ 1] = p;
                    p->m_child[sn] = b;
                    g->m_child[sn ^ 1] = a;
                    if (gg != s_empty) gg->m_child[sg] = n;
                    n->m_size = g->m_size;
                    p->m_size = b->m_size + c->m_size + p->m_count;
                    g->m_size = a->m_size + d->m_size + g->m_count;
                }
            }
            m_root = n;
        }
    };
    template <typename KeyType, typename ValueType>
    SplayTree<KeyType, ValueType>::Node *SplayTree<KeyType, ValueType>::s_empty = new SplayTree<KeyType, ValueType>::Node;
}