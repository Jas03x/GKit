#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <stdio.h>

template<typename T>
struct Array
{
private:
    unsigned int m_size;
    T* m_array;

public:
    Array(unsigned int size)
    {
        m_size = size;
        m_array = new T[size];
    }

    ~Array()
    {
        delete[] m_array;
    }

    unsigned int size() { return m_size; }

    T& operator[] (unsigned int i)
    {
        return m_array[i];
    }
};

#endif // ARRAY_HPP
