#include <fstream>    // For std::ifstream
#include <string>     // For std::string
#include <iostream>   // For std::cout, std::cerr
#include <sstream>  // For std::istreambuf_iterator
#include <array>
#include <string.h>

#include <cish.hpp>
#include <cish/token.hpp>
#include <cish/interpret.hpp>
#include <cish/symtab.hpp>
#include "lexer/lexer.hpp"
#include "parser/parser.hpp"


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


    std::array<cish::Token, 512> tokbuf;
    size_t tokcount = cish::lexerMain(buffer.c_str(), tokbuf.data(), tokbuf.size());

    // std::cout << "---------------- LEXER  ----------------\n";
    // for (size_t i=0; i<tokcount; i++)
    // {
    //     if (tokbuf[i].type != cish::Type::SemiColon)
    //         printf("%s ", cish::TypeToStr(tokbuf[i].type));
    //     else
    //         printf(";\n");
    // }
    // std::cout << "\n";
    // std::cout << "----------------------------------------\n\n\n";


    std::cout << "---------------- PARSER ----------------\n";
    std::cout << "tokcount: " << tokcount << "\n";
    cish::Parser parser;
    auto *ptree = parser.parse(tokbuf.data(), tokcount);
          ptree->print();
    std::cout << "----------------------------------------\n\n";


    // std::cout << "---------------- RUNNING ---------------\n";
    // // int result = cish::interpret(ptree);
    // int64_t *ptree_stack = new int64_t[512];
    // int64_t *rsp = ptree_stack;
    // memset(ptree_stack, 0, sizeof(ptree_stack));

    // ptree->compile(nullptr, rsp);
    // printf("Result: %ld\n", ptree_stack[0]);
    // std::cout << "----------------------------------------\n\n";


    // void *symtab = cish::sym_create(2048);
    // cish::sym_add(symtab, "main", (void*)main);
    // cish::sym_add(symtab, "SYMTAB_BEG", (void*)0xA0A0A0A0);
    // cish::sym_add(symtab, "__boob_babe__", (void*)0xB0081E5);
    // cish::sym_add(symtab, "DeezNuts", (void*)0xAABB);
    // cish::sym_add(symtab, "BitchAss", (void*)0xBABECAFE);
    // cish::sym_add(symtab, "SomeValue", (void*)main);
    // cish::sym_add(symtab, "SomeFunction", (void*)main);
    // cish::sym_add(symtab, "-D__is_kernel", (void*)0x00);

    // void *symbol = cish::sym_next(symtab, nullptr);
    // while (symbol)
    // {
    //     printf("symbol: %s\n", (char*)symbol);
    //     symbol = cish::sym_next(symtab, symbol);
    // }
    // printf("find __boob_babe__: 0x%lx\n", cish::sym_find(symtab, "__boob_babe__"));

    return 0;
}


static void reeee()
{

}
