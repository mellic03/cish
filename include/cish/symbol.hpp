// #pragma once
// #include <stddef.h>
// #include <stdint.h>


// namespace cish
// {
//     static constexpr uint32_t SYM_MAGIC = 0xDEADBEBE;

//     // struct StrTab;
//     struct SymTab;
//     struct Symbol;

//     template <typename T>
//     struct sym_pair
//     {
//         T      *tsym;
//         Symbol *sym;
//         sym_pair( T *t, Symbol *s )
//         :   tsym(t), sym(s) {  }
//     };


//     namespace sym
//     {
//         enum class Tag: int
//         {
//             None,
//             Kwd,
//             Type,
//             Func,
//             Var,
//         };

//         template <Tag Tg>
//         struct Base;

//         struct Kwd;
//         struct Type;
//         struct Func;
//         struct Var;
//     }
// }



// // struct cish::StrTab
// // {
// // private:
// //     char *m_base;
// //     char *m_end;
// //     char *m_tail;

// // public:
// //     StrTab( size_t size );

// //     int write( const char *str );
// //     const char *read( int offset );
// // };


// struct alignas(16) cish::Symbol
// {
//     uint64_t    magic;
//     const char *label;
//     int32_t     tag;
//     int32_t     size;
//     uint8_t     data[];

//     Symbol( const char *str, int tg, int sz )
//     :   magic(cish::SYM_MAGIC), label(str), tag(tg), size(sz) {  };
// };




// struct cish::SymTab
// {
// public:

//     SymTab(): m_parent(nullptr), m_count(0) {  }
//     SymTab( size_t size, SymTab *parent = nullptr );
//     void loadGlobalSymbols();

//     bool    hasParent() { return (m_parent != nullptr); }
//     SymTab *getParent() { return m_parent; }

//     int write( const char *label, int tag, const void *data, size_t size );
//     void *read( int offset, size_t size );
//     Symbol *find( const char *label );

//     template <typename sym_t>
//     int insert( const char *label, const sym_t &sym )
//     {
//         return write(label, (int)sym_t::tag, &sym, sizeof(sym_t));
//     }

//     template <typename T>
//     sym_pair<T> find( const char *label )
//     {
//         Symbol *sym = find(label);
//         if (sym == nullptr)
//             return sym_pair<T>(nullptr, nullptr);
//         return sym_pair<T>((T*)(sym->data), sym);
//     }


// private:
//     uint8_t *m_base;
//     uint8_t *m_end;
//     uint8_t *m_tail;
//     size_t   m_count;
//     SymTab  *m_parent;
// };




// template <cish::sym::Tag Tg>
// struct alignas(16) cish::sym::Base 
// {
//     static constexpr Tag tag = Tg;
// };


// struct cish::sym::Kwd: Base<Tag::Kwd>
// {

// };


// struct cish::sym::Type: Base<Tag::Type>
// {
//     int64_t size;
//     int64_t align;
//     uint8_t is_signed;

//     Type( int64_t sz, int64_t al, uint8_t sign=0 )
//     :   size(sz), align(al), is_signed(sign) {  }
// };


// struct cish::sym::Func: Base<Tag::Func>
// {
//     const char *m_type; // return type
//     int64_t addr;
//     int64_t argc;
//     int64_t allocsz;

//     Func( const char *type, int64_t address=0 )
//     :   m_type(type), addr(address) {  }
// };


// struct cish::sym::Var: Base<Tag::Var>
// {
//     const char *m_type;
//     int64_t     m_offset;

//     Var( const char *type="", int64_t rbpoff=0 )
//     :   m_type(type), m_offset(rbpoff) {  }
// };