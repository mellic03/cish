#pragma once

#include <stddef.h>
#include <stdint.h>
#include <cish/type.hpp>
#include <cish/token.hpp>


namespace cish
{
    struct Node;

    void compileNode( Node* );

    // template <uint32_t tp_>
    // struct Node;

    struct NodeGrouping;
    struct NodeBinary;
    struct NodeUnary;

    struct NodeKeyword;
    struct NodeSymbol;
    struct NodeLiteral;
    struct NodeString;
    struct NodeNumber;
    struct NodeFunc;
}

struct cish::NodeBinary
{
    Token *op;
    Node *lhs, *rhs;
};

struct cish::NodeUnary
{
    Token *op;
    Node *rhs;
};

struct cish::NodeKeyword
{
    const char *symkey;
};

struct cish::NodeSymbol
{
    const char *symkey;
};



struct cish::NodeString
{
    const char *str;
};

struct cish::NodeNumber
{
    uint32_t type; // e.g. Type::i64

    union {
        int32_t  as_i32;
        uint32_t as_u32;
        float    as_f32;
    };
};

struct cish::NodeLiteral
{
    uint32_t type; // e.g. Type::String
    union {
        NodeString asString;
        NodeNumber asNumber;
    };
};

struct cish::NodeFunc
{
    const char *symkey;
};



struct cish::Node
{
    uint32_t type;

    union {
        NodeBinary  asBinary;
        NodeUnary   asUnary;
        NodeSymbol  asSymbol;
        NodeLiteral asLiteral;
        NodeFunc    asFunc;
    };

};



