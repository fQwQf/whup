cmake_minimum_required(VERSION 3.15)

project(whup)

aux_source_directory(./src WHUPC_LIST)

include_directories ( ./include )

add_executable(whupc  ${WHUPC_LIST})