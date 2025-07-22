#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <new>
#include <linkedlist.hpp>


namespace cish
{
    class Parser;
    class TokenStream;

    enum class ProdTag: uint8_t
    {
        Base,
        True,
        List,

        Program,        // ::= <StmntList>
    
        StmntList,      // ::= <Stmnt>*
        Stmnt,          // ::= <Decl> | <Assign>
    
        Decl,           // ::= <VarDecl> | <FunDecl>

        VarDecl,        // ::= idnt <VarDeclList> ";"
        // VarDeclList, // ::= idnt ("," idnt)*

        FunDecl,        // ::= idnt idnt "(" <FunDeclList>? ")" (";" | <FunBody>)
        FunDeclList,     // ::= <FunDeclArg> ("," idnt idnt)*
        FunDeclArg,     // ::= idnt idnt
        FunBody,        // ::= "{" <Stmnt>* "}"

        FunCall,        // ::= idnt "(" <FunCallList>? ")"
        FunCallList,    // ::= <FunCallArg> ("," idnt)*
        FunCallArg,     // ::= idnt

        Assign,         // ::= idnt "=" <Expr> ";"

        Expr,           // ::= <Binary1> <Expr>*
        Binary1,        // ::= <Binary2> (("+" | "-") <Binary2>)*
        Binary2,        // ::= <Prefix>  (("*" | "/") <Prefix>)*
        Prefix,         // ::= ("++" | "--" | "!" | "~")? <Postfix>
        Postfix,        // ::= ("++" | "--")? <Primary>

        Primary,        // ::= <Atom> | FunCall | "(" <Expr> ")"
        Atom,           // ::= idnt | string | number
    };

    inline const char *ProdTagStr( ProdTag tag )
    {
        switch (tag)
        {
            default: return "INVALID TAG";
            case ProdTag::Base: return "Base";
            case ProdTag::True: return "True";
            case ProdTag::List: return "List";
            case ProdTag::Program: return "Program";
            case ProdTag::StmntList: return "StmntList";
            case ProdTag::Stmnt: return "Stmnt";
            case ProdTag::Decl: return "Decl";
            case ProdTag::VarDecl: return "VarDecl";
            case ProdTag::FunDecl: return "FunDecl";
            case ProdTag::FunDeclList: return "FunDeclList";
            case ProdTag::FunDeclArg: return "FunDeclArg";
            case ProdTag::FunBody: return "FunBody";
            case ProdTag::FunCall: return "FunCall";
            case ProdTag::FunCallList: return "FunCallList";
            case ProdTag::FunCallArg: return "FunCallArg";
            case ProdTag::Assign: return "Assign";
            case ProdTag::Expr: return "Expr";
            case ProdTag::Binary1: return "Binary1";
            case ProdTag::Binary2: return "Binary2";
            case ProdTag::Prefix: return "Prefix";
            case ProdTag::Postfix: return "Postfix";
            case ProdTag::Primary: return "Primary";
            case ProdTag::Atom: return "Atom";
        }
    }


    template <ProdTag Tg = ProdTag::Base>
    struct ProdBase;

    struct ProdTrue;
    struct ProdList;
    struct Program;
    struct StmntList;
    struct Stmnt;
    struct Decl;
    struct VarDecl;
    struct FunDecl;
    struct FunDeclList;
    struct FunDeclArg;
    struct FunBody;
    struct FunCall;
    struct FunCallList;
    struct FunCallArg;
    struct Assign;
    struct Expr;
    struct Binary1;
    struct Binary2;
    struct Prefix;
    struct Postfix;
    struct Primary;
    struct Atom;
    union  ProdNode;
}




template <cish::ProdTag Tg>
struct cish::ProdBase: knl::LinkedListNode
{
    static constexpr ProdTag tag = Tg;
};


struct cish::ProdTrue: ProdBase<ProdTag::True>
{
    
};


struct cish::ProdList: ProdBase<ProdTag::List>, knl::LinkedList<>
{
    ProdList() {  }
    void push( ProdNode *node ) { LinkedList::push((LinkedListNode*)node); }
};


struct cish::Program: ProdBase<ProdTag::Program>
{
    Program( TokenStream& );
};


// StmntList ::= <Stmnt>*
struct cish::StmntList: ProdBase<ProdTag::StmntList>
{
    StmntList( TokenStream& );
};


// Stmnt ::= <Decl> | <Assign>
struct cish::Stmnt: ProdBase<ProdTag::Stmnt>
{
    Decl   *as_Decl;
    Assign *as_Assign;
    Stmnt( TokenStream& );
};


// Decl ::= <VarDecl> | <FunDecl>
struct cish::Decl: ProdBase<ProdTag::Decl>
{
    Decl( TokenStream& );
};


// VarDecl ::= idnt <VarDeclList> ";"
struct cish::VarDecl: ProdBase<ProdTag::VarDecl>
{
    const char *m_type;
    const char *m_name;
    VarDecl    *m_next;

    VarDecl( const char *type, const char *name, VarDecl *next = nullptr )
    :   m_type(type), m_name(name), m_next(next) {  }
    VarDecl( TokenStream& );
};


// FunDecl ::= idnt idnt "(" <FunDeclList>? ")" (";" | <FunBody>)
struct cish::FunDecl: ProdBase<ProdTag::FunDecl>
{
    const char *m_type;
    const char *m_name;
    FunDeclList *m_arglist;
    FunBody    *m_body;

    FunDecl() {  };
    FunDecl( TokenStream& );
};

// FunDeclList ::= <FunDeclArg> ("," idnt idnt)*
struct cish::FunDeclList: ProdBase<ProdTag::FunDeclList>
{
    knl::LinkedList<ProdBase> m_args; // Must be list of FunDeclArg
    FunDeclList( TokenStream& );
};

// FunDeclArg ::= idnt idnt
struct cish::FunDeclArg: ProdBase<ProdTag::FunDeclArg>
{
    const char *m_type;
    const char *m_name;
    FunDeclArg( TokenStream& );
};

// FunBody ::= "{" <Stmnt>* "}"
struct cish::FunBody: ProdBase<ProdTag::FunBody>
{
    FunBody( TokenStream& );
};



// ::= idnt "(" <FunCallList>? ")"
struct cish::FunCall: ProdBase<ProdTag::FunCall>
{
    const char *m_name;
    FunCallList *m_arglist;
    FunCall( TokenStream& );
};

// ::= <FunCallArg> ("," idnt)*
struct cish::FunCallList: ProdBase<ProdTag::FunCallList>
{
    knl::LinkedList<ProdBase> m_args; // Must be list of FunCallArg
    FunCallList( TokenStream& );
};

// ::= idnt
struct cish::FunCallArg: ProdBase<ProdTag::FunCallArg>
{
    const char *m_name;
    FunCallArg( TokenStream& );
};



// Assign ::= idnt "=" <Expr> ";"
struct cish::Assign: ProdBase<ProdTag::Assign>
{
    Assign( TokenStream& );

};


// Expr ::= <Binary1> <Expr>*
struct cish::Expr: ProdBase<ProdTag::Expr>
{
    Expr( TokenStream& );

};


// Binary1 ::= <Binary2> (("+" | "-") <Binary2>)*
struct cish::Binary1: ProdBase<ProdTag::Binary1>
{
    Binary1( TokenStream& );

};


// Binary2 ::= <Prefix>  (("*" | "/") <Prefix>)*
struct cish::Binary2: ProdBase<ProdTag::Binary2>
{
    Binary2( TokenStream& );

};


// Prefix ::= ("++" | "--" | "!" | "~")? <Postfix>
struct cish::Prefix: ProdBase<ProdTag::Prefix>
{
    Prefix( TokenStream& );

};


// Postfix ::= ("++" | "--")? <Primary>
struct cish::Postfix: ProdBase<ProdTag::Postfix>
{
    Postfix( TokenStream& );

};


// Primary ::= <Atom> | FunCall | "(" <Expr> ")"
struct cish::Primary: ProdBase<ProdTag::Primary>
{
    Primary( TokenStream& );

};


// Atom ::= idnt | string | number
struct cish::Atom: ProdBase<ProdTag::Atom>
{
    uint32_t m_type;
    const char *m_name;

    Atom( TokenStream& );
    Atom( uint32_t type, const char *name )
    :   m_type(type), m_name(name) {  };

};




union cish::ProdNode
{
    ProdTag         as_Tag;
    ProdBase<>      as_Base;
    ProdList        as_List;
    Program         as_Program;
    StmntList       as_StmntList;
    Stmnt           as_Stmnt;
    Decl            as_Decl;
    VarDecl         as_VarDecl;
    FunDecl         as_FunDecl;
    FunDeclList     as_FunDeclList;
    FunDeclArg      as_FunDeclArg;
    FunBody         as_FunBody;
    FunCall         as_FunCall;
    FunCallList     as_FunCallList;
    FunCallArg      as_FunCallArg;
    Assign          as_Assign;
    Expr            as_Expr;
    Binary1         as_Binary1;
    Binary2         as_Binary2;
    Prefix          as_Prefix;
    Postfix         as_Postfix;
    Primary         as_Primary;
    Atom            as_Atom;

    template <typename T, typename... Args>
    static T *New( Args... args )
    {
        T *ptr = new (malloc(sizeof(ProdNode))) T(args...);
        ((ProdNode*)ptr)->as_Tag = T::tag;
        return ptr;
    }
};


// namespace cish
// {
//     template <typename T>
//     inline T *newNode( cish::Parser &P )
//     {
//         using namespace cish;
//         ProdNode *prod = (ProdNode*)malloc(sizeof(ProdNode));
//         return new (prod) T(P);
//     }

//     template <typename T>
//     inline T *newNode( cish::Parser &P, const T &data )
//     {
//         using namespace cish;
//         ProdNode *prod = (ProdNode*)malloc(sizeof(ProdNode));
//         return new (prod) T(data);
//     }
// }

