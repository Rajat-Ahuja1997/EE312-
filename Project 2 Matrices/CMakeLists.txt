cmake_minimum_required(VERSION 3.6)
project(Project_2_Matrices)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        MatrixMultiply.h
        Project2.cpp)

add_executable(Project_2_Matrices ${SOURCE_FILES})