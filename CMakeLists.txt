cmake_minimum_required(VERSION 3.15)

project(whup)

aux_source_directory(./src WHUPC_LIST)

include_directories ( ./include )

file(GLOB LIBS "src/*.cpp")

#将所有.cpp文件添加到库列表中
add_library(var SHARED src/var.cpp "include/if.h")
add_library(block SHARED src/block.cpp "include/if.h")
add_library(expression SHARED src/expression.cpp "include/if.h")
add_library(while SHARED src/while.cpp "include/if.h")


target_link_libraries(while PUBLIC expression)

target_link_libraries(block PUBLIC var)


add_executable(whupc  ${WHUPC_LIST} "include/if.h")