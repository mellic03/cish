#include <fstream>    // For std::ifstream
#include <string>     // For std::string
#include <iostream>   // For std::cout, std::cerr
#include <sstream>  // For std::istreambuf_iterator
#include <array>
#include <string.h>

#include <cish.hpp>
#include <cish/symtab.hpp>
#include <cish/lexer.hpp>
#include <cish/token-stream.hpp>
#include <cish/parser.hpp>
#include <cish/compile.hpp>
#include <cish/compile-ctx.hpp>

#include "vm/bytecode.hpp"
#include "vm/vm.hpp"


// extern void printAst( cish::AstNode* );
// extern void ParseTest( cish::CompileCtx&, cish::TokenStream& );

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
    size_t tokcount = cish::lexerMain(buffer.c_str(), tokbuf.data(), tokbuf.size());

    std::cout << "---------------- LEXER -----------------\n";
    for (size_t i=0; i<tokcount; i++)
    {
        if (tokbuf[i].type != cish::Type::SemiColon)
            printf("%s ", cish::TypeToStr(tokbuf[i].type));
        else
            printf(";\n");
    }
    std::cout << "\n";
    std::cout << "----------------------------------------\n\n\n";



    using namespace cish;

    cish::CompileCtx ctx(new VmOp[512], 512*sizeof(VmOp));
    TokenStream stream(tokbuf.data(), tokbuf.size());
    iNode *parseTree = cish::Parse(stream);

    // std::cout << "---------------- PARSER ----------------\n";
    // std::cout << "tokcount: " << tokcount << "\n";
    // cish::Parser parser(ctx, tokbuf.data(), tokbuf.size());
    // auto *parseTree = parser.buildPT();
    // std::cout << "----------------------------------------\n\n";


    std::cout << "---------------- COMPILER --------------\n";
    cish::compile(ctx, parseTree);
    std::cout << "----------------------------------------\n\n";

    // // std::cout << "---------------- DISASSEMBLY -----------\n";
    // // cish::disassemble(ctx.m_base, ctx.m_size);
    // // std::cout << "----------------------------------------\n\n";

    // std::cout << "---------------- EXEC ------------------\n";
    // int res = cish::exec(ctx.m_base, ctx.m_size);
    // std::cout << "res: " << res << "\n";
    // std::cout << "----------------------------------------\n\n";

    return 0;
}

