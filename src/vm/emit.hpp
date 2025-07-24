#pragma once
#include "bytecode.hpp"
#include "ctx.hpp"
#include <cish/compile-ctx.hpp>
#include <cish/compile.hpp>


#define EMIT(opcode, dstreg, srcreg, value) \
    ctx.emit(VmOp(opcode, dstreg, srcreg, value))

#define EMIT_xxx(Name) \
    inline constexpr void \
    Emit_##Name( CompileCtx &ctx ) \
    { EMIT(VmOp_##Name, 0, 0, 0); }

#define EMIT_xxi(Name) \
    inline constexpr void \
    Emit_##Name( CompileCtx &ctx, int32_t imm ) \
    { EMIT(VmOp_##Name, 0, 0, imm); }


namespace cish
{
    inline constexpr void Emit_prntImm( CompileCtx &ctx, int32_t imm )
    { EMIT(OpI_prnt, 0, 0, imm); }

    inline constexpr void Emit_prntReg( CompileCtx &ctx, Reg_ dst )
    { EMIT(OpR_prnt, dst, 0, 0); }

    inline constexpr void Emit_movImm( CompileCtx &ctx, Reg_ dst, int32_t imm )
    { EMIT(OpRI_mov, dst, 0, imm); }

    inline constexpr void Emit_movReg( CompileCtx &ctx, Reg_ dst, Reg_ src )
    { EMIT(OpRR_mov, dst, src, 0); }

    inline constexpr void Emit_addImm( CompileCtx &ctx, Reg_ dst, int32_t imm )
    { EMIT(OpRI_add, dst, 0, imm); }

    inline constexpr void Emit_subImm( CompileCtx &ctx, Reg_ dst, int32_t imm )
    { EMIT(OpRI_sub, dst, 0, imm); }

    inline constexpr void Emit_addReg( CompileCtx &ctx, Reg_ dst, Reg_ src )
    { EMIT(OpRR_add, dst, src, 0); }

    inline constexpr void Emit_subReg( CompileCtx &ctx, Reg_ dst, Reg_ src )
    { EMIT(OpRR_sub, dst, src, 0); }

    
    inline constexpr void Emit_gload( CompileCtx &ctx, int32_t imm )
    { EMIT(OpI_gload, 0, 0, imm); }

    inline constexpr void Emit_gstor( CompileCtx &ctx, int32_t imm )
    { EMIT(OpI_gstor, 0, 0, imm); }


    inline constexpr void Emit_lload( CompileCtx &ctx, int32_t rbpoff )
    { EMIT(OpI_lload, 0, 0, rbpoff); }

    inline constexpr void Emit_lstor( CompileCtx &ctx, int32_t rbpoff )
    { EMIT(OpI_lstor, 0, 0, rbpoff); }


    inline constexpr void Emit_pushImm( CompileCtx &ctx, int32_t imm )
    { EMIT(OpI_push, 0, 0, imm); }

    inline constexpr void Emit_pushReg( CompileCtx &ctx, Reg_ src )
    { EMIT(OpR_push, 0, src, 0); }

    inline constexpr void Emit_popReg( CompileCtx &ctx, Reg_ dst=Reg_rnul )
    { EMIT(OpR_pop, dst, 0, 0); }

    EMIT_xxx(swap);

    EMIT_xxx(add);
    EMIT_xxx(sub);
    EMIT_xxx(mul);
    EMIT_xxx(div);

    EMIT_xxx(and);
    EMIT_xxx(or);
    EMIT_xxx(xor);
    EMIT_xxx(not);
    EMIT_xxx(neg);

    EMIT_xxx(equ);
    EMIT_xxx(les);
    EMIT_xxx(leq);
    EMIT_xxx(gtr);
    EMIT_xxx(geq);


    inline constexpr VmOp Op_jmp( int32_t addr )
    { return VmOp(VmOp_jmp, 0, 0, addr); }

    inline constexpr VmOp Op_jeq( int32_t addr )
    { return VmOp(VmOp_jeq, 0, 0, addr); }


    EMIT_xxi(call);
    EMIT_xxx(ret);

}


#undef EMIT
#undef EMIT_xxx
#undef EMIT_xxi

