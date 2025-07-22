#include <fstream>    // For std::ifstream
#include <string>     // For std::string
#include <iostream>   // For std::cout, std::cerr
#include <sstream>  // For std::istreambuf_iterator
#include <array>
#include <string.h>

#include <cish.hpp>
#include <cish/symtab.hpp>
#include <cish/compile-ctx.hpp>
#include <cish/compile.hpp>
#include <cish/token.hpp>
#include <cish/interpret.hpp>
#include <cish/lexer.hpp>
#include "assembler/assembler.hpp"
#include "parser/parser.hpp"
#include "vm/bytecode.hpp"
#include "vm/vm.hpp"


#include "../include/mpc.h"

extern void printAst( cish::AstNode* );
extern void functional_test( cish::Parser& );


int main( int argc, char **argv )
{
    if (argc != 2)
    {
        printf("Usage: cish [filepath]\n");
        return 1;
    }

    std::ifstream t(argv[1]);

    if (!t.is_open())
    {
        printf("Could not open file \"%s\"\n", argv[1]);
        return 1;
    }

    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);
    const char *src = buffer.c_str();


    std::array<cish::Token, 512> tokbuf;
    // cish::Lexer lexer;
    // size_t tokcount = lexer.tokenize(buffer.c_str(), tokbuf.data(), tokbuf.size());
    size_t tokcount = cish::lexerMain(buffer.c_str(), tokbuf.data(), tokbuf.size());

    // std::cout << "---------------- LEXER -----------------\n";
    // for (size_t i=0; i<tokcount; i++)
    // {
    //     if (tokbuf[i].type != cish::Type::SemiColon)
    //         printf("%s ", cish::TypeToStr(tokbuf[i].type));
    //     else
    //         printf(";\n");
    // }
    // std::cout << "\n";
    // std::cout << "----------------------------------------\n\n\n";



    using namespace cish;

    cish::CompileCtx ctx(new VmOp[512], 512*sizeof(VmOp));

    cish::Parser psr(ctx, tokbuf.data(), tokbuf.size());
    functional_test(psr);
    return 0;

    std::cout << "---------------- PARSER ----------------\n";
    std::cout << "tokcount: " << tokcount << "\n";
    cish::Parser parser(ctx, tokbuf.data(), tokbuf.size());
    auto *ast = parser.buildAST();
    std::cout << "----------------------------------------\n\n";


    std::cout << "---------------- COMPILER --------------\n";
    cish::compile(ctx, ast);
    std::cout << "----------------------------------------\n\n";

    std::cout << "---------------- DISASSEMBLY -----------\n";
    cish::disassemble(ctx.m_base, ctx.m_size);
    std::cout << "----------------------------------------\n\n";

    std::cout << "---------------- EXEC ------------------\n";
    int res = cish::exec(ctx.m_base, ctx.m_size);
    std::cout << "res: " << res << "\n";
    std::cout << "----------------------------------------\n\n";


    // std::cout << "---------------- RUNNING ---------------\n";
    // // // int result = cish::interpret(ptree);
    // // int64_t *ptree_stack = new int64_t[512];
    // // int64_t *rsp = ptree_stack;
    // // memset(ptree_stack, 0, sizeof(ptree_stack));

    // cish::CompileCtx ctx;
    // ctx.symtab   = cish::sym_create(2048);;
    // ctx.text_out = stdout;

    // ptree->compile(ctx);
    // // printf("Result: %ld\n", ptree_stack[0]);
    // std::cout << "----------------------------------------\n\n";

    // mpc_parser_t *Expr  = mpc_new("expression");
    // mpc_parser_t *Prod  = mpc_new("product");
    // mpc_parser_t *Value = mpc_new("value");
    // mpc_parser_t *Maths = mpc_new("maths");

    // mpca_lang(MPCA_LANG_DEFAULT,
    // " expression : <product> (('+' | '-') <product>)*; "
    // " product    : <value>   (('*' | '/')   <value>)*; "
    // " value      : /[0-9]+/ | '(' <expression> ')';    "
    // " maths      : /^/ <expression> /$/;               ",
    // Expr, Prod, Value, Maths, NULL);

    // mpc_result_t r;

    // if (mpc_parse("input", src, Maths, &r))
    // {
    //     mpc_ast_print((mpc_ast_t*)(r.output));
    //     mpc_ast_delete((mpc_ast_t*)(r.output));
    // }
    // else
    // {
    //     mpc_err_print(r.error);
    //     mpc_err_delete(r.error);
    // }

    // mpc_cleanup(4, Expr, Prod, Value, Maths);


    return 0;
}

