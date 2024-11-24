cmake_minimum_required(VERSION 3.15)

project(whup)

aux_source_directory(./src WHUPC_LIST)

include_directories ( ./include )

file(GLOB LIBS "src/*.cpp")

add_library(block SHARED src/block.cpp src/if.cpp src/while.cpp src/var.cpp src/print.cpp src/assign.cpp src/while.cpp src/break.cpp src/continue.cpp src/return.cpp src/function.cpp src/expression.cpp src/object.cpp scr/class.cpp scr/classfunction.cpp)
add_library(parser SHARED src/parser.cpp)
add_library(lexer SHARED src/lexer.cpp)
add_library(extractor SHARED src/extractor.cpp)
add_library(generator SHARED src/generator.cpp)
add_library(io SHARED src/io.cpp)

target_link_libraries(block PUBLIC parser lexer)
target_link_libraries(generator PUBLIC parser)

add_executable(whupc src/whupc.cpp)

target_link_libraries(whupc PUBLIC block parser lexer extractor generator io)