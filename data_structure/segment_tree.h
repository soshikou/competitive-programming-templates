#pragma once
#include <bits/stdc++.h>

namespace Rem {
    /*
    struct LazyType {
        void merge(const LazyType &rhs);
        void init();
    };
    struct ResponseType {
        void merge(const LazyType &rhs, int32_t length);
        void merge(const ResponseType &rhs);
        template <typename T>
        void init(T *src, int32_t pos);
    };
    */
    template <typename LazyType, typename ResponseType>
    class SegmentTree {
        struct Node {
            int32_t m_l, m_mid, m_r;
            bool has_lazy;
            LazyType m_lazy;
            ResponseType m_response;
            Node *m_lchild, *m_rchild;
            Node() : m_lchild(NULL), m_rchild(NULL), has_lazy(false) { m_lazy.init(); }
        };
        Node *m_root;
    public:
        SegmentTree() : m_root(new Node) {}
        template <typename T>
        void build(T *src, int32_t l, int32_t r) { _build(m_root, src, l, r); }
        void update(int32_t l, int32_t r, const LazyType &lazy) { _update(m_root, l, r, lazy); }
        ResponseType query(int32_t l, int32_t r) { return _query(m_root, l, r); }
    private:
        void push_up(Node* root) {
            root->m_response = root->m_lchild->m_response;
            root->m_response.merge(root->m_rchild->m_response);
        }
        void push_down(Node* root) {
            if (!root->has_lazy) return;
            root->m_lchild->m_response.merge(root->m_lazy, root->m_lchild->m_r - root->m_lchild->m_l);
            root->m_lchild->m_lazy.merge(root->m_lazy);
            root->m_rchild->m_response.merge(root->m_lazy, root->m_rchild->m_r - root->m_rchild->m_l);
            root->m_rchild->m_lazy.merge(root->m_lazy);
            root->m_lchild->has_lazy = root->m_rchild->has_lazy = true;
            root->m_lazy.init();
            root->has_lazy = false;
        }
        template <typename T>
        void _build(Node* root, T *src, int32_t l, int32_t r) {
            root->m_l = l, root->m_r = r;
            if (l + 1 == r) {
                root->m_lazy.init();
                root->m_response.init(src, l);
                return;
            }
            root->m_mid = (l + r) >> 1;
            root->m_lchild = new Node;
            root->m_rchild = new Node;
            _build(root->m_lchild, src, l, root->m_mid);
            _build(root->m_rchild, src, root->m_mid, r);
            push_up(root);
        }
        void _update(Node *root, int32_t l, int32_t r, const LazyType &lazy) {
            if (l == root->m_l && r == root->m_r) {
                root->has_lazy = true;
                root->m_lazy.merge(lazy);
                root->m_response.merge(lazy, root->m_r - root->m_l);
                return;
            }
            push_down(root);
            if (l < root->m_mid) _update(root->m_lchild, l, r < root->m_mid ? r : root->m_mid, lazy);
            if (root->m_mid < r) _update(root->m_rchild, root->m_mid < l ? l : root->m_mid, r, lazy);
            push_up(root);
        }
        ResponseType _query(Node* root, int32_t l, int32_t r) {
            if (l == root->m_l && r == root->m_r) return root->m_response;
            push_down(root);
            if (r <= root->m_mid) return _query(root->m_lchild, l, r);
            if (root->m_mid <= l) return _query(root->m_rchild, l, r);
            ResponseType res = _query(root->m_lchild, l, root->m_mid);
            res.merge(_query(root->m_rchild, root->m_mid, r));
            return res;
        }
    };
    /*
    struct LazyType {};
    struct ResponseType {
        void merge(const LazyType &rhs, int32_t length);
        void merge(const ResponseType &rhs);
        template <typename T>
        void init(T *src, int32_t pos);
    };
    */
    template <typename LazyType, typename ResponseType>
    class PersistentSegmentTree {
        struct Node {
            int32_t m_l, m_mid, m_r;
            ResponseType m_response;
            Node *m_lchild, *m_rchild;
            Node() : m_lchild(NULL), m_rchild(NULL) {}
        };
        std::vector<Node *> m_roots;
    public:
        PersistentSegmentTree() { m_roots.push_back(new Node); }
        template <typename T>
        void build(T *src, int32_t l, int32_t r) { _build(m_roots[0], src, l, r); }
        void update(int32_t pos, const LazyType &lazy, uint32_t ver = 0) {
            m_roots.push_back(new Node);
            _update(m_roots[ver], m_roots.back(), pos, lazy);
        }
        ResponseType query(int32_t l, int32_t r, uint32_t ver = 0) { return _query(m_roots[ver], l, r); }
    private:
        void push_up(Node* root) {
            root->m_response = root->m_lchild->m_response;
            root->m_response.merge(root->m_rchild->m_response);
        }
        template <typename T>
        void _build(Node* root, T *src, int32_t l, int32_t r) {
            root->m_l = l, root->m_r = r;
            if (l + 1 == r) {
                root->m_response.init(src, l);
                return;
            }
            root->m_mid = (l + r) >> 1;
            root->m_lchild = new Node;
            root->m_rchild = new Node;
            _build(root->m_lchild, src, l, root->m_mid);
            _build(root->m_rchild, src, root->m_mid, r);
            push_up(root);
        }
        void _update(Node *root, Node *nroot, int32_t pos, const LazyType &lazy) {
            memcpy(nroot, root, sizeof(Node));
            if (root->m_l == pos && root->m_l + 1 == root->m_r) {
                nroot->m_response.merge(lazy);
                return;
            }
            if (pos < root->m_mid) {
                nroot->m_lchild = new Node;
                _update(root->m_lchild, nroot->m_lchild, pos, lazy);
            } else {
                nroot->m_rchild = new Node;
                _update(root->m_rchild, nroot->m_rchild, pos, lazy);
            }
            push_up(nroot);
        }
        ResponseType _query(Node* root, int32_t l, int32_t r) {
            if (l == root->m_l && r == root->m_r) return root->m_response;
            if (r <= root->m_mid) return _query(root->m_lchild, l, r);
            if (root->m_mid <= l) return _query(root->m_rchild, l, r);
            ResponseType res = _query(root->m_lchild, l, root->m_mid);
            res.merge(_query(root->m_rchild, root->m_mid, r));
            return res;
        }
    };
}