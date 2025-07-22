#include <cish/symbol.hpp>
#include <util/bitmanip.hpp>

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <new>




cish::StrTab::StrTab( size_t size )
{
    m_base = (char*)malloc(size);
    m_end  = m_base + size;
    m_tail = m_base;
}


int cish::StrTab::write( const char *str )
{
    size_t size = strlen(str) + 1;
    if (m_tail+size >= m_end)
        return -1;

    size_t offset = m_tail - m_base;
    *(m_tail++) = '-';
    *(m_tail++) = '>';

    memcpy(m_tail, str, size);
    m_tail += size;

    return (int)offset;
}


const char *cish::StrTab::read( int offset )
{
    const char *str = m_base + offset;
    assert((*(str++) == '-'));
    assert((*(str++) == '>'));
    return str;
}








cish::SymTab::SymTab( size_t size, SymTab *parent )
:   m_parent(parent)
{
    m_base = (uint8_t*)malloc(size);
    m_base = (uint8_t*)align_up((uintptr_t)m_base, 16);

    m_end  = m_base + align_down(size, (size_t)16);
    m_tail = m_base;
}


void cish::SymTab::loadGlobalSymbols()
{
    #define ADD_TYPE(Nm, Tp) insert(Nm, sym::Type(sizeof(Tp), alignof(Tp)))
    ADD_TYPE("char",     char);
    ADD_TYPE("int",      int);
    ADD_TYPE("float",    float);
    ADD_TYPE("double",   double);
    ADD_TYPE("int8_t",   int8_t);
    ADD_TYPE("int16_t",  int16_t);
    ADD_TYPE("int32_t",  int32_t);
    ADD_TYPE("int32_t",  int32_t);
    ADD_TYPE("int64_t",  int64_t);
    ADD_TYPE("uint8_t",  uint8_t);
    ADD_TYPE("uint16_t", uint16_t);
    ADD_TYPE("uint32_t", uint32_t);
    ADD_TYPE("uint64_t", uint64_t);
    #undef ADD_TYPE

    #define ADD_KWD(Nm) insert(Nm, sym::Kwd())
    ADD_KWD("switch");
    ADD_KWD("const");
    ADD_KWD("if");
    ADD_KWD("else");
    ADD_KWD("for");
    ADD_KWD("while");
    ADD_KWD("return");
    #undef ADD_KWD
}



int cish::SymTab::write( const char *label, int tag, const void *data, size_t size )
{
    m_tail = (uint8_t*)align_up((uintptr_t)m_tail, 16);

    size_t offset = m_tail - m_base;
    uint8_t *next = m_tail + size + sizeof(Symbol);

    if (next >= m_end)
    {
        return -1;
    }

    if (size > 0)
    {
        Symbol *sym = new (m_tail) Symbol(label, tag, size);
        memcpy(sym->data, data, size);
    }

    m_tail = next;

    return (int)offset;
}


void *cish::SymTab::read( int offset, size_t size )
{
    auto *H = (Symbol*)(m_base + offset);
    assert(((H->magic == MAGIC) && (H->size == size)));
    return (void*)(H->data);
}


void *cish::SymTab::find( const char *label, int tag )
{
    uint8_t *curr = m_base;

    while (curr < m_end)
    {
        auto *sym = (Symbol*)curr;

        if (tag==sym->tag && strcmp(sym->label, label) == 0)
        {
            return (void*)(sym->data);
        }

        curr = sym->data + sym->size;
    }

    if (m_parent)
    {
        return m_parent->find(label, tag);
    }

    return nullptr;
}





// void reserer()
// {
//     using namespace cish;
//     static cish::SymTab tab(512 * 1024);

//     #define ADD_TYPE(Nm, Tp) tab.insert(Nm, sym::Type(sizeof(Tp), alignof(Tp)))
//     ADD_TYPE("char",     char);
//     ADD_TYPE("int",      int);
//     ADD_TYPE("float",    float);
//     ADD_TYPE("double",   double);
//     ADD_TYPE("int8_t",   int8_t);
//     ADD_TYPE("int16_t",  int16_t);
//     ADD_TYPE("int32_t",  int32_t);
//     ADD_TYPE("int32_t",  int32_t);
//     ADD_TYPE("int64_t",  int64_t);
//     ADD_TYPE("uint8_t",  uint8_t);
//     ADD_TYPE("uint16_t", uint16_t);
//     ADD_TYPE("uint32_t", uint32_t);
//     ADD_TYPE("uint64_t", uint64_t);
//     #undef ADD_TYPE

//     #define ADD_KWD(Nm) tab.insert(Nm, sym::Kwd())
//     ADD_KWD("switch");
//     ADD_KWD("const");
//     ADD_KWD("if");
//     ADD_KWD("else");
//     ADD_KWD("for");
//     ADD_KWD("while");
//     ADD_KWD("return");
//     #undef ADD_KWD

//     auto *sym = tab.find<sym::Kwd>("");
//     sym->TAG;
// }

