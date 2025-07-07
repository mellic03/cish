// #include "assembler.hpp"
// #include "symtab.hpp"
// #include <stdio.h>
// #include <string.h>
// #include "../vm/vm.hpp"
// #include "../vm/bytecode.hpp"
// using namespace cish;



// void visit_List( AstList& );
// void visit_Binary( AstBinary& );
// void visit_Prefix( AstPrefix& );
// void visit_Postfix( AstPostfix& );
// void visit_Scope( AstScope& );
// void visit_Assign( AstAssign& );
// void visit_Cond( AstCond& );
// void visit_Return( AstReturn& );
// void visit_Type( AstType& );
// void visit_Decl( AstDecl& );
// void visit_Func( AstFunc& );
// void visit_Call( AstCall& );
// void visit_Var( AstVar& );
// void visit_String( AstString& );
// void visit_Number( AstNumber& );


// /*
//     https://www.reddit.com/r/Compilers/comments/18wjm7g/how_do_variables_work_under_the_hood/

//     Well, typically on x64, rbp is pushed, rsp is copied to rbp,
//     and rsp is decremented to 'allocate' space for the stack frame
//     which contains space for the locals, any extra temporaries the
//     compilers will need, and take care of things like keeping rsp
//     16-byte aligned.

//     At the end, rsp is restored, and rbp is popped. All this depends
//     heavily on ABI, language, compiler, and compiler options. 


//     push rbp
//     mov rbp, rsp
//     rsp += sizeof(local variables)
//     {
//         // local scope stuff
//     }
//     mov rsp, rbp // restore rsp

// */



// static VmOp *op_base;
// static VmOp *op_top;
// static ProgramScope *pScope;

// template <typename... Args>
// static VmOp *emit( Args... args )
// {
//     // T *ptr = new (op_top) T(data);
//     // ptr->opcode = opcode;
//     // op_top += sizeof(T);
//     *(op_top++) = VmOp(args...);
//     return op_top-1;
// }

// static void emit( uint8_t opcode )
// {
//     emit(opcode);
// }


// static void visit( AstNode *N )
// {
//     switch (N->type)
//     {
//         default: return;
//         case Ast_List:      visit_List(N->as_List);         break;
//         case Ast_Binary:    visit_Binary(N->as_Binary);     break;
//         case Ast_Prefix:    visit_Prefix(N->as_Prefix);     break;
//         case Ast_Postfix:   visit_Postfix(N->as_Postfix);   break;
//         case Ast_Scope:     visit_Scope(N->as_Scope);       break;
//         case Ast_Assign:    visit_Assign(N->as_Assign);     break;
//         case Ast_Cond:      visit_Cond(N->as_Cond);         break;
//         case Ast_Return:    visit_Return(N->as_Return);     break;
//         case Ast_Type:      visit_Type(N->as_Type);         break;
//         case Ast_Decl:      visit_Decl(N->as_Decl);         break;
//         case Ast_Func:      visit_Func(N->as_Func);         break;
//         case Ast_Call:      visit_Call(N->as_Call);         break;
//         case Ast_Var:       visit_Var(N->as_Var);           break;
//         case Ast_String:    visit_String(N->as_String);     break;
//         case Ast_Number:    visit_Number(N->as_Number);     break;
//     }

// }


// uint32_t *cish::assemble( AstNode *ast, uint32_t *buf, size_t bufsz )
// {
//     op_base = (VmOp*)buf;
//     op_top  = (VmOp*)buf;
//     pScope  = new ProgramScope(new symtab_t[512]);

//     visit(ast);
//     emit(VmOp_exit);

//     return nullptr;
// }



// void visit_List( AstList &N )
// {
//     for (auto *child: N)
//     {
//         visit(child);
//     }
// }



// void visit_Binary( AstBinary &N )
// {
//     visit(N.m_rhs);
    
//     visit(N.m_lhs);

//     switch (N.m_tok->type)
//     {
//         default: break;
//         case Type::Plus:         emit(VmOp_add);  break;
//         case Type::Minus:        emit(VmOp_sub);  break;
//         case Type::Star:         emit(VmOp_mul);  break;
//         case Type::Slash:        emit(VmOp_div);  break;

//         case Type::Ampsnd:       emit(VmOp_and);  break;
//         case Type::Bar:          emit(VmOp_or);   break;
//         case Type::Hat:          emit(VmOp_xor);  break;

//         case Type::EqualEqual:   emit(VmOp_equ);  break;
//         case Type::Less:         emit(VmOp_les);  break;
//         case Type::LessEqual:    emit(VmOp_leq);  break;
//         case Type::Greater:      emit(VmOp_gtr);  break;
//         case Type::GreaterEqual: emit(VmOp_geq);  break;
//     }

// }



// void visit_Prefix( AstPrefix &N )
// {

// }



// void visit_Postfix( AstPostfix &N )
// {

// }



// void visit_Scope( AstScope &N )
// {
//     // pScope->push();
//     // emit(VmOp_pushr, VmOpReg(Reg_rbp));             // push rbp
//     // emit(VmOp_movrr, VmOpRegReg(Reg_rbp, Reg_rsp)); // mov  rbp, rsp

//     visit(N.m_body);

//     // emit(VmOp_movrr, VmOpRegReg(Reg_rsp, Reg_rbp)); // mov  rsp, rbp
//     // pScope->pop();
// }



// void visit_Assign( AstAssign &N )
// {
//     visit(N.m_expr);
// }



// void visit_Cond( AstCond &N )
// {
//     visit(N.m_cond);        // push expr
//     emit(VmOp_pushd, 0);    // push 0/false

//     if (N.m_kwd->type == Type::KwdIf)
//     {
//         VmOp *jeq = emit(VmOp_jeq, 0);  // jmp d32 if [rsp-1] == [rsp-2]

//         visit(N.m_body);
//         emit(VmOp_nop);
//         size_t cond_end = op_top-op_base;

//         jeq->d32 = cond_end;
//     }

//     // else if (N.m_kwd->type == Type::KwdWhile)
//     // {
//     //     size_t loop_beg = op_top-op_base;
//     //     auto *jeqd = emit(VmOp_jeqd, VmOpData(0));  // if expr==0: jmp to loop_end

//     //     visit(N.m_body);
//     //     emit(VmOp_jeqd, VmOpData(loop_beg));        // jmp to loop_beg

//     //     size_t loop_end = op_top-op_base;
//     //     jeqd->data = VmOpU64(loop_end);
//     // }

// }



// void visit_Return( AstReturn &N )
// {
//     visit(N.m_expr);
//     emit(VmOp_ret);
//     // printf("ret\n");
// }



// void visit_Type( AstType &N )
// {

// }



// static AstType *findType( const char *key )
// {
//     static AstType temp("i32", 4, 4);
//     return &temp;
// }


// void visit_Decl( AstDecl &N )
// {
//     // AstType &dtype = N.m_typekey->as_Type;
//     AstType *dtype = findType(N.m_typekey);
//     pScope->insert(N.m_symkey, dtype->m_size);

//     // emit(VmOp_addrd, VmOpRegData(Reg_rsp, dtype.m_size));
//     // printf("addrd %u, %lu\n", Reg_rsp, dtype.m_size);

// }



// void visit_Func( AstFunc &N )
// {
//     // N.m_rtype
//     // emit(VmOp_)
// }



// void visit_Call( AstCall &N )
// {

// }



// void visit_Var( AstVar &N )
// {
//     // offset relative to rbp
//     const auto &[key, offset, size] = pScope->find(N.m_symkey);

//     if (offset == -1)
//     {
//         fprintf(stderr, "[visit_Var] Ruh roh\n");
//         exit(1);
//     }

//     // // mov rax, [rbp + offset]
//     // emit(VmOp_movrrd, VmOpRegRegData(Reg_rax, Reg_rbp, offset));

//     // // push rax
//     // emit(VmOp_pushr, VmOpReg(Reg_rax));

// }



// void visit_String( AstString &N )
// {

// }



// void visit_Number( AstNumber &N )
// {
//     int64_t value = atol(N.m_str);
//     emit(VmOp_pushd, value);

//     // printf("pushd %ld\n", value);
// }

