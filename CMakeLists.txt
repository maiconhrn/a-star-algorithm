cmake_minimum_required(VERSION 3.17)
project(a_star)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(a_star
        src/15_puzzle.cpp
        src/15_puzzle.h
        src/a_star.cpp
        src/a_star.h
        src/main.cpp
        Makefile)
