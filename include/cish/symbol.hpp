#pragma once
#include <stddef.h>
#include <stdint.h>


namespace cish
{
    struct StrTab;
    struct SymTab;
    struct Symbol;

    namespace sym
    {
        enum class Tag: int
        {
            None,
            Kwd,
            Type,
            Func,
            Var,
        };

        template <Tag Tg>
        struct Base;

        struct Kwd;
        struct Type;
        struct Func;
        struct Var;
    }
}



struct cish::StrTab
{
private:
    char *m_base;
    char *m_end;
    char *m_tail;

public:
    StrTab( size_t size );

    int write( const char *str );
    const char *read( int offset );
};




struct cish::SymTab
{
public:
    static constexpr uint32_t MAGIC = 0xDEADBEBE;

    SymTab(): m_parent(nullptr) {  }
    SymTab( size_t size, SymTab *parent = nullptr );
    void loadGlobalSymbols();

    bool    hasParent() { return (m_parent != nullptr); }
    SymTab *getParent() { return m_parent; }

    /** Return object offset if enough space, otherwise -1. */
    int write( const char *label, int tag, const void *data, size_t size );

    /** Return object if offset is valid, otherwise nullptr. */
    void *read( int offset, size_t size );

    /** Return object if matching label and tag found, otherwise nullptr. */
    void *find( const char *label, int tag );


    template <typename sym_t>
    int insert( const char *label, const sym_t &sym )
    {
        return write(label, (int)sym_t::TAG, &sym, sizeof(sym_t));
    }

    template <typename sym_t>
    sym_t *find( const char *label )
    {
        return (sym_t*)find(label, (int)sym_t::TAG);
    }

private:
    uint8_t *m_base;
    uint8_t *m_end;
    uint8_t *m_tail;
    SymTab  *m_parent;
};



struct alignas(16) cish::Symbol
{
    uint64_t    magic;
    const char *label;
    int32_t     tag;
    int32_t     size;
    uint8_t     data[];

    Symbol( const char *str, int tg, int sz )
    :   magic(cish::SymTab::MAGIC), label(str), tag(tg), size(sz) {  };
};







template <cish::sym::Tag Tg>
struct alignas(16) cish::sym::Base 
{
    static constexpr Tag TAG = Tg;
};


struct cish::sym::Kwd: Base<Tag::Kwd>
{

};


struct cish::sym::Type: Base<Tag::Type>
{
    int64_t size;
    int64_t align;
    uint8_t is_signed;

    Type( int64_t sz, int64_t al, uint8_t sign=0 )
    :   size(sz), align(al), is_signed(sign) {  }
};


struct cish::sym::Func: Base<Tag::Func>
{
    const char *retkey;
    int64_t addr;
    int64_t argc;
    int64_t allocsz;

    Func( const char *ret_type, int64_t address=0 )
    :   retkey(ret_type), addr(address) {  }
};


struct cish::sym::Var: Base<Tag::Var>
{
    const char *typekey;
    int64_t     offset;

    Var( const char *tpkey="", int64_t rbpoff=0 )
    :   typekey(tpkey), offset(rbpoff) {  }
};