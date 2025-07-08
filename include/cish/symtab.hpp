#pragma once
#include <stddef.h>
#include <stdint.h>
#include <cish/stack.hpp>
#include <cish/node.hpp>
#include <linkedlist.hpp>


namespace cish
{
    enum Sym_: uint8_t
    {
        Sym_Invalid,
        Sym_Type,
        Sym_Func,
        Sym_Var,
    };

    template <uint8_t Tg>
    struct SymBase
    {
        static constexpr uint8_t get_tag() { return Tg; }
    };

    struct SymType;
    struct SymFunc;
    struct SymVar;
    struct Symbol;
    class  Symtab;
}




struct cish::SymType: SymBase<Sym_Type>
{
    size_t size;
    size_t align;

    SymType( size_t sz, size_t al )
    :   size(sz), align(al) {  }
};



struct cish::SymFunc: SymBase<Sym_Func>
{
    const char *return_type;
    size_t addr;
    size_t argc;
    size_t resb;

    SymFunc( const char *ret_typename, size_t address )
    :   return_type(ret_typename), addr(address) {  }
};



struct cish::SymVar: SymBase<Sym_Var>
{
    const char *type_name;
    size_t rbpoff;

    SymVar( const char *tpname, size_t rbp_offset )
    :   type_name(tpname), rbpoff(rbp_offset) {  }
};



struct cish::Symbol
{
    const char *key;
    uint8_t tag;

    union {
        SymType as_Type;
        SymFunc as_Func;
        SymVar  as_Var;
        uint8_t as_raw[];
    };

    Symbol( const char *str="" )
    :   key(str) {  }
};




class cish::Symtab: public knl::LinkedListNode
{
private:
    template <typename T, size_t Cap>
    using Stack = cish::fixedsize_stack<T, Cap>;

    Stack<Symbol, 128>      m_symbols;
    size_t                  m_offset;

public:
    cish::Symtab           *m_parent;

    Symtab();

    size_t frameAlloc( size_t nbytes, size_t align );
    size_t frameOffset();

    Symbol *find( const char* );
    Symbol *insert( const char*, const SymType& );
    Symbol *insert( const char*, const SymFunc& );
    Symbol *insert( const char*, const SymVar& );

};
