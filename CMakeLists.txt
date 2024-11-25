cmake_minimum_required(VERSION 3.15)

project(whup)

aux_source_directory(./src WHUPC_LIST)

include_directories ( ./include )

file(GLOB LIBS "src/*.cpp")

#将所有.cpp文件添加到库列表中

add_library(block SHARED src/block.cpp src/if.cpp src/while.cpp src/var.cpp src/print.cpp src/assign.cpp src/while.cpp src/break.cpp)
add_library(expression SHARED src/expression.cpp)
add_library(parser SHARED src/parser.cpp)
add_library(lexer SHARED src/lexer.cpp)
add_library(extractor SHARED src/extractor.cpp)
add_library(generator SHARED src/generator.cpp)
add_library(io SHARED src/io.cpp)
add_library(check SHARED src/check.cpp)

target_link_libraries(expression PUBLIC parser check)
target_link_libraries(block PUBLIC parser expression lexer check)
target_link_libraries(generator PUBLIC parser)
target_link_libraries(check PUBLIC lexer parser)

add_executable(whupc src/whupc.cpp)

target_link_libraries(whupc PUBLIC block parser expression lexer extractor generator io check)