// #include <cish/ast.hpp>
// #include "lexer/lexer.hpp"
// #include <stdio.h>
// #include <stdlib.h>


// AstUnaryOp::AstUnaryOp( Token *tok, iAstNode *R )
// :   optype(tok->type), lexeme(tok->lexeme), rhs(R)
// {
//     if (R) R->parent = this;
// }


// void AstUnaryOp::walk()
// {
//     printf(" %s ", AstOpStr((Type::)optype));
//     rhs->walk();
// }


// AstBinaryOp::AstBinaryOp( iAstNode *L, Token *op, iAstNode *R )
// :   optype(op->type), lexeme(op->lexeme), lhs(L), rhs(R)
// {
//     if (L) L->parent = this;
//     if (R) R->parent = this;
// }
// void AstBinaryOp::walk()
// {
//     printf("(");
//     lhs->walk();
//     printf(" %s ", AstOpStr((Type::)optype));
//     rhs->walk();
//     printf(")");
// }



// void AstGrouping::walk()
// {
//     // printf("%s --> ", cish::TypeToStr(type()));
//     for (auto *N: nodes)
//     {
//         N->walk();
//     }
// }



// void AstLitNumber::walk()
// {
//     switch (littype)
//     {
//         default: printf("? "); break;
//         case AstLit_i32: printf("%d",  as_i32); break;
//         case AstLit_i64: printf("%ld", as_i64); break;
//         case AstLit_u32: printf("%u",  as_u32); break;
//         case AstLit_u64: printf("%lu", as_u64); break;
//     }
// }


// void AstString::walk()
// {
//     printf("\"%s\"", this->value);
// }


// void AstIdnt::walk()
// {
//     printf("%s", this->name);
// }





// // void cish::walkAST( AstNode *E )
// // {
// //     if (!E)
// //     {
// //         printf("[cish::walkAST] Node is nullptr!\n");
// //         return;
// //     }

// //     static int Indent = 0;
// //     #define INDENT for (int i=0; i<Indent; i++) printf(" ")

// //     switch (E->type())
// //     {
// //         default: break;

// //         case Type::Root:
// //         {
// //             for (AstNode *child: E->asRoot.children)
// //                 walkAST(child);
// //             printf("\n");
// //             break;
// //         }

// //         case Type::OpAssign: walkAST(E->asOpAssign.L);  printf(" = ");  walkAST(E->asOpAssign.R); break;
// //         case Type::OpAdd:    walkAST(E->asOpAssign.L);  printf(" + ");  walkAST(E->asOpAssign.R); break;
// //         case Type::OpSub:    walkAST(E->asOpAssign.L);  printf(" - ");  walkAST(E->asOpAssign.R); break;
// //         case Type::OpMul:    walkAST(E->asOpAssign.L);  printf(" * ");  walkAST(E->asOpAssign.R); break;
// //         case Type::OpDiv:    walkAST(E->asOpAssign.L);  printf(" / ");  walkAST(E->asOpAssign.R); break;

// //         case Type::LiteralInt:   printf("%d", E->asLiteralInt.value); break;
// //         case Type::LiteralFloat: printf("%f", E->asLiteralFloat.value); break;
// //         case Type::Variable:     printf("%s", E->asVariable.name); break;

// //         case Type::Function:
// //             INDENT; printf("function %s() {\n", E->asFunction.name);
// //             Indent += 4;
// //             INDENT; walkAST(E->asFunction.body);
// //             Indent -= 4;
// //             INDENT; printf("\n}\n");
// //             break;

// //         case Type::FunctionCall:
// //         {
// //             auto &call = E->asFunctionCall;
// //             auto *func = call.target;
        
// //             printf("%s(", func->name);
// //             for (AstNode *arg: call.args)
// //                 walkAST(arg);
// //             printf(")");
// //             break;
// //         }

// //         case Type::Terminal:
// //             printf(";\n");
// //             break;
// //     }
// // }


// // void EmitLiteralInt( cish::AstLiteralInt *E )
// // {
// // 	printf("mov rax, %d\n", E->value);
// // }

// // void EmitLiteralFloat( cish::AstLiteralFloat *E )
// // {
// // 	printf("mov rax, %f\n", E->value);
// // }

// // void EmitVariable( cish::AstVariable *E )
// // {
// //     uintptr_t offset = 32; // getOffset(E->name);
// // 	printf("mov rax, [rsp+%lu]\n", offset);
// // }

// // void EmitFunctionCall( cish::AstFunctionCall *E )
// // {
// //     uintptr_t addr = 0x00; // getAddr(E->target->name);
// // 	printf("call %lu\n", addr);
// // }

