
#include <cish/compile-ctx.hpp>
#include <util/bitmanip.hpp>
#include "../vm/bytecode.hpp"

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <new>

using namespace cish;


CompileCtx::CompileCtx( VmOp *base, size_t size )
:   m_base(base),
    m_size(size),
    vmstack()
{
    // m_symtabs.push(SymTab(512*1024, nullptr));
    m_symtabs.push(SymTab(nullptr));
    m_global = &(m_symtabs.front());
    m_global->loadGlobalSymbols();
    m_local  = m_global;
    clearRegs();
}


void CompileCtx::clearRegs()
{
    memset(m_regs, 0, sizeof(m_regs));
    m_rip = 0;
    vmstack.clear();
}

void CompileCtx::pushTab()
{
    // m_local = new SymTab(512*1024, m_local);
    m_local = new SymTab(m_local);
}

void CompileCtx::popTab()
{
    if (m_local->hasParent())
        m_local = m_local->getParent();
}


VmOp *CompileCtx::emit( const VmOp &op )
{
    return new (m_base + m_rip++) VmOp(op);
}

