#pragma once
#include "bytecode.hpp"
#include "ctx.hpp"
#include <cish/compile-ctx.hpp>
#include <cish/compile.hpp>




// #define VMOP_XXX_RegReg(name_, op_)\
//     inline constexpr VmOp \
//     VmOp##name_## RegReg( uint8_t dst, uint8_t src )\
//     { return VmOp(VmOp_##op_## _rr, dst, src, 0); }


// #define VMOP_IMM(name_, op_) \
//     inline constexpr VmOp VmOp##name_( int32_t imm ) \
//     { return VmOp(VmOp_##op_, 0, 0, imm); }


#define EMIT(opcode, dstreg, srcreg, value) \
    ctx.emit(VmOp(opcode, dstreg, srcreg, value))


// #define EMIT_IMM(name_, op_) \
//     inline constexpr void Emit##name_( CompileCtx &ctx, int32_t imm ) \
//     { ctx.emit(VmOp(VmOp_##op_, 0, 0, imm)); }

// #define EMIT_XXX_Imm(name_, op_) \
//     inline constexpr void Emit##name_## Imm( CompileCtx &ctx, int32_t imm ) \
//     { EmitMovRegImm(ctx, Reg_tmp, imm); \
//       EMIT(VmOp_##op_## _r, 0, Reg_tmp, 0); }


// #define EMIT_XXX(name_, op_) \
//     inline constexpr void Emit##name_ ( CompileCtx &ctx ) \
//     { EMIT(VmOp_##op_, 0, 0, 0); }


// #define EMIT_XXX_Reg(name_, op_) \
//     inline constexpr void Emit##name_## Reg( CompileCtx &ctx, uint8_t src ) \
//     { EMIT(VmOp_##op_## _r, 0, src, 0); }


// #define EMIT_XXX_RegReg(name_, op_)\
//     inline constexpr void Emit##name_## RegReg( CompileCtx &ctx, uint8_t dst, uint8_t src )\
//     { EMIT(VmOp_##op_## _rr, dst, src, 0); }


// #define EMIT_XXX_RegImm(name_, op_)\
//     inline constexpr void Emit##name_## RegImm( CompileCtx &ctx, uint8_t dst, int32_t imm )\
//     { EMIT(VmOp_##op_## _ri, dst, 0, imm); }
//     // { EmitMovRegImm(ctx, Reg_tmp, imm);
//     //   Emit##name_## RegReg(ctx, dst, Reg_tmp); }


// template <VmOp_ Op> inline constexpr
// void Emit_xxi( CompileCtx &ctx, int32_t imm )
// { EMIT(Op, 0, 0, imm); }

// template <VmOp_ Op> inline constexpr
// void Emit_rxx( CompileCtx &ctx, Reg_ dst )
// { EMIT(Op, dst, 0, 0); }

// template <VmOp_ Op> inline constexpr
// void Emit_rrx( CompileCtx &ctx, Reg_ dst, Reg_ src )
// { EMIT(Op, dst, src, 0); }

// template <VmOp_ Op> inline constexpr
// void Emit_rxi( CompileCtx &ctx, Reg_ dst, int32_t imm )
// { EMIT(Op, dst, 0, imm); }

// template <VmOp_ Op> inline constexpr
// void Emit_rri( CompileCtx &ctx, Reg_ dst, Reg_ src, int32_t imm )
// { EMIT(Op, dst, 0, imm); }

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

    // inline constexpr VmOp VmOpMovRegReg( uint8_t dst, uint8_t src )
    // { return VmOp(VmOp_mov_rr, dst, src, 0); }

    // inline constexpr VmOp VmOpMovRegImm( uint8_t dst, int32_t imm )
    // { return VmOp(VmOp_mov_ri, dst, 0, imm); }

    // inline constexpr void EmitMovRegReg( CompileCtx &ctx, uint8_t dst, uint8_t src )
    // {
    //     EMIT(VmOp_mov_rr, dst, src, 0);
    // }

    // inline constexpr void EmitMovRegImm( CompileCtx &ctx, uint8_t dst, int32_t imm )
    // {
    //     EMIT(VmOp_mov_ri, dst, 0, imm);
    // }

    // inline constexpr void EmitMovRegLocal( CompileCtx &ctx, uint8_t dst, size_t rbpoff )
    // {
    //     EMIT(VmOp_mov_rl, dst, 0, rbpoff);
    // }

    // inline constexpr void EmitMovLocalReg( CompileCtx &ctx, size_t rbpoff, uint8_t src )
    // {
    //     EMIT(VmOp_mov_lr, 0, src, rbpoff);
    // }

    // inline constexpr void EmitMovLocalImm( CompileCtx &ctx, size_t rbpoff, int32_t imm )
    // {
    //     EmitMovRegImm(ctx, Reg_tmp, imm);
    //     EmitMovLocalReg(ctx, rbpoff, Reg_tmp);
    // }

    // inline constexpr void EmitPopReg( CompileCtx &ctx, uint8_t dst )
    // { EMIT(VmOp_pop_r, dst, 0, 0); }


    // VMOP_XXX_RegReg(Add, add);  // VMOP_XXX_RegImm(Add, add)
    // VMOP_XXX_RegReg(Sub, sub);  // VMOP_XXX_RegImm(Sub, sub)
    // VMOP_XXX_RegReg(Mul, mul);  // VMOP_XXX_RegImm(Mul, mul)
    // VMOP_XXX_RegReg(Div, div);  // VMOP_XXX_RegImm(Div, div)
    // VMOP_IMM(Jmp, jmp);
    // VMOP_IMM(Call, call);

    // EMIT_XXX_RegReg(Add, add);  EMIT_XXX_RegImm(Add, add)
    // EMIT_XXX_RegReg(Sub, sub);  EMIT_XXX_RegImm(Sub, sub)
    // EMIT_XXX_RegReg(Mul, mul);  EMIT_XXX_RegImm(Mul, mul)
    // EMIT_XXX_RegReg(Div, div);  EMIT_XXX_RegImm(Div, div)
    // EMIT_XXX_Reg(Push, push);   EMIT_XXX_Imm(Push, push)
    // EMIT_XXX(Ret, ret)
    // EMIT_XXX(Swap, swap)
    
    // EMIT_IMM(Jmp, jmp);
    // EMIT_IMM(Call, call);
}



// #undef EMIT
// #undef VMOP_XXX_RegReg
// #undef VMOP_XXX_RegImm

