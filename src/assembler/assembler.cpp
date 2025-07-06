// #include "assembler.hpp"
// #include <cish/token.hpp>
// #include <stdio.h>
// #include "../vm/vm.hpp"
// #include "../vm/bytecode.hpp"
// #include "../parser/astnode.hpp"
// using namespace cish;


// struct AstVisitorAsm: AstVisitor
// {
//     VmOp *m_top;

//     AstVisitorAsm( uint64_t *text )
//     {
//         m_top = (VmOp*)(text);
//     }

//     void _emit( VmOp_ opcode )
//     {
//         m_top->opcode = opcode;
//         // m_top->sign = 0;
//         m_top->data = 0;
//         m_top++;
//     }

//     void _emit( VmOp_ opcode, uint32_t data )
//     {
//         m_top->opcode = opcode;
//         // m_top->sign = 0;
//         m_top->data = data;
//         m_top++;
//     }

//     virtual void visitGroup( AstGroup *N ) final;
//     virtual void visitCond( AstCond* ) final;
//     virtual void visitBinary( AstBinary *N ) final;
//     virtual void visitPrefix( AstPrefix *N ) final;
//     virtual void visitPostfix( AstPostfix *N ) final;
//     virtual void visitDecl( AstDecl *N ) final;
//     virtual void visitAssign( AstAssign *N ) final;
//     virtual void visitVariable( AstVariable *N ) final;
//     virtual void visitDeclType( AstDeclType* ) final;
//     virtual void visitNumber( AstNumber* ) final;
//     virtual void visitFunctionCall( AstFunctionCall* ) final;
//     virtual void visitReturn( AstReturn *N ) final;
// };




// uint8_t *cish::assemble( AstNode *ast, uint64_t *buf, size_t bufsz )
// {
//     AstVisitorAsm V(buf);
//     V.visit(ast);
//     // V._emit(VmOp_)
//     // V._emit(VmOp_exit);
//     return (uint8_t*)(V.m_top);
// }






// void AstVisitorAsm::visitGroup( AstGroup *N )
// {
//     for (auto *child: *N)
//     {
//         visit(child);
//     }
// }



// void AstVisitorAsm::visitCond( AstCond *N )
// {
//     visit(N->m_expr);
//     // printf(".cond_start:\n");
//     // printf("    jeq .cond_end\n");
//     visit(N->m_body);
//     // printf("    jmp .cond_start\n");
//     // printf(".cond_end:\n");
// }



// void AstVisitorAsm::visitBinary( AstBinary *N )
// {
//     visit(N->m_rhs);

//     visit(N->m_lhs);

//     switch (N->m_tok->type)
//     {
//         default: break;
//         case Type::Plus:    _emit(VmOp_addu); break;
//         case Type::Minus:   _emit(VmOp_subu); break;
//         case Type::Star:    _emit(VmOp_mulu); break;
//         case Type::Slash:   _emit(VmOp_divu); break;
//         case Type::Ampsnd:  _emit(VmOp_and);  break;
//         case Type::Bar:     _emit(VmOp_or);   break;
//         case Type::Hat:     _emit(VmOp_xor);  break;
//     }

// }



// void AstVisitorAsm::visitPrefix( AstPrefix *N )
// {
//     visit(N->m_rhs);

//     switch (N->m_tok->type)
//     {
//         default: break;
//         // case Type::PlusPlus:   printf("    add rax, 1\n"); m_rax += 1; break;
//         // case Type::MinusMinus: printf("    sub rax, 1\n"); m_rax -= 1; break;
//     }
// }



// void AstVisitorAsm::visitPostfix( AstPostfix *N )
// {
//     visit(N->m_lhs);

//     switch (N->m_tok->type)
//     {
//         default: break;
//         // case Type::PlusPlus:   printf("    add rax, 1\n"); m_rax += 1; break;
//         // case Type::MinusMinus: printf("    sub rax, 1\n"); m_rax -= 1; break;
//     }
// }


// void AstVisitorAsm::visitDecl( AstDecl *N )
// {
//     // _emit(VmOp_pushn, sizeof(dtype));
// }


// void AstVisitorAsm::visitAssign( AstAssign *N )
// {
//     visit(N->m_expr);
//     visit(N->m_dst);
// }


// void AstVisitorAsm::visitDeclType( AstDeclType *N )
// {

// }


// void AstVisitorAsm::visitVariable( AstVariable *N )
// {
//     // _emit(VmOp_push, address);
// }


// void AstVisitorAsm::visitNumber( AstNumber *N )
// {
//     _emit(VmOp_push, N->m_data);
// }


// void AstVisitorAsm::visitFunctionCall( AstFunctionCall *N )
// {

// }


// void AstVisitorAsm::visitReturn( AstReturn *N )
// {
//     visit(N->m_expr);
//     _emit(VmOp_ret);
// }

