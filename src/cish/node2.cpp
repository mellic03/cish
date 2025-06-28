#include <cish/node2.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

using namespace cish;


static void compileString( const NodeString &N )
{
    printf("resb anonstr0, %lu;\n", strlen(N.str)+1);
}

static void compileNumber( const NodeNumber &N )
{
    switch (N.type)
    {
        default: assert(0); break;
        case Type::i32: printf("push %d;\n", N.as_i32); break;
        case Type::u32: printf("push %u;\n", N.as_u32); break;
        case Type::f32: printf("pushf %f;\n", N.as_f32); break;
    }
}

static void compileLiteral( const NodeLiteral &N )
{
    switch (N.type)
    {
        default: assert(0); break;
        case Type::String: compileString(N.asString); break;
        case Type::Number: compileNumber(N.asNumber); break;
    }
}


void cish::compileNode( Node *N )
{

    switch (N->type)
    {
        default: assert(0); break;

        case Type::Binary:
            compileNode(N->asBinary.lhs);
            compileNode(N->asBinary.rhs);
            break;

        case Type::Unary:
            compileNode(N->asBinary.rhs);
            
            break;

        case Type::Symbol: break;

        case Type::Literal:
            compileLiteral(N->asLiteral);
            break;

        case Type::Function: break;
    }
}
