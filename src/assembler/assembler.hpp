#pragma once

#include <stddef.h>
#include <stdint.h>


namespace cish
{
    class Assembler;


    struct sym_t
    {
        const char *name;
        uintptr_t   addr;
    };

    using symtab_t = char*;
    // {
    //     char [];
    // };
    

    sym_t *sym_find( symtab_t*, const char *key );

    // extern char global_strab[];
    // extern symtab_t global_symtab[];

    // symtab_t *findSym( const char *name );
    // symtab_t *createSym( const char *name, size_t size );

}


// struct iAstNode;

// class cish::Assembler
// {
//     void assemble( iAstNode* );
// };