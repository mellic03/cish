#pragma once
#include <cish/token.hpp>
#include <cish/token-stream.hpp>
#include <util/tuple.hpp>

#include <stdint.h>
#include <stdlib.h>
#include <new>
#include <assert.h>


namespace cish
{
    struct SymTab;

    #define PARSE_TAGS \
    X(ErrorNode) \
    X(NodeList) X(DeclList) X(CallList) \
    X(BlockScope) X(VarDecl) X(FunDecl) \
    X(RetNode) X(IfNode) X(WhileNode) X(ForNode) \
    X(PrefixOp) X(PostfixOp) X(BinaryOp) \
    X(Assign) X(FunCall) X(Idnt) X(String) X(Number) \


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

    struct iNode;

    #define X(Name) struct Name;
    PARSE_TAGS
    #undef X
}


struct cish::iNode
{
    ParseTag tag;
    SymTab *m_symtab;
    iNode( ParseTag tg ): tag(tg), m_symtab(nullptr) {  }
    bool IsError() const { return (tag == ParseTag::ErrorNode); }
};



struct cish::ErrorNode: iNode
{
    Token *m_state;
    ErrorNode( Token *state ): iNode(ParseTag::ErrorNode), m_state(state) {  };
};


#define LIST_ITERATOR(Name, Deref) \
struct iterator \
{ \
    Name *m_list; \
    iterator( Name *list ): m_list(list) {  } \
    iterator( const iterator &other ): m_list(other.m_list) {  }; \
    iterator &operator++() { m_list = m_list->m_next; return *this; }; \
    iterator operator++(int) { return iterator(m_list->m_next); }; \
    bool operator==( const iterator &rhs ) { return m_list == rhs.m_list; }; \
    bool operator!=( const iterator &rhs ) { return m_list != rhs.m_list; }; \
    auto operator*() Deref; \
}; \


struct cish::NodeList: iNode
{
    iNode     *m_node;
    NodeList  *m_next;
    size_t     m_size;

    NodeList( iNode *node=nullptr )
    :   iNode(ParseTag::NodeList), m_node(nullptr), m_next(nullptr), m_size(0)
    { if (node) insert(node); }

    NodeList *back();
    void insert( iNode* );
    size_t size() { return m_size; };

    LIST_ITERATOR(NodeList, { return m_list->m_node; })
    iterator begin() { return iterator((m_size==0) ? nullptr : this); };
    iterator end()   { return iterator(nullptr); };
};


struct cish::DeclList: iNode
{
    VarDecl    *m_decl;
    DeclList   *m_next;
    size_t      m_size;

    DeclList()
    :   iNode(ParseTag::DeclList), m_next(nullptr), m_size(0) {  }

    template <typename... Items>
    DeclList( Items... items )
    :   iNode(ParseTag::DeclList), m_next(nullptr), m_size(0) { insert(items...); }

    template <typename... Rest>
    void insert( VarDecl *H, Rest... R ) { insert(H); insert(R...); }
    void insert( VarDecl* );
    size_t size() { return m_size; };

    LIST_ITERATOR(DeclList, { return m_list->m_decl; })
    iterator begin() { return (m_decl) ? iterator(this) : end(); };
    iterator end()   { return iterator(nullptr); };

};


struct cish::CallList: iNode
{
    Idnt     *m_idnt;
    CallList *m_next;
    size_t    m_size;

    CallList(): iNode(ParseTag::CallList), m_idnt(nullptr), m_next(nullptr), m_size(0) {  }
    CallList( Idnt *idnt ): CallList() { insert(idnt); }

    void insert( Idnt* );
    size_t size() { return m_size; };

    LIST_ITERATOR(CallList, { return m_list->m_idnt; })
    iterator begin() { return iterator(this); };
    iterator end()   { return iterator(nullptr); };
};



struct cish::BlockScope: iNode
{
    NodeList *m_body;

    BlockScope( NodeList *body )
    :   iNode(ParseTag::BlockScope), m_body(body) {  }
};



struct cish::VarDecl: iNode
{
    const char *m_type;
    const char *m_name;

    VarDecl( const char *type, const char *name )
    :   iNode(ParseTag::VarDecl), m_type(type), m_name(name) {  }

    VarDecl( Token *tptok, Token *nmtok )
    :   iNode(ParseTag::VarDecl), m_type(tptok->lexeme), m_name(nmtok->lexeme) {  }
};


struct cish::FunDecl: iNode
{
    const char *m_type;
    const char *m_name;
    DeclList   *m_args;
    BlockScope *m_body;

    FunDecl( const char *type, const char *name, DeclList *args, BlockScope *body )
    :   iNode(ParseTag::FunDecl), m_type(type), m_name(name), m_args(args), m_body(body) {  }
};


struct cish::RetNode: iNode
{
    iNode *m_expr;
    RetNode( iNode *expr ): iNode(ParseTag::RetNode), m_expr(expr) {  }
};

struct cish::IfNode: iNode
{
    iNode    *m_cond;
    NodeList *m_body;
    IfNode( iNode *cond, NodeList *body ): iNode(ParseTag::IfNode), m_cond(cond), m_body(body) {  }
};

struct cish::WhileNode: iNode
{
    iNode    *m_cond;
    NodeList *m_body;
    WhileNode( iNode *cond, NodeList *body ): iNode(ParseTag::WhileNode), m_cond(cond), m_body(body) {  }
};

struct cish::ForNode: iNode
{
    ForNode()
    : iNode(ParseTag::ForNode) {  }
};

struct cish::PrefixOp: iNode
{
    uint32_t    m_op;
    const char *m_opstr;
    iNode      *m_rhs;

    PrefixOp( Token *op, iNode *rhs )
    :   iNode(ParseTag::PrefixOp), m_op(op->type), m_opstr(op->lexeme), m_rhs(rhs) {  }
};


struct cish::PostfixOp: iNode
{
    iNode      *m_lhs;
    uint32_t    m_op;
    const char *m_opstr;

    PostfixOp( iNode *lhs, Token *op )
    :   iNode(ParseTag::PostfixOp), m_lhs(lhs), m_op(op->type), m_opstr(op->lexeme) {  }
};


struct cish::BinaryOp: iNode
{
    iNode      *m_lhs;
    uint32_t    m_op;
    const char *m_opstr;
    iNode      *m_rhs;

    BinaryOp( iNode *lhs, Token *op, iNode *rhs )
    :   iNode(ParseTag::BinaryOp), m_lhs(lhs), m_op(op->type), m_opstr(op->lexeme), m_rhs(rhs) {  }
};



struct cish::Assign: iNode
{
    const char *m_idnt;
    iNode      *m_expr;
    Assign( const char *idnt, iNode *expr ): iNode(ParseTag::Assign), m_idnt(idnt), m_expr(expr) {  }
};


struct cish::FunCall: iNode
{
    const char *m_name;
    NodeList   *m_args;

    FunCall( const char *name, NodeList *args )
    :   iNode(ParseTag::FunCall), m_name(name), m_args(args) {  }
};


struct cish::Idnt: iNode
{
    const char *m_name;
    Idnt( const char *name ): iNode(ParseTag::Idnt), m_name(name) {  }
};


struct cish::String: iNode
{
    const char *m_str;
    String( const char *str ): iNode(ParseTag::String), m_str(str) {  }
};


struct cish::Number: iNode
{
    const char *m_lexeme;
    Number( const char *lexeme ): iNode(ParseTag::Number), m_lexeme(lexeme) {  }
};





namespace cish
{
    template <typename T, typename... Args>
    T *newNd( Args... args )
    {
        return new (malloc(sizeof(T))) T(args...);
    }

    template <typename T>
    T *newNd( const T &data )
    {
        return new (malloc(sizeof(T))) T(data);
    }
}
