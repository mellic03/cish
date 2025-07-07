#pragma once
#include <stddef.h>


namespace cish
{
    template <typename T>
    class inplace_stack;

    template <typename T, size_t Cap>
    class fixedsize_stack;
}


template <typename T>
class cish::inplace_stack
{
private:
    T *m_buf;
    int m_idx;

public:
    inplace_stack( T *buf )
    :   m_buf(buf), m_idx(0) {  };

    void push( const T &x ) { m_buf[m_idx++] = x; }
    T  pop() { return m_buf[--m_idx]; }
    T &top() { return m_buf[m_idx-1]; }
};



template <typename T, size_t Cap>
class cish::fixedsize_stack
{
private:
    T m_buf[Cap];
    int m_idx;

public:
    fixedsize_stack()
    :   m_idx(0) {  };

    T *push( const T &x )
    {
        m_buf[m_idx++] = x;
        return &m_buf[m_idx-1];
    }

    T  pop() { return m_buf[--m_idx]; }
    T &top() { return m_buf[m_idx-1]; }
    int size() { return m_idx; }

    T &operator[](int i) { return m_buf[i]; }
    const T &operator[](int i) const { return m_buf[i]; }

    struct iterator;
    iterator begin() { return iterator(m_buf + 0); };
    iterator end()   { return iterator(m_buf + m_idx);  };
};



template <typename T, size_t Cap>
struct cish::fixedsize_stack<T, Cap>::iterator
{
    T *ptr;

    iterator( T *p )                 : ptr(p)         {  };
    iterator( const iterator &other ): ptr(other.ptr) {  };

    iterator &operator++()
    {
        ptr++;
        return *this;
    };

    iterator operator++(int)
    {
        return iterator(ptr++);
    };

    bool operator == ( const iterator &rhs ) { return ptr == rhs.ptr; };
    bool operator != ( const iterator &rhs ) { return ptr != rhs.ptr; };
    T &operator * () { return *ptr; };

};

