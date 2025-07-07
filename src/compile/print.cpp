// #include <cish/compile.hpp>
// #include <cish/compile-ctx.hpp>
// #include <cish/node.hpp>

// #include <stdio.h>
// #include <string.h>
// #include <assert.h>
// #include "../vm/vm.hpp"
// #include "../vm/ctx.hpp"
// #include "../vm/bytecode.hpp"

// using namespace cish;

// void printAst( AstNode* );
// void print( AstNode* );
// void print_List( AstList& );
// void print_Binary( AstBinary& );
// void print_Prefix( AstPrefix& );
// void print_Postfix( AstPostfix& );
// void print_Scope( AstScope& );
// void print_Assign( AstAssign& );
// void print_Cond( AstCond& );
// void print_Return( AstReturn& );
// void print_Type( AstType& );
// void print_Decl( AstDecl& );
// void print_Func( AstFunc& );
// void print_Call( AstCall& );
// void print_Var( AstVar& );
// void print_String( AstString& );
// void print_Number( AstNumber& );


// void printAst( AstNode *ast )
// {
//     print(ast);
// }



// void print( AstNode *N )
// {
//     switch (N->type)
//     {
//         default: return;
//         case Ast_List:      print_List(N->as_List);         break;
//         case Ast_Binary:    print_Binary(N->as_Binary);     break;
//         case Ast_Prefix:    print_Prefix(N->as_Prefix);     break;
//         case Ast_Postfix:   print_Postfix(N->as_Postfix);   break;
//         case Ast_Scope:     print_Scope(N->as_Scope);       break;
//         case Ast_Assign:    print_Assign(N->as_Assign);     break;
//         case Ast_Cond:      print_Cond(N->as_Cond);         break;
//         case Ast_Return:    print_Return(N->as_Return);     break;
//         case Ast_Type:      print_Type(N->as_Type);         break;
//         case Ast_Decl:      print_Decl(N->as_Decl);         break;
//         case Ast_Func:      print_Func(N->as_Func);         break;
//         case Ast_Call:      print_Call(N->as_Call);         break;
//         case Ast_Var:       print_Var(N->as_Var);           break;
//         case Ast_String:    print_String(N->as_String);     break;
//         case Ast_Number:    print_Number(N->as_Number);     break;
//     }

// }



// void print_List( AstList &N )
// {
//     for (auto *child: N)
//     {
//         print(child);
//         printf("\n");
//     }
// }



// void print_Binary( AstBinary &N )
// {
//     printf("(");
//     print(N.m_lhs);
//     printf(" %s ", N.m_tok->lexeme);
//     print(N.m_rhs);
//     printf(")");
// }



// void print_Prefix( AstPrefix &N )
// {

// }



// void print_Postfix( AstPostfix &N )
// {

// }



// void print_Scope( AstScope &N )
// {
//     printf("{");
//     print(N.m_body);
//     printf("}");
// }


// void print_Assign( AstAssign &N )
// {
//     printf("%s = ", N.m_var);
//     print(N.m_expr);
// }


// void print_Cond( AstCond &N )
// {
//     printf("if (");
//     print(N.m_cond);
//     printf(")\n{\n");
//     print(N.m_body);
//     printf("\n}");
// }


// void print_Return( AstReturn &N )
// {
//     printf("return ");
//     print(N.m_expr);
// }


// void print_Type( AstType &N )
// {
//     printf("[type]%s", N.m_name);
// }


// void print_Decl( AstDecl &N )
// {
//     printf("let %s %s;", N.m_typekey, N.m_symkey);
// }


// void print_Func( AstFunc &N )
// {
//     printf("func %s %s(", N.m_rtype, N.m_name);
//     print(N.m_args);
//     printf(")\n{\n");
//     print(N.m_body);
//     printf("\n}");
// }


// void print_Call( AstCall &N )
// {
//     printf("%s()", N.m_callee);
// }


// void print_Var( AstVar &N )
// {
//     printf("%s", N.m_symkey);
// }


// void print_String( AstString &N )
// {

// }


// void print_Number( AstNumber &N )
// {
//     int64_t value = atol(N.m_str);
//     printf("%ld", value);
// }

