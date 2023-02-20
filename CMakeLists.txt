cmake_minimum_required(VERSION 3.24)
project(parallel_search)
#project(linear_search)

set(CMAKE_CXX_STANDARD 14)

add_executable(parallel_search parallel.cpp common.h)
#add_executable(parallel_search linear.cpp)
