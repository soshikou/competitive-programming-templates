#pragma once
#include <bits/stdc++.h>
#include "../../util/buffer.h"

namespace Rem {
    template <typename KeyType = int32_t, typename ValueType = int32_t, template <typename> typename BufferType = BufferWithoutCollect>
    class SizeBalancedTree {
        class Node {
        private:
            KeyType m_key;
            ValueType m_value;
            Node *m_child[2];
            uint32_t m_size;
            friend class SizeBalancedTree;
        public:
            const KeyType &key() const { return m_key; }
            const ValueType &value() const { return m_value; }
            void set(ValueType v) { m_value = v; }
            const uint32_t &size() const { return m_size; }
        };
        Node *m_root, *m_null, *m_bonus_iu;
        bool m_succeed;
        ValueType m_bouns_e;
    public:
        static void resize(uint32_t length) { 
            BufferType<Node>::resize(length);
            Node *null = BufferType<Node>::null();
            null->m_child[0] = null->m_child[1] = null;
        }
        void clear() {
            if constexpr (BufferType<Node>::need_clct()) collect(m_root);
            m_root = m_null = BufferType<Node>::null();
        }
        uint32_t size() { return m_root->size(); }
        const Node *null() const { return m_null; }
        Node *insert(KeyType k) {
            m_root = insert(m_root, k);
            return m_bonus_iu;
        }
        ValueType erase(KeyType k) {
            m_succeed = false, m_root = erase(m_root, k);
            return m_bouns_e;
        }
        Node *update(KeyType k, ValueType v) { 
            m_succeed = false, m_root = update(m_root, k, v);
            return m_bonus_iu;
        }
        Node *find(KeyType k) { return find(m_root, k); }
        Node *rank(uint32_t k) { return rank(m_root, k); }
        Node *last(Node *root) {
            root = root->m_child[0];
            if (root == m_null) return m_null;
            while (root->m_child[1] != m_null) root = root->m_child[1];
            return root;
        }
        Node *next(Node *root) {
            root = root->m_child[1];
            if (root == m_null) return m_null;
            while (root->m_child[0] != m_null) root = root->m_child[0];
            return root;
        }
        uint32_t count_less_than(KeyType k) { return count_less_than(m_root, k); }
        uint32_t count_greater_than(KeyType k) { return count_greater_than(m_root, k); }
        Node *maximum_less_than(KeyType k) { return maximum_less_than(m_root, k); }
        Node *minimum_greater_than(KeyType k) { return minimum_greater_than(m_root, k); }
    private:
        void collect(Node* root) {
            if (root == m_null) return;
            collect(root->m_child[0]), collect(root->m_child[1]);
            BufferType<Node>::collect(root);
        }
        Node *rotate(Node *root, uint32_t p) {
            Node *b = root->m_child[p], *e = root->m_child[p]->m_child[p ^ 1];
            b->m_child[p ^ 1] = root;
            b->m_size = root->m_size;
            root->m_child[p] = e;
            root->m_size = 1 + e->m_size + root->m_child[p ^ 1]->m_size;
            return b;
        }
        Node *adjust(Node *root, uint32_t p) {
            Node *b = root->m_child[p], *c = root->m_child[p ^ 1], *d = b->m_child[p], *e = b->m_child[p ^ 1];
            if (d->m_size > c->m_size) root = rotate(root, p);
            else if (e->m_size > c->m_size) {
                root->m_child[p] = rotate(b, p ^ 1);
                root = rotate(root, p);
            }
            else return root;
            root->m_child[0] = adjust(root->m_child[0], 0);
            root->m_child[1] = adjust(root->m_child[1], 0);
            root = adjust(root, 0);
            root = adjust(root, 1);
            return root;
        }
        Node *insert(Node *root, KeyType k) {
            if (root == m_null) {
                root = BufferType<Node>::allocate();
                root->m_key = k;
                root->m_child[0] = root->m_child[1] = m_null;
                root->m_size = 1;
                m_bonus_iu = root;
                return root;
            } else {
                root->m_size++;
                int p = k < root->m_key ? 0 : 1;
                root->m_child[p] = insert(root->m_child[p], k);
                return adjust(root, p);
            }
        }
        Node *erase(Node *root, KeyType k) {
            if (root == m_null) return m_null;
            if (root->m_key == k) {
                m_bouns_e = root->m_value;
                m_succeed = true;
                if (root->m_child[0] == m_null) {
                    if constexpr (BufferType<Node>::need_clct()) BufferType<Node>::collect(root);
                    return root->m_child[1];
                }
                if (root->m_child[1] == m_null) {
                    if constexpr (BufferType<Node>::need_clct()) BufferType<Node>::collect(root);
                    return root->m_child[0];
                }
                Node *u = last(root);
                std::swap(u->m_key, root->m_key);
                std::swap(u->m_value, root->m_value);
                root->m_size--;
                root->m_child[0] = erase(root->m_child[0], k);
            } else {
                int p = k < root->m_key ? 0 : 1;
                root->m_child[p] = erase(root->m_child[p], k);
                if (m_succeed) root->m_size--;
            }
            return root;
        }
        Node *update(Node *root, KeyType k, ValueType v) {
            if (root == m_null) {
                root = BufferType<Node>::allocate();
                root->m_key = k;
                root->m_value = v;
                root->m_child[0] = root->m_child[1] = m_null;
                root->m_size = 1;
                m_bonus_iu = root;
                m_succeed = true;
                return root;
            } else if (root->m_key == k) {
                root->m_value = v;
                return root;
            } else {
                int p = k < root->m_key ? 0 : 1;
                root->m_child[p] = update(root->m_child[p], k, v);
                root->m_size += m_succeed;
                return m_succeed ? adjust(root, p) : root;
            }
        }
        Node *find(Node *root, KeyType k) {
            if (root == m_null) return m_null;
            if (root->m_key == k) return root;
            return find(root->m_child[root->m_key <= k], k);
        }
        Node *rank(Node *root, uint32_t k) {
            if (root == m_null) return m_null;
            if (root->m_child[0]) {
                if (k < root->m_child[0]->m_size) return rank(root->m_child[0], k);
                else k -= root->m_child[0]->m_size;
            }
            if (!k) return root;
            else k--;
            return rank(root->m_child[1], k);
        }
        uint32_t count_less_than(Node *root, KeyType k) {
            if (root == m_null) return 0;
            if (k <= root->m_key) return count_less_than(root->m_child[0], k);
            return root->m_child[0]->m_size + 1 + count_less_than(root->m_child[1], k);
        }
        uint32_t count_greater_than(Node *root, KeyType k) {
            if (root == m_null) return 0;
            if (root->m_key <= k) return count_greater_than(root->m_child[1], k);
            return count_greater_than(root->m_child[0], k) + 1 + root->m_child[1]->m_size;
        }
        Node *maximum_less_than(Node *root, KeyType k) {
            if (root == m_null) return m_null;
            if (k <= root->m_key) return maximum_less_than(root->m_child[0], k);
            Node *z = maximum_less_than(root->m_child[1], k);
            return z == m_null ? root : z;
        }
        Node *minimum_greater_than(Node *root, KeyType k) {
            if (root == m_null) return m_null;
            if (root->m_key <= k) return minimum_greater_than(root->m_child[1], k);
            Node *z = minimum_greater_than(root->m_child[0], k);
            return z == m_null ? root : z;
        }
    };
}