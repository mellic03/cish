
#include "../vm/bytecode.hpp"
#include <cish/compile-ctx.hpp>
#include <util/bitmanip.hpp>
#include <string.h>
#include <assert.h>



cish::StackFrame::StackFrame()
:   m_offset(0)
{

}


size_t cish::StackFrame::frameAlloc( size_t nbytes, size_t align )
{
    size_t prev = align_up(m_offset, align);
    m_offset = prev + nbytes;
    return prev;
}




cish::Symbol*
cish::StackFrame::find( const char *key )
{
    for (auto &sym: m_symbols)
        if (strcmp(key, sym.key) == 0)
            return &sym;
    return nullptr;
}


#define LOL_LMAO(Tp)\
cish::Symbol *cish::StackFrame::insert( const char *key, const Tp &data )\
{\
    assert((find(key) == nullptr));\
    auto *sym = m_symbols.push(Symbol(key));\
    new (sym->as_raw) Tp(data);\
    sym->tag = data.get_tag();\
    return sym;\
}

LOL_LMAO(cish::SymType);
LOL_LMAO(cish::SymFunc);
LOL_LMAO(cish::SymVar);

#undef LOL_LMAO



cish::CompileCtx::CompileCtx( uint32_t *buf, size_t bufsz )
:   m_buf((VmOp*)buf),
    m_rip(0)
{
    pushScope();

    createSymbol("i08", SymType(sizeof(int8_t), alignof(int8_t)));
    createSymbol("i16", SymType(sizeof(int16_t), alignof(int16_t)));
    createSymbol("i32", SymType(sizeof(int32_t), alignof(int32_t)));
    createSymbol("u08", SymType(sizeof(uint8_t), alignof(uint8_t)));
    createSymbol("u16", SymType(sizeof(uint16_t), alignof(uint16_t)));
    createSymbol("u32", SymType(sizeof(uint32_t), alignof(uint32_t)));

}


void cish::CompileCtx::pushScope()
{
    m_frames.push(StackFrame());
}


void cish::CompileCtx::popScope()
{
    m_frames.pop();
}





// cish::typesym_t*
// cish::CompileCtx::findType( const char *key )
// {
//     for (auto &sym: m_types)
//         if (strcmp(key, sym.key) == 0)
//             return &sym;
//     return nullptr;
// }

// cish::typesym_t*
// cish::CompileCtx::createType( const char *key, size_t size, size_t align )
// {
//     if (auto *sym = findType(key))
//         return sym;
//     return m_types.push(typesym_t(key, size, align));
// }





// cish::funcsym_t*
// cish::CompileCtx::findFunction( const char *key )
// {
//     for (auto &sym: m_funcs)
//         if (strcmp(key, sym.key) == 0)
//             return &sym;
//     return nullptr;
// }

// cish::funcsym_t*
// cish::CompileCtx::createFunction( const char *key, size_t addr )
// {
//     if (auto *sym = findFunction(key))
//         return sym;
//     return m_funcs.push({key, addr, 0});
// }


size_t cish::CompileCtx::frameAlloc( size_t nbytes, size_t align )
{
    return m_globals.frameAlloc(nbytes, align);
}


cish::Symbol*
cish::CompileCtx::findSymbol( const char *key )
{
    return m_globals.find(key);
    // for (int d=m_frames.size()-1; d>=0; d--)
    //     if (auto *sym = m_frames[d].find(key))
    //         return sym;
    // return nullptr;
}


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


