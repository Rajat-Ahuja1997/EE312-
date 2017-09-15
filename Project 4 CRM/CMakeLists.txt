cmake_minimum_required(VERSION 3.6)
project(Project_4_CRM)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        Invent.h
        main.cpp
        MyString.cpp
        MyString.h
        Project4.cpp)

add_executable(Project_4_CRM ${SOURCE_FILES})