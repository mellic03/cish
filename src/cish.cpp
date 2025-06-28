#include "cish.hpp"
#include <string.h>
#include <stdio.h>


// static bool isKeyword( const char *str, uint32_t *kw=nullptr )
// {
//     for (size_t i=0; i<sizeof(keys)/sizeof(keys[0]); i++)
//     {
//         if (strncmp(str, keys[i], strlen(keys[i])) == 0)
//         {
//             if (kw) *kw=values[i];
//             return true;
//         }
//     }
//     return false;
// }

// cish::symtab_t *cish::findSym( const char *name )
// {
//     for (int i=0; i<symtab_top-keyword_symtab; i++)
//         if (strcmp(keyword_symtab[i].name, name) == 0)
//             return keyword_symtab + i;
//     return nullptr;
// }


// cish::symtab_t *cish::createSym( const char *name, size_t size )
// {
//     auto *sym = symtab_top++;
//     sym->name = strtab_write(name);
//     return sym;;
// }