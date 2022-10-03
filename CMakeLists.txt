cmake_minimum_required(VERSION 3.16)
project(spiceLang)

set(CMAKE_CXX_STANDARD 23)

add_executable(spiceLang main.cpp lexerToken.h lexer.cpp lexer.h misc.cpp misc.h ASTNode.cpp ASTNode.h parser.cpp parser.h)
