#include <cish/symtab.hpp>
#include <cish/compile-ctx.hpp>
#include <util/bitmanip.hpp>
#include "../vm/bytecode.hpp"

#include <string.h>
#include <stdio.h>
#include <assert.h>

using namespace cish;


cish::SymTab::SymTab()
{
    m_parent = nullptr;
    clear();
}

cish::SymTab::SymTab( SymTab *parent )
{
    m_parent = parent;
    clear();
}


void cish::SymTab::loadGlobalSymbols()
{
    #define ADD_TYPE(Nm, Tp, Sn) insert<SymType>(Nm, sizeof(Tp), alignof(Tp), Sn)
    ADD_TYPE("char",   char,   true);
    ADD_TYPE("int",    int,    true);
    ADD_TYPE("float",  float,  true);
    ADD_TYPE("double", double, true);
    ADD_TYPE("int8_t",  int8_t,  true);
    ADD_TYPE("int16_t", int16_t, true);
    ADD_TYPE("int32_t", int32_t, true);
    ADD_TYPE("int64_t", int64_t, true);
    ADD_TYPE("uint8_t",  uint8_t,  false);
    ADD_TYPE("uint16_t", uint16_t, false);
    ADD_TYPE("uint32_t", uint32_t, false);
    ADD_TYPE("uint64_t", uint64_t, false);
    insert<SymType>("void", 0, 0, false);
    #undef ADD_TYPE

    #define ADD_KWD(Nm) insert<SymKwd>(Nm)
    ADD_KWD("switch");
    ADD_KWD("const");
    ADD_KWD("if");
    ADD_KWD("else");
    ADD_KWD("for");
    ADD_KWD("while");
    ADD_KWD("return");
    #undef ADD_KWD
}




void SymTab::clear()
{
    memset(&m_symbols[0], 0, sizeof(m_symbols));
    m_symbols.clear();
}


SymTab *SymTab::spawnChild()
{
    return new SymTab(this);
}



Symbol *SymTab::_find( const char *key )
{
    for (auto &sym: m_symbols)
    {
        // printf("strcmmp(%s, %s)\n", key, sym.as_Kwd.key);
        if (strcmp(key, sym.key) == 0)
            return &sym;
    }
    if (m_parent)
        return m_parent->_find(key);
    return nullptr;
}


Symbol *SymTab::_insert( const char *key )
{
    m_symbols.push(Symbol(key));
    return &m_symbols.top();
}



Symbol *SymTab::find( const char *key )
{
    Symbol *sym = _find(key);

    if (sym == nullptr)
    {
        fprintf(stderr, "[SymTab::find] could not find symbol \"%s\"\n", key);
        assert((false));
    }

    return sym;
}


Symbol *SymTab::insert( const char *key )
{
    if (_find(key))
    {
        printf("Multiple definition of symbol \"%s\"\n", key);
        assert(false);
    }

    m_symbols.push(Symbol(key));
    return &m_symbols.top();
}



// template <typename T>
// cish::Symbol insert( const char *key, const T &data )
// {
//     cish::Symbol sym(key);
//     new (sym.as_bytes) T(data);
//     return sym;
// }


// Symbol *SymTab::addType( const char *key, const SymType &data )
// {
//     assert((find(key) == nullptr));
//     m_symbols.push(insert(key, data));
//     return &m_symbols.top();
// }

// cish::Symbol *cish::SymTab::addFunc( const char *key, const SymFunc &data )
// {
//     assert((find(key) == nullptr));
//     m_symbols.push(insert(key, data));
//     return &m_symbols.top();
// }

// cish::Symbol *cish::SymTab::addVar( const char *idntkey, const char *typekey )
// {
//     assert((find(idntkey) == nullptr));
//     auto *sym = m_symbols.push(Symbol(Sym_Var, idntkey));
//     sym->as_Var.typekey = typekey;
//     // sym->as_Var.rbpoff  = align_up(m_offset,) = typekey;
//     return sym;
// }

