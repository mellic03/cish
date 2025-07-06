#pragma once



namespace cish
{
    struct sym_t
    {
        const char *key;
        int offset;
        int size;
    };


    struct symtab_t
    {
    private:
        sym_t m_syms[64];
        int   m_idx;
        int   m_offset;

    public:
        symtab_t(): m_idx(0), m_offset(0) {  }
        const sym_t &find( const char *key );
        void insert( const char *key, int size );
    };


    struct ProgramScope
    {
    private:
        symtab_t *m_tabs;
        int m_idx;

    public:
        ProgramScope( symtab_t *buf ): m_tabs(buf), m_idx(0) {  }
        void push() { m_idx++; }
        void pop() { m_idx--; }
        symtab_t &top() { return m_tabs[m_idx]; }

        const sym_t &find( const char *key );
        void insert( const char *key, int size );
    };
}


// struct VmScope
// {
//     struct symt {
//         const char *key;
//         int offset;
//     };

//     VmScope *parent;
//     int   m_offset;
//     symt  m_symtab[64];
//     symt *m_top;

//     VmScope()
//     :   m_offset(0), m_top(m_symtab) {  };


//     void push( const char *key, int size )
//     {
//         *(m_top++) = {key, m_offset};
//         m_offset += size;
//     }

//     int find( const char *key )
//     {
//         for (symt *sym=m_symtab; sym<m_top; sym++)
//             if (strcmp(key, sym->key) == 0)
//                 return sym->offset;
//         return -1;
//     }
// };
