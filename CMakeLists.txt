cmake_minimum_required(VERSION 3.15)

# set the project name
project(whup)

set(WHUPC_LIST expression.cpp parser.cpp lexer.cpp whupc.cpp)


# add the executable
add_executable(whupc  ${WHUPC_LIST})