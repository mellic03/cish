#pragma once
#include <stddef.h>
#include <stdint.h>
// #include <stdio.h>
#include <cish/stack.hpp>
#include <linkedlist.hpp>


namespace cish
{
    enum Sym_: uint8_t
    {
        Sym_None,
        Sym_Type,
        Sym_Func,
        Sym_Var,
    };

    template <Sym_ Tg=Sym_None>
    struct SymBase
    {
        static constexpr Sym_ GetTag() { return Tg; }
    };

    struct SymType;
    struct SymFunc;
    struct SymVar;
    struct Symbol;
    class  Symtab;
}



struct cish::SymType: SymBase<Sym_Type>
{
    int64_t size;
    int64_t align;
    uint8_t is_signed;

    SymType( int64_t sz, int64_t al, uint8_t sign=0 )
    :   size(sz), align(al), is_signed(sign) {  }
};


struct cish::SymFunc: SymBase<Sym_Func>
{
    const char *retkey;
    int64_t addr;
    int64_t argc;
    int64_t allocsz;

    SymFunc( const char *ret_type, int64_t address=0 )
    :   retkey(ret_type), addr(address) {  }
};


struct cish::SymVar: SymBase<Sym_Var>
{
    const char *typekey;
    int64_t     offset;

    SymVar( const char *tpkey="", int64_t rbpoff=0 )
    :   typekey(tpkey), offset(rbpoff) {  }
};


struct cish::Symbol
{
    static constexpr uint8_t NotDefined   = 0;
    static constexpr uint8_t SemiDefined  = 1;
    static constexpr uint8_t FullyDefined = 2;

    const char *key;
    uint8_t tag;
    uint8_t status;

    union {
        uint8_t   as_bytes[];
        SymBase<> as_Base;
        SymType   as_Type;
        SymFunc   as_Func;
        SymVar    as_Var;
    };

    Symbol( const char *str="" )
    :   key(str), tag(Sym_None), status(NotDefined) {  }

    template <typename T>
    Symbol( const char *K, const T &data )
    :   key(K), tag(T::SymTag())
    {
        new (as_bytes) T(data);
    }
};




class cish::Symtab: public knl::LinkedListNode
{
private:
    template <typename T, size_t Cap>
    using Stack = cish::fixedsize_stack<T, Cap>;
    Stack<Symbol, 128>  m_symbols;
    Symbol *_find( const char* );
    Symbol *_insert( const char* );


public:
    Symtab *parent;
    int     depth;

    Symtab();
    Symtab( Symtab *P );

    void    clear();
    Symtab *spawnChild();
    Symbol *find( const char* );
    Symbol *insert( const char* );

    template <typename T>
    struct sym_pair
    {
        T *tsm; Symbol *sym;

        sym_pair( T *t, Symbol *s )
        :   tsm(t), sym(s) {  };
    };

    template <typename T>
    sym_pair<T> find( const char *key )
    {
        if (auto *sym = _find(key))
            if (sym->tag == T::GetTag())
                return sym_pair<T>((T*)(sym->as_bytes), sym);
        // printf( "[Symtab::find<T>] No symbol \"%s\" with tag %u\n", key, T::GetTag());
        assert((false));
        return sym_pair<T>(nullptr, nullptr);
    }

    template <typename T>
    sym_pair<T> insert( const char *key, const T &data )
    {
        auto *sym = insert(key);
              sym->tag = T::GetTag();
        auto *tsm = new (sym->as_bytes) T(data);
        return sym_pair<T>(tsm, sym);
    }
};




namespace cish
{
    struct Strtab;
}


struct cish::Strtab
{
    char *m_beg;
    char *m_end;

    Strtab() {  }



};



