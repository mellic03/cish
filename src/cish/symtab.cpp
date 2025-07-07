// #include "symtab.hpp"
// #include <string.h>


// const cish::sym_t &cish::symtab_t::find( const char *key )
// {
//     static const sym_t dummy = {"", -1, -1};
//     for (int i=0; i<m_idx; i++)
//         if (strcmp(key, m_syms[i].key) == 0)
//             return m_syms[i];
//     return dummy;
// }


// void cish::symtab_t::insert( const char *key, int size )
// {
//     m_syms[m_idx++] = {key, m_offset, size};
//     m_offset += size;
// }



// const cish::sym_t &cish::ProgramScope::find( const char *key )
// {
//     // for (int i=m_idx-1; i>=0; i--)
//     // {
//     //     int offset = m_tabs[i].find(key);
//     //     if (offset >= 0)
//     //     {
//     //         return offset;
//     //     }
//     // }
//     return top().find(key);
// }


// void cish::ProgramScope::insert( const char *key, int size )
// {
//     top().insert(key, size);
// }

