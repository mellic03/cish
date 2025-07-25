#pragma once
#include <stddef.h>
#include <assert.h>
#include <util/bitmanip.hpp>


namespace cish
{
    template <typename T>
    class inplace_stack;

    template <typename T, size_t Cap>
    class fixedsize_stack;

    template <int64_t Cap>
    struct VmStack;
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




inline int minfn(int x, int y) { return (x < y) ? x : y; };
inline int maxfn(int x, int y) { return (x > y) ? x : y; };
inline int clampfn(int x, int lo, int hi) { return maxfn(lo, minfn(x, hi)); };

template <typename T, size_t Cap>
class cish::fixedsize_stack
{
private:
    T  *m_buf;
    int m_idx;

public:
    fixedsize_stack()
    :   m_buf(new T[Cap]), m_idx(0) {  };

    void push( const T &x )
    {
        m_buf[m_idx++] = x;
    }

    T pop()
    {
        T *ptr = &m_buf[--m_idx];
        m_idx = clampfn(m_idx, 0, Cap-1);
        return *ptr;
    }

    T &top()
    {
        int idx = m_idx-1;
        assert((0<=idx && idx<Cap));
        return m_buf[idx];
    }

    int size() { return m_idx; }
    int capacity() { return (int)Cap; }
    void clear() { m_idx = 0; }

    T &front() { return m_buf[0]; }
    const T &front() const { return m_buf[0]; }

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




template <size_t Cap>
struct VmStack
{
    uint8_t data[Cap];
    uint8_t *end;
    int64_t &rsp;
    int64_t &rbp;

    VmStack( int64_t &sp, int64_t &bp )
    : end(data+Cap), rsp(sp), rbp(bp)
    {
        rsp = Cap;
        rbp = Cap;
    }

    VmStack( const VmStack& ) = delete;
    VmStack( VmStack&& ) = delete;
    VmStack &operator=( const VmStack& ) = delete;
    VmStack &operator=( VmStack&& ) = delete;

    template <typename T>
    void push( T x )
    {
        rsp -= sizeof(T);
        *(T*)(data+rsp) = x;
    }

    template <typename T>
    T pop()
    {
        rsp += sizeof(T);
        return *(T*)(data+rsp - sizeof(T));
    }

    template <typename T>
    void swap()
    {
        T A = pop<T>();
        T B = pop<T>();
        push<T>(A);
        push<T>(B);
    }

    template <typename T>
    T &get( int64_t offset )
    {
        return *(T*)(data + offset);
    }

    size_t capacity() { return Cap; }
};




template <int64_t Cap>
struct cish::VmStack
{
    uint8_t  m_base[Cap];
    int64_t  rbp;
    int64_t  rsp;

    VmStack()
    {
        clear();
    }

    VmStack( const VmStack& ) = delete;
    VmStack( VmStack&& ) = delete;
    VmStack &operator=( const VmStack& ) = delete;
    VmStack &operator=( VmStack&& ) = delete;

    template <typename T>
    void pushq( T x )
    {
        rsp -= 8;
        assert((0 <= rsp));

        *(int64_t*)(m_base + rsp) = x;
    }

    template <typename T>
    void popq( T *x = nullptr)
    {
        if (x) x = *(int64_t*)(m_base + rsp);
    
        rsp += 8;
        assert((0 <= rsp));
        assert((rsp <= Cap+8));
    }

    void clear()
    {
        rbp = Cap;
        rsp = Cap;
    }

    size_t frame_resv( size_t size, size_t align )
    {
        rsp = align_down(rsp-size, align);
        return rbp - rsp;
    }

    void frame_push()
    {
        pushq(rbp);
        rbp = rsp;
    }

    void frame_pop()
    {
        rsp = rbp;
        popq(&rbp);
    }
};

