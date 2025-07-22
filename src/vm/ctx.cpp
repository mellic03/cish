#include "ctx.hpp"
#include "bytecode.hpp"


cish::VmCtx::VmCtx( VmOp *base, size_t size )
:   text(base),
    m_stkmin(new uint8_t[2048]),
    m_stkmax(m_stkmax + 2048)
{
    memset(regs, 0, sizeof(regs));

    rbp = m_stkmax;
    rsp = m_stkmax;
    rip = 0;
}




// #define PUSHQ(X) m_rsp -= 8;   *(int64_t*)(m_stack + m_rsp) = X;
// #define POPQ(X)  X = *(int64_t*)(m_stack + m_rsp);   m_rsp += 8;


// uint32_t cish::VmCtx::pop32()
// {
//     return stack[--rsp];
// }

// uint32_t &cish::VmCtx::top32()
// {
//     return stack[rsp-1];
// }

// void cish::VmCtx::push32( int32_t *x )
// {
//     // printf("[pushFrame] top=%ld\n", *(int64_t*)(m_stack + m_rbp));
//     PUSHQ(m_rbp);
//     m_rbp = m_rsp;
// }


// void CompileCtx::popFrame()
// {
//     m_rsp = m_rbp;
//     POPQ(m_rbp);
//     // printf("[pushFrame] top=%ld\n", *(int64_t*)(m_stack + m_rbp));

//     assert((0 <= m_rsp));
//     assert((m_rsp <= m_stacksz));
// }





// void cish::VmCtx::callpush()
// {
//     callstack.push({rip, rsp, rbp, 0, 0});
//     rbp = align_up(rsp, 16);
//     // vbp = align_up(vsp, 16);
// }


// void cish::VmCtx::callpop()
// {
//     auto [ip, sp, bp, v_sp, v_bp] = callstack.pop();
//     rip=ip;
//     rsp=sp; rbp=bp;
//     // vsp=v_sp; vbp=v_bp;
// }


