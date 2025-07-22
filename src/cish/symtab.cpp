
// #include "../vm/bytecode.hpp"
// #include <cish/compile-ctx.hpp>
// #include <util/bitmanip.hpp>
// #include <string.h>
// #include <stdio.h>
// #include <assert.h>

// using namespace cish;


// cish::SymTab::SymTab()
// :   parent(nullptr), depth(0) { clear(); }

// cish::SymTab::SymTab( SymTab *prnt )
// :   parent(prnt), depth(prnt->depth+1) { clear(); }


// void SymTab::clear()
// {
//     memset(&m_symbols[0], 0, sizeof(m_symbols));
//     m_symbols.clear();
// }


// SymTab *SymTab::spawnChild()
// {
//     return new SymTab(this);
// }



// Symbol *SymTab::_find( const char *key )
// {
//     for (auto &sym: m_symbols)
//         if (strcmp(key, sym.key) == 0)
//             return &sym;
//     if (parent)
//         return parent->_find(key);
//     return nullptr;
// }


// Symbol *SymTab::_insert( const char *key )
// {
//     m_symbols.push(Symbol(key));
//     return &m_symbols.top();
// }



// Symbol *SymTab::find( const char *key )
// {
//     Symbol *sym = _find(key);

//     if (sym == nullptr)
//     {
//         fprintf(stderr, "[SymTab::find] could not find symbol \"%s\"\n", key);
//         assert((false));
//     }

//     return sym;
// }


// Symbol *SymTab::insert( const char *key )
// {
//     if (auto *sym = _find(key))
//         return sym;
//     m_symbols.push(Symbol(key));
//     return &m_symbols.top();
// }



// // template <typename T>
// // cish::Symbol insert( const char *key, const T &data )
// // {
// //     cish::Symbol sym(key);
// //     new (sym.as_bytes) T(data);
// //     return sym;
// // }


// // Symbol *SymTab::addType( const char *key, const SymType &data )
// // {
// //     assert((find(key) == nullptr));
// //     m_symbols.push(insert(key, data));
// //     return &m_symbols.top();
// // }

// // cish::Symbol *cish::SymTab::addFunc( const char *key, const SymFunc &data )
// // {
// //     assert((find(key) == nullptr));
// //     m_symbols.push(insert(key, data));
// //     return &m_symbols.top();
// // }

// // cish::Symbol *cish::SymTab::addVar( const char *idntkey, const char *typekey )
// // {
// //     assert((find(idntkey) == nullptr));
// //     auto *sym = m_symbols.push(Symbol(Sym_Var, idntkey));
// //     sym->as_Var.typekey = typekey;
// //     // sym->as_Var.rbpoff  = align_up(m_offset,) = typekey;
// //     return sym;
// // }
