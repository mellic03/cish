
#include "../vm/bytecode.hpp"
#include <cish/compile-ctx.hpp>
#include <util/bitmanip.hpp>
#include <string.h>
#include <assert.h>


cish::CompileCtx::CompileCtx( uint32_t *buf, size_t bufsz )
:   m_buf((VmOp*)buf),
    m_rip(0)
{
    pushLocal();

    Symtab &tab = getGlobal();
    tab.insert("i08", SymType(sizeof(int8_t), alignof(int8_t)));
    tab.insert("i16", SymType(sizeof(int16_t), alignof(int16_t)));
    tab.insert("i32", SymType(sizeof(int32_t), alignof(int32_t)));
    tab.insert("u08", SymType(sizeof(uint8_t), alignof(uint8_t)));
    tab.insert("u16", SymType(sizeof(uint16_t), alignof(uint16_t)));
    tab.insert("u32", SymType(sizeof(uint32_t), alignof(uint32_t)));
    tab.insert("uchp", SymType(sizeof(char*), alignof(char*)));

}

void cish::CompileCtx::pushLocal()
{
    if (m_symtabs.size() == 0)
    {
        m_symtabs.push(new Symtab());
        m_symtabs.top()->m_parent = nullptr;
    }

    else
    {
        Symtab *prev = m_symtabs.top();
        m_symtabs.push(new Symtab());
        m_symtabs.top()->m_parent = prev;
    }
}


void cish::CompileCtx::popLocal()
{
    delete m_symtabs.pop();
}



// size_t cish::CompileCtx::frameAlloc( size_t nbytes, size_t align )
// {
//     return m_globals.frameAlloc(nbytes, align);
// }


// cish::Symbol*
// cish::CompileCtx::findSymbol( const char *key )
// {
//     return m_symtabs.top()->find(key);
// }


// cish::scope_sym_t
// cish::CompileCtx::findObject( const char *key )
// {
//     for (int d=m_frames.size()-1; d>=0; d--)
//         if (sym_t *sym = m_frames[d].findLocal(key))
//             return scope_sym_t(d, sym);
//     return scope_sym_t(-1, nullptr);
// }

// cish::scope_sym_t
// cish::CompileCtx::createObject( const char *key, size_t size, size_t align )
// {
//     auto ssym = findObject(key);
//     if (ssym.sym)
//         return ssym;
//     int depth = m_frames.size() - 1;
//     sym_t *sym = m_frames.top().createLocal(key, size, align);
//     return scope_sym_t(depth, sym);
// }


