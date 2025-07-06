// #pragma once
// #include <cish/type.hpp>
// #include <cish/token.hpp>
// #include <linkedlist.hpp>


// struct AstList;
// struct AstBinary;
// struct AstPrefix;
// struct AstPostfix;

// struct AstCond;
// struct AstReturn;

// struct AstType;
// struct AstFunction;
// struct AstCall;

// struct AstVariable;
// struct AstString;
// struct AstNumber;

// struct AstNode;


// enum Ast_: uint32_t
// {
//     Ast_List,
//     Ast_Binary,
//     Ast_Prefix,
//     Ast_Postfix,
//     Ast_Cond,
//     Ast_Return,
//     Ast_Type,
//     Ast_Function,
//     Ast_Call,
//     Ast_Variable,
//     Ast_String,
//     Ast_Number,
// };


// template <Ast_ tp_>
// struct AstBase
// {
//     static constexpr Ast_ m_type = tp_;
// };


// struct AstList: AstBase<Ast_List>, knl::LinkedList<AstNode>
// {
//     template <typename... Args>
//     AstList( Args... args )
//     {
//         (push(args), ...);
//     }
// };


// struct AstBinary: AstBase<Ast_Binary>
// {
//     cish::Token *m_tok;
//     AstNode *m_lhs, *m_rhs;

//     AstBinary( AstNode *lhs, cish::Token *tok, AstNode *rhs )
//     :   m_tok(tok), m_lhs(lhs), m_rhs(rhs) {  }

// };


// struct AstPrefix: AstBase<Ast_Prefix>
// {
//     cish::Token *m_tok;
//     AstNode *m_rhs;

//     AstPrefix( cish::Token *tok, AstNode *rhs )
//     :   m_tok(tok), m_rhs(rhs) {  }

// };


// struct AstPostfix: AstBase<Ast_Postfix>
// {
//     cish::Token *m_tok;
//     AstNode *m_lhs;

//     AstPostfix( AstNode *lhs, cish::Token *tok )
//     :   m_tok(tok), m_lhs(lhs) {  }

// };



// struct AstCond: AstBase<Ast_Cond>
// {
//     AstNode *m_cond, *m_body;

//     AstCond( AstNode *cond, AstNode *body )
//     :   m_cond(cond), m_body(body) {  }
// };


// struct AstReturn: AstBase<Ast_Return>
// {
//     AstNode *m_expr;

//     AstReturn( AstNode *expr )
//     :   m_expr(expr) {  }
// };


// struct AstType: AstBase<Ast_Type>
// {
//     const char *m_symkey;

//     AstType( cish::Token *idnt )
//     :   m_symkey(idnt->lexeme) {  }
// };


// struct AstFunction: AstBase<Ast_Function>
// {
//     const char *m_symkey;
//     AstNode *m_body;

//     AstFunction( cish::Token *name, AstNode *body )
//     :   m_symkey(name->lexeme), m_body(body) {  }
// };


// struct AstCall: AstBase<Ast_Call>
// {
//     const char *m_callee;
//     AstNode *m_expr;

//     AstCall( cish::Token *callee, AstNode *expr )
//     :   m_callee(callee->lexeme), m_expr(expr) {  }
// };


// struct AstVariable: AstBase<Ast_Variable>
// {
//     const char *m_symkey;

//     AstVariable( cish::Token *tok )
//     :   m_symkey(tok->lexeme) {  }
// };


// struct AstString: AstBase<Ast_String>
// {
//     const char *m_str;

//     AstString( cish::Token *tok )
//     :   m_str(tok->lexeme) {  }
// };


// struct AstNumber: AstBase<Ast_Number>
// {
//     const char *m_str;

//     AstNumber( cish::Token *tok )
//     :   m_str(tok->lexeme) {  }
// };


// struct AstNode
// {
//     uint32_t type;

//     union {
//         uint8_t   as_bytes[];
//         AstBinary as_Binary;
//         AstNumber as_Number;
//     };
// };


// template <typename T, typename... Args>
// AstNode *allocNode( Args... args )
// {
//     auto *N = new AstNode();
//     auto *t = new (N->as_bytes) T(args...);
//     N->type = t->m_type;
//     return N;
// }

