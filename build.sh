#!/bin/bash

# gcc src/mpc.c -c mpc.o


g++ \
    -g -std=c++23 \
    -I ./include/ \
    src/main.cpp \
    src/compile/compile-ctx.cpp \
    src/compile/compile.cpp \
    src/compile/pass1.cpp \
    src/compile/pass2.cpp \
    src/compile/print.cpp \
    src/cish/keyword.cpp \
    src/cish/token.cpp \
    src/lexer/lexer-state.cpp \
    src/lexer/lexer.cpp \
    src/parser/parser.cpp \
    src/vm/ctx.cpp \
    src/vm/vm.cpp \
    -o cish



