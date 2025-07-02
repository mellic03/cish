#pragma once
#include <cish/type.hpp>
#include <cish/token.hpp>

struct AstNode;
AstNode *ProdPrimary( cish::Token* );
AstNode *ProdFactor( cish::Token* );
AstNode *ProdTerm( cish::Token* );
AstNode *ProdExpr( cish::Token* );

