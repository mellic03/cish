// #include <cish/parser.hpp>
// #include <cish/parse-node.hpp>

// #include <util/tuple.hpp>
// #include <algorithm>
// #include <type_traits>

// using namespace cish;
// using namespace cish;

// Program::Program( Parser &P )
// {
//     m_list = newNode<StmntList>(P);
// }


// Stmnt::Stmnt( Parser &P )
// {
//     if (P.peek() == Type::Identifier)
//         as_Assign = newNode<Assign>(P);
//     P.consume(Type::SemiColon);
// }


// StmntList::StmntList( Parser &P )
// {
//     m_stmnt = newNode<Stmnt>(P);
//     if (!P.isAtEnd())
//         m_next = newNode<StmntList>(P);
// }


// Decl::Decl( Parser &P )
// {
//     m_VarDecl = newNode<VarDecl>(P);

//     if (P.peek() == Type::LeftParen)
//     {
//         m_fundecl = newNode<FunDecl>(P);
//     }

//     else
//     {
//         P.consume(Type::SemiColon);
//     }
// }


// Assign::Assign( Parser &P )
// {
//     m_idnt = newNode<Atom>(P);
//     m_expr = newNode<Expr>(P);
// }





// VarDecl::VarDecl( Parser &P )
// {
//     m_type = newNode<Atom>(P);
//     m_idnt = newNode<Atom>(P);
// }


// FunDecl::FunDecl( Parser &P)
// :   m_list(nullptr)
// {
//     P.consume(Type::LeftParen);
//     if (P.peek() == Type::Identifier)
//         m_list = newNode<DeclList>(P);
//     P.consume(Type::RightParen);
// }


// DeclList::DeclList( Parser &P )
// // :   m_arg(VarDecl(P)),
//     // m_next(nullptr)
// {
//     m_arg = newNode<VarDecl>(P);

//     if (P.match(Type::SemiColon))
//         m_next = newNode<DeclList>(P);
//     else
//         m_next = nullptr;
// }




// Call::Call( Parser &P )
// {
    
// }


// CallList::CallList( Parser &P )
// {

// }



// Expr::Expr( Parser &P )
// {
//     m_lhs = newNode<Term>(P);
//     m_rhs = newNode<Expr>(P);
// }


// Term::Term( Parser &P )
// {
//     is_atom = (P.peek() == Type::Identifier);
//     if (is_atom) as_Atom = newNode<Atom>(P);
//     else         as_Expr = newNode<Expr>(P);
// }


// Atom::Atom( Parser &P )
// {
//     Token *tok = nullptr;
//     if (tok = P.match(Type::Identifier, Type::Number, Type::String))
//     {
//         m_type = tok->type;
//         m_str  = tok->lexeme;
//     }
// }


// // template <typename fn_type, typename... Args>
// // struct OnFrameExit
// // {
// //     fn_type mF;

// //     OnFrameExit( fn_type F, Args... args ): mF(F), m_args(args...) {  }
// //     OnFrameExit( const OnFrameExit& ) = delete;
// //     OnFrameExit( OnFrameExit&& ) = delete;
// //     OnFrameExit &operator=( OnFrameExit& ) = delete;
// //     OnFrameExit &operator=( OnFrameExit&& ) = delete;
// //     ~OnFrameExit() { mF(m_args.value, m_args.as_base().value); }
// // };


