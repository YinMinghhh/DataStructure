cmake_minimum_required(VERSION 3.24)
project(DataStructure C)

set(CMAKE_C_STANDARD 11)

add_executable(Calculator main.c lib/ymStack.h lib/ymSeqList.h APP/ReversePolishNotation.h APP/ReversePolishNotation.c lib/ymString.h lib/ymString.c lib/ymStack.c)
