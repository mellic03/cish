
#pragma once
#include <stddef.h>
#include <stdint.h>
#include <new>
#include <cish/stack.hpp>


namespace cish
{
    enum Sym_: uint8_t
    {
        Sym_None,
        Sym_Type,
        Sym_Func,
        Sym_Var,
        Sym_Kwd,
    };

    struct SymBase;
    struct SymType;
    struct SymFunc;
    struct SymVar;
    struct SymKwd;

    struct Symbol;
    class  SymTab;
}


struct cish::SymBase
{
    Sym_ tag;
    SymBase( Sym_ tg ): tag(tg) {  }
};


struct cish::SymType: SymBase
{
    int64_t size;
    int64_t align;
    uint8_t is_signed;

    SymType( int64_t sz, int64_t al, uint8_t sign=0 )
    :   SymBase(Sym_Type), size(sz), align(al), is_signed(sign) {  }
};


struct cish::SymFunc: SymBase
{
    const char *type; // return type
    int64_t addr;
    int64_t argc;
    int64_t size; // total size of locals on stack 

    SymFunc( const char *type, int64_t addr=0 )
    :   SymBase(Sym_Func), type(type), addr(addr) {  }
};


struct cish::SymVar: SymBase
{
    const char *type; // type name
    int64_t     offset;

    SymVar( const char *type, int64_t offset=0 )
    :   SymBase(Sym_Var), type(type), offset(offset) {  }
};


struct cish::SymKwd: SymBase
{
    SymKwd(): SymBase(Sym_Kwd) {  }
};


struct cish::Symbol
{
    const char *key;

    union {
        uint8_t as_bytes[];
        SymBase as_Base;
        SymType as_Type;
        SymFunc as_Func;
        SymVar  as_Var;
        SymKwd  as_Kwd;
    };

    Symbol( const char *key="" )
    :   key(key)
    {
        new (&as_Base) SymBase(Sym_None);
    }

    template <typename T>
    Symbol( const char *key, const T &data )
    :   key(key)
    {
        new (&as_Base) T(data);
    }
};



#include <stdio.h>
class cish::SymTab
{
private:
    template <typename T, size_t Cap>
    using Stack = cish::fixedsize_stack<T, Cap>;
    Stack<Symbol, 128>  m_symbols;
    Symbol *_find( const char* );
    Symbol *_insert( const char* );
    SymTab *m_parent;

public:
    SymTab();
    SymTab( SymTab *P );

    void    loadGlobalSymbols();
    bool    hasParent() { return (m_parent != nullptr); }
    SymTab *getParent() { return m_parent; }

    void    clear();
    SymTab *spawnChild();
    Symbol *find( const char* );
    Symbol *insert( const char* );

    template <typename T>
    struct sym_pair
    {
        T *tsym; Symbol *sym;
        sym_pair( T *t, Symbol *s ): tsym(t), sym(s) {  };
    };

    template <typename T>
    T *find( const char *key )
    {
        if (Symbol *sym = _find(key))
            return (T*)(sym->as_bytes);
        return nullptr;
        // assert((false));
    }

    // template <typename T>
    // T *insert( const char *key, const T &data )
    // {
    //     if (Symbol *sym = insert(key))
    //         return new (sym->as_bytes) T(data);
    //     return nullptr;
    // }

    template <typename T, typename... Args>
    T *insert( const char *key, Args... args )
    {
        if (Symbol *sym = insert(key))
            return new (sym->as_bytes) T(args...);
        return nullptr;
    }
};
