#include <cish/keyword.hpp>
#include <string.h>
#include <stdio.h>


// static bool kwdIsValid( uint32_tkwd )
// {
//     using namespace cish;
//     return Kwd::If<=kwd && kwd<=Kwd::Return;
// }


// uint32_t cish::TypeGetEnum( const char *str )
uint32_t cish::getKwdType( const char *str )
{
    uint32_t kwd;
    if (isKeyword(str, &kwd))
        return kwd;
    return Type::Error;
}


// const char *cish::TypeGetStr( uint32_tkwd )
// {
//     if (kwdIsValid(kwd))
//         return Keywords[(uint32_t)kwd];
//     return nullptr;
// }



bool cish::isKeyword( const char *str, uint32_t *type )
{
    for (uint32_t i=1; i<sizeof(Keywords)/sizeof(Keywords[0]); i++)
    {
        if (strncmp(str, Keywords[i], strlen(Keywords[i])) == 0)
        {
            if (type)
                *type = Type::Keyword + i;
            return true;
        }
    }
    return false;
}

