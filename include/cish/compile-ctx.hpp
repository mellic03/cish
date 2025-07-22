#pragma once
#include <stddef.h>
#include <stdint.h>

#include <cish/stack.hpp>
#include <cish/symbol.hpp>


namespace cish
{
    struct VmOp;
    class CompileCtx;
}



class cish::CompileCtx
{
private:
    template <typename T, size_t Cap>
    using Stack = cish::fixedsize_stack<T, Cap>;

    Stack<SymTab, 128>  m_symtabs;
    SymTab             *m_global;
    SymTab             *m_local;

    union { int64_t m_regs[4]; struct { int64_t m_rip, m_rsp, m_rbp, m_rxx; }; };

public:
    VmOp         *m_base;
    size_t        m_size;
    VmStack<2048> vmstack;

    CompileCtx(): vmstack() {  }
    CompileCtx( VmOp *base, size_t size );

    int64_t rip() { return m_rip; }
    int64_t rsp() { return m_rsp; }
    int64_t rbp() { return m_rbp; }
    // int64_t rbpoff() { return m_rbp - m_rsp; }

    void clearRegs();
    // void pushFrame();
    // void popFrame();
    int64_t resvFrame( int64_t size, int64_t align );

    SymTab *globalTab() { return m_global; }
    SymTab *localTab()  { return m_local; }
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
