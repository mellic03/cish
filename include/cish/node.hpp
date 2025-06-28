#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cish/type.hpp>
#include <cish/token.hpp>
#include <cish/keyword.hpp>
#include <linkedlist.hpp>


namespace cish
{
    struct Node;

    // template <uint32_t tp_>
    // struct Node;

    struct NodeGrouping;
    struct NodeBinary;
    struct NodeUnary;
    struct NodeLeaf;

    struct iDataType;
    struct DataPrimitive;
    struct DataComposite;

    struct NodeAssign;
    struct NodeDecl;

    struct NodeIdentifier;
    struct NodeString;
    struct NodeNumber;
    // struct NodeFunction;

}




// template <uint32_t type_>
struct cish::Node: knl::LinkedListNode
{
public:
    const uint32_t m_type;
    const uint32_t m_optype;

    Node(): m_type(0), m_optype(0) {  };
    Node( uint32_t type ): m_type(type), m_optype(0) {  };
    Node( uint32_t type, uint32_t optype ): m_type(type), m_optype(optype) {  };

    virtual void print() { };
    virtual void compile( Node*, int64_t*& ) {  };

protected:

};











struct cish::NodeGrouping: Node // <Type::Grouping>
{
    knl::LinkedList<Node> m_nodes;

    template <typename... Args>
    NodeGrouping( Args... args )
    :   Node(Type::Grouping) { init(args...); }

    virtual void print() final;
    virtual void compile( Node*, int64_t*& ) final;

private:
    template <typename... Args>
    void init( Node *N, Args... args )
    {
        m_nodes.insert(N);
        init(args...);
    }

    void init() {  }

};





struct cish::NodeBinary: Node // <Type::Binary>
{
    Node *m_lhs, *m_rhs;

    NodeBinary( uint32_t optype, Node *lhs, Node *rhs )
    :   Node(Type::Binary, optype), m_lhs(lhs), m_rhs(rhs) {  };

    NodeBinary( Node *lhs, Node *rhs )
    :   Node(Type::Binary), m_lhs(lhs), m_rhs(rhs) {  };

    NodeBinary( Node *lhs, Token *tok, Node *rhs )
    :   Node(Type::Binary, tok->type), m_lhs(lhs), m_rhs(rhs) {  };

    virtual void print() override;
    virtual void compile( Node*, int64_t*& ) override;
};


struct cish::NodeUnary: Node // <Type::Unary>
{
    Node *m_rhs;

    NodeUnary( Token *t, Node *rhs )
    :   Node(Type::Unary, t->type), m_rhs(rhs) {  };

    NodeUnary( uint32_t optype, Node *rhs )
    :   Node(Type::Unary, optype), m_rhs(rhs) {  };

    virtual void print() override;
    // virtual void compile( Node*, int64_t*& ) override;
};


struct cish::NodeLeaf: Node // <Type::Leaf>
{
    const char *m_str;

    NodeLeaf( Token *t )
    : Node(Type::Leaf, t->type), m_str(t->lexeme) {  };

    NodeLeaf( uint32_t type, Token *t )
    : Node(type, t->type), m_str(t->lexeme) {  };

    // NodeLeaf( uint32_t ntype, Token *t )
    // : Node(ntype, t->type), m_str(t->lexeme) {  };

    virtual uint32_t type() final { return m_type; }
    virtual void print() override;
    // virtual void compile( Node*, int64_t*& ) override;
};






struct cish::iDataType: Node
{
    size_t m_size, m_align;
    const char *m_str;

    iDataType( uint32_t st, size_t sz, size_t al, const char *str )
    : Node(Type::DataType, st), m_size(sz), m_align(al), m_str(str)
    {

    };
};

struct cish::DataPrimitive: iDataType
{
    DataPrimitive( size_t sz, size_t al, const char *str )
    : iDataType(Type::DTypePrimitive, sz, al, str) {  };

    template <typename T>
    static DataPrimitive *as_type( const char *str ) {
        return new DataPrimitive(sizeof(T), alignof(T), str);
    }
};

struct cish::DataComposite: iDataType
{
    iDataType *m_members;

    DataComposite( size_t sz, size_t al, const char *str )
    :   iDataType(Type::DTypeComposite, sz, al, str) {  };
};






struct cish::NodeAssign: NodeBinary
{
    NodeAssign( Node *lhs, Node *rhs )
    :   NodeBinary(lhs, rhs) {  };

    virtual void print() final;
};


struct cish::NodeDecl: Node
{
    const char *m_target;
    const char *m_dtype;

    NodeDecl( const char *target, const char *dtype )
    :   Node(Type::Leaf, Type::Decl),
        m_target(target), m_dtype(dtype)
    {

    };

    virtual void print() override;
    virtual void compile( Node*, int64_t*& ) override;
};


struct cish::NodeIdentifier: NodeLeaf
{
    // using NodeLeaf::NodeLeaf;
    NodeIdentifier( Token *t )
    :   NodeLeaf(Type::Identifier, t) {  };
    // const char *m_str;
    // NodeIdentifier( Token* );
    virtual void compile( Node*, int64_t*& ) override;
};



struct cish::NodeString: NodeLeaf
{
    using NodeLeaf::NodeLeaf;
    // const char *m_str;
    // NodeString( Token* );
    // virtual void print() final;
};


struct cish::NodeNumber: NodeLeaf
{
    union {
        int32_t  as_i32;
        int64_t  as_i64;
        uint32_t as_u32;
        uint64_t as_u64;
    };
    NodeNumber( Token* );
    virtual void print() final;
    virtual void compile( Node*, int64_t*& ) override;
};


// struct cish::NodeFunction: Node
// {
//     const char   *m_str;
//     iDataType    *m_rtype;
//     // NodeGrouping *m_args;
//     Node         *m_body;

//     NodeFunction( Token *idnt, iDataType *rtype, Node *body )
//     :   Node(Type::Function),
//         m_str(idnt->lexeme),
//         m_rtype(rtype),
//         m_body(body),
//     {

//     };

//     virtual void compile( Node*, int64_t*& ) override;
// };

