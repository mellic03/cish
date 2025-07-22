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
    size_t   m_stacksz;
    uint8_t *m_stack;

    Symtab  *m_global;
    Symtab  *m_local;

    union { int64_t m_regs[4]; struct { int64_t m_rip, m_rsp, m_rbp, m_rxx; }; };

public:
    VmOp  *m_base;
    size_t m_size;

    CompileCtx() {  }
    CompileCtx( VmOp *base, size_t size );

    int64_t rip() { return m_rip; }
    int64_t rsp() { return m_rsp; }
    int64_t rbp() { return m_rbp; }
    // int64_t rbpoff() { return m_rbp - m_rsp; }

    void clearRegs();
    void pushFrame();
    void popFrame();
    int64_t resvFrame( int64_t size, int64_t align );

    Symtab *globalTab() { return m_global; }
    Symtab *localTab()  { return m_local; }
    void    pushTab();
    void    popTab();

    template <typename... Args>
    VmOp *emit( Args... args )
    {
        return new (m_base + m_rip++) VmOp(args...);
    }

    VmOp *emit( const VmOp &op );

    // Symbol *findSymbol( const char *key );

    // template <typename T>
    // Symbol *insertSymbol( const char *key, const T &data )
    // {
    //     return getLocal().insert(key, data);
    // }

};
