#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cish/type.hpp>
#include <cish/token.hpp>
#include <cish/keyword.hpp>
#include <linkedlist.hpp>


namespace cish
{
    struct CompileCtx;

    struct Node;
    struct NodeError;

    struct NodeProgData;
    struct NodeProgRoot;

    struct NodeGrouping;
    struct NodeBinary;
    struct NodeUnary;

    struct NodeIf;
    struct NodeDecl;
    struct NodeAssign;
    struct NodeVariable;
    struct NodeString;
    struct NodeNumber;
}



struct cish::Node: knl::LinkedListNode
{
public:
    const uint32_t m_type;
    Node( uint32_t type ): m_type(type) {  };
    Node( Token *t ): m_type(t->type) {  };

    virtual void print() = 0;
    virtual void compile( CompileCtx& ) = 0;

protected:

};



struct cish::NodeError: Node
{
    NodeError( const char *fmt, ... );

    virtual void print() final {  };
    virtual void compile( CompileCtx& ) final {  };
};



struct cish::NodeGrouping: Node
{
    knl::LinkedList<Node> m_nodes;

    NodeGrouping( uint32_t type=Type::Grouping )
    :   Node(type) {  };

    virtual void print() override;
    virtual void compile( CompileCtx& ) override;

    void push( Node *N ) { m_nodes.push(N); };
    void pop() { m_nodes.pop(); };

};




struct cish::NodeProgData: Node
{
    uint8_t *m_base;
    size_t   m_size;

    NodeProgData( size_t size )
    :   Node(Type::ProgData),
        m_base(new uint8_t[size]),
        m_size(size) {  };

    virtual void print() final {  };
    virtual void compile( CompileCtx& ) final;
};


struct cish::NodeProgRoot: NodeGrouping
{
    NodeProgData *m_data;
    // knl::LinkedList<Node> m_text;

    NodeProgRoot()
    :   NodeGrouping(Type::ProgRoot),
        m_data(new NodeProgData(2048))
    {

    }

    virtual void print() final;
    virtual void compile( CompileCtx& ) final;
};






struct cish::NodeBinary: Node
{
    Node *m_lhs, *m_rhs;

    NodeBinary( Node *lhs, Token *t, Node *rhs )
    :   Node(t), m_lhs(lhs), m_rhs(rhs) {  };

    virtual void print() override;
    virtual void compile( CompileCtx& ) override;
};


struct cish::NodeUnary: Node 
{
    Node *m_rhs;

    NodeUnary( Token *t, Node *rhs )
    :   Node(t), m_rhs(rhs) {  };

    virtual void print() override;
    virtual void compile( CompileCtx& ) override;
};



struct cish::NodeIf: Node
{
    Node *m_cond, *m_thenBranch, *m_elseBranch;

    NodeIf( Node *cond, Node *thenBranch, Node *elseBranch )
    :   Node(Type::KwdIf),
        m_cond(cond),
        m_thenBranch(thenBranch),
        m_elseBranch(elseBranch)
    {

    };

    virtual void print() override {  };
    virtual void compile( CompileCtx& ) override {  };
};


// struct cish::NodeDecl: Node
// {
//     const char *m_symkey;
//     Node       *m_rhs;

//     NodeDecl( const char *symkey, Node *rhs )
//     :   Node(Type::Decl), m_symkey(symkey), m_rhs(rhs) {  };

//     virtual void print() override;
//     virtual void compile( CompileCtx& ) override {  };
// };


struct cish::NodeAssign: Node
{
    const char *m_symkey;
    Node       *m_rhs;

    NodeAssign( const char *key, Node *rhs )
    :   Node(Type::Equal), m_symkey(key), m_rhs(rhs) {  };

    virtual void print() override;
    virtual void compile( CompileCtx& ) override {  };
};



struct cish::NodeVariable: Node
{
    const char *m_symkey;

    NodeVariable( Token *t )
    :   Node(t), m_symkey(t->lexeme) {  };

    virtual void print() final;
    virtual void compile( CompileCtx& ) override;
};


struct cish::NodeString: Node
{
    const char *m_value;

    NodeString( Token *t );

    virtual void print() override;
    virtual void compile( CompileCtx& ) override {  };
};


struct cish::NodeNumber: Node
{
    int64_t as_i64;

    NodeNumber( Token* );

    virtual void print() final;
    virtual void compile( CompileCtx& ) override;
};

