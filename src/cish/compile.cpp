#include <cish/compile.hpp>
#include <stdarg.h>


uintptr_t cish::CompileCtx::resb( size_t size, size_t align )
{
    size_t offset = m_dataTop;
    m_dataTop = (m_dataTop+size + align-1) & ~(align-1);
    return offset;
}


void cish::CompileCtx::writeText( const char *fmt, ... )
{
    va_list vlist;
    va_start(vlist, fmt);
    vfprintf(this->text_out, fmt, vlist);
    va_end(vlist);
}
