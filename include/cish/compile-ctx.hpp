#pragma once
#include <stddef.h>
#include <stdint.h>
#include <cish/stack.hpp>
#include <cish/node.hpp>
#include <cish/symtab.hpp>

namespace cish
{
    struct VmOp;
    class CompileCtx;
}



class cish::CompileCtx
{
private:
    fixedsize_stack<Symtab*, 128> m_symtabs;
    VmOp  *m_buf;
    size_t m_rip;

public:
    CompileCtx() {  }
    CompileCtx( uint32_t *buf, size_t bufsz );

    // size_t frameAlloc( size_t nbytes, size_t align );
    uint32_t rip() { return m_rip; }
    void ripReset() { m_rip = 0; }

    template <typename... Args>
    VmOp *emit( Args... args )
    {
        return new (m_buf + m_rip++) VmOp(args...);
    }

    VmOp *emit( const VmOp &op )
    {
        return new (m_buf + m_rip++) VmOp(op);
    }

    Symtab &getGlobal() { return *m_symtabs[0]; }
    Symtab &getLocal()  { return *m_symtabs.top(); }
    void    pushLocal();
    void    popLocal();
    // Symbol *findSymbol( const char *key );

    // template <typename T>
    // Symbol *insertSymbol( const char *key, const T &data )
    // {
    //     return getLocal().insert(key, data);
    // }

};
