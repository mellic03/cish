
#pragma once
#include <cish/token.hpp>

#include <stdint.h>
#include <stdlib.h>
#include <new>
#include <assert.h>


// ParenExpr   ::= "(" <Expr> ")"
// BlockScope  ::= "{" <Stmnt> "}"

// VarDecl     ::= idnt idnt ("," idnt)* ";"

// FunDeclArg  ::= idnt idnt;
// FunDeclList ::= <FunDeclArg> ("," <FunDeclArg>)*
// FunDecl     ::= idnt idnt "(" <FunDeclList>? ")" <BlockScope>

// FunCallList ::= <Expr> ("," <Expr>)*
// FunCall     ::= idnt "(" <FunCallList>? ")"

// Atom        ::= idnt | string | number;
// Primary     ::= <ParenExpr> | <FunCall> | <Atom>

// Postfix     ::= <Primary> ("++" | "--")?
// Prefix      ::= ("++" | "--" | "!" | "~")? <Postfix>

// Binary4     ::= <Prefix>  (("|" | "&" | "^")? <Prefix>)*
// Binary3     ::= <Binary4> (("*" | "/") <Binary4>)*
// Binary2     ::= <Binary3> (("+" | "-") <Binary3>)*
// Binary1     ::= <Binary2> (("==" | "!=" | "<" | "<=" | ">" | ">=") <Binary2>)*
// Binary      ::= <Binary1> ("=" <Expr>)?

// Expr        ::= <Binary>

// Decl        ::= <FunDecl> | <VarDecl>
// Return      ::= "return" <Expr> ";"
// Cond        ::= ("if" | "while" | "for") <ParenExpr> <BlockScope>
// Stmnt       ::= <Return> | <Cond> | <Decl> | (<Expr> ";")

namespace cish
{
    class TokenStream;

    #define PARSE_TAGS\
    X(Base)\
    X(NodeList) X(DeclList) X(CallList)\
    X(Stmnt) X(Idnt) X(String) X(Number)\
    X(VarDecl) X(FunDecl) X(FunCall)\
    X(BinaryOp) X(PrefixOp) X(PostfixOp)\
    X(RetNode) X(IfNode) X(WhileNode) X(ForNode) \

    enum class ParseTag : uint8_t
    {
        #define X(name) name,
        PARSE_TAGS
        #undef X
    };


    inline const char *ParseTagStr(ParseTag tag)
    {
        switch (tag)
        {
            #define X(name) case ParseTag::name: return #name;
            PARSE_TAGS
            #undef X
            default: return "INVALID TAG";
        }
    }

    // struct iParseNode;

    template <ParseTag Tag = ParseTag::Base>
    struct ParseBase;

    struct NodeList;
    struct DeclList;
    struct CallList;

    struct Stmnt;
    struct Idnt;
    struct String;
    struct Number;

    struct VarDecl;
    struct FunDecl;
    struct FunCall;

    struct BinaryOp;
    struct PrefixOp;
    struct PostfixOp;

    struct RetNode;
    struct IfNode;
    struct WhileNode;
    struct ForNode;

    union  ParseNode;
}


template <cish::ParseTag Tg>
struct cish::ParseBase // : public iParseNode
{
    ParseTag tag;
    ParseBase(): tag(Tg) {  }
};



struct cish::NodeList: public ParseBase<ParseTag::NodeList>
{
    ParseNode *m_node;
    NodeList  *m_next;
    size_t     m_size;

    NodeList( ParseNode *node=nullptr)
    :   m_node(nullptr), m_next(nullptr), m_size(0)
    {
        if (node)
            insert(node);
    }

    void insert( ParseNode* );
    void for_each( void (*func)(ParseNode*) );
    size_t size() { return m_size; };

private:

};


struct cish::DeclList: public ParseBase<ParseTag::DeclList>
{
    const char *m_type;
    const char *m_name;
    DeclList   *m_next;
    size_t      m_size;

    DeclList( const char *type=nullptr, const char *name=nullptr )
    :   m_type(nullptr), m_name(nullptr), m_next(nullptr), m_size(0)
    {
        if (type && name)
            insert(type, name);
    }

    void insert( const char *type, const char *name );
    void for_each( void (*func)(const char *type, const char *name) );
    size_t size() { return m_size; };
};


struct cish::CallList: public ParseBase<ParseTag::CallList>
{
    const char *m_name;
    CallList   *m_next;
    size_t      m_size;

    CallList( const char *name )
    :   m_name(nullptr), m_next(nullptr), m_size(0)
    {
        if (name)
            insert(name);
    }

    void insert( const char *name );
    void for_each( void (*func)(const char *name) );
    size_t size() { return m_size; };
};



struct cish::Stmnt: public ParseBase<ParseTag::Stmnt>
{
    ParseNode *m_body;
    Stmnt( ParseNode *body ): m_body(body) {  }
};

struct cish::Idnt: public ParseBase<ParseTag::Idnt>
{
    const char *m_name;
    Idnt( const char *name ): m_name(name) {  }
};

struct cish::String: public ParseBase<ParseTag::String>
{
    const char *m_str;
    String( const char *str ): m_str(str) {  }
};

struct cish::Number: public ParseBase<ParseTag::Number>
{
    const char *m_lexeme;
    Number( const char *lexeme ): m_lexeme(lexeme) {  }
};



struct cish::VarDecl: public ParseBase<ParseTag::VarDecl>
{
    const char *m_type;
    const char *m_name;

    VarDecl( const char *type, const char *name )
    :   m_type(type), m_name(name) {  }
};


struct cish::FunDecl: public ParseBase<ParseTag::FunDecl>
{
    const char *m_type;
    const char *m_name;
    DeclList   *m_args;
    NodeList   *m_body;

    FunDecl( const char *type, const char *name, DeclList *args, NodeList *body )
    :   m_type(type), m_name(name), m_args(args), m_body(body) {  }
};

struct cish::FunCall: public ParseBase<ParseTag::FunCall>
{
    const char *m_name;
    CallList   *m_args;

    FunCall( const char *name, CallList *args )
    :   m_name(name), m_args(args) {  }
};



struct cish::BinaryOp: public ParseBase<ParseTag::BinaryOp>
{
    ParseNode  *m_lhs;
    uint32_t    m_op;
    const char *m_opstr;
    ParseNode  *m_rhs;

    BinaryOp( ParseNode *lhs, Token *op, ParseNode *rhs )
    :   m_lhs(lhs), m_op(op->type), m_opstr(op->lexeme), m_rhs(rhs) {  }
};


struct cish::PrefixOp: public ParseBase<ParseTag::PrefixOp>
{
    uint32_t    m_op;
    const char *m_opstr;
    ParseNode  *m_rhs;

    PrefixOp( Token *op, ParseNode *rhs )
    :   m_op(op->type), m_opstr(op->lexeme), m_rhs(rhs) {  }
};


struct cish::PostfixOp: public ParseBase<ParseTag::PostfixOp>
{
    ParseNode  *m_lhs;
    uint32_t    m_op;
    const char *m_opstr;

    PostfixOp( ParseNode *lhs, Token *op )
    :   m_lhs(lhs), m_op(op->type), m_opstr(op->lexeme) {  }
};


struct cish::RetNode: public ParseBase<ParseTag::RetNode>
{
    ParseNode *m_expr;

    RetNode( ParseNode *expr )
    :   m_expr(expr) {  }
};


struct cish::IfNode: public ParseBase<ParseTag::IfNode>
{
    ParseNode *m_cond;
    NodeList  *m_body;

    IfNode( ParseNode *cond, NodeList *body )
    :   m_cond(cond), m_body(body) {  }
};


struct cish::WhileNode: public ParseBase<ParseTag::WhileNode>
{
    ParseNode *m_cond;
    NodeList  *m_body;

    WhileNode( ParseNode *cond, NodeList *body )
    :   m_cond(cond), m_body(body) {  }
};


struct cish::ForNode: public ParseBase<ParseTag::ForNode>
{

};




union cish::ParseNode
{
    ParseBase<> as_Base;
    NodeList    as_NodeList;
    DeclList    as_DeclList;
    CallList    as_CallList;
    Stmnt       as_Stmnt;
    Idnt        as_Idnt;
    String      as_String;
    Number      as_Number;
    VarDecl     as_VarDecl;
    FunDecl     as_FunDecl;
    FunCall     as_FunCall;
    BinaryOp    as_BinaryOp;
    PrefixOp    as_PrefixOp;
    PostfixOp   as_PostfixOp;

    RetNode     as_Ret;
    IfNode      as_If;
    WhileNode   as_While;
    ForNode     as_For;

    auto Tag() { return as_Base.tag; }
};


namespace cish
{
    template <typename T, typename... Args>
    ParseNode *newNd( Args... args )
    {
        auto *node = (ParseNode*)malloc(sizeof(ParseNode));
        new (node) T(args...);
        return node;
    }

    template <typename T>
    ParseNode *newNd( const T &data )
    {
        auto *node = (ParseNode*)malloc(sizeof(ParseNode));
        new (node) T(data);
        return node;
    }
}