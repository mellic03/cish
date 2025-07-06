#!/bin/bash

# gcc src/mpc.c -c mpc.o


g++ \
    -g -std=c++23 \
    -I ./include/ \
    src/main.cpp \
    src/assembler/assembler2.cpp \
    src/assembler/symtab.cpp \
    src/cish/compile.cpp \
    src/cish/keyword.cpp \
    src/cish/node.cpp \
    src/cish/symtab.cpp \
    src/cish/token.cpp \
    src/lexer/lexer.cpp \
    src/lexer/lexer-state.cpp \
    src/parser/parser2.cpp \
    src/vm/vm.cpp \
    -o cish



