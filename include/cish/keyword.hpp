#pragma once
#include <cish/type.hpp>

namespace cish
{
    static constexpr
    const char *Keywords[] = {
        "\0\0",
        "if", "else", "while", "switch",
        "let", "const",
        "function", "return"
    };

    uint32_t getKwdType( const char* );
    bool isKeyword( const char *str, uint32_t *kwtype=nullptr );
}

