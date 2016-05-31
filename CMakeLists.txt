cmake_minimum_required(VERSION 2.8.12)
project(overload_cast)

add_executable(overload_cast overload_cast.hpp test.cpp)
target_compile_options(overload_cast PUBLIC -std=c++14)
