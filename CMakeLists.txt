cmake_minimum_required(VERSION 3.22)
project(untitled)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(untitled ex3.cpp BQ.cpp BQ.h UBQ.cpp UBQ.h Producer.cpp Producer.h ex3.h)
