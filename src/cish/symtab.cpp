// #include <cish/symtab.hpp>
// #include <stdio.h>
// #include <kstring.h>
// #include <new>


// struct sym_t
// {
//     // size_t prev_size;
//     sym_t *next;
//     void  *value;
//     char   key[];

//     sym_t( const sym_t& ) = delete;
//     sym_t( sym_t&& ) = delete;

//     sym_t( const char *str, void *data )
//     {
//         value = data;
//         strncpy(key, str, strlen(str));

//         uintptr_t top = (uintptr_t)this;
//         top  = (uintptr_t)this + sizeof(sym_t) + strlen(str)+1;
//         top  = (top + 16-1) & ~(16-1);
//         next = (sym_t*)top;
//     }
// };

// struct symtab_t
// {
//     size_t  magic;
//     size_t  size;
//     size_t  count;
//     sym_t  *beg;
//     sym_t  *end;
//     sym_t  *top;
//     char    body[];
// };


// void *cish::sym_create( size_t nbytes )
// {
//     static constexpr char SYMTAB_BEG[] = "__symtab_begin__";
//     static constexpr char SYMTAB_END[] = "__symtab_end__";

//     size_t size = sizeof(symtab_t) + nbytes;

//     char *base = new char[size];
//     char *end  = (base+size) - (sizeof(sym_t) + sizeof(SYMTAB_END));

//     symtab_t *tab = (symtab_t*)base;
//     tab->beg = new (tab->body) sym_t(SYMTAB_BEG, (void*)0x0A0A0A0A);
//     tab->end = new (end) sym_t(SYMTAB_BEG, (void*)0xF0F0F0F0);
//     tab->top = tab->beg->next;
//     tab->count = 0;

//     return base;
// }


// void *cish::sym_add( void *handle, const char *key, void *value )
// {
//     auto  *symtab = (symtab_t*)handle;
//     sym_t *symbol = new (symtab->top) sym_t(key, value);
//     symtab->top = symbol->next;
//     return (void*)(symbol->value);
// }



// void *cish::sym_find( void *handle, const char *key )
// {
//     auto  *symtab = (symtab_t*)handle;
//     sym_t *curr = symtab->beg;
//     sym_t *end  = symtab->top;

//     while (curr < end)
//     {
//         // printf("curr->key: %s\n", curr->key);
//         if (strcmp(curr->key, key) == 0)
//         {
//             return curr->value;
//         }

//         curr = curr->next;
//     }

//     // printf("end->key: %s\n", symtab->end->key);
//     // printf("[sym_find] symbol %d: %s\n", idx, curr);

//     return nullptr;
// }


// void *cish::sym_next( void *handle, void *ptr )
// {
//     symtab_t *symtab = (symtab_t*)handle;
//     sym_t *curr, *next;

//     if (ptr) { curr = (sym_t*)( (uint8_t*)ptr - sizeof(sym_t) ); }
//     else     { curr = symtab->beg; }

//     next = curr->next;
//     if (next == symtab->top)
//     {
//         return nullptr;
//         // uint8_t *next = (uint8_t*)(symbol->next);
//         // return (void*)(next + sizeof(sym_t));
//     }

//     return ((uint8_t*)next + sizeof(sym_t));
// }

