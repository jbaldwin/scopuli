cmake_minimum_required(VERSION 3.10)

project(scopuli_bench CXX)
add_executable(scopuli_bench bench.cpp)
target_link_libraries(scopuli_bench PUBLIC scopuli)
