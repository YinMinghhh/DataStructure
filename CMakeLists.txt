cmake_minimum_required(VERSION 3.24)
project(DataStruct01 C)

set(CMAKE_C_STANDARD 11)

add_executable(DataStruct01 main.c lib/ymStack.h lib/ymSeqList.h APP/ReversePolishNotation.h APP/ReversePolishNotation.c lib/ymString.h)
