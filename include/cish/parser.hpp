#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <cish/compile-ctx.hpp>
#include <cish/symtab.hpp>
#include <cish/type.hpp>
#include <cish/token.hpp>
#include <cish/parse-node.hpp>
#include <cish/prod-node.hpp>



namespace cish
{
    iNode *Parse( TokenStream& );
}


// namespace cish
// {
//     class CompileCtx;
//     class Parser;
// }


// class cish::Parser
// {
// private:
//     CompileCtx &m_ctx;

// public:
//     Token *m_beg,  *m_end;
//     Token *m_prev, *m_curr;

//     Parser( CompileCtx &ctx, Token *buf, size_t bufsz );

//     // Parser( const Parser &P )
//     // :   m_ctx(P.m_ctx), m_beg(P.m_beg), m_prev(P.m_prev), m_curr(P.m_curr) {  }

//     ParseNode *buildPT();
//     // ParseNode *buildAST();

//     ParseNode *ProdProgram();
//     ParseNode *ProdStmnt();
//     ParseNode *ProdExprStmnt();
//     ParseNode *ProdReturn();
//     ParseNode *ProdBlock();
//     ParseNode *ProdTypeName();
//     ParseNode *ProdCond();
//     ParseNode *ProdVar();
//     ParseNode *ProdFun();
//     ParseNode *ProdFunArgs();
//     ParseNode *ProdTypeIdnt();
//     ParseNode *ProdScope();

//     ParseNode *ProdExpr();
//     ParseNode *ProdPrecedence();
//     ParseNode *ProdPostfix();
//     ParseNode *ProdPrefix();
//     ParseNode *ProdList();
//     ParseNode *ProdPrimary();

//     Token *expect( uint32_t type, const char *fmt, ... );
//     Token *consume( uint32_t type, const char *fmt, ... );
//     Token *consume( uint32_t type );
//     Token *advance();
//     uint32_t peek( int offset=0 );
//     Token *check( uint32_t type );
//     bool   isAtEnd();
//     Token *save();
//     void   restore( Token* );

//     // template <uint32_t... types>
//     template <typename... Args>
//     Token *match( uint32_t type, Args... rest )
//     {
//         if (Token *tok = match(type))
//             return tok;
//         return match(rest...);
//     }

//     Token *match( uint32_t type )
//     {
//         if (m_curr->type == type)
//             return advance();
//         return nullptr;
//     }

//     template <typename... Args>
//     Token *check( uint32_t type, Args... rest )
//     {
//         Token *tok = check(type);
//         if (tok) return tok;
//         return check(rest...);
//     }
// };

