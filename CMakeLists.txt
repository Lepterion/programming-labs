cmake_minimum_required(VERSION 3.28.3)
project(programming_labs)

set(CMAKE_CXX_STANDARD 26)

add_executable(programming_labs main.cpp
        main.cpp
        sorts.h
        sorts.cpp
        utils.h
        utils.cpp
        explicit_stack_sorting.cpp
        explicit_stack_sorting.h
        file_sort.cpp
        file_sort.h)