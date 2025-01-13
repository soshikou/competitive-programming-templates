#pragma once
#include <bits/stdc++.h>

namespace Rem {
    template <typename T>
    struct BufferWithoutCollect {
    private:
        static T *s_buf;
        static uint32_t s_available;
    public:
        static void resize(uint32_t length) {
            if (s_buf != NULL) std::free(s_buf);
            s_buf = (T *)std::malloc(sizeof(T) * length);
            std::memset(s_buf, 0, sizeof(T) * length);
            s_available = 1;
        }
        static T *null() { return s_buf; }
        static T *allocate() { return s_buf + s_available++; }
        static void collect(T* ptr) {}
        static consteval bool need_clct() { return false; }
    };
    template <typename T>
    T *BufferWithoutCollect<T>::s_buf = NULL;
    template <typename T>
    uint32_t BufferWithoutCollect<T>::s_available = 0;

    template <typename T>
    struct BufferWithCollect {
    private:
        static T *s_buf;
        static T **s_available;
        static uint32_t s_tail;

    public:
        static void resize(uint32_t length) {
            if (s_buf != NULL) std::free(s_buf);
            s_buf = (T *)std::malloc(sizeof(T) * length);
            std::memset(s_buf, 0, sizeof(T) * length);
            if (s_available != NULL) std::free(s_available);
            s_available = (T **)std::malloc(sizeof(T *) * length);
            T *ptr = s_buf + 1;
            for (uint32_t i = 0; i < length; i++) s_available[i] = ptr++;
            s_tail = length - 2;
        }
        static T *null() { return s_buf; }
        static T *allocate() { return s_available[s_tail--]; }
        static void collect(T *ptr) { s_available[++s_tail] = ptr; }
        static consteval bool need_clct() { return true; }
    };
    template <typename T>
    T *BufferWithCollect<T>::s_buf = NULL;
    template <typename T>
    T **BufferWithCollect<T>::s_available = NULL;
    template <typename T>
    uint32_t BufferWithCollect<T>::s_tail = 0;
}