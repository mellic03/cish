#!/bin/bash

g++ \
    -g -std=c++23 \
    -I ./include/ \
    src/main.cpp \
    src/cish/keyword.cpp \
    src/cish/node.cpp \
    src/cish/symtab.cpp \
    src/cish/token.cpp \
    src/interpret/interpret.cpp \
    src/lexer/lexer.cpp \
    src/lexer/lexer-state.cpp \
    src/parser/parser.cpp \
    src/vm/vm.cpp \
    -o cish

