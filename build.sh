#!/bin/bash

mkdir -p ./build
mkdir -p ./output/data

cd build
cmake ..
make -j8
cd ..

cp build/cish output/cish


# g++ \
#     -g -std=c++23 \
#     -I ./include/ \
#     src/main.cpp \
#     \
#     src/assembler/assembler.cpp \
#     \
#     src/compile/compile-ctx.cpp \
#     src/compile/compile.cpp \
#     src/compile/pass1.cpp \
#     src/compile/pass2.cpp \
#     src/compile/print.cpp \
#     \
#     src/cish/keyword.cpp \
#     src/cish/symtab.cpp \
#     src/cish/token.cpp \
#     \
#     src/lexer/lexer-state.cpp \
#     src/lexer/lexer.cpp \
#     \
#     src/parser/functional.cpp \
#     src/parser/production.cpp \
#     src/parser/parser.cpp \
#     src/parser/precedence.cpp \
#     \
#     src/vm/ctx.cpp \
#     src/vm/vm.cpp \
#     \
#     -o build/cish


