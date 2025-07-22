
#include "../vm/bytecode.hpp"
#include <cish/compile-ctx.hpp>
#include <util/bitmanip.hpp>
#include <string.h>
#include <stdio.h>
#include <assert.h>

using namespace cish;


CompileCtx::CompileCtx( VmOp *base, size_t size )
:   m_stacksz(2048),
    m_stack(new uint8_t[m_stacksz]),
    m_base(base),
    m_size(size)
{
    m_global = new Symtab();
    m_local  = m_global;
    clearRegs();

    Symtab *tab = m_global;

    #define ADD_TYPE(Nm, Tp) tab->insert(Nm, SymType(sizeof(Tp), alignof(Tp)))
    ADD_TYPE("i08", sizeof(int8_t));
    ADD_TYPE("i16", sizeof(int16_t));
    ADD_TYPE("i32", sizeof(int32_t));
    ADD_TYPE("i64", sizeof(int64_t));
    ADD_TYPE("u08", sizeof(uint8_t));
    ADD_TYPE("u16", sizeof(uint16_t));
    ADD_TYPE("u32", sizeof(uint32_t));
    ADD_TYPE("u64", sizeof(uint64_t));
    #undef ADD_TYPE

}


void CompileCtx::clearRegs()
{
    memset(m_regs, 0, sizeof(m_regs));
    m_rip = 0;
    m_rbp = m_stacksz;
    m_rsp = m_stacksz;
}


#define PUSHQ(X) m_rsp -= 8;   *(int64_t*)(m_stack + m_rsp) = X;
#define POPQ(X)  X = *(int64_t*)(m_stack + m_rsp);   m_rsp += 8;


void CompileCtx::pushFrame()
{
    // printf("[pushFrame] top=%ld\n", *(int64_t*)(m_stack + m_rbp));
    PUSHQ(m_rbp);
    m_rbp = m_rsp;
}


void CompileCtx::popFrame()
{
    m_rsp = m_rbp;
    POPQ(m_rbp);
    // printf("[pushFrame] top=%ld\n", *(int64_t*)(m_stack + m_rbp));

    assert((0 <= m_rsp));
    assert((m_rsp <= m_stacksz));
}


int64_t CompileCtx::resvFrame( int64_t size, int64_t align )
{
    m_rsp = align_down(m_rsp-size, align);
    return m_rbp - m_rsp;
}


void CompileCtx::pushTab()
{
    m_local = m_local->spawnChild();
}


void CompileCtx::popTab()
{
    if (m_local->parent)
        m_local = m_local->parent;
}




VmOp *CompileCtx::emit( const VmOp &op )
{
    return new (m_base + m_rip++) VmOp(op);
}

