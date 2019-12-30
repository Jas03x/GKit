#ifndef CONST_VECTOR_HPP
#define CONST_VECTOR_HPP

#include <stdio.h>

template<typename T, unsigned int N>
struct ConstVector 
{
private:
    unsigned int m_size;
    T* m_array;

public:
    ConstVector()
    {
        m_size = 0;
        m_array = new T[N];
    }

    ~ConstVector()
    {
        delete[] m_array;
    }

    unsigned int size()     { return m_size; }
    unsigned int capacity() { return N;      }

    void push_back(T* obj)
    {
        if(m_size < N) {
            m_array[m_size++] = obj;
        } else {
            printf("warning: array length exceeded\n");
        }
    }

    T& operator[] (unsigned int i)
    {
        return m_array[i];
    }
};

#endif // CONST_VECTOR_HPP
