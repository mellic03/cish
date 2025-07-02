#include <cish/keyword.hpp>
#include <string.h>
#include <stdio.h>

using namespace cish;


size_t cish::isKeyword( const char *str )
{
    for (auto &[kwstr, value]: Type::KwTypes)
        if (strcmp(str, kwstr) == 0)
            return 1;
    return 0;
}

uint32_t cish::getKwdType( const char *str )
{
    for (auto &[kwstr, value]: Type::KwTypes)
        if (strcmp(str, kwstr) == 0)
            return value;
    return Type::Error;
}

const char *cish::getKwdStr( uint32_t kwd )
{
    if (Type::Kwd_BEGIN <= kwd && kwd < Type::Kwd_END)
        return Type::KwTypes[kwd - Type::Kwd_BEGIN].str;
    return "[cish::getKwdStr] kwd is not a valid keyword!";
}






size_t cish::isOperator( const char *str )
{
    for (auto &[opstr, optype]: Type::OpTypes)
        if (strcmp(str, opstr) == 0)
            return 1;
    return 0;
}

uint32_t cish::getOpType( const char *str )
{
    for (auto &[opstr, value]: Type::OpTypes)
        if (strcmp(str, opstr) == 0)
            return value;
    return Type::Error;
}

const char *cish::getOpStr( uint32_t op )
{
    if (Type::Op_BEGIN <= op && op < Type::Op_END)
        return Type::OpTypes[op - Type::Op_BEGIN].str;
    return "[cish::getKwdStr] kwd is not a valid keyword!";
}






size_t cish::isDataType( const char *str )
{
    for (auto &[s, v]: Type::DataTypes)
        if (strcmp(str, s) == 0)
            return 1;
    return 0;
}


uint32_t cish::getDataType( const char *str )
{
    for (auto &[s, v]: Type::DataTypes)
        if (strcmp(str, s) == 0)
            return v;
    return Type::Error;
}


