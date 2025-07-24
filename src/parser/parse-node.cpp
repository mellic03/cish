#include <cish/parse-node.hpp>
#include <cish/token-stream.hpp>

#include <util/tuple.hpp>
#include <algorithm>
#include <type_traits>

using namespace cish;


template <typename list_type>
list_type *get_last( list_type *list )
{
    list_type *curr = list;
    while (curr->m_next)
        curr = curr->m_next;
    return curr;
}



NodeList *NodeList::back()
{
    return get_last(this);
}


void NodeList::insert( iNode *node )
{
    if (m_size++ == 0)
    {
        m_node = node;
        return;
    }

    NodeList *back = get_last(this);
    back->m_next = (NodeList*)newNd<NodeList>(node);
}


void DeclList::insert( VarDecl *decl )
{
    if (m_size++ == 0)
    {
        m_decl = decl;
        return;
    }

    auto *back = get_last(this);
    back->m_next = newNd<DeclList>(decl);      
}


void CallList::insert( Idnt *idnt )
{
    if (m_size++ == 0)
    {
        m_idnt = idnt;
        return;
    }

    auto *back = get_last(this);
    back->m_next = newNd<CallList>(idnt);  
}




/*
    Atom        ::= idnt | string | number;

    ParenExpr   ::= "(" <Expr> ")"
    BlockScope  ::= "{" <Stmnt> "}"

    VarDecl     ::= idnt idnt ("," idnt)* ";"

    FunDeclArg  ::= idnt idnt;
    FunDeclList ::= <FunDeclArg> ("," <FunDeclArg>)*
    FunDecl     ::= idnt idnt "(" <FunDeclList>? ")" <BlockScope>

    FunCallList ::= <Expr> ("," <Expr>)*
    FunCall     ::= idnt "(" <FunCallList>? ")"

    Primary     ::= <ParenExpr> | <FunCall> | <Atom>

    Postfix     ::= <Primary> ("++" | "--")?
    Prefix      ::= ("++" | "--" | "!" | "~")? <Postfix>

    Binary4     ::= <Prefix>  (("|" | "&" | "^")? <Prefix>)*
    Binary3     ::= <Binary4> (("*" | "/") <Binary4>)*
    Binary2     ::= <Binary3> (("+" | "-") <Binary3>)*
    Binary1     ::= <Binary2> (("==" | "!=" | "<" | "<=" | ">" | ">=") <Binary2>)*
    Binary      ::= <Binary1> ("=" <Expr>)?
    Expr        ::= <Binary>

    Decl        ::= <FunDecl> | <VarDecl>
    Return      ::= "return" <Expr> ";"
    Cond        ::= ("if" | "while" | "for") <ParenExpr> <BlockScope>
    Stmnt       ::= <Return> | <Cond> | <Decl> | (<Expr> ";")
*/


// cish::ErrorNode::ErrorNode( TokenStream &stream )
// {

// }


// // cish::ListNode::ListNode()
// // {

// // }


// cish::BinaryNode::BinaryNode( TokenStream &stream )
// {

// }


// cish::UnaryNode::UnaryNode( TokenStream &stream )
// {

// }

