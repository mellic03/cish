#pragma once
#include <cish/type.hpp>

namespace cish
{
    size_t isKeyword( const char *str );
    uint32_t getKwdType( const char* );
    const char *getKwdStr( uint32_t );

    size_t isOperator( const char* );
    uint32_t getOpType( const char* );
    const char *getOpStr( uint32_t );

    size_t isDataType( const char* );
    uint32_t getDataType( const char* );
    // const char *getOpStr( uint32_t );

}

