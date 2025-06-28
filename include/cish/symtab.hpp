#pragma once
#include <stddef.h>
#include <stdint.h>


namespace cish
{
    void *sym_create( size_t size );
    void *sym_add( void *handle, const char *key, void *value );
    void *sym_find( void *handle, const char *key );
    void *sym_next( void *handle, void *curr );
    // extern char global_strab[];
    // extern symtab_t global_symtab[];

    // symtab_t *findSym( const char *name );
    // symtab_t *createSym( const char *name, size_t size );

}
