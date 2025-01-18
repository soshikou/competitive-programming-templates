#pragma once
#include <bits/stdc++.h>

namespace Rem {
    class Buffer {
        static char *s_buf;
        static char *s_ptr;
    public:
        static void reset() { s_ptr = s_buf; }
        template <typename T>
        static T *allocate() {
            T *res = (T *)s_ptr;
            s_ptr += sizeof(T);
            return res;
        }
        template <typename T>
        static T *allocate(uint32_t n) {
            T *res = (T *)s_ptr;
            s_ptr += sizeof(T) * n;
            return res;
        }
        static uint32_t status() { return (s_ptr - s_buf) / 1048576; }
    };
#ifndef REM_MAX_BUFFER_SIZE
#define REM_MAX_BUFFER_SIZE 128
#endif
    char *Buffer::s_buf = (char *)std::malloc(REM_MAX_BUFFER_SIZE * 1048576);
    char *Buffer::s_ptr = s_buf;
    template <typename T>
    struct vector {
        uint32_t m_size;
        T *m_data;
    };
}