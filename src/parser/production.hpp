#pragma once
#include <stdint.h>
#include <linkedlist.hpp>


namespace cish
{
    class Parser;
    class Token;
}

namespace cish::Prod
{
    enum Tag_: uint8_t
    {
        Tag_Base,
        Tag_ParseError,
        // Tag_Dummy,
        Tag_List,
        Tag_Atom,
        Tag_Program,
        Tag_Stmnt,
        Tag_StmntList,
        Tag_Decl,
        Tag_Assign,
        Tag_Expr,
        Tag_NameDecl,
        Tag_FunDecl,
        Tag_DeclList,
        Tag_Call,
        Tag_CallList,
        Tag_Term,
        Tag_AllProd,
    };

    const char *ProdTagStr( uint8_t tag )
    {
        switch (tag)
        {
            default: return "INVALID TAG";
            case Tag_Base: return "Base";
            // case Tag_ParseError: return "ParseError";
            // case Tag_Dummy: return "Dummy";
            case Tag_List: return "List";
            case Tag_Atom: return "Atom";
            case Tag_Program: return "Program";
            case Tag_Stmnt: return "Stmnt";
            case Tag_StmntList: return "StmntList";
            case Tag_Decl: return "Decl";
            case Tag_Assign: return "Assign";
            case Tag_Expr: return "Expr";
            case Tag_NameDecl: return "NameDecl";
            case Tag_FunDecl: return "FunDecl";
            case Tag_DeclList: return "DeclList";
            case Tag_Call: return "Call";
            case Tag_CallList: return "CallList";
            case Tag_Term: return "Term";
            case Tag_AllProd: return "AllProd";
        }
    }


    template <uint8_t Tg=Tag_Base>
    struct Base;

    // struct ParseError;
    // struct Dummy;
    struct List;
    struct Atom;
    struct Program;
    struct Stmnt;
    struct StmntList;
    struct Decl;
    struct Assign;
    struct Expr;
    struct NameDecl;
    struct FunDecl;
    struct DeclList;
    struct Call;
    struct CallList;
    struct Term;
    struct AllProd;
}

#define PROD_STRUCT(Nm) struct cish::Prod:: Nm : public Prod::Base<Tag_##Nm>


template <uint8_t Tg>
struct cish::Prod::Base
{
    const uint8_t m_tag;
    // uint8_t m_err;

    Base(): m_tag(Tg) {  };
    // void ToError() { m_err = true; }
    // bool isError() { return m_err; }
    static constexpr uint8_t ProdTag() { return Tg; }
};


// PROD_STRUCT(ParseError) { ParseError( Parser& ) {  } };
// PROD_STRUCT(Dummy)      { Dummy( Parser& ) {  } };

struct cish::Prod::List: public Base<Tag_List>, public knl::LinkedList<AllProd>
{
    int m_type;
    const char *m_str;
    List(): Base() {  };

    static bool likely( Parser& ) { return true; };
};

// <atom> ::= idnt | string | number
PROD_STRUCT(Atom)
{
    int m_type;
    const char *m_str;
    Atom(): Base() {  };
    Atom( Parser& );
    Atom( uint8_t tp, const char *str )
    :   m_type(tp), m_str(str) {  }

    static bool likely( Parser& ) { return true; };
};


// <program> ::= <stmntList>
PROD_STRUCT(Program)
{
    StmntList *m_list;
    Program( Parser& );
    // static bool likely( Parser& );
};


// <stmnt> ::= (<decl> | <assign> | <expr>) ";"
PROD_STRUCT(Stmnt)
{
    // union {
        Decl   *as_Decl;
        Assign *as_Assign;
        Expr   *as_Expr;
    // };

    Stmnt( Parser& );
    // static bool likely( Parser& );
};

// <stmntList> ::= <stmnt> <stmntList>*
PROD_STRUCT(StmntList)
{
    Stmnt     *m_stmnt;
    StmntList *m_next;
    StmntList( Parser& );
    // static bool likely( Parser& );
};


// <decl> ::= <namedecl> | <fundecl>
PROD_STRUCT(Decl)
{
    NameDecl *m_namedecl;
    FunDecl  *m_fundecl;
    Decl( Parser& );
    // static bool likely( Parser& );
};


// <assign> ::= idnt "=" <expr>
PROD_STRUCT(Assign)
{
    Atom *m_idnt;
    Expr *m_expr;
    Assign( Parser& );
    // static bool likely( Parser& );
};


// <expr> ::= <term> ((/|*|-|+) <expr>)* 
PROD_STRUCT(Expr)
{
    Term *m_lhs;
    Expr *m_rhs;
    Expr( Parser& );
    // static bool likely( Parser& );
};


// <decl_list> ::= <namedecl> ("," <decl_list>)*
PROD_STRUCT(DeclList)
{
    NameDecl *m_arg;
    DeclList *m_next;
    DeclList( Parser& );
    // static bool likely( Parser& );
};

// <fundecl> ::= "(" <decl_list>? ")"
PROD_STRUCT(FunDecl)
{
    DeclList *m_list;
    FunDecl( Parser& );
    // static bool likely( Parser& );
};

// <namedecl> ::= type idnt
PROD_STRUCT(NameDecl)
{
    Atom *m_type;
    Atom *m_idnt;
    NameDecl( Parser& );
    // static bool likely( Parser& );
};


// <call> ::= idnt "(" <call_list>? ")"
PROD_STRUCT(Call)
{
    CallList *m_list;
    Call( Parser& );
    // static bool likely( Parser& );

};


// <call_list> ::= <expr> ("," <expr>)*
PROD_STRUCT(CallList)
{
    Expr     *m_expr;
    CallList *m_next;
    CallList( Parser& );
    // static bool likely( Parser& );
};


// <term> ::= <atom> | <expr>
PROD_STRUCT(Term)
{
    bool is_atom;
    Atom *as_Atom;
    Expr *as_Expr;
    Term( Parser& );
    // static bool likely( Parser& );
};



struct cish::Prod::AllProd: public knl::LinkedListNode
{
    uint8_t utag;

    union {
        uint8_t    as_Bytes[];
        Base<>     as_Base;
        // ParseError as_ParseError;
        // Dummy      as_Dummy;
        List       as_List;
        Atom       as_Atom;
        Program    as_Program;
        Stmnt      as_Stmnt;
        StmntList  as_StmntList;
        Decl       as_Decl;
        Assign     as_Assign;
        Expr       as_Expr;
        NameDecl   as_NameDecl;
        FunDecl    as_FunDecl;
        DeclList   as_DeclList;
        Call       as_Call;
        CallList   as_CallList;
        Term       as_Term;
    };

    // bool IsError() { return as_Base.isError(); }
};




#undef PROD_STRUCT
