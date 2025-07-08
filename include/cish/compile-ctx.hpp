#pragma once
#include <stddef.h>
#include <stdint.h>
#include <cish/stack.hpp>
#include <cish/node.hpp>

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

    struct SymType: SymBase<Sym_Type>
    {
        size_t size;
        size_t align;

        SymType( size_t sz, size_t al )
        :   size(sz), align(al) {  }
    };

    struct SymFunc: SymBase<Sym_Func>
    {
        const char *return_type;
        size_t addr;
        size_t argc;
        size_t resb;

        SymFunc( const char *ret_typename, size_t address )
        :   return_type(ret_typename), addr(address) {  }
    };

    struct SymVar: SymBase<Sym_Var>
    {
        const char *type_name;
        size_t addr;

        SymVar( const char *tpname, size_t address )
        :   type_name(tpname), addr(address) {  }
    };
    
    struct Symbol
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

    struct VmOp;
    class StackFrame;
    class CompileCtx;
}


class cish::StackFrame
{
private:
    template <typename T, size_t Cap>
    using Stack = cish::fixedsize_stack<T, Cap>;

    Stack<Symbol, 64> m_symbols;
    size_t            m_offset;

public:
    StackFrame();

    size_t frameAlloc( size_t nbytes, size_t align );
    size_t allocSize() { return m_offset; }

    Symbol *find( const char *key );
    Symbol *insert( const char *key, const SymType& );
    Symbol *insert( const char *key, const SymFunc& );
    Symbol *insert( const char *key, const SymVar& );

    // template <typename T>
    // Symbol *insert( const char *key, const T &data )
    // {
    //     if (auto *sym = find(key))
    //         return nullptr;
    //     Symbol sym; sym.key = key;
    //     new (sym.as_raw) T(data);
    //     m_symbols.push(sym);
    // }

};



class cish::CompileCtx
{
private:
    template <typename T, size_t Cap>
    using Stack = cish::fixedsize_stack<T, Cap>;
    Stack<StackFrame, 64> m_frames;
    StackFrame m_globals;
    // Stack<funcsym_t, 128> m_funcs;
    // Stack<typesym_t, 128> m_types;
    
    VmOp  *m_buf;
    size_t m_rip;

public:
    CompileCtx() {  }
    CompileCtx( uint32_t *buf, size_t bufsz );

    size_t frameAlloc( size_t nbytes, size_t align );
    uint32_t rip() { return m_rip; }
    void ripReset() { m_rip = 0; }

    template <typename... Args>
    VmOp *emit( Args... args )
    {
        return new (m_buf + m_rip++) VmOp(args...);
    }

    VmOp *emit( const VmOp &op )
    {
        return new (m_buf + m_rip++) VmOp(op);
    }

    void pushScope();
    void popScope();
    StackFrame &topScope() { return m_frames.top(); }
    StackFrame &globals() { return m_globals; }

    Symbol *findSymbol( const char *key );

    template <typename T>
    Symbol *createSymbol( const char *key, const T &data )
    {
        return m_globals.insert(key, data);
    }

};
