#pragma once
#include <cish/type.hpp>
#include <cish/token.hpp>


struct AstGroup;
struct AstCond;
struct AstReturn;
struct AstBinary;
struct AstPrefix;
struct AstPostfix;

struct AstFunction;
struct AstVariable;

struct AstLiteral;
struct AstNumber;
struct AstString;

struct AstNode;




struct AstGroup
{

};


struct AstCond
{

};

struct AstReturn
{

};


struct AstBinary
{

};


struct AstPrefix
{

};


struct AstPostfix
{

};


struct AstFunction
{

};


struct AstVariable
{

};



struct AstNumber
{
    union {
        int64_t as_i64;
        uint64_t as_u64;
    };
};

struct AstString
{
    const char *str;
};

struct AstLiteral
{
    uint32_t type;
    union {
        AstNumber as_Number;
        AstString as_String;
    };
};








struct AstNode
{
    uint32_t type;

    union {
        AstGroup    as_Group;
        AstCond     as_Cond;
        AstReturn   as_Return;
        AstPrefix   as_Prefix;
        AstPostfix  as_Postfix;
        AstFunction as_Function;
        AstLiteral  as_Literal;
    };
};

