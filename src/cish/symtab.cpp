
#include "../vm/bytecode.hpp"
#include <cish/compile-ctx.hpp>
#include <util/bitmanip.hpp>
#include <string.h>
#include <assert.h>



cish::Symtab::Symtab()
:   m_parent(nullptr),
    m_offset(0)
{

}


size_t cish::Symtab::frameAlloc( size_t nbytes, size_t align )
{
    size_t prev = align_up(m_offset, align);
    m_offset = prev + nbytes;
    return prev;
}

size_t cish::Symtab::frameOffset()
{
    return m_offset;
}



// cish::Symtab*
// cish::Symtab::addChild( Symtab *tab )
// {
//     tab->m_parent = this;
//     m_children.push(tab);
//     return tab;
// }


// void
// cish::Symtab::clear()
// {
//     m_offset = 0;
// }


cish::Symbol*
cish::Symtab::find( const char *key )
{
    for (auto &sym: m_symbols)
        if (strcmp(key, sym.key) == 0)
            return &sym;
    if (m_parent)
        return m_parent->find(key);
    return nullptr;
}


#define LOL_LMAO(Tp)\
cish::Symbol *cish::Symtab::insert( const char *key, const Tp &data )\
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

