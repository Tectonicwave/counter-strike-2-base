#pragma once

#include <stdexcept>

namespace utils
{
    template <typename T>
    class CUtlVector
    {
    public:
        using iterator = T*;
        using const_iterator = const T*;

        iterator begin() { return elements; }

        iterator end() { return elements + size; }

        const_iterator begin() const { return elements; }

        const_iterator end() const { return elements + size; }

        T& operator[](int i) { return elements[i]; }

        int size;
        T* elements;
    };

    template <typename T>
    class C_NetworkUtlVectorBase {
    public:
        uint32_t size;
    private:
        uint32_t pad;
    public:
        T* data;
    };
}